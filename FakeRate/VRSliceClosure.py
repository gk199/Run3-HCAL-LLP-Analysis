#!/usr/bin/env python3
"""
VR closure of the sideband transfer-factor prediction in slices of the
inclusive score (reviewer Q10).

MOTIVATION
----------
The nominal VR closure (SidebandParameterization_BkgPred.py) integrates the VR
over CR_cut <= S_incl < MR boundary, where S_incl is the inclusive DNN score of
the inclusive-tag jet (jet1 for LJDC, jet0 for SJDC) and the MR boundary is the
era/category inclusive cut.  An integrated closure cannot show whether the
transfer factor (TF) drifts with S_incl toward the SR.  This script splits the
VR into fixed S_incl slices -- the last one ending exactly at the MR boundary --
and compares observed and predicted depth-tagged counts in each slice:

    TF   = N(CR, depth tagged) / N(CR, depth sideband)        per pT bin
    pred = sum_pT  TF x N(VR slice, depth sideband)
    obs  = N(VR slice, depth tagged)

The TF, binning, depth sideband, run/HLT exclusion and event selection are
taken from SidebandParameterization_BkgPred.py, so summing the slices
reproduces the nominal PV-inclusive ("depth") VR numbers exactly; the script
checks this.

A weighted linear fit of obs/pred against the mean S_incl of each slice gives
the drift, and is extrapolated to the mean S_incl of the SR depth SIDEBAND (the
events the SR prediction is built from).

ERRORS
------
The TF uncertainty is common to every slice of a cell (fully correlated), so
the per-slice ratio error used in the fit includes only the uncorrelated terms
(observed Poisson + slice-sideband stat).  The TF normalisation error is quoted
separately.

BLINDING
--------
Only the CR, the VR, and the SR depth SIDEBAND (for its mean S_incl and count)
are booked.  No histogram, count or mean of the SR tagged cell (inclusive
>= cut AND depth >= cut) is ever booked; book() raises if one is requested.

Usage
-----
  python3 VRSliceClosure.py                          # my_scan.txt, edges 0.2 0.4 0.6
  python3 VRSliceClosure.py --edges 0.2 0.35 0.5     # other slice edges
  python3 VRSliceClosure.py --eras 2023_preBPix      # subset of eras
"""

import argparse
import math
import os

import numpy as np
import ROOT

import SidebandParameterization_BkgPred as sbp

ROOT.gROOT.SetBatch(True)
ROOT.gStyle.SetOptStat(0)
ROOT.gStyle.SetEndErrorSize(4)

DEFAULT_ERAS = ["2022", "2023_preBPix", "2023_postBPix", "2022_23_preBPix"]
ERA_LABEL = {
    "2022":            "2022",
    "2023_preBPix":    "2023 preBPix",
    "2023_postBPix":   "2023 postBPix",
    "2022_23_preBPix": "2022 + 2023 preBPix",
}
CATS = ["LJDC", "SJDC"]
CELLS = CATS + ["combined"]
LOW_STAT = 10          # flag slices with fewer observed tagged events than this

# Category -> (depth-tag jet, inclusive-tag jet, candidate requirement)
CAT_DEF = {
    "LJDC": ("jet0", "jet1", "jet0_DepthTagCand == 1 && jet1_InclTagCand == 1"),
    "SJDC": ("jet1", "jet0", "jet1_DepthTagCand == 1 && jet0_InclTagCand == 1"),
}

OUT_DIR = ("SidebandBkgPred_exclHLT_VRslices" if sbp.hlts_to_exclude
           else "SidebandBkgPred_VRslices")

_log_lines = []


def log(msg=""):
    print(msg, flush=True)
    _log_lines.append(msg)


# ------------------------------------------------------------------------------
# Booking
# ------------------------------------------------------------------------------

