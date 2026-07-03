import re
import os
import argparse
import ROOT

# ──────────────────────────────────────────────────────────────────────────────
# Filename parser
# ──────────────────────────────────────────────────────────────────────────────
# Recognised patterns
#   Signal : minituple_HToSSTo4B_<mH>_<mS>_CTau<ctau>_scores.root
#   Data   : minituple_data_<era>_scores.root

def parse_filename(file_path):
    """
    Return a dict with keys:
        kind        – 'signal' | 'data' | 'unknown'
        plain       – human-readable one-line description
        latex       – LaTeX one-line description
        short       – very short tag for plain-text table headers
    """
    stem = os.path.basename(file_path)          # e.g. minituple_HToSSTo4B_125_50_CTau3000_scores.root
    stem = stem.replace(".root", "")

    # ── signal ────────────────────────────────────────────────────────────────
    m = re.search(
        r"HToSSTo4B_(?:MH)?(?P<mH>\d+)_(?:MS)?(?P<mS>\d+)_CTau(?P<ctau>\d+)",
        stem, re.IGNORECASE
    )
    if m:
        mH, mS, ctau = m.group("mH"), m.group("mS"), m.group("ctau")
        ctau_mm = int(ctau)
        if ctau_mm % 1000 == 0:
            ctau_short = rf"{ctau_mm // 1000}~m"
        elif ctau_mm % 100 == 0:
            ctau_short = rf"{ctau_mm / 1000:.1f}~m"
        else:
            ctau_short = rf"{ctau_mm}~mm"
        return dict(
            kind        = "signal",
            plain       = f"H→SS→4b  mH={mH} GeV  mS={mS} GeV  cτ={ctau} mm",
            latex       = (
                r"$H \to SS \to 4b$, "
                rf"$m_{{H}}={mH}$~GeV, $m_{{S}}={mS}$~GeV, "
                rf"$c\tau={ctau}$~mm"
            ),
            short_latex = rf"{mH}~GeV, {mS}~GeV, {ctau_short}",
            short       = f"HToSS_mH{mH}_mS{mS}_CTau{ctau}",
        )

    # ── data ──────────────────────────────────────────────────────────────────
    m = re.search(r"data_(?P<era>[A-Za-z0-9]+)", stem)
    if m:
        era = m.group("era")
        return dict(
            kind        = "data",
            plain       = f"Data  era={era}",
            latex       = rf"Data, era {era}",
            short_latex = rf"Data, era {era}",
            short       = f"data_{era}",
        )

    # ── fallback ──────────────────────────────────────────────────────────────
    return dict(
        kind        = "unknown",
        plain       = stem,
        latex       = stem.replace("_", r"\_"),
        short_latex = stem.replace("_", r"\_")[:40],
        short       = stem[:30],
    )


# ──────────────────────────────────────────────────────────────────────────────
# Helpers for LaTeX output
# ──────────────────────────────────────────────────────────────────────────────

def latex_setup(sample_latex, apply_llp_truth, weight_expr=None):
    truth_note = (
        r", with LLP truth matching ($177 \leq R_{\text{decay}} < 295$~cm, $\abs{\eta_{LLP}} < 1.26$)"
        if apply_llp_truth else ""
    )
    _weight_label = {
        "weight":                                             "cross-section and luminosity weighted",
        "L1_prescale_weight":                                 "L1 prescale weighted",
        "weight * L1_prescale_weight":                        "cross-section, luminosity, and L1 prescale weighted",
        "HLT_prescale_weight":                                "HLT prescale weighted",
        "weight * HLT_prescale_weight":                       "cross-section, luminosity, and HLT prescale weighted",
        "L1_prescale_weight * HLT_prescale_weight":           "L1 and HLT prescale weighted",
        "weight * L1_prescale_weight * HLT_prescale_weight":  "cross-section, luminosity, L1, and HLT prescale weighted",
    }
    weight_note = f", {_weight_label.get(weight_expr, weight_expr)}" if weight_expr else ""
    caption = rf"Cutflow for {sample_latex}{truth_note}{weight_note}."
    print(r"\begin{table}[h!]")
    print(r"\centering")
    print(r"\begin{tabular}{lrrr}")
    print(r"\hline")
    print(r"\textbf{Selection} & \textbf{Events} & \textbf{\% of All} & \textbf{\% of HLT} \\")
    print(r"\hline")
    # store caption for latex_end
    return caption

