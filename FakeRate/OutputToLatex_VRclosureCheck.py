import re
import os
from collections import defaultdict
import argparse

# ---------------------------------------------------------------------------
# Filename construction — mirrors MisTagParametrization_3D_optimized.py
# ---------------------------------------------------------------------------

def _pt(v):
    """Convert a score float/string to the filename tag, e.g. 0.93 -> 'pt93'."""
    return str(v).replace("0.", "pt")


def _make_filename(depth_ljdc, inc_ljdc, depth_sjdc, inc_sjdc, era, btag_str):
    """
    Reconstruct the output filename produced by MisTagParametrization_3D_optimized.py.
    Symmetric case (LJDC == SJDC cuts) uses the compact format; otherwise the
    explicit LJDC/SJDC format is used.
    """
    if float(depth_ljdc) == float(depth_sjdc) and float(inc_ljdc) == float(inc_sjdc):
        return f"DNN_{_pt(depth_ljdc)}_inc{_pt(inc_ljdc)}_{era}_forPython{btag_str}.txt"
    else:
        return (f"DNN_LJDC{_pt(depth_ljdc)}_SJDC{_pt(depth_sjdc)}"
                f"_incLJDC{_pt(inc_ljdc)}_incSJDC{_pt(inc_sjdc)}"
                f"_{era}_forPython{btag_str}.txt")


# ---------------------------------------------------------------------------
# Config file parsing — same format as MisTagParametrization_3D_optimized.py
# ---------------------------------------------------------------------------

def parse_config_file(path):
    """
    Parse a whitespace-delimited config file.  Lines starting with # are
    comments.  The first non-comment line is the header defining column order.
    All subsequent non-empty lines are data rows.

    Required columns: era, DNN_cut_LJDC, DNN_cut_inc_LJDC, DNN_cut_SJDC, DNN_cut_inc_SJDC
    Optional column:  CR_cut_inc  (not used here but accepted for compatibility)

    Example
    -------
    # era   DNN_cut_LJDC  DNN_cut_inc_LJDC  DNN_cut_SJDC  DNN_cut_inc_SJDC
    2022    0.93          0.97              0.93           0.97
    2023    0.93          0.97              0.95           0.95
    """
    required = ["era", "DNN_cut_LJDC", "DNN_cut_inc_LJDC", "DNN_cut_SJDC", "DNN_cut_inc_SJDC"]

    rows = []
    header = None
    with open(path) as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith("#"):
                continue
            if header is None:
                header = line.split()
                continue
            values = line.split()
            if len(values) != len(header):
                raise ValueError(
                    f"Config row has {len(values)} columns, expected {len(header)}: {line!r}")
            row = dict(zip(header, values))
            for col in required:
                if col not in row:
                    raise ValueError(f"Config file is missing required column: {col}")
            row["era"] = str(row["era"])
            for col in ["DNN_cut_LJDC", "DNN_cut_inc_LJDC", "DNN_cut_SJDC", "DNN_cut_inc_SJDC"]:
                row[col] = float(row[col])
            rows.append(row)

    if not rows:
        raise ValueError(f"Config file {path!r} contains no data rows.")
    return rows


# ---------------------------------------------------------------------------
# Output file parsing
# ---------------------------------------------------------------------------

def parse_file(filename):
    with open(filename, 'r') as f:
        lines = [line.strip() for line in f if line.strip()]

    data = defaultdict(lambda: {'central': {}, 'low': {}, 'high': {}})

    current_tag = None
    current_jet = None
    current_mode = 'central'

    for line in lines:
        line_lower = line.lower()

        # --- Detect PV mode from context
        if "low pv" in line_lower:
            current_mode = 'low'
        elif "high pv" in line_lower:
            current_mode = 'high'
        elif "depth" in line_lower and "pv" not in line_lower:
            current_mode = 'central'

        # --- Detect jet category
        if line_lower in {"leading", "sub-leading", "combined"}:
            current_jet = line_lower

        # --- Parse actual data line
        if "predicted mistagged events in vr" in line_lower:
            match = re.search(r"([\d.]+)\s*±\s*([\d.]+)", line)
            if match:
                val, err = map(float, match.groups())
                key = current_jet
                data[key][current_mode]['Predicted VR'] = (val, err)

        elif "predicted mistagged events in sr" in line_lower:
            match = re.search(r"([\d.]+)\s*±\s*([\d.]+)", line)
            if match:
                val, err = map(float, match.groups())
                key = current_jet
                data[key][current_mode]['Predicted SR'] = (val, err)

        elif "observed mistagged events in vr" in line_lower:
            match = re.search(r"([\d.]+)\s*±\s*([\d.]+)", line)
            if match:
                val, err = map(float, match.groups())
                key = current_jet
                data[key]['central']['Observed VR'] = (val, err)

    return data