def book_era(rdf_base, row, edges):
    """
    Lazily book every histogram / mean needed for one era.

    Returns booked[cat] = {
        "cuts": {...},
        "CR_side", "CR_pass": RResultPtr<TH3D>,
        "VR_side", "VR_pass": integrated VR (for the closure check),
        "slices": [ {"lo","hi","side","pass","mean","count"} ],
        "SRSB_mean", "SRSB_count": SR depth-SIDEBAND only,
    }
    """
    sb_low = row["sideband_low"] if row.get("sideband_low") is not None \
        else sbp.sideband_low_for_era(row["era"])
    cr_cut = row["CR_cut_inc"]
    booked = {}

    for cat in CATS:
        djet, ijet, cand = CAT_DEF[cat]
        depth_cut = row[f"DNN_cut_{cat}"]
        inc_cut   = row[f"DNN_cut_inc_{cat}"]
        depth = f"{djet}_scores_depth_LLPanywhere"
        inc   = f"{ijet}_scores_inc_train80"
        side_str = f"{depth} >= {sb_low} && {depth} < {depth_cut}"
        pass_str = f"{depth} >= {depth_cut} && {depth} < 1.1"
        axes = (f"{djet}_PtAxis", f"{djet}_EtaAxis", f"{djet}_Phi")

        if abs(edges[0] - cr_cut) > 1e-9:
            raise ValueError(f"First slice edge {edges[0]} must equal the CR cut {cr_cut}.")
        for e in edges:
            if e >= inc_cut:
                raise ValueError(f"{row['era']} {cat}: slice edge {e} is not below the "
                                 f"inclusive cut (MR boundary) {inc_cut}.")
        slice_edges = list(edges) + [inc_cut]

        rdf_c = rdf_base.Filter(cand, f"{cat}_cand")

        def book(name, inc_lo, inc_hi, depth_cell):
            # ---- BLINDING TRIPWIRE: nothing tagged at or above the MR boundary.
            if depth_cell == "pass" and inc_hi > inc_cut + 1e-12:
                raise RuntimeError(
                    f"BLINDING VIOLATION: {row['era']} {cat} '{name}' would book depth-tagged "
                    f"events with inclusive score up to {inc_hi} > inclusive cut {inc_cut}.")
            dstr = side_str if depth_cell == "side" else pass_str
            return rdf_c.Filter(f"{inc} >= {inc_lo} && {inc} < {inc_hi} && {dstr}",
                                f"{cat}_{name}")

        crs = book("CR_side", 0.0, cr_cut, "side")
        b = {
            "cuts": dict(depth_cut=depth_cut, inc_cut=inc_cut, sb_low=sb_low, cr_cut=cr_cut,
                         side_str=side_str, pass_str=pass_str, inc=inc, depth=depth),
            "CR_side": crs.Histo3D(sbp._h3_model(f"h_{row['era']}_{cat}_CR_side"), *axes),
            "CR_mean": crs.Mean(inc),
            "CR_count": crs.Count(),
            "CR_pass": book("CR_pass", 0.0, cr_cut, "pass").Histo3D(
                sbp._h3_model(f"h_{row['era']}_{cat}_CR_pass"), *axes),
            "VR_side": book("VR_side", cr_cut, inc_cut, "side").Histo3D(
                sbp._h3_model(f"h_{row['era']}_{cat}_VR_side"), *axes),
            "VR_pass": book("VR_pass", cr_cut, inc_cut, "pass").Histo3D(
                sbp._h3_model(f"h_{row['era']}_{cat}_VR_pass"), *axes),
            "slices": [],
        }
        for k, (lo, hi) in enumerate(zip(slice_edges[:-1], slice_edges[1:])):
            rs = book(f"S{k}_side", lo, hi, "side")
            b["slices"].append({
                "lo": lo, "hi": hi,
                "side": rs.Histo3D(sbp._h3_model(f"h_{row['era']}_{cat}_S{k}_side"), *axes),
                "pass": book(f"S{k}_pass", lo, hi, "pass").Histo3D(
                    sbp._h3_model(f"h_{row['era']}_{cat}_S{k}_pass"), *axes),
                "mean": rs.Mean(inc),
                "count": rs.Count(),
            })
        # SR depth SIDEBAND only (inclusive >= cut, depth BELOW the tag cut).
        srsb = book("SRSB", inc_cut, 1.1, "side")
        b["SRSB_mean"]  = srsb.Mean(inc)
        b["SRSB_count"] = srsb.Count()
        booked[cat] = b
    return booked


# ------------------------------------------------------------------------------
# Arithmetic
# ------------------------------------------------------------------------------

def bin_cells(h):
    nx, ny, nz = h.GetNbinsX(), h.GetNbinsY(), h.GetNbinsZ()
    return [(i, j, k) for i in range(1, nx + 1) for j in range(1, ny + 1) for k in range(1, nz + 1)]


def predict(side, tf):
    """pred, error from sideband stat only, error from TF stat only, nominal total error."""
    pred = var_side = var_tf = 0.0
    for c in bin_cells(side):
        s, es = side.GetBinContent(*c), side.GetBinError(*c)
        t, et = tf.GetBinContent(*c), tf.GetBinError(*c)
        pred     += s * t
        var_side += (t * es) ** 2
        var_tf   += (s * et) ** 2
    return pred, math.sqrt(var_side), math.sqrt(var_tf), math.sqrt(var_side + var_tf)


def ratio(obs, eobs, pred, epred_unc, epred_tf):
    if obs <= 0 or pred <= 0:
        return float("nan"), float("nan"), float("nan")
    r = obs / pred
    return r, r * math.hypot(eobs / obs, epred_unc / pred), r * epred_tf / pred


def linear_fit(x, y, ey):
    """Weighted fit y = a + b x.  Returns a, b, cov (2x2), chi2 of the line, chi2 of a constant."""
    x, y, ey = map(np.asarray, (x, y, ey))
    w = 1.0 / ey ** 2
    A = np.vstack([np.ones_like(x), x]).T
    cov = np.linalg.inv(A.T @ (A * w[:, None]))
    a, b = cov @ (A.T @ (w * y))
    chi2_lin = float(np.sum(w * (y - a - b * x) ** 2))
    c0 = np.sum(w * y) / np.sum(w)
    chi2_flat = float(np.sum(w * (y - c0) ** 2))
    return float(a), float(b), cov, chi2_lin, chi2_flat, float(c0)


