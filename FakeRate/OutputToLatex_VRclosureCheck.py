import re
from collections import defaultdict
import argparse

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


def generate_latex_table(data, year, score, stat_only=False):
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
    lines.append("\\begin{table}[h!]")
    lines.append("    \\centering")
    # lines.append("    % Requires in preamble: \\usepackage{colortbl} and \\definecolor{lightred}{rgb}{1.0, 0.8, 0.8}")
    lines.append("    \\begin{tabular}{c|c|c}")

    lines.append("        \\multicolumn{2}{c|}{\\textbf{Categories}} & \\textbf{DNN Score = " + score + "} \\\\ \\hline")

    disagreements = []  # collect (jet_label, pull) for incompatible entries
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

    # Build caption, appending disagreement info if any
    caption = f"Depth DNN score = {score} for {year_text}."
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
    lines.append("    \\label{Table:VRclosure_pt" + score.replace("0.", "") + "_" + year + "_combined}")
    lines.append("\\end{table}")
    return "\n".join(lines)

def parseArgs():
    """ Parse command-line arguments
    """
    parser = argparse.ArgumentParser(
        add_help=True,
        description=''
    )

    parser.add_argument("-e", "--era",              action="store", help="era (2022, 2023, or specify year and era)", required=True) 
    parser.add_argument("-s", "--DNN_cut",          action="store", default=0.8, help="Depth DNN score cut (default: 0.8)")
    parser.add_argument("-b", "--b_tag_combined",   action="store_true", help="combined b-tag categories (True if -b passed) or not (False no -b)")
    parser.add_argument("--stat_only",              action="store_true", help="evaluate closure using statistical uncertainties only (ignore systematics)")
    
    args = parser.parse_args()

    return args 

# ---- USAGE ----
if __name__ == "__main__":

    print (" ")

    args = parseArgs()
    year = args.era   
    score = args.DNN_cut
    b_tag_combined = args.b_tag_combined
    stat_only = args.stat_only
    btag_str = "_combined" if b_tag_combined else ""

    filename = "HigherInclusiveCut_pt97/DNN_pt" + score.replace("0.", "") + "_" + year + "_forPython" + btag_str + ".txt"
    data = parse_file(filename)

    debug = False
    if debug:
        for key, modes in data.items():
            print(f"\n{key}:")
            for mode, values in modes.items():
                print(f"  {mode}:")
                for name, val in values.items():
                    print(f"    {name}: {val}")

    latex = generate_latex_table(data, year, score, stat_only=stat_only)
    print(latex)