# ---------------------------------------------------------------------------
# Formatting helpers
# ---------------------------------------------------------------------------

def compute_syst(central, low, high):
    if central is None:
        return 0.0, 0.0

    c = central[0]
    l = low[0] if low is not None else c
    h = high[0] if high is not None else c

    diff_low = l - c
    diff_high = h - c

    syst_up = max(diff_low, diff_high, 0.0)
    syst_down = max(-diff_low, -diff_high, 0.0)

    return syst_up, syst_down


def format_predicted(val, stat, syst_up, syst_down):
    return f"{val:.2f} $\\pm$ {stat:.2f} (stat) + $^{{+{syst_up:.2f}}}_{{-{syst_down:.2f}}}$ (syst)"


def format_observed(val, stat):
    return f"{val:.2f} $\\pm$ {stat:.2f} (stat)"


def compute_pull(obs, pred, syst_up, syst_down, stat_only=False):
    """
    Compute the pull: (obs - pred) / sigma_total.

    If stat_only=False (default):
        sigma_total = sqrt(obs_stat^2 + pred_stat^2 + pred_syst^2)
        where pred_syst is the asymmetric syst bound facing the discrepancy.

    If stat_only=True:
        sigma_total = sqrt(obs_stat^2 + pred_stat^2)
        (systematic uncertainty on the predicted is ignored)

    Returns the signed pull value.
    """
    obs_val,  obs_stat  = obs
    pred_val, pred_stat = pred
    diff = obs_val - pred_val
    if stat_only:
        sigma_total = (obs_stat**2 + pred_stat**2)**0.5
    else:
        # Use the syst bound that opposes the discrepancy (conservative)
        pred_syst = syst_up if diff > 0 else syst_down
        sigma_total = (obs_stat**2 + pred_stat**2 + pred_syst**2)**0.5
    return diff / sigma_total if sigma_total > 0 else 0.0


# ---------------------------------------------------------------------------
# LaTeX table generation
# ---------------------------------------------------------------------------