def summed(hists, name):
    h = hists[0].Clone(name)
    h.SetDirectory(0)
    for o in hists[1:]:
        h.Add(o)
    return h


def materialise(ptr, name):
    """Clone with Sumw2 active, as materialise() in the nominal script does;
    without it TH1::Divide propagates sqrt(content) errors onto the TF."""
    h = ptr.GetValue().Clone(name)
    h.SetDirectory(0)
    if not h.GetSumw2N():
        h.Sumw2()
    return h


def tf_of(pass_h, side_h):
    """
    Transfer factor N_tag / N_side, pT-integrated and per pT bin.  Tagged and
    sideband are disjoint samples, so the relative errors add in quadrature.
    Returns {"all": (p, s, tf, err), "pt": [(pt_lo, pt_hi, p, s, tf, err), ...]}.
    """
    def one(p, s):
        if s <= 0:
            return (p, s, float("nan"), float("nan"))
        t = p / s
        e = t * math.sqrt(1.0 / p + 1.0 / s) if p > 0 else 1.0 / s   # 0 tags: quote 1/N_side
        return (p, s, t, e)
    ax = pass_h.GetXaxis()
    pt = []
    for i in range(1, ax.GetNbins() + 1):
        p = sum(pass_h.GetBinContent(i, j, k) for j in range(1, pass_h.GetNbinsY() + 1)
                for k in range(1, pass_h.GetNbinsZ() + 1))
        s = sum(side_h.GetBinContent(i, j, k) for j in range(1, side_h.GetNbinsY() + 1)
                for k in range(1, side_h.GetNbinsZ() + 1))
        pt.append((ax.GetBinLowEdge(i), ax.GetBinUpEdge(i)) + one(p, s))
    return {"all": one(sum(x[2] for x in pt), sum(x[3] for x in pt)), "pt": pt}


