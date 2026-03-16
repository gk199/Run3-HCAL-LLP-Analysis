import re
import os
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

def latex_setup(sample_latex, apply_llp_truth):
    truth_note = (
        r", with LLP truth matching ($177 \leq R_{\text{decay}} < 295$~cm, \abs{\eta_{LLP}} < 1.26)"
        if apply_llp_truth else ""
    )
    caption = rf"Cutflow for {sample_latex}{truth_note}."
    print(r"\begin{table}[h!]")
    print(r"\centering")
    print(r"\begin{tabular}{lrr}")
    print(r"\hline")
    print(r"\textbf{Selection} & \textbf{Events} & \textbf{Fraction [\%]} \\")
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
    tree_name    = "Events",
    apply_llp_truth = False,
    print_latex  = False,
):
    """
    Print a cutflow table for the displaced-jet search.

    Parameters
    ----------
    file_path       : str   – path to the input ROOT file
    tree_name       : str   – name of the TTree inside the file
    apply_llp_truth : bool  – if True, prepend the LLP-HCAL truth requirement
                              (one of jet0 / jet1 matched to an LLP with
                               177 cm ≤ DecayR < 295 cm) to every cut level
    print_latex     : bool  – emit LaTeX table rows instead of plain text
    """

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

    # DNN cuts applied per-category so the correct jet role is used
    # "inc" cut (> 0.97): applied to the inclusive-tagged jet
    dnn_inc = (
        f"(({SJDC} && jet0_scores_inc_train80 > 0.97) || "
        f" ({LJDC} && jet1_scores_inc_train80 > 0.97))"
    )
    # "depth" cut (> 0.95): applied to the depth-tagged jet
    dnn_depth = (
        f"(({SJDC} && jet1_scores_depth_LLPanywhere > 0.95) || "
        f" ({LJDC} && jet0_scores_depth_LLPanywhere > 0.95))"
    )

    # ── LLP truth cut ─────────────────────────────────────────────────────────
    # At least one of the two leading jets must be matched to an LLP that
    # decays inside the HCAL barrel depth (177 cm ≤ R < 295 cm).
    LLP_truth = (
        "((jet0_MatchedLLP_DecayR >= 177 && jet0_MatchedLLP_DecayR < 295 && abs(jet0_MatchedLLP_Eta) < 1.26) || "
        " (jet1_MatchedLLP_DecayR >= 177 && jet1_MatchedLLP_DecayR < 295 && abs(jet1_MatchedLLP_Eta) < 1.26))"
    )

    # ── parse filename for human/LaTeX labels ────────────────────────────────
    sample = parse_filename(file_path)
    truth_note = (
        "  |  LLP truth: jet matched to LLP with 177 ≤ R < 295 cm and |η| < 1.26"
        if apply_llp_truth else ""
    )

    # ── ordered list of (label, incremental cut string) ──────────────────────
    # Each entry appends its cut to all previous ones.
    # MET filters are only applied to data, not LLP MC signal.
    steps = [
        ("All",                             ""),
        ("Trigger (L1)",                    "Pass_L1SingleLLPJet == 1"),
        ("Trigger (HLT)",                   "Pass_HLTDisplacedJet == 1"),
        ("LJDC or SJDC",                    JDC_OR),
        ("$\Delta\phi$ (beam halo) veto",   "abs(jet0_jet1_dPhi) > 0.2"),
        ("DNN $>0.97$ (inc)",               dnn_inc),
        ("DNN $>0.95$ (depth)",             dnn_depth),
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

    # ── header ────────────────────────────────────────────────────────────────
    print("\n")
    print(f"Cutflow — {sample['plain']}{truth_note}")
    print("\n")

    caption = None
    if print_latex:
        caption = latex_setup(sample["latex"], apply_llp_truth)

    col_w = 30
    if not print_latex:
        header = f"{'Selection':<{col_w}}  {'N events':>12}  {'Fraction [%]':>13}"
        print(header)
        print("─" * len(header))

    # ── loop over cut levels ──────────────────────────────────────────────────
    init = -1

    for i, (label, _) in enumerate(steps):
        sel   = cum_cuts[i]
        n_evt = tree.GetEntries(sel) if sel != "" else tree.GetEntries()

        if i == 0:
            init = n_evt           # denominator is always the first row

        frac = n_evt / init if init > 0 else 0.0

        if print_latex:
            # strip LaTeX math markers for the plain-label check; keep as-is for output
            print(f"{label} & {n_evt} & {100.0*frac:.2f}\\% \\\\")
        else:
            print(f"{label:<{col_w}}  {n_evt:>12}  {100.0*frac:>12.2f}%")

    if print_latex:
        latex_end(caption)

    f.Close()


# ──────────────────────────────────────────────────────────────────────────────
# Entry point – edit file_path and tree_name as needed
# ──────────────────────────────────────────────────────────────────────────────

if __name__ == "__main__":

    file_path = "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.1/minituple_HToSSTo4B_125_50_CTau3000_scores.root"   # ← set your input file here
    tree_name = "NoSel"           # ← set your tree name here

    # --- plain-text cutflow, no truth matching --------------------------------
    run_cutflow(file_path, tree_name,
                apply_llp_truth=False,
                print_latex=True)

    # --- plain-text cutflow, with LLP HCAL truth matching --------------------
    run_cutflow(file_path, tree_name,
                apply_llp_truth=True,
                print_latex=True)

    # --- LaTeX cutflow, no truth matching -------------------------------------
    # run_cutflow(file_path, tree_name,
    #             apply_llp_truth=False,
    #             print_latex=True)