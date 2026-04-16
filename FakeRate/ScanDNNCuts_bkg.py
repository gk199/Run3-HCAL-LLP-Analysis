"""
ScanDNNCuts_bkg.py
==================
Wrapper that runs MisTagParametrization_3D.py over a 2-D grid of
(inclusive, depth) DNN score thresholds for one or more eras and prints
summary tables of the predicted SR background yield for leading and
sub-leading jets at each cut point.

Parsing mirrors OutputToLatex_combined.py: jet type is identified by lines
equal to "leading", "sub-leading", or "combined" in the output text file.

Typical use
-----------
    python ScanDNNCuts_bkg.py -e 2022 2023
    python ScanDNNCuts_bkg.py -e 2022 -d 0.90 0.93 0.95 0.97 -i 0.95 0.97 0.99
    python ScanDNNCuts_bkg.py -e 2022 --dry-run      # print commands only
    python ScanDNNCuts_bkg.py -e 2022 --parse-only   # re-parse existing files
"""

import argparse
import re
import subprocess
import sys
import os
from collections import defaultdict

# ──────────────────────────────────────────────────────────────────────────────
# Defaults
# ──────────────────────────────────────────────────────────────────────────────
DEFAULT_ERAS       = ["2022", "2023"]
DEFAULT_DEPTH_CUTS = [0.90, 0.93, 0.95, 0.97]
DEFAULT_INC_CUTS   = [0.93, 0.95, 0.97, 0.98, 0.99]
MISTAG_SCRIPT      = os.path.join(os.path.dirname(__file__), "MisTagParametrization_3D_optimized.py")


# ──────────────────────────────────────────────────────────────────────────────
# Run MisTagParametrization_3D.py for one (era, depth, inc) point
# ──────────────────────────────────────────────────────────────────────────────

def run_mistag(era, depth_cut, inc_cut, dry_run=False):
    """
    Invoke MisTagParametrization_3D.py and return the output filename.
    Always runs with -b (combined b-tag categories).
    """
    cmd = [
        sys.executable, MISTAG_SCRIPT,
        "-e", era,
        "-d", str(depth_cut),
        "-i", str(inc_cut),
        "-b",
    ]

    out_file = _output_filename(era, depth_cut, inc_cut)

    if dry_run:
        print("DRY RUN:", " ".join(cmd))
        return out_file

    print(f"  Running: {' '.join(cmd)}")
    result = subprocess.run(cmd, capture_output=True, text=True)
    if result.returncode != 0:
        print(f"  WARNING: non-zero exit ({result.returncode}) for era={era} "
              f"depth={depth_cut} inc={inc_cut}")
        if result.stderr:
            print("  STDERR:", result.stderr[-500:])

    return out_file


def _output_filename(era, depth_cut, inc_cut):
    dnn_str     = str(depth_cut).replace("0.", "pt")
    dnn_inc_str = str(inc_cut).replace("0.", "pt")
    era_name    = era.replace(" ", "")
    return f"DNN_{dnn_str}_inc{dnn_inc_str}_{era_name}_forPython_combined.txt"


# ──────────────────────────────────────────────────────────────────────────────
# Parse predicted SR yield from the output text file
# Mirrors the parsing logic in OutputToLatex_combined.py
# ──────────────────────────────────────────────────────────────────────────────

_NUM_RE = re.compile(r"([\d.]+)\s*±\s*([\d.]+)")

def parse_sr_yields(out_file):
    """
    Parse the output text file and return a dict:
        { jet_type: (predicted_SR_value, predicted_SR_error) }
    where jet_type is one of 'leading', 'sub-leading', 'combined'.
    Uses only the central (non-PV-split) block for each jet type.
    """
    if not os.path.exists(out_file):
        return {}

    # Track state the same way OutputToLatex_combined.py does
    data = defaultdict(lambda: {'central': {}, 'low': {}, 'high': {}})
    current_jet  = None
    current_mode = 'central'

    with open(out_file) as fh:
        for line in fh:
            stripped   = line.strip()
            line_lower = stripped.lower()

            # Detect PV mode
            if "low pv" in line_lower:
                current_mode = 'low'
            elif "high pv" in line_lower:
                current_mode = 'high'
            elif "depth" in line_lower and "pv" not in line_lower:
                current_mode = 'central'

            # Detect jet category — exact match like OutputToLatex_combined.py
            if line_lower in {"leading", "sub-leading", "combined"}:
                current_jet = line_lower

            # Extract SR prediction
            if current_jet and "predicted mistagged events in sr" in line_lower:
                m = _NUM_RE.search(stripped)
                if m:
                    val, err = float(m.group(1)), float(m.group(2))
                    data[current_jet][current_mode]['Predicted SR'] = (val, err)

    # Return central values for each jet type
    result = {}
    for jet in ('leading', 'sub-leading', 'combined'):
        entry = data[jet]['central'].get('Predicted SR')
        if entry:
            result[jet] = entry
    return result