def latex_end(caption):
    print(r"\hline")
    print(r"\end{tabular}")
    print(rf"\caption{{{caption}}}")
    print(r"\end{table}")


# ──────────────────────────────────────────────────────────────────────────────
# Prescale impact summary table
# ──────────────────────────────────────────────────────────────────────────────

def prescale_impact_table(
    file_paths,
    tree_name          = "NoSel",
    dnn_inc_cut        = 0.97,
    dnn_depth_cut      = 0.95,
    dnn_inc_cut_sjdc   = None,
    dnn_depth_cut_sjdc = None,
):
    """
    Print a LaTeX table with one row per signal sample showing the fractional
    impact of L1 and HLT prescale weights relative to lumi×xsec weighting only.

    Columns:
        Sample | L1 @ LJDC+SJDC | L1 @ final | L1+HLT @ LJDC+SJDC | L1+HLT @ final

    Each cell = yield(with prescale weight) / yield(weight only) at that cut level.
    """
    rows = []

    for file_path in file_paths:
        sample = parse_filename(file_path)
        f = ROOT.TFile.Open(file_path)
        if not f or f.IsZombie():
            print(f"WARNING: cannot open {file_path}; skipping.")
            continue
        tree = f.Get(tree_name)
        if not tree:
            print(f"WARNING: tree '{tree_name}' not found in {file_path}; skipping.")
            f.Close()
            continue

        has_l1  = bool(tree.GetBranch("L1_prescale_weight"))
        has_hlt = bool(tree.GetBranch("HLT_prescale_weight"))

        _inc_ljdc   = dnn_inc_cut
        _inc_sjdc   = dnn_inc_cut_sjdc   if dnn_inc_cut_sjdc   is not None else dnn_inc_cut
        _depth_ljdc = dnn_depth_cut
        _depth_sjdc = dnn_depth_cut_sjdc if dnn_depth_cut_sjdc is not None else dnn_depth_cut

        LJDC   = "(jet0_DepthTagCand == 1 && jet1_InclTagCand == 1)"
        SJDC   = "(jet1_DepthTagCand == 1 && jet0_InclTagCand == 1)"
        JDC_OR = f"({LJDC} || {SJDC})"
        PT_ETA = (
            "((jet0_Pt > 60 && abs(jet0_Eta) < 1.26 && jet1_Pt > 40 && abs(jet1_Eta) < 2.0) || "
            " (jet1_Pt > 60 && abs(jet1_Eta) < 1.26 && jet0_Pt > 40 && abs(jet0_Eta) < 2.0))"
        )
        dnn_inc = (
            f"(({SJDC} && jet0_scores_inc_train80 > {_inc_sjdc}) || "
            f" ({LJDC} && jet1_scores_inc_train80 > {_inc_ljdc}))"
        )
        dnn_depth = (
            f"(({SJDC} && jet1_scores_depth_LLPanywhere > {_depth_sjdc}) || "
            f" ({LJDC} && jet0_scores_depth_LLPanywhere > {_depth_ljdc}))"
        )

        trig_sel  = "Pass_L1SingleLLPJet == 1 && Pass_HLTDisplacedJet == 1"
        jdc_sel   = f"{trig_sel} && {PT_ETA} && {JDC_OR}"
        final_sel = f"{jdc_sel} && abs(jet0_jet1_dPhi) > 0.2 && {dnn_inc} && {dnn_depth}"

        _ctr = [0]

        def get_yield(sel, wexpr):
            _ctr[0] += 1
            hname = f"_pi_h{_ctr[0]}"
            draw_expr = f"{wexpr}>>{hname}(1,-1e6,1e6)"
            cut = f"({wexpr})*({sel})"
            tree.Draw(draw_expr, cut, "goff")
            h = ROOT.gDirectory.Get(hname)
            result = h.GetSumOfWeights() if h else 0.0
            if h:
                h.Delete()
            return result

        w_base    = "weight "
        w_l1      = "weight * L1_prescale_weight"                    if has_l1              else None
        w_l1_hlt  = "weight * L1_prescale_weight * HLT_prescale_weight" if (has_l1 and has_hlt) else w_l1

        n_jdc_base   = get_yield(jdc_sel,   w_base)
        n_jdc_l1     = get_yield(jdc_sel,   w_l1)     if w_l1     else float("nan")
        n_jdc_l1_hlt = get_yield(jdc_sel,   w_l1_hlt) if w_l1_hlt else float("nan")
        n_fin_base   = get_yield(final_sel,  w_base)
        n_fin_l1     = get_yield(final_sel,  w_l1)     if w_l1     else float("nan")
        n_fin_l1_hlt = get_yield(final_sel,  w_l1_hlt) if w_l1_hlt else float("nan")

        def ratio(num, den):
            return num / den if den > 0 and num == num else float("nan")

        rows.append((
            sample["short_latex"],
            ratio(n_jdc_l1,     n_jdc_base),
            ratio(n_fin_l1,     n_fin_base),
            ratio(n_jdc_l1_hlt, n_jdc_base),
            ratio(n_fin_l1_hlt, n_fin_base),
        ))

        f.Close()

    def _pct(x):
        return f"{100.0*x:.1f}\\%" if x == x else "--"  # x != x iff NaN

    print(r"\begin{table}[h!]")
    print(r"\centering")
    print(r"\begin{tabular}{lrrrr}")
    print(r"\hline")
    print(
        r"\textbf{Sample ($m_H$, $m_S$, $c\tau$)} & "
        r"\textbf{L1 @ LJDC+SJDC} & "
        r"\textbf{L1 @ final} & "
        r"\textbf{L1+HLT @ LJDC+SJDC} & "
        r"\textbf{L1+HLT @ final} \\"
    )
    print(r"\hline")
    for label, f_l1_jdc, f_l1_fin, f_l1hlt_jdc, f_l1hlt_fin in rows:
        print(
            f"{label} & {_pct(f_l1_jdc)} & {_pct(f_l1_fin)} & "
            f"{_pct(f_l1hlt_jdc)} & {_pct(f_l1hlt_fin)} \\\\"
        )
    print(r"\hline")
    print(r"\end{tabular}")
    print(
        r"\caption{Fractional impact of L1 and HLT prescale weights on signal yields "
        r"(prescale-weighted yield / lumi$\times$xsec-weighted yield).}"
    )
    print(r"\end{table}")