def evaluate_era(era, booked):
    """Materialise the booked results and compute per-cell, per-slice closure."""
    get = {cat: {k: materialise(booked[cat][k], f"m_{era}_{cat}_{k}")
                 for k in ("CR_side", "CR_pass", "VR_side", "VR_pass")}
           for cat in CATS}
    for cat in CATS:
        for k in get[cat]:
            sbp.check_axis_ranges(get[cat][k], f"{era} {cat} {k}")
    nsl = len(booked["LJDC"]["slices"])
    sl = {cat: [{
        "lo": s["lo"], "hi": s["hi"],
        "side": materialise(s["side"], f"m_{era}_{cat}_S{k}_side"),
        "pass": materialise(s["pass"], f"m_{era}_{cat}_S{k}_pass"),
        "mean": s["mean"].GetValue(), "count": s["count"].GetValue(),
    } for k, s in enumerate(booked[cat]["slices"])] for cat in CATS}
    srsb = {cat: (booked[cat]["SRSB_mean"].GetValue(), booked[cat]["SRSB_count"].GetValue())
            for cat in CATS}
    crm = {cat: (booked[cat]["CR_mean"].GetValue(), booked[cat]["CR_count"].GetValue())
           for cat in CATS}
    n_cr = sum(crm[c][1] for c in CATS)
    crm["combined"] = (sum(crm[c][0] * crm[c][1] for c in CATS) / n_cr, n_cr)
    inc_cut = {cat: booked[cat]["cuts"]["inc_cut"] for cat in CATS}
    cr_cut = booked["LJDC"]["cuts"]["cr_cut"]

    # "combined" exactly as in the nominal script: summed CR -> one TF, summed VR.
    get["combined"] = {k: summed([get[c][k] for c in CATS], f"h_{era}_comb_{k}")
                       for k in get["LJDC"]}
    sl["combined"] = []
    for k in range(nsl):
        parts = [sl[c][k] for c in CATS]
        n = sum(p["count"] for p in parts)
        sl["combined"].append({
            "lo": parts[0]["lo"],
            "hi": None if k == nsl - 1 else parts[0]["hi"],   # top edge differs per category
            "side": summed([p["side"] for p in parts], f"h_{era}_comb_S{k}_side"),
            "pass": summed([p["pass"] for p in parts], f"h_{era}_comb_S{k}_pass"),
            "mean": sum(p["mean"] * p["count"] for p in parts) / n if n else float("nan"),
            "count": n,
        })
    n_sr = sum(srsb[c][1] for c in CATS)
    srsb["combined"] = (sum(srsb[c][0] * srsb[c][1] for c in CATS) / n_sr if n_sr else float("nan"),
                        n_sr)

    out = {}
    for cell in CELLS:
        g = get[cell]
        tf = g["CR_pass"].Clone(f"tf_{era}_{cell}")
        tf.SetDirectory(0)
        tf.Divide(g["CR_side"])

        # integrated VR, nominal-style errors
        obs_i, eobs_i = sbp.get_total_and_error(g["VR_pass"])
        pred_i, eu_i, etf_i, etot_i = predict(g["VR_side"], tf)

        rows = []
        for s in sl[cell]:
            obs, eobs = sbp.get_total_and_error(s["pass"])
            pred, eu, etf, etot = predict(s["side"], tf)
            r, er, ertf = ratio(obs, eobs, pred, eu, etf)
            rows.append(dict(lo=s["lo"], hi=s["hi"], mean=s["mean"], nside=s["count"],
                             obs=obs, eobs=eobs, pred=pred, epred=etot, epred_unc=eu,
                             r=r, er=er, ertf=ertf))

        # closure check: slices must add up to the integrated VR
        sum_obs  = sum(x["obs"] for x in rows)
        sum_pred = sum(x["pred"] for x in rows)
        if abs(sum_obs - obs_i) > 1e-6 or abs(sum_pred - pred_i) > 1e-6 * max(1.0, pred_i):
            raise RuntimeError(f"{era} {cell}: slices do not add up to the integrated VR "
                               f"(obs {sum_obs} vs {obs_i}, pred {sum_pred} vs {pred_i}).")

        r_i, er_i, ertf_i = ratio(obs_i, eobs_i, pred_i, eu_i, etf_i)
        tf_rel = etf_i / pred_i if pred_i > 0 else float("nan")

        good = [x for x in rows if math.isfinite(x["r"]) and x["er"] > 0]
        fit = None
        if len(good) >= 2:
            a, b, cov, chi2_lin, chi2_flat, c0 = linear_fit(
                [x["mean"] for x in good], [x["r"] for x in good], [x["er"] for x in good])
            xs = srsb[cell][0]
            y_sr = a + b * xs
            ey_sr = math.sqrt(cov[0, 0] + xs ** 2 * cov[1, 1] + 2 * xs * cov[0, 1])
            fit = dict(a=a, b=b, eb=math.sqrt(cov[1, 1]), cov=cov, chi2_lin=chi2_lin,
                       chi2_flat=chi2_flat, ndf_flat=len(good) - 1, ndf_lin=len(good) - 2,
                       c0=c0, x_sr=xs, y_sr=y_sr, ey_sr=ey_sr)

        # Actual transfer factors: CR, then each VR slice (never the SR).
        tf_regions = [dict(name="CR", lo=0.0, hi=cr_cut, mean=crm[cell][0],
                           **tf_of(g["CR_pass"], g["CR_side"]))]
        for k, s in enumerate(sl[cell]):
            hi = s["hi"] if s["hi"] is not None else max(inc_cut.values())
            tf_regions.append(dict(name=f"VR{k}", lo=s["lo"], hi=hi, mean=s["mean"],
                                   **tf_of(s["pass"], s["side"])))

        out[cell] = dict(tf_regions=tf_regions, sb_low=booked['LJDC']['cuts']['sb_low'],
                         rows=rows, obs=obs_i, eobs=eobs_i, pred=pred_i, epred=etot_i,
                         r=r_i, er=er_i, ertf=ertf_i, tf_rel=tf_rel, fit=fit,
                         srsb_mean=srsb[cell][0], srsb_count=srsb[cell][1])
    return out


# ------------------------------------------------------------------------------
# Reporting
# ------------------------------------------------------------------------------

def fmt_range(lo, hi, cell, row):
    if hi is None:   # combined: top edge is the category's own MR boundary
        hi_txt = f"cut ({row['DNN_cut_inc_LJDC']}/{row['DNN_cut_inc_SJDC']})"
    else:
        hi_txt = f"{hi:.3f}".rstrip("0").rstrip(".")
    return f"[{lo:.2f}, {hi_txt})"