def generate_latex_table(data, year, depth_score, inclusive_score, stat_only=False,
                          depth_score_sjdc=None, inclusive_score_sjdc=None):
    """
    Generate a LaTeX table for VR closure.

    depth_score / inclusive_score  — LJDC (leading jet depth candidate) cuts
    depth_score_sjdc / inclusive_score_sjdc — SJDC cuts; if None, assumed same as LJDC
    """
    # Normalise: accept "0.93" or "93"
    def _label(s):
        s = str(s)
        return "0." + s if not s.startswith("0.") else s

    def _tag(s):
        s = str(s)
        return s.replace("0.", "") if s.startswith("0.") else s

    depth_label = _label(depth_score)
    inc_label   = _label(inclusive_score)
    depth_tag   = _tag(depth_score)
    inc_tag     = _tag(inclusive_score)

    symmetric = (depth_score_sjdc is None and inclusive_score_sjdc is None) or \
                (float(_label(depth_score_sjdc or depth_score)) == float(depth_label) and
                 float(_label(inclusive_score_sjdc or inclusive_score)) == float(inc_label))

    if symmetric:
        score_label = f"depth = {depth_label}, inclusive = {inc_label}"
        caption_scores = f"Depth DNN score = {depth_label}, inclusive DNN score = {inc_label}"
        label_tag = f"pt{depth_tag}_incpt{inc_tag}"
    else:
        # Either depth or inc (but not both) may be None when only one of the
        # two cuts differs between LJDC and SJDC — fall back to the LJDC value.
        _d_sjdc = depth_score_sjdc if depth_score_sjdc is not None else depth_score
        _i_sjdc = inclusive_score_sjdc if inclusive_score_sjdc is not None else inclusive_score
        depth_label_sjdc = _label(_d_sjdc)
        inc_label_sjdc   = _label(_i_sjdc)
        depth_tag_sjdc   = _tag(_d_sjdc)
        inc_tag_sjdc     = _tag(_i_sjdc)
        score_label = (f"LJDC: depth={depth_label}, inc={inc_label}; "
                       f"SJDC: depth={depth_label_sjdc}, inc={inc_label_sjdc}")
        caption_scores = (f"LJDC depth DNN = {depth_label}, inclusive = {inc_label}; "
                          f"SJDC depth DNN = {depth_label_sjdc}, inclusive = {inc_label_sjdc}")
        label_tag = (f"LJDC_pt{depth_tag}_incpt{inc_tag}"
                     f"_SJDC_pt{depth_tag_sjdc}_incpt{inc_tag_sjdc}")

    def entry(jet, key):
        if key == 'Observed VR':
            val, stat = data[jet]['central'][key]
            return format_observed(val, stat)
        else:
            val, stat = data[jet]['central'][key]
            low = data[jet]['low'].get(key, (val, stat))
            high = data[jet]['high'].get(key, (val, stat))
            syst_up, syst_down = compute_syst((val, stat), low, high)
            return format_predicted(val, stat, syst_up, syst_down)

    lines = []
    # lines.append("% Requires in preamble: \\usepackage{multirow, colortbl}")
    # lines.append("% and \\definecolor{lightred}{rgb}{1.0, 0.8, 0.8}")
    lines.append("\\begin{table}[h!]")
    lines.append("    \\centering")
    lines.append("    \\begin{tabular}{c|c|c}")
    lines.append("        \\multicolumn{2}{c|}{\\textbf{Categories}} & \\textbf{Mistag Events} \\\\ \\hline")

    disagreements = []
    for jet in ['leading', 'sub-leading']:
        obs  = data[jet]['central'].get('Observed VR')
        pred = data[jet]['central'].get('Predicted VR')
        if obs and pred:
            low  = data[jet]['low'].get('Predicted VR', pred)
            high = data[jet]['high'].get('Predicted VR', pred)
            syst_up, syst_down = compute_syst(pred, low, high)
            pull = compute_pull(obs, pred, syst_up, syst_down, stat_only=stat_only)
            compatible = abs(pull) <= 1.0
            if not compatible:
                disagreements.append((jet, pull))
        else:
            compatible = True
        highlight = "\\cellcolor{lightred}" if not compatible else ""

        lines.append(f"        \\multirow{{3}}{{*}}{{{jet.capitalize()} Jet}} & {highlight}Observed VR  & {highlight}{entry(jet, 'Observed VR')} \\\\")
        lines.append(f"        & {highlight}Predicted VR & {highlight}{entry(jet, 'Predicted VR')} \\\\")
        lines.append(f"        & Predicted SR & {entry(jet, 'Predicted SR')} \\\\ \\hline")

    year_text = year.replace("_", " ")
    lines.append("    \\end{tabular}")

    caption = f"{caption_scores} for {year_text}."
    if disagreements:
        parts = []
        for jet, pull in disagreements:
            parts.append(f"disagreement in high-scoring VR for the {jet} jet is {abs(pull):.2f}$\\sigma$")
        sigma_formula = \
            "$\\sigma = |\\text{obs} - \\text{pred}| \\,/\\, \\sqrt{\\sigma^2_{\\text{obs,stat}} + \\sigma^2_{\\text{pred,stat}}}$" \
            if stat_only else \
            "$\\sigma = |\\text{obs} - \\text{pred}| \\,/\\, \\sqrt{\\sigma^2_{\\text{obs,stat}} + \\sigma^2_{\\text{pred,stat}} + \\sigma^2_{\\text{pred,syst}}}$"
        caption += " " + "; ".join(p.capitalize() for p in parts) + \
                   f", where {sigma_formula}."

    lines.append("    \\caption{" + caption + "}")
    lines.append("    \\label{Table:VRclosure_" + label_tag + "_" + year + "_combined}")
    lines.append("\\end{table}")
    return "\n".join(lines)


