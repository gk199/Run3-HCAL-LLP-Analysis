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
        r"HToSSTo4B_(?P<mH>\d+)_(?P<mS>\d+)_CTau(?P<ctau>\d+)",
        stem, re.IGNORECASE
    )
    if m:
        mH, mS, ctau = m.group("mH"), m.group("mS"), m.group("ctau")
        return dict(
            kind  = "signal",
            plain = f"H→SS→4b  mH={mH} GeV  mS={mS} GeV  cτ={ctau} mm",
            latex = (
                r"$H \to SS \to 4b$, "
                rf"$m_{{H}}={mH}$~GeV, $m_{{S}}={mS}$~GeV, "
                rf"$c\tau={ctau}$~mm"
            ),
            short = f"HToSS_mH{mH}_mS{mS}_CTau{ctau}",
        )

    # ── data ──────────────────────────────────────────────────────────────────
    m = re.search(r"data_(?P<era>[A-Za-z0-9]+)", stem)
    if m:
        era = m.group("era")
        return dict(
            kind  = "data",
            plain = f"Data  era={era}",
            latex = rf"Data, era {era}",
            short = f"data_{era}",
        )

    # ── fallback ──────────────────────────────────────────────────────────────
    return dict(
        kind  = "unknown",
        plain = stem,
        latex = stem.replace("_", r"\_"),
        short = stem[:30],
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
        "weight":                       "cross-section and luminosity weighted",
        "L1_prescale_weight":           "L1 prescale weighted",
        "weight * L1_prescale_weight":  "cross-section, luminosity, and L1 prescale weighted",
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
# Core cutflow function
# ──────────────────────────────────────────────────────────────────────────────

def run_cutflow(
    file_path,
    tree_name         = "Events",
    apply_llp_truth   = False,
    print_latex       = False,
    use_weights       = None,
    use_l1_prescale   = None,
    dnn_inc_cut       = 0.97,
    dnn_depth_cut     = 0.95,
    dnn_inc_cut_sjdc  = None,
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
#        ("Trigger (L1)",                        "Pass_L1SingleLLPJet == 1"),
#        ("Trigger (L1 DoubleLLPJet40)",         "L1_DoubleLLPJet40 == 1"),
        ("Trigger (L1, but not L1 DoubleLLPJet40)",         "L1_DoubleLLPJet40 == 0 && Pass_L1SingleLLPJet == 1"),
        ("Trigger (HLT)",                       "Pass_HLTDisplacedJet == 1"),
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

    # Build the effective weight expression for the four modes:
    #   use_weights=F, use_l1_prescale=F  →  None  (raw counts via GetEntries)
    #   use_weights=T, use_l1_prescale=F  →  "weight"
    #   use_weights=F, use_l1_prescale=T  →  "L1_prescale_weight"
    #   use_weights=T, use_l1_prescale=T  →  "weight * L1_prescale_weight"
    if use_weights and use_l1_prescale:
        weight_expr = "weight * L1_prescale_weight"
    elif use_weights:
        weight_expr = "weight"
    elif use_l1_prescale:
        weight_expr = "L1_prescale_weight"
    else:
        weight_expr = None

    _hist_counter = [0]   # unique name counter to avoid ROOT name collisions

    def get_yield(selection):
        """Return weighted yield for a selection, or raw count if weight_expr is None."""
        if weight_expr is None:
            return float(tree.GetEntries(selection) if selection else tree.GetEntries())
        _hist_counter[0] += 1
        hname = f"_cutflow_h{_hist_counter[0]}"
        draw_expr = f"{weight_expr}>>{hname}(1,-1e6,1e6)"
        cut = f"({weight_expr})*({selection})" if selection else weight_expr
        tree.Draw(draw_expr, cut, "goff")
        h = ROOT.gDirectory.Get(hname)
        result = h.GetSumOfWeights() if h else 0.0
        if h:
            h.Delete()
        return result

    # ── header ────────────────────────────────────────────────────────────────
    if weight_expr is None:
        weight_note = ""
    else:
        weight_note = f" [weighted by: {weight_expr}]"
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
        n_evt = get_yield(sel)

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
    return parser.parse_args()


if __name__ == "__main__":

    args = _parse_args()

    # --- L1 prescale weighted counts (no gen/PU weight) -----------------------
    run_cutflow(args.file, args.tree,
                apply_llp_truth = args.truth,
                use_weights     = args.weights,
                use_l1_prescale = args.l1_prescale,
                print_latex     = args.latex,
                dnn_inc_cut     = args.inc,
                dnn_depth_cut   = args.depth)