def report_era(era, row, res):
    log(f"\n{'=' * 78}\n{ERA_LABEL.get(era, era)}  ({era})\n{'=' * 78}")
    for cell in CELLS:
        r = res[cell]
        log(f"\n--- {cell} ---")
        log("  Transfer factor N_tag/N_SB  (pT-integrated | per pT bin)")
        for reg in r["tf_regions"]:
            p_, s_, t_, e_ = reg["all"]
            ptxt = "  ".join(f"[{b[0]:g},{b[1]:g}): {b[4]:.3f}±{b[5]:.3f} ({b[2]:.0f}/{b[3]:.0f})"
                             for b in reg["pt"])
            log(f"    {reg['name']:4s} [{reg['lo']:.2f}, {reg['hi']:.3f})  <S_incl>={reg['mean']:.3f}  "
                f"{t_:.3f} ± {e_:.3f} ({p_:.0f}/{s_:.0f})   |  {ptxt}")
        log(f"  {'S_incl slice':<26s}{'<S_incl>':>9s}{'N_side':>9s}{'obs':>14s}"
            f"{'pred':>16s}{'obs/pred (uncorr)':>22s}")
        for x in r["rows"]:
            flag = "   <-- LOW STAT" if x["obs"] < LOW_STAT else ""
            log(f"  {fmt_range(x['lo'], x['hi'], cell, row):<26s}{x['mean']:9.3f}{x['nside']:9d}"
                f"{x['obs']:8.0f} ± {x['eobs']:4.1f}{x['pred']:9.2f} ± {x['epred']:5.2f}"
                f"{x['r']:12.3f} ± {x['er']:.3f}{flag}")
        log(f"  {'VR integrated':<26s}{'':9s}{'':9s}{r['obs']:8.0f} ± {r['eobs']:4.1f}"
            f"{r['pred']:9.2f} ± {r['epred']:5.2f}{r['r']:12.3f} ± {r['er']:.3f}"
            f"   (slices add up: OK)")
        log(f"  TF normalisation (correlated across slices): ±{100 * r['tf_rel']:.1f}% "
            f"on every prediction / ratio")
        log(f"  SR depth sideband: N = {r['srsb_count']}, <S_incl> = {r['srsb_mean']:.3f}")
        f = r["fit"]
        if f:
            sig = f["b"] / f["eb"] if f["eb"] > 0 else float("nan")
            log(f"  Linear fit obs/pred = a + b <S_incl>: b = {f['b']:+.3f} ± {f['eb']:.3f}"
                f"  ({sig:+.1f} sigma), chi2/ndf = {f['chi2_lin']:.2f}/{f['ndf_lin']}")
            log(f"  Constant fit: {f['c0']:.3f}, chi2/ndf = {f['chi2_flat']:.2f}/{f['ndf_flat']}"
                f"  (p = {ROOT.TMath.Prob(f['chi2_flat'], f['ndf_flat']):.2f})")
            log(f"  Linear fit extrapolated to the SR sideband <S_incl> = {f['x_sr']:.3f}: "
                f"obs/pred = {f['y_sr']:.3f} ± {f['ey_sr']:.3f} (fit) ± "
                f"{f['y_sr'] * r['tf_rel']:.3f} (TF)")
        else:
            log("  Fit: not enough populated slices.")


def _num(x, nd=0):
    return f"{x:.{nd}f}"


def _tf_tex(t):
    p, s, v, e = t
    if not math.isfinite(v):
        return "--"
    return f"${v:.3f} \\pm {e:.3f}$ \\scriptsize{{({p:.0f}/{s:.0f})}}"


def _region_header(results):
    """Column headers: CR, then the VR slices (top edge = MR boundary)."""
    any_res = next(iter(results.values()))["LJDC"]["tf_regions"]
    hdr = []
    for k, r in enumerate(any_res):
        if r["name"] == "CR":
            hdr.append(f"CR $[0, {r['hi']:g})$")
        elif k == len(any_res) - 1:
            hdr.append(f"VR $[{r['lo']:g}, \\mathrm{{MR}})$")
        else:
            hdr.append(f"VR $[{r['lo']:g}, {r['hi']:g})$")
    return hdr