# ---------------------------------------------------------------------------
# Argument parsing
# ---------------------------------------------------------------------------

def parseArgs():
    parser = argparse.ArgumentParser(
        add_help=True,
        description="Generate LaTeX VR-closure tables from MisTagParametrization_3D_optimized output."
    )

    # --- Config file mode ---
    parser.add_argument("--config", action="store", default=None,
                        help="Config file listing parameter sets to process (same format as the "
                             "scan config used by MisTagParametrization_3D_optimized.py). "
                             "When provided, -e/-d/-i are ignored.")

    # --- Single-file mode ---
    parser.add_argument("-e", "--era",             action="store", default=None,
                        help="era (2022, 2023) — required without --config")
    parser.add_argument("-d", "--depth_score",     action="store", default="0.93",
                        help="Depth DNN score cut for LJDC, e.g. 0.93 or 93 (default: 0.93)")
    parser.add_argument("-i", "--inclusive_score", action="store", default="0.95",
                        help="Inclusive DNN score VR/SR boundary for LJDC, e.g. 0.95 (default: 0.95)")
    parser.add_argument("--depth_score_sjdc",      action="store", default=None,
                        help="Depth DNN score cut for SJDC (default: same as -d)")
    parser.add_argument("--inclusive_score_sjdc",  action="store", default=None,
                        help="Inclusive DNN score boundary for SJDC (default: same as -i)")

    # --- Always applicable ---
    parser.add_argument("-b", "--b_tag_combined",  action="store_true",
                        help="combined b-tag categories")
    parser.add_argument("--stat_only",             action="store_true",
                        help="evaluate closure using statistical uncertainties only")

    return parser.parse_args()


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------

if __name__ == "__main__":

    args = parseArgs()
    btag_str  = "_combined" if args.b_tag_combined else ""
    stat_only = args.stat_only

    # Build list of parameter dicts to process
    if args.config:
        rows = parse_config_file(args.config)
    else:
        if args.era is None:
            raise ValueError("Either --config or -e/--era must be provided.")
        rows = [{
            "era":              args.era,
            "DNN_cut_LJDC":     float(args.depth_score),
            "DNN_cut_inc_LJDC": float(args.inclusive_score),
            "DNN_cut_SJDC":     float(args.depth_score_sjdc)     if args.depth_score_sjdc     else float(args.depth_score),
            "DNN_cut_inc_SJDC": float(args.inclusive_score_sjdc) if args.inclusive_score_sjdc else float(args.inclusive_score),
        }]

    debug = False

    for row in rows:
        era           = row["era"]
        depth_ljdc    = row["DNN_cut_LJDC"]
        inc_ljdc      = row["DNN_cut_inc_LJDC"]
        depth_sjdc    = row["DNN_cut_SJDC"]
        inc_sjdc      = row["DNN_cut_inc_SJDC"]

        filename = _make_filename(depth_ljdc, inc_ljdc, depth_sjdc, inc_sjdc, era, btag_str)

        if not os.path.exists(filename):
            print(f"# WARNING: file not found, skipping: {filename}")
            continue

        print(f"\n% --- {filename} ---")

        data = parse_file(filename)

        if debug:
            for key, modes in data.items():
                print(f"\n{key}:")
                for mode, values in modes.items():
                    print(f"  {mode}:")
                    for name, val in values.items():
                        print(f"    {name}: {val}")

        # Pass SJDC scores only when they differ from LJDC
        sjdc_depth = str(depth_sjdc) if depth_sjdc != depth_ljdc else None
        sjdc_inc   = str(inc_sjdc)   if inc_sjdc   != inc_ljdc   else None

        latex = generate_latex_table(
            data, era,
            str(depth_ljdc), str(inc_ljdc),
            stat_only=stat_only,
            depth_score_sjdc=sjdc_depth,
            inclusive_score_sjdc=sjdc_inc,
        )
        print(latex)
