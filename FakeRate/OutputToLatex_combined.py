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


def generate_latex_table(data, year, depth_score, inclusive_score):
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

    score_label = f"depth = {depth_score}, inclusive = {inclusive_score}"
    depth_tag = depth_score.replace("0.", "")
    inc_tag = inclusive_score.replace("0.", "")

    lines = []
    lines.append("\\begin{table}[h!]")
    lines.append("    \\centering")
    lines.append("    \\begin{tabular}{c|c|c}")

    lines.append("        \\multicolumn{2}{c|}{\\textbf{Categories}} & \\textbf{DNN Score (" + score_label + ")} \\\\ \\hline")

    for jet in ['leading', 'sub-leading']:
        lines.append(f"        \\multirow{{3}}{{*}}{{{jet.capitalize()} Jet}} & Observed VR  & {entry(jet, 'Observed VR')} \\\\")
        lines.append(f"        & Predicted VR & {entry(jet, 'Predicted VR')} \\\\")
        lines.append(f"        & Predicted SR & {entry(jet, 'Predicted SR')} \\\\ \\hline")

    year_text = year.replace("_", " ")
    lines.append("    \\end{tabular}")
    lines.append("    \\caption{Depth DNN score = " + depth_score + ", inclusive DNN score = " + inclusive_score + " for " + year_text + ".}")
    lines.append("    \\label{Table:VRclosure_pt" + depth_tag + "_incpt" + inc_tag + "_" + year + "_combined}")
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
    parser.add_argument("-d", "--depth_score",      action="store", default="0.93", help="Depth DNN score cut, e.g. 0.93 or 93 (default: 0.93)")
    parser.add_argument("-i", "--inclusive_score",  action="store", default="0.95", help="Inclusive DNN score cut, e.g. 0.95 or 95 (default: 0.95)")
    parser.add_argument("-b", "--b_tag_combined",   action="store_true", help="combined b-tag categories (True if -b passed) or not (False no -b)")
    
    args = parser.parse_args()

    return args 

# ---- USAGE ----
if __name__ == "__main__":

    #print("Parsing arguments...")
    print (" ")

    args = parseArgs()
    year = args.era
    b_tag_combined = args.b_tag_combined
    btag_str = "_combined" if b_tag_combined else ""

    # Normalize scores: accept either "0.93" or "93" style; store both forms
    def normalize_score(s):
        s = str(s)
        tag = s.replace("0.", "") if s.startswith("0.") else s          # "93"
        label = "0." + tag if not s.startswith("0.") else s             # "0.93"
        return tag, label

    depth_tag, depth_label = normalize_score(args.depth_score)
    inc_tag, inc_label     = normalize_score(args.inclusive_score)

    filename = "DNN_pt" + depth_tag + "_incpt" + inc_tag + "_" + year + "_forPython" + btag_str + ".txt"
    data = parse_file(filename)

    debug = False
    if debug:
        for key, modes in data.items():
            print(f"\n{key}:")
            for mode, values in modes.items():
                print(f"  {mode}:")
                for name, val in values.items():
                    print(f"    {name}: {val}")

    latex = generate_latex_table(data, year, depth_label, inc_label)
    print(latex)