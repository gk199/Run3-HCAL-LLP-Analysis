"""
ScanDNNCuts.py
==============
Wrapper that calls run_cutflow() over a 2-D grid of (inclusive, depth) DNN
score thresholds and prints a summary table of the final yield at each point.

Typical use
-----------
    python ScanDNNCuts.py
    python ScanDNNCuts.py --file my_file.root --tree NoSel
    python ScanDNNCuts.py --inc  0.90 0.95 0.97 0.99 --depth 0.90 0.93 0.95 0.97
    python ScanDNNCuts.py --truth          # add LLP truth matching
"""

import argparse
import sys
import os
import ROOT

sys.path.insert(0, os.path.dirname(__file__))
from FinalAnalysisCutflow import run_cutflow, parse_filename

# ──────────────────────────────────────────────────────────────────────────────
# Defaults
# ──────────────────────────────────────────────────────────────────────────────

DEFAULT_FILE  = (
    "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.3/"
    "minituple_HToSSTo4B_125_50_CTau3000_scores.root"
)
DEFAULT_TREE  = "NoSel"
DEFAULT_INC   = [0.90, 0.93, 0.95, 0.97, 0.99]
DEFAULT_DEPTH = [0.90, 0.93, 0.95, 0.97, 0.99]


# ──────────────────────────────────────────────────────────────────────────────
# Yield extractor – runs run_cutflow with stdout captured
# ──────────────────────────────────────────────────────────────────────────────

def _get_final_yield(file_path, tree_name, inc_cut, depth_cut,
                     apply_llp_truth, use_weights, use_l1_prescale):
    """Return (final_yield, hlt_yield, all_yield) for given cut values."""
    import io
    from contextlib import redirect_stdout

    captured = io.StringIO()
    with redirect_stdout(captured):
        run_cutflow(
            file_path, tree_name,
            apply_llp_truth = apply_llp_truth,
            print_latex     = False,
            use_weights     = use_weights,
            use_l1_prescale = use_l1_prescale,
            dnn_inc_cut     = inc_cut,
            dnn_depth_cut   = depth_cut,
        )

    lines = [l for l in captured.getvalue().splitlines() if l.strip()]
    # The last data row is the final cut level
    # Format: "<label>   <yield>   <% all>   <% hlt>"
    all_yield   = None
    hlt_yield   = None
    final_yield = None
    for line in lines:
        parts = line.split()
        if not parts:
            continue
        # Try to parse the numeric yield (second-to-last field before percentages)
        try:
            # Find the yield column: last float-like token before the first '%'
            tokens = line.replace('%', '').split()
            floats = []
            for t in tokens:
                try:
                    floats.append(float(t))
                except ValueError:
                    pass
            if len(floats) >= 1:
                final_yield = floats[0]   # overwritten each row → ends up as last row
            if "All" in line and len(floats) >= 1:
                all_yield = floats[0]
            if "Trigger (HLT)" in line and len(floats) >= 1:
                hlt_yield = floats[0]
        except Exception:
            pass

    return final_yield, hlt_yield, all_yield


# ──────────────────────────────────────────────────────────────────────────────
# Summary table
# ──────────────────────────────────────────────────────────────────────────────

def print_scan_table(file_path, tree_name, inc_cuts, depth_cuts,
                     apply_llp_truth=False, use_weights=None, use_l1_prescale=None):
    sample = parse_filename(file_path)
    truth_tag = "  [LLP truth]" if apply_llp_truth else ""
    print(f"\nDNN cut scan — {sample['plain']}{truth_tag}")
    print("Cells show yield after all cuts (inc rows, depth columns)\n")

    col_w = 10
    inc_depth_label = "inc \\ depth"
    header = f"  {inc_depth_label:>12}" + "".join(f"  {d:>{col_w}.2f}" for d in depth_cuts)
    print(header)
    print("─" * len(header))

    for inc in inc_cuts:
        row = f"  {inc:>12.2f}"
        for depth in depth_cuts:
            final, _, _ = _get_final_yield(
                file_path, tree_name, inc, depth,
                apply_llp_truth, use_weights, use_l1_prescale,
            )
            val = final if final is not None else float("nan")
            row += f"  {val:>{col_w}.2f}"
        print(row)

    print("─" * len(header))


# ──────────────────────────────────────────────────────────────────────────────
# CLI
# ──────────────────────────────────────────────────────────────────────────────

def main():
    parser = argparse.ArgumentParser(
        description="Scan inclusive and depth DNN score thresholds."
    )
    parser.add_argument("--file",  default=DEFAULT_FILE, help="Input ROOT file.")
    parser.add_argument("--tree",  default=DEFAULT_TREE, help="TTree name.")
    parser.add_argument("--inc",   nargs="+", type=float, default=DEFAULT_INC,
                        metavar="SCORE", help="Inclusive DNN thresholds to scan.")
    parser.add_argument("--depth", nargs="+", type=float, default=DEFAULT_DEPTH,
                        metavar="SCORE", help="Depth DNN thresholds to scan.")
    parser.add_argument("--truth", action="store_true",
                        help="Apply LLP truth matching.")
    args = parser.parse_args()

    print_scan_table(
        args.file, args.tree,
        inc_cuts        = sorted(args.inc),
        depth_cuts      = sorted(args.depth),
        apply_llp_truth = args.truth,
    )


if __name__ == "__main__":
    main()