# ──────────────────────────────────────────────────────────────────────────────
# Core cutflow function
# ──────────────────────────────────────────────────────────────────────────────

def run_cutflow(
    file_path,
    tree_name          = "Events",
    apply_llp_truth    = False,
    print_latex        = False,
    use_weights        = None,
    use_l1_prescale    = None,
    use_hlt_prescale   = None,
    dnn_inc_cut        = 0.97,
    dnn_depth_cut      = 0.95,
    dnn_inc_cut_sjdc   = None,
    dnn_depth_cut_sjdc = None,
):
    """
    Print a cutflow table for the displaced-jet search.

    Parameters
    ----------
    file_path          : str        – path to the input ROOT file
    tree_name          : str        – name of the TTree inside the file
    apply_llp_truth    : bool       – if True, prepend the LLP-HCAL truth requirement
                                      (one of jet0 / jet1 matched to an LLP with
                                       177 cm ≤ DecayR < 295 cm) to every cut level
    print_latex        : bool       – emit LaTeX table rows instead of plain text
    use_weights        : bool|None  – True  → always use the "weight" branch
                                      False → always use raw event counts
                                      None  → auto: weighted for signal, counts for data
    use_l1_prescale    : bool|None  – True  → multiply weight by "L1_prescale_weight"
                                      False → ignore L1_prescale_weight even if present
                                      None  → auto: apply if branch exists and use_weights
    use_hlt_prescale   : bool|None  – True  → multiply weight by "HLT_prescale_weight"
                                               (applied from the HLT row onward; L1 row
                                               still uses only weight * L1_prescale_weight)
                                      False → ignore HLT_prescale_weight even if present
                                      None  → auto: apply if branch exists and use_l1_prescale
    dnn_inc_cut        : float      – inclusive DNN threshold for LJDC (and SJDC if
                                      dnn_inc_cut_sjdc is not given); default 0.97
    dnn_depth_cut      : float      – depth DNN threshold for LJDC (and SJDC if
                                      dnn_depth_cut_sjdc is not given); default 0.95
    dnn_inc_cut_sjdc   : float|None – inclusive DNN threshold for SJDC; if None, uses
                                      dnn_inc_cut (same cut for both categories)
    dnn_depth_cut_sjdc : float|None – depth DNN threshold for SJDC; if None, uses
                                      dnn_depth_cut (same cut for both categories)
    """
    # Per-category cut values (fall back to shared value when SJDC override not given)
    _inc_ljdc   = dnn_inc_cut
    _inc_sjdc   = dnn_inc_cut_sjdc   if dnn_inc_cut_sjdc   is not None else dnn_inc_cut
    _depth_ljdc = dnn_depth_cut
    _depth_sjdc = dnn_depth_cut_sjdc if dnn_depth_cut_sjdc is not None else dnn_depth_cut

    # ── open file / tree ──────────────────────────────────────────────────────
    f    = ROOT.TFile.Open(file_path)
    tree = f.Get(tree_name)
    if not tree:
        raise RuntimeError(f"Tree '{tree_name}' not found in {file_path}")

    # ── category strings ─────────────────────────────────────────────────────
    # LJDC: jet0 is the depth-tagged candidate, jet1 is the inclusive candidate
    # SJDC: jet1 is the depth-tagged candidate, jet0 is the inclusive candidate
    LJDC = "(jet0_DepthTagCand == 1 && jet1_InclTagCand == 1)"
    SJDC = "(jet1_DepthTagCand == 1 && jet0_InclTagCand == 1)"
    JDC_OR = f"({LJDC} || {SJDC})"

    # DNN cuts applied per-category so the correct jet role and threshold are used
    # "inc" cut: applied to the inclusive-tagged jet
    dnn_inc = (
        f"(({SJDC} && jet0_scores_inc_train80 > {_inc_sjdc}) || "
        f" ({LJDC} && jet1_scores_inc_train80 > {_inc_ljdc}))"
    )
    # "depth" cut: applied to the depth-tagged jet
    dnn_depth = (
        f"(({SJDC} && jet1_scores_depth_LLPanywhere > {_depth_sjdc}) || "
        f" ({LJDC} && jet0_scores_depth_LLPanywhere > {_depth_ljdc}))"
    )

    # ── LLP truth cut ─────────────────────────────────────────────────────────
    # At least one of the two leading jets must be matched to an LLP that
    # decays inside the HCAL barrel depth (177 cm ≤ R < 295 cm).
    LLP_truth = (
        "((jet0_MatchedLLP_DecayR >= 177 && jet0_MatchedLLP_DecayR < 295) || "
        " (jet1_MatchedLLP_DecayR >= 177 && jet1_MatchedLLP_DecayR < 295))"
    )

    # ── parse filename for human/LaTeX labels ────────────────────────────────
    sample = parse_filename(file_path)
    truth_note = (
        "  |  LLP truth: jet matched to LLP with 177 ≤ R < 295 cm, |eta| < 1.26"
        if apply_llp_truth else ""
    )

    # ── ordered list of (label, incremental cut string) ──────────────────────
    # Each entry appends its cut to all previous ones.
    # MET filters are only applied to data, not LLP MC signal.
    steps = [
        ("All",                                 ""),
        # ("Trigger (L1)",                        "Pass_L1SingleLLPJet == 1"),
    #    ("Trigger (L1 DoubleLLPJet40)",         "L1_DoubleLLPJet40 == 1"),
       ("Trigger (L1, but not L1 DoubleLLPJet40)",         "L1_DoubleLLPJet40 == 0 && Pass_L1SingleLLPJet == 1"),
        # ("L1 trigger matched, $pT > 60$",       "((jet0_L1trig_Matched == 1 && jet0_Pt > 60 && abs(jet0_Eta) < 1.26 && jet1_Pt > 40 && abs(jet1_Eta) < 2.0) || (jet1_L1trig_Matched == 1 && jet1_Pt > 60 && abs(jet1_Eta) < 1.26 && jet0_Pt > 40 && abs(jet0_Eta) < 2.0))"),
        ("Trigger (HLT)",                       "Pass_HLTDisplacedJet == 1"),
        ("$pT>60, |\eta|<1.26; pT>40, |\eta|<2.0$", "((jet0_Pt > 60 && abs(jet0_Eta) < 1.26 && jet1_Pt > 40 && abs(jet1_Eta) < 2.0) || (jet1_Pt > 60 && abs(jet1_Eta) < 1.26 && jet0_Pt > 40 && abs(jet0_Eta) < 2.0))"),
        ("LJDC or SJDC",                        JDC_OR),
        ("$\Delta\phi$ (beam halo) veto",       "abs(jet0_jet1_dPhi) > 0.2"),
        (f"DNN $>{_inc_ljdc}$ / $>{_inc_sjdc}$ (inc)"     if _inc_ljdc != _inc_sjdc
         else f"DNN $>{_inc_ljdc}$ (inc)",        dnn_inc),
        (f"DNN $>{_depth_ljdc}$ / $>{_depth_sjdc}$ (depth)" if _depth_ljdc != _depth_sjdc
         else f"DNN $>{_depth_ljdc}$ (depth)",    dnn_depth),
    ]

    if sample["kind"] == "data":
        # Insert MET filters immediately after LJDC/SJDC (index 3)
        steps.insert(4, ("MET filters", "Flag_METFilters_2022_2023_PromptReco == 1"))

    # ── build cumulative selection strings ────────────────────────────────────
    cum_cuts = []
    running  = ""
    for _, inc in steps:
        if inc == "":
            running = ""
        elif running == "":
            running = inc
        else:
            running = f"{running} && {inc}"
        cum_cuts.append(running)

    # Optionally prepend the LLP truth requirement to every level
    if apply_llp_truth:
        cum_cuts = [
            LLP_truth if c == "" else f"{LLP_truth} && {c}"
            for c in cum_cuts
        ]

    # ── weighted sum helper (signal MC only) ────────────────────────────────
    # For signal MC the "weight" branch holds per-event generator/pileup weights.
    # ROOT's GetEntries() ignores weights, so we use a TH1 to accumulate the
    # weighted sum instead.  Data always uses unweighted counts.
    # use_weights: None → auto-detect from filename, otherwise honour caller's choice
    if use_weights is None:
        use_weights = (sample["kind"] == "signal")

    # Auto-detect L1 prescale weight: apply if branch exists and we are weighting
    has_l1_prescale_branch = bool(tree.GetBranch("L1_prescale_weight"))
    if use_l1_prescale is None:
        use_l1_prescale = use_weights and has_l1_prescale_branch
        if use_weights and not has_l1_prescale_branch:
            print("NOTE: 'L1_prescale_weight' branch not found; running without L1 prescale reweighting.")
    elif use_l1_prescale and not has_l1_prescale_branch:
        print("WARNING: use_l1_prescale=True but 'L1_prescale_weight' branch not found; ignoring.")
        use_l1_prescale = False

    # Auto-detect HLT prescale weight: apply if branch exists and L1 prescale is active
    has_hlt_prescale_branch = bool(tree.GetBranch("HLT_prescale_weight"))
    if use_hlt_prescale is None:
        use_hlt_prescale = use_l1_prescale and has_hlt_prescale_branch
        if use_l1_prescale and not has_hlt_prescale_branch:
            print("NOTE: 'HLT_prescale_weight' branch not found; running without HLT prescale reweighting.")
    elif use_hlt_prescale and not has_hlt_prescale_branch:
        print("WARNING: use_hlt_prescale=True but 'HLT_prescale_weight' branch not found; ignoring.")
        use_hlt_prescale = False

    # Build three weight expressions, each used at a different cut level:
    #   base_weight_expr : "All"  row – lumi×xsec only
    #   l1_weight_expr   : "L1"   row – adds L1 prescale factor
    #   weight_expr      : HLT+  rows – adds HLT prescale factor on top
    base_weight_expr = "weight" if use_weights else None

    if use_weights and use_l1_prescale:
        l1_weight_expr = "weight * L1_prescale_weight"
    elif use_weights:
        l1_weight_expr = "weight"
    elif use_l1_prescale:
        l1_weight_expr = "L1_prescale_weight"
    else:
        l1_weight_expr = None

    if l1_weight_expr and use_hlt_prescale:
        weight_expr = f"{l1_weight_expr} * HLT_prescale_weight"
    elif use_hlt_prescale:
        weight_expr = "HLT_prescale_weight"
    else:
        weight_expr = l1_weight_expr

    _hist_counter = [0]   # unique name counter to avoid ROOT name collisions

    def get_yield(selection, wexpr=None):
        """Return weighted yield for a selection, or raw count if weight_expr is None."""
        wexpr = wexpr if wexpr is not None else weight_expr
        if wexpr is None:
            return float(tree.GetEntries(selection) if selection else tree.GetEntries())
        _hist_counter[0] += 1
        hname = f"_cutflow_h{_hist_counter[0]}"
        draw_expr = f"{wexpr}>>{hname}(1,-1e6,1e6)"
        cut = f"({wexpr})*({selection})" if selection else wexpr
        tree.Draw(draw_expr, cut, "goff")
        h = ROOT.gDirectory.Get(hname)
        result = h.GetSumOfWeights() if h else 0.0
        if h:
            h.Delete()
        return result

    # ── header ────────────────────────────────────────────────────────────────
    if weight_expr is None:
        weight_note = ""
    elif use_hlt_prescale:
        weight_note = (
            f" [HLT+ rows: {weight_expr}; "
            f"L1 row: {l1_weight_expr}; "
            f"All row: {base_weight_expr}]"
        )
    else:
        weight_note = f" [weighted by: {weight_expr}; 'All' row by: {base_weight_expr}]"
    print("\n")
    print(f"Cutflow — {sample['plain']}{truth_note}{weight_note}")
    print("\n")

    caption = None
    if print_latex:
        caption = latex_setup(sample["latex"], apply_llp_truth, weight_expr)

    col_w = 30
    if not print_latex:
        n_col  = f"Yield ({weight_expr})" if weight_expr else "N events"
        header = (f"{'Selection':<{col_w}}  {n_col:>15}"
                  f"  {'% of All':>10}  {'% of HLT':>10}")
        print(header)
        print("─" * len(header))

    # ── loop over cut levels ──────────────────────────────────────────────────
    init     = -1.0   # denominator: all events
    hlt_init = -1.0   # denominator: events passing HLT trigger

    for i, (label, _) in enumerate(steps):
        sel   = cum_cuts[i]
        # Row-specific weights: All=base, L1=l1_weight_expr, HLT+=weight_expr (default)
        if i == 0:
            row_wexpr = base_weight_expr
        elif i == 1:
            row_wexpr = l1_weight_expr
        else:
            row_wexpr = None  # falls back to weight_expr inside get_yield
        n_evt = get_yield(sel, wexpr=row_wexpr)

        if i == 0:
            init = n_evt      # "All" row sets the primary denominator

        # Detect the HLT row by label and record its yield as second denominator
        if label == "Trigger (HLT)":
            hlt_init = n_evt

        frac_all = n_evt / init     if init     > 0 else 0.0
        frac_hlt = n_evt / hlt_init if hlt_init > 0 else float("nan")

        if print_latex:
            hlt_str = f"{100.0*frac_hlt:.2f}" if hlt_init > 0 else "--"
            fmt = ".2f" if weight_expr else ".0f"
            print(f"{label} & {n_evt:{fmt}} & {100.0*frac_all:.2f}\% & {hlt_str}\% \\\\")
        else:
            hlt_str = f"{100.0*frac_hlt:>10.2f}%" if hlt_init > 0 else f"{'--':>11}"
            fmt = ">15.2f" if weight_expr else ">15.0f"
            print(f"{label:<{col_w}}  {n_evt:{fmt}}"
                  f"  {100.0*frac_all:>9.2f}%{hlt_str}")
    # ── fraction of final analysis passing only one specific HLT ─────────────
    # Show how many final-analysis events fired
    # HLT_HT200_L1SingleLLPJet_DisplacedDijet35_Inclusive1PtrkShortSig5
    # but NO other HLT whose name contains "L1SingleLLPJet"
    # (passing HLT_L1SingleLLPJet itself is allowed).
    TARGET_HLT   = "HLT_HT200_L1SingleLLPJet_DisplacedDijet35_Inclusive1PtrkShortSig5"
    ALLOWED_PASS = {"HLT_L1SingleLLPJet"} # ideally this would be removed but the prescale isn't handled right...

    # Use GetListOfLeaves so individual HLT flag branches (which may be leaves
    # inside a parent branch) are found by their full path name.
    branch_names   = [b.GetName() for b in tree.GetListOfBranches()]
    leaf_names     = [l.GetName() for l in tree.GetListOfLeaves()]
    all_names      = set(branch_names) | set(leaf_names)

    other_llp_hlts = sorted(
        b for b in all_names
        if b.startswith("HLT_") and "L1SingleLLPJet" in b
        and b != TARGET_HLT and b not in ALLOWED_PASS
    )

    final_sel = cum_cuts[-1]
    n_final   = get_yield(final_sel)

    if TARGET_HLT in all_names:
        veto_str = " && ".join(f"{b} == 0" for b in other_llp_hlts)
        excl_sel = f"{TARGET_HLT} == 1" + (f" && {veto_str}" if veto_str else "")
        if final_sel:
            excl_sel = f"{final_sel} && {excl_sel}"

        n_excl     = get_yield(excl_sel)
        frac_all   = n_excl / init      if init     > 0 else 0.0
        frac_hlt   = n_excl / hlt_init  if hlt_init > 0 else float("nan")
        frac_final = n_excl / n_final   if n_final  > 0 else float("nan")

        tex_label  = (r"~~$\hookrightarrow$ only \texttt{HLT\_HT200\_L1SingleLLPJet"
                      r"\_DisplacedDijet35\_Inclusive1PtrkShortSig5}")
        plain_label = f"  only {TARGET_HLT}"

        if print_latex:
            fmt = ".2f" if weight_expr else ".0f"
            frac_hlt_str   = f"{100.0*frac_hlt:.2f}"  if hlt_init > 0 else "--"
            frac_final_str = f"{100.0*frac_final:.2f}" if n_final  > 0 else "--"
            print(r"\hline")
            # print(f"{tex_label} & {n_excl:{fmt}} & {100.0*frac_all:.2f}\\% & "
            #       f"{frac_hlt_str}\\% (HLT); {frac_final_str}\\% (final) \\\\")
        else:
            fmt = ">15.2f" if weight_expr else ">15.0f"
            hlt_str   = f"{100.0*frac_hlt:>9.2f}%"   if hlt_init > 0 else f"{'--':>10}"
            final_str = f"{100.0*frac_final:>9.2f}%"  if n_final  > 0 else f"{'--':>10}"
            print("─" * (col_w + 50))
            print(f"{plain_label:<{col_w}}  {n_excl:{fmt}}"
                  f"  {100.0*frac_all:>9.2f}%{hlt_str}  {final_str} of final")
    else:
        llp_found = sorted(b for b in all_names if "L1SingleLLPJet" in b)
        print(f"\nNOTE: branch/leaf '{TARGET_HLT}' not found in tree; skipping exclusive-HLT line.")
        if llp_found:
            print(f"      L1SingleLLPJet branches/leaves found: {llp_found}")
        else:
            print("      No branches/leaves containing 'L1SingleLLPJet' found at all.")

    if print_latex:
        latex_end(caption)

    f.Close()