def latex_tables(results, rows_cfg, edges):
    hdr = _region_header(results)
    ncol = len(hdr)
    L = ["% Generated by FakeRate/VRSliceClosure.py",
         f"% Slice edges: {edges} + MR boundary (era/category inclusive cut)", ""]

    # ---- Table 1: transfer factors, pT-integrated
    L += ["\\begin{table}[htbp]", "\\centering",
          "\\caption{Transfer factor $N_\\mathrm{tag}/N_\\mathrm{SB}$ of the depth-tag "
          "jet, integrated over $p_\\mathrm{T}$, in the CR and in slices of the inclusive score "
          "$S_\\mathrm{incl}$ of the inclusive-tag jet in the VR. The last VR slice ends at the MR "
          "boundary (the inclusive DNN cut of the era and category: preBPix LJDC 0.785, SJDC 0.955; "
          "postBPix LJDC 0.995, SJDC 0.975). Numbers in parentheses are $N_\\mathrm{tag}/N_\\mathrm{SB}$. "
          "The prediction applies the CR transfer factor per $p_\\mathrm{T}$ bin "
          "(Table~\\ref{tab:VRslices_TF_pt}).}",
          "\\label{tab:VRslices_TF}",
          "\\resizebox{\\textwidth}{!}{",
          "\\begin{tabular}{ll" + "c" * ncol + "}", "\\hline",
          "Era & Category & " + " & ".join(hdr) + " \\\\", "\\hline"]
    for era, res in results.items():
        for ci, cell in enumerate(CELLS):
            era_txt = ERA_LABEL.get(era, era) if ci == 0 else ""
            L.append(f"{era_txt} & {cell} & " +
                     " & ".join(_tf_tex(r["all"]) for r in res[cell]["tf_regions"]) + " \\\\")
        L.append("\\hline")
    L += ["\\end{tabular}}", "\\end{table}", ""]

    # ---- Table 2: transfer factors per pT bin
    L += ["\\begin{table}[htbp]", "\\centering",
          "\\caption{As Table~\\ref{tab:VRslices_TF}, per $p_\\mathrm{T}$ bin of the depth-tag jet. "
          "The CR column is the transfer factor used in the background prediction.}",
          "\\label{tab:VRslices_TF_pt}",
          "\\resizebox{\\textwidth}{!}{",
          "\\begin{tabular}{lll" + "c" * ncol + "}", "\\hline",
          "Era & Category & $p_\\mathrm{T}$ [GeV] & " + " & ".join(hdr) + " \\\\", "\\hline"]
    for era, res in results.items():
        for ci, cell in enumerate(CELLS):
            regs = res[cell]["tf_regions"]
            for b in range(len(regs[0]["pt"])):
                lo, hi = regs[0]["pt"][b][:2]
                hi_txt = f"{hi:g}" if b < len(regs[0]["pt"]) - 1 else "$\\infty$"
                era_txt = ERA_LABEL.get(era, era) if (ci == 0 and b == 0) else ""
                cat_txt = cell if b == 0 else ""
                L.append(f"{era_txt} & {cat_txt} & $[{lo:g}, $ {hi_txt}$)$ & " +
                         " & ".join(_tf_tex(r["pt"][b][2:]) for r in regs) + " \\\\")
            L.append("\\hline" if ci == len(CELLS) - 1 else f"\\cline{{2-{3 + ncol}}}")
    L += ["\\end{tabular}}", "\\end{table}", ""]

    # ---- Table 3: closure, observed vs predicted (pT-binned CR TF applied)
    def _rt(o, eo, pr, ep):
        if o <= 0 or pr <= 0:
            return "--"
        r = o / pr
        return f"${r:.2f} \\pm {r * math.hypot(eo / o, ep / pr):.2f}$"

    L += ["\\begin{table}[htbp]", "\\centering",
          "\\caption{VR closure per $S_\\mathrm{incl}$ slice. Observed: depth-tagged events in the "
          "slice, with Poisson uncertainty. Predicted: CR transfer factor, applied per "
          "$p_\\mathrm{T}$ bin to the depth-sideband events of the slice; the uncertainty combines "
          "the transfer-factor and sideband statistics. All uncertainties are statistical. The "
          "transfer-factor part is common to the slices of a category.}",
          "\\label{tab:VRslices_closure}",
          "\\begin{tabular}{lllccc}", "\\hline",
          "Era & Category & VR slice & Observed & Predicted & Obs/Pred \\\\", "\\hline"]
    for era, res in results.items():
        for ci, cell in enumerate(CELLS):
            r = res[cell]
            nrow = len(r["rows"])
            for k, x in enumerate(r["rows"]):
                rng = (f"$[{x['lo']:g}, \\mathrm{{MR}})$" if k == nrow - 1
                       else f"$[{x['lo']:g}, {x['hi']:g})$")
                era_txt = ERA_LABEL.get(era, era) if (ci == 0 and k == 0) else ""
                cat_txt = cell if k == 0 else ""
                L.append(f"{era_txt} & {cat_txt} & {rng} & ${x['obs']:.0f} \\pm {x['eobs']:.1f}$ & "
                         f"${x['pred']:.1f} \\pm {x['epred']:.1f}$ & "
                         f"{_rt(x['obs'], x['eobs'], x['pred'], x['epred'])} \\\\")
            L.append(f" & & VR total & ${r['obs']:.0f} \\pm {r['eobs']:.1f}$ & "
                     f"${r['pred']:.1f} \\pm {r['epred']:.1f}$ & "
                     f"{_rt(r['obs'], r['eobs'], r['pred'], r['epred'])} \\\\")
            L.append("\\hline" if ci == len(CELLS) - 1 else "\\cline{2-6}")
    L += ["\\end{tabular}", "\\end{table}"]
    return "\n".join(L) + "\n"


# ------------------------------------------------------------------------------
# Plot: actual transfer factor vs S_incl
# ------------------------------------------------------------------------------

COLOR = {"LJDC": ROOT.kRed + 1, "SJDC": ROOT.kAzure + 2, "combined": ROOT.kGreen + 3}