# ──────────────────────────────────────────────────────────────────────────────
# Summary tables — one grid per jet type
# ──────────────────────────────────────────────────────────────────────────────

def print_summary_tables(era, depth_cuts, inc_cuts, results):
    """
    results: dict[(depth, inc)] = { jet_type: (val, err) }
    Prints one 2-D table per jet type (leading, sub-leading).
    Rows = inclusive cut, columns = depth cut, cells = yield ± stat.
    """
    col_w       = 18
    inc_label   = "inc \\ depth"

    # combined = leading + sub-leading histograms added; slightly overcounts
    # events where both jets pass the depth tag (LJDC AND SJDC overlap).
    # Use combined for the total SR background estimate.
    for jet in ('leading', 'sub-leading', 'combined'):
        print(f"\n{'='*66}")
        print(f"Predicted SR yield — era {era} — {jet} jet")
        if jet == 'combined':
            print("  (leading + sub-leading added; small overlap where both jets are depth-tagged)")
        print(f"{'='*66}")
        print("Cells: yield ± stat  (rows = inc cut, columns = depth cut)\n")

        header = f"  {inc_label:>12}" + "".join(f"  {d:>{col_w}.2f}" for d in depth_cuts)
        print(header)
        print("─" * len(header))

        for inc in inc_cuts:
            row = f"  {inc:>12.2f}"
            for depth in depth_cuts:
                entry = results.get((depth, inc), {}).get(jet)
                cell  = f"{entry[0]:.2f}±{entry[1]:.2f}" if entry else "N/A"
                row  += f"  {cell:>{col_w}}"
            print(row)

        print("─" * len(header))


# ──────────────────────────────────────────────────────────────────────────────
# CLI
# ──────────────────────────────────────────────────────────────────────────────

def main():
    parser = argparse.ArgumentParser(
        description="Scan inclusive and depth DNN cuts for background prediction."
    )
    parser.add_argument("-e", "--eras", nargs="+", default=DEFAULT_ERAS,
                        metavar="ERA", help="Era(s) to process (e.g. 2022 2023).")
    parser.add_argument("-d", "--depth", nargs="+", type=float, default=DEFAULT_DEPTH_CUTS,
                        metavar="SCORE", help="Depth DNN thresholds to scan.")
    parser.add_argument("-i", "--inc", nargs="+", type=float, default=DEFAULT_INC_CUTS,
                        metavar="SCORE", help="Inclusive DNN thresholds to scan.")
    # Always runs with -b (combined b-tag categories), matching standard usage.
    parser.add_argument("--dry-run", action="store_true",
                        help="Print commands without running them.")
    parser.add_argument("--parse-only", action="store_true",
                        help="Skip running MisTagParametrization_3D.py and just "
                             "parse any existing output text files.")
    args = parser.parse_args()

    depth_cuts = sorted(args.depth)
    inc_cuts   = sorted(args.inc)

    for era in args.eras:
        print(f"\nProcessing era: {era}")
        results = {}

        for depth in depth_cuts:
            for inc in inc_cuts:
                if not args.parse_only:
                    out_file = run_mistag(era, depth, inc, args.dry_run)
                else:
                    out_file = _output_filename(era, depth, inc)

                results[(depth, inc)] = parse_sr_yields(out_file)

        if not args.dry_run:
            print_summary_tables(era, depth_cuts, inc_cuts, results)


if __name__ == "__main__":
    main()
