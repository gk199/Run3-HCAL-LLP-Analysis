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

        # --- Detect b-tagging category
        if "depth, b-tagged" in line_lower or "depth, b tagged" in line_lower:
            current_tag = "b tagged"
        elif "depth, not b-tagged" in line_lower or "depth, not b tagged" in line_lower:
            current_tag = "not b tagged"

        # --- Detect jet category
        if line_lower in {"leading", "sub-leading", "combined"}:
            current_jet = line_lower

        # --- Parse actual data line
        if "predicted mistagged events in vr" in line_lower:
            match = re.search(r"([\d.]+)\s*±\s*([\d.]+)", line)
            if match:
                val, err = map(float, match.groups())
                key = (current_jet, current_tag)
                data[key][current_mode]['Predicted VR'] = (val, err)

        elif "predicted mistagged events in sr" in line_lower:
            match = re.search(r"([\d.]+)\s*±\s*([\d.]+)", line)
            if match:
                val, err = map(float, match.groups())
                key = (current_jet, current_tag)
                data[key][current_mode]['Predicted SR'] = (val, err)

        elif "observed mistagged events in vr" in line_lower:
            match = re.search(r"([\d.]+)\s*±\s*([\d.]+)", line)
            if match:
                val, err = map(float, match.groups())
                key = (current_jet, current_tag)
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


def generate_latex_table(data, year, score):
    def entry(jet, tag, key):
        if key == 'Observed VR':
            val, stat = data[(jet, tag)]['central'][key]
            return format_observed(val, stat)
        else:
            val, stat = data[(jet, tag)]['central'][key]
            low = data[(jet, tag)]['low'].get(key, (val, stat))
            high = data[(jet, tag)]['high'].get(key, (val, stat))
            syst_up, syst_down = compute_syst((val, stat), low, high)
            return format_predicted(val, stat, syst_up, syst_down)

    lines = []
    lines.append("\\begin{table}[h!]")
    lines.append("    \\centering")
    lines.append("    \\begin{tabular}{c|c|c|c}")
    lines.append("        \\multicolumn{2}{c|}{\\textbf{Categories}} & \\textbf{b-tagged} & \\textbf{not b-tagged} \\\\ \\hline")

    for jet in ['leading', 'sub-leading']:
        lines.append(f"        \\multirow{{3}}{{*}}{{{jet.capitalize()} Jet}} & Observed VR  & {entry(jet, 'b tagged', 'Observed VR')} & {entry(jet, 'not b tagged', 'Observed VR')} \\\\")
        lines.append(f"        & Predicted VR & {entry(jet, 'b tagged', 'Predicted VR')} & {entry(jet, 'not b tagged', 'Predicted VR')} \\\\")
        lines.append(f"        & Predicted SR & {entry(jet, 'b tagged', 'Predicted SR')} & {entry(jet, 'not b tagged', 'Predicted SR')} \\\\ \\hline")

    lines.append("    \\end{tabular}")
    lines.append("    \\caption{Depth DNN score = " + score + " for " + year + ".}")
    lines.append("    \\label{Table:VRclosure_pt" + score[-1] + "_" + year + "}")
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
    
    args = parser.parse_args()

    return args 

# ---- USAGE ----
if __name__ == "__main__":

    print("Parsing arguments...")

    args = parseArgs()
    year = args.era   
    score = args.DNN_cut
    b_tag_combined = args.b_tag_combined
    btag_str = "_combined" if b_tag_combined else ""

    filename = "DNN_pt" + score[-1] + "_" + year + "_forPython" + btag_str + ".txt"  # Change this to your input file
    data = parse_file(filename)

    debug = False
    if debug:
        for key, modes in data.items():
            print(f"\n{key}:")
            for mode, values in modes.items():
                print(f"  {mode}:")
                for name, val in values.items():
                    print(f"    {name}: {val}")

    latex = generate_latex_table(data, year, score)
    print(latex)