def draw_tf_panel(pad, era, cat, row, res, keep, show_cms=True):
    pad.cd()
    pad.SetLeftMargin(0.14)
    pad.SetRightMargin(0.04)
    pad.SetTopMargin(0.10)
    pad.SetBottomMargin(0.14)
    col = COLOR[cat]
    regs = res[cat]["tf_regions"]
    cr, vrs = regs[0], regs[1:]
    # MR boundary.  For "combined" it differs per category: the SR is shaded from
    # the higher cut, and the band between the two cuts (SR of one category, VR of
    # the other) is hatched.
    cuts = [row[f"DNN_cut_inc_{c}"] for c in (CATS if cat == "combined" else [cat])]
    inc_cut, cut_lo = max(cuts), min(cuts)

    ys = [(r["all"][2] + r["all"][3]) for r in regs if math.isfinite(r["all"][2])]
    ymax = (2.1 if cat == "combined" else 1.85) * max(ys)   # headroom for legend + labels
    frame = pad.DrawFrame(0.0, 0.0, 1.0, ymax)
    frame.GetXaxis().SetTitle("S_{incl} (inclusive score of the inclusive-tag jet)")
    frame.GetYaxis().SetTitle("Transfer factor N_{tag} / N_{SB}")
    for ax in (frame.GetXaxis(), frame.GetYaxis()):
        ax.SetTitleSize(0.048)
        ax.SetLabelSize(0.043)
    frame.GetYaxis().SetTitleOffset(1.3)
    frame.GetXaxis().SetTitleOffset(1.2)
    frame.GetXaxis().SetNdivisions(510)

    # SR: blinded, nothing drawn in it
    sr = ROOT.TBox(inc_cut, 0.0, 1.0, ymax)
    sr.SetFillColorAlpha(ROOT.kGray, 0.45)
    sr.SetLineWidth(0)
    sr.Draw()
    keep.append(sr)
    if cut_lo < inc_cut:
        mixed = ROOT.TBox(cut_lo, 0.0, inc_cut, ymax)
        mixed.SetFillColor(ROOT.kGray + 1)
        mixed.SetFillStyle(3354)
        mixed.SetLineWidth(0)
        mixed.Draw()
        keep.append(mixed)
    # CR TF band, extended across the VR to the MR boundary
    t, e = cr["all"][2], cr["all"][3]
    band = ROOT.TBox(0.0, t - e, inc_cut, t + e)
    band.SetFillColorAlpha(col, 0.18)
    band.SetLineWidth(0)
    band.Draw()
    keep.append(band)
    lcr = ROOT.TLine(0.0, t, inc_cut, t)
    lcr.SetLineColor(col)
    lcr.SetLineStyle(2)
    lcr.SetLineWidth(2)
    lcr.Draw()
    keep.append(lcr)
    # region boundaries
    for x in [cr["hi"]] + cuts:
        lb = ROOT.TLine(x, 0.0, x, ymax)
        lb.SetLineColor(ROOT.kGray + 2)
        lb.SetLineStyle(3 if x == cr["hi"] else 1)
        lb.Draw()
        keep.append(lb)

    def graph(regions, marker, fill_col):
        g = ROOT.TGraphAsymmErrors(len(regions))
        for i, r in enumerate(regions):
            hi = min(r["hi"], inc_cut)
            g.SetPoint(i, r["mean"], r["all"][2])
            g.SetPointError(i, r["mean"] - r["lo"], hi - r["mean"],
                            r["all"][3], r["all"][3])
        g.SetMarkerStyle(marker)
        g.SetMarkerSize(1.4)
        g.SetMarkerColor(fill_col)
        g.SetLineColor(fill_col)
        g.SetLineWidth(2)
        g.Draw("P same")
        keep.append(g)
        return g

    gcr = graph([cr], 24, ROOT.kBlack)
    gvr = graph(vrs, 20, col)

    # N_tag / N_SB above each point
    lab = ROOT.TLatex()
    lab.SetTextFont(42)
    lab.SetTextSize(0.034)
    lab.SetTextAlign(21)
    for r in regs:
        if math.isfinite(r["all"][2]):
            lab.DrawLatex(r["mean"], (r["all"][2] + r["all"][3]) + 0.025 * ymax,
                          f"{r['all'][0]:.0f}/{r['all'][1]:.0f}")
    keep.append(lab)

    txt = ROOT.TLatex()
    txt.SetTextFont(42)
    txt.SetTextSize(0.036 if 1.0 - inc_cut > 0.1 else 0.028)
    txt.SetTextAngle(90)
    txt.SetTextAlign(23)
    if 1.0 - inc_cut > 0.03:   # too narrow to label in postBPix
        txt.DrawLatex(0.5 * (inc_cut + 1.0), 0.5 * ymax, "#color[921]{SR (blinded)}")
    if inc_cut - cut_lo > 0.03:
        low_cat = CATS[cuts.index(cut_lo)]
        txt.SetTextSize(0.028)
        txt.DrawLatex(0.5 * (cut_lo + inc_cut), 0.5 * ymax,
                      f"#color[921]{{{low_cat} SR (blinded)}}")
    keep.append(txt)

    leg = ROOT.TLegend(0.17, 0.70, 0.62, 0.86)
    leg.SetBorderSize(0)
    leg.SetFillStyle(0)
    leg.SetTextSize(0.036)
    leg.AddEntry(gcr, "CR (TF used in prediction)", "lp")
    leg.AddEntry(band, "CR TF #pm stat, extended over VR", "f")
    leg.AddEntry(gvr, "VR slices", "lp")
    leg.Draw()
    keep.append(leg)

    stamp = ROOT.TLatex()
    stamp.SetNDC()
    stamp.SetTextFont(42)
    if show_cms:
        stamp.SetTextSize(0.046)
        stamp.DrawLatex(0.14, 0.925, "#bf{CMS} #scale[0.8]{#it{Private Work}}")
    stamp.SetTextSize(0.040)
    stamp.SetTextAlign(31)
    stamp.DrawLatex(0.96, 0.925, f"{ERA_LABEL.get(era, era)}, {cat}")
    stamp.SetTextAlign(11)
    stamp.SetTextSize(0.032)
    for i, c in enumerate(CATS if cat == "combined" else [cat]):
        pre = f"{c}: " if cat == "combined" else ""
        stamp.DrawLatex(0.17, 0.645 - 0.045 * i,
                        f"{pre}depth SB [{res[c]['sb_low']:g}, {row[f'DNN_cut_{c}']:g}),"
                        f" tag #geq {row[f'DNN_cut_{c}']:g}")
    keep.append(stamp)
    keep.append(frame)