# ──────────────────────────────────────────────────────────────────────────────
# Entry point
# ──────────────────────────────────────────────────────────────────────────────

def _parse_args():
    parser = argparse.ArgumentParser(
        description="Print a displaced-jet search cutflow table."
    )
    parser.add_argument(
        "--file", default=(
            "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.3/"
            "minituple_HToSSTo4B_125_50_CTau3000_scores.root"
        ),
        help="Input ROOT file.",
    )
    parser.add_argument(
        "--tree", default="NoSel",
        help="TTree name inside the file (default: NoSel).",
    )
    parser.add_argument(
        "--inc", type=float, default=0.97,
        help="Inclusive DNN score threshold for LJDC (default: 0.97).",
    )
    parser.add_argument(
        "--depth", type=float, default=0.95,
        help="Depth DNN score threshold for LJDC (default: 0.95).",
    )
    parser.add_argument(
        "--inc-sjdc", type=float, default=None,
        help="Inclusive DNN score threshold for SJDC; if omitted, uses --inc.",
    )
    parser.add_argument(
        "--depth-sjdc", type=float, default=None,
        help="Depth DNN score threshold for SJDC; if omitted, uses --depth.",
    )
    parser.add_argument(
        "--truth", action="store_true",
        help="Apply LLP truth matching.",
    )
    parser.add_argument(
        "--latex", action="store_true",
        help="Print LaTeX-formatted output.",
    )
    parser.add_argument(
        "--no-weights", dest="weights", action="store_false", default=None,
        help="Use raw event counts (disable generator/PU weights).",
    )
    parser.add_argument(
        "--no-l1-prescale", dest="l1_prescale", action="store_false", default=None,
        help="Disable L1 prescale reweighting.",
    )
    parser.add_argument(
        "--no-hlt-prescale", dest="hlt_prescale", action="store_false", default=None,
        help="Disable HLT prescale reweighting.",
    )
    parser.add_argument(
        "--prescale-table", nargs="+", metavar="FILE", default=None,
        help=(
            "Print a LaTeX prescale-impact summary table instead of a per-file cutflow. "
            "Accepts one or more ROOT files; each becomes one table row."
        ),
    )
    return parser.parse_args()


if __name__ == "__main__":

    args = _parse_args()

    if args.prescale_table:
        prescale_impact_table(
            args.prescale_table,
            tree_name          = args.tree,
            dnn_inc_cut        = args.inc,
            dnn_depth_cut      = args.depth,
            dnn_inc_cut_sjdc   = args.inc_sjdc,
            dnn_depth_cut_sjdc = args.depth_sjdc,
        )
    else:
        # --- full cutflow for a single file -----------------------------------
        run_cutflow(args.file, args.tree,
                apply_llp_truth    = args.truth,
                use_weights        = args.weights,
                use_l1_prescale    = args.l1_prescale,
                use_hlt_prescale   = args.hlt_prescale,
                print_latex        = args.latex,
                dnn_inc_cut        = args.inc,
                dnn_depth_cut      = args.depth,
                dnn_inc_cut_sjdc   = args.inc_sjdc,
                dnn_depth_cut_sjdc = args.depth_sjdc)