def make_tf_plots(results, rows_cfg, tag):
    keep = []
    # one file per era group: LJDC | SJDC | combined
    for era, res in results.items():
        c = ROOT.TCanvas(f"c_tf_{era}", "", 2250, 620)
        c.Divide(3, 1)
        for i, cat in enumerate(CELLS):
            draw_tf_panel(c.cd(i + 1), era, cat, rows_cfg[era], res, keep)
        c.Update()
        for ext in ("png", "pdf"):
            path = os.path.join(OUT_DIR, f"VRslices_TF_{era}{tag}.{ext}")
            c.SaveAs(path)
            log(f"wrote {path}")
        keep.append(c)
    # summary: rows = era groups, columns = LJDC | SJDC | combined
    eras = list(results)
    c = ROOT.TCanvas("c_tf_all", "", 2250, 620 * len(eras))
    c.Divide(3, len(eras))
    for j, era in enumerate(eras):
        for i, cat in enumerate(CELLS):
            draw_tf_panel(c.cd(3 * j + i + 1), era, cat, rows_cfg[era], results[era], keep)
    c.Update()
    for ext in ("png", "pdf"):
        path = os.path.join(OUT_DIR, f"VRslices_TF_all{tag}.{ext}")
        c.SaveAs(path)
        log(f"wrote {path}")


# ------------------------------------------------------------------------------

def main():
    p = argparse.ArgumentParser(description=__doc__,
                                formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument("--config", default="my_scan.txt")
    p.add_argument("--edges", type=float, nargs="+", default=[0.2, 0.4, 0.6],
                   help="lower slice edges; the first must be the CR cut, and the MR boundary "
                        "(category inclusive cut) is appended as the last upper edge")
    p.add_argument("--eras", nargs="+", default=DEFAULT_ERAS)
    p.add_argument("-t", "--threads", type=int, default=0)
    args = p.parse_args()

    if args.threads > 0:
        ROOT.EnableImplicitMT(args.threads)
    else:
        ROOT.EnableImplicitMT()
    os.makedirs(OUT_DIR, exist_ok=True)

    cfg = {r["era"]: r for r in sbp.parse_config_file(args.config)}
    missing = [e for e in args.eras if e not in cfg]
    if missing:
        raise ValueError(f"Eras {missing} not in {args.config}")
    for e in args.eras:
        if e == "WPlusJets":
            raise ValueError("This script uses the data branch names; W+Jets MC is not supported.")

    log("VR closure in slices of the inclusive score (reviewer Q10)")
    log(f"config: {args.config}   slice edges: {args.edges} + MR boundary")
    log(f"pT bins: {list(sbp.pT_bins)}   |eta| bins: {list(sbp.eta_bins)}   "
        f"phi bins: {len(sbp.phi_bins) - 1}")
    log(f"HLT exclusion: {sbp.hlts_to_exclude}   PV: inclusive (nominal 'depth' option)")

    results, rows_cfg = {}, {}
    for era in args.eras:
        row = cfg[era]
        rows_cfg[era] = row
        sb = row["sideband_low"] if row.get("sideband_low") is not None \
            else sbp.sideband_low_for_era(era)
        log(f"\n[{era}] building RDataFrame ...")
        for cat in CATS:
            log(f"  {cat}: depth sideband [{sb}, {row[f'DNN_cut_{cat}']}), tagged >= "
                f"{row[f'DNN_cut_{cat}']};  CR S_incl < {row['CR_cut_inc']}, "
                f"MR boundary S_incl = {row[f'DNN_cut_inc_{cat}']}")
        rdf = sbp._build_rdf_base(era)
        booked = book_era(rdf, row, args.edges)
        results[era] = evaluate_era(era, booked)
        report_era(era, row, results[era])

    # Tag outputs with the slice edges so runs with different edges coexist.
    tag = "_edges_" + "_".join(f"{e:g}".replace(".", "p") for e in args.edges)
    tex_path = os.path.join(OUT_DIR, f"VRslices_closure{tag}.tex")
    with open(tex_path, "w") as f:
        f.write(latex_tables(results, rows_cfg, args.edges))
    log(f"\nwrote {tex_path}")
    make_tf_plots(results, rows_cfg, tag)

    txt_path = os.path.join(OUT_DIR, f"VRslices_closure{tag}.txt")
    with open(txt_path, "w") as f:
        f.write("\n".join(_log_lines) + "\n")
    print(f"wrote {txt_path}")


if __name__ == "__main__":
    main()
