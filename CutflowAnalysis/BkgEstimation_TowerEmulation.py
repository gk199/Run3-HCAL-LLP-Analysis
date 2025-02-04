from math import *
import sys
import argparse
import os
import time
import numpy as np
import copy
import textwrap
import fileinput
from datetime import date
import ROOT

ROOT.gROOT.SetBatch(1)
ROOT.gStyle.SetOptStat(0)
ROOT.gStyle.SetEndErrorSize(0)

# print_latex = False
print_latex = True

# ------------------------------------------------------------------------------
def latex_setup(file_path):
	print("\\begin{table}[ht]")
	print("\\centering")
	if "Run2023" in file_path: 
		print("\\begin{tabular}{l|llll}")
		print("\\hline")
		print("\\textbf{Selection} & \\multicolumn{2}{l}{\\textbf{Number of Entries}} & \\multicolumn{2}{l}{\\textbf{Fraction of Entries}} \\\\ \\hline")
		print(" & \\textbf{LLP 0} & \\textbf{LLP 1} & \\textbf{LLP 0} & \\textbf{LLP 1} \\\\ \\hline \\hline")
	else:
		print("\\begin{tabular}{l|llll}")
		print("\\hline")
		print("\\textbf{Selection} & \\multicolumn{2}{l}{\\textbf{Number of Entries}} & \\multicolumn{2}{l}{\\textbf{Fraction of Entries}} \\\\ \\hline") # Number of Weighted Entries
		print(" & \\textbf{LLP 0} & \\textbf{LLP 1} & \\textbf{LLP 0} & \\textbf{LLP 1} \\\\ \\hline \\hline")

def jetSplit_event_latex_setup(file_path):
	print("\\begin{table}[ht]")
	print("\\centering")
	if "Run2023" in file_path: 
		print("\\begin{tabular}{l|lll}")
		print("\\hline")
		print("\\textbf{Selection} & \\textbf{Number of Entries} & \\multicolumn{2}{l}{\\textbf{Fraction of Entries}} \\\\ \\hline")
		print(" &  & All & Valid Jet \\\\ \\hline \\hline")
	else:
		print("\\begin{tabular}{l|lll}")
		print("\\hline")
		print("\\textbf{Selection} & \\textbf{Number of Entries} & \\multicolumn{2}{l}{\\textbf{Fraction of Entries}} \\\\ \\hline")
		print(" &  & All & Valid Jet \\\\ \\hline \\hline")

def event_latex_setup(file_path):
	print("\\begin{table}[ht]")
	print("\\centering")
	if "Run2023" in file_path: 
		print("\\begin{tabular}{l|ll}")
		print("\\hline")
		print("\\textbf{Selection} & \\textbf{Number of Entries} & \\textbf{Fraction of Entries} \\\\ \\hline \\hline")
	else:
		print("\\begin{tabular}{l|ll}")
		print("\\hline")
		print("\\textbf{Selection} & \\textbf{Number of Entries} & \\textbf{Fraction of Entries} \\\\ \\hline \\hline")

def latex_end(file_path):
    print("\\hline")
    print("\\end{tabular}")
    print("\\caption{\\protect\\url{" + file_path + "}}") # to save which file this cutflow is from
    print("\\label{9-Table:BkgEst_Tower}")
    print("\\end{table}")

# ------------------------------------------------------------------------------
def main():
	file_path = sys.argv[1]
	jet_energy = "40"
	# DepthTowers = sys.argv[2] # "0.999"
	# TimingTowers = sys.argv[3] # "0.999"

	jet_string = "((jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26) || (jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26) || (jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26)"
	jet_string += "|| (jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26) || (jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26) || (jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26))"

	jet_string_noFlag = "((jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && (jet0_DepthTowers + jet0_TimingTowers == 0)) || (jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26 && (jet1_DepthTowers + jet1_TimingTowers == 0)) || (jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26 && (jet2_DepthTowers + jet2_TimingTowers == 0))"
	jet_string_noFlag += "|| (jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26 && (jet3_DepthTowers + jet3_TimingTowers == 0)) || (jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26 && (jet4_DepthTowers + jet4_TimingTowers == 0)) || (jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26 && (jet5_DepthTowers + jet5_TimingTowers == 0)))"

	jet0_depth = " ( jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_DepthTowers >= 2)"
	jet1_depth = " ( jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26 && jet1_DepthTowers >= 2)"
	jet2_depth = " ( jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26 && jet2_DepthTowers >= 2)"
	jet3_depth = " ( jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26 && jet3_DepthTowers >= 2)"
	jet4_depth = " ( jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26 && jet4_DepthTowers >= 2)"
	jet5_depth = " ( jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26 && jet5_DepthTowers >= 2)"

	jet0_timing = " ( jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_TimingTowers >= 2)"
	jet1_timing = " ( jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26 && jet1_TimingTowers >= 2)"
	jet2_timing = " ( jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26 && jet2_TimingTowers >= 2)"
	jet3_timing = " ( jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26 && jet3_TimingTowers >= 2)"
	jet4_timing = " ( jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26 && jet4_TimingTowers >= 2)"
	jet5_timing = " ( jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26 && jet5_TimingTowers >= 2)"

	jet0_depth_timing = " ( jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_DepthTowers == 1 && jet0_TimingTowers == 1)"
	jet1_depth_timing = " ( jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26 && jet1_DepthTowers == 1 && jet1_TimingTowers == 1)"
	jet2_depth_timing = " ( jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26 && jet2_DepthTowers == 1 && jet2_TimingTowers == 1)"
	jet3_depth_timing = " ( jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26 && jet3_DepthTowers == 1 && jet3_TimingTowers == 1)"
	jet4_depth_timing = " ( jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26 && jet4_DepthTowers == 1 && jet4_TimingTowers == 1)"
	jet5_depth_timing = " ( jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26 && jet5_DepthTowers == 1 && jet5_TimingTowers == 1)"

	# defining single and double tag strings, no trigger requirements
	one_jet_tagged_string = " ( " + jet0_depth + " || " + jet1_depth + " || " + jet2_depth + " || " + jet3_depth + " || " + jet4_depth + " || " + jet5_depth + ")"

	two_jet_tagged_string = " ( " + jet0_timing + " || " + jet1_timing + " || " + jet2_timing + " || " + jet3_timing + " || " + jet4_timing + " || " + jet5_timing + ")"

	three_jet_tagged_string = " ( " + jet0_depth_timing + " || " + jet1_depth_timing + " || " + jet2_depth_timing + " || " + jet3_depth_timing + " || " + jet4_depth_timing + " || " + jet5_depth_timing + ")"

	# defining single and double tag strings, require jet is matched to L1 LLP triggered jet
	jet_string_triggered = "  ( ( jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_L1trig_Matched == 1) || (jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26 && jet1_L1trig_Matched == 1)"
	jet_string_triggered += " || (jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26 && jet2_L1trig_Matched == 1) || (jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26 && jet3_L1trig_Matched == 1)"
	jet_string_triggered += " || (jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26 && jet4_L1trig_Matched == 1) || (jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26 && jet5_L1trig_Matched == 1))"	

	jet_string_triggered_noFlag = "((jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_L1trig_Matched == 1 && (jet0_DepthTowers + jet0_TimingTowers == 0)) || (jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26 && jet1_L1trig_Matched == 1 && (jet1_DepthTowers + jet1_TimingTowers == 0)) || (jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26 && jet2_L1trig_Matched == 1 && (jet2_DepthTowers + jet2_TimingTowers == 0))"
	jet_string_triggered_noFlag += "|| (jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26 && jet3_L1trig_Matched == 1 && (jet3_DepthTowers + jet3_TimingTowers == 0)) || (jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26 && jet4_L1trig_Matched == 1 && (jet4_DepthTowers + jet4_TimingTowers == 0)) || (jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26 && jet5_L1trig_Matched == 1 && (jet5_DepthTowers + jet5_TimingTowers == 0)))"

	one_jet_tagged_string_triggered = " ( (   " + jet0_depth + " && jet0_L1trig_Matched == 1 )" # jet 0 condition
	one_jet_tagged_string_triggered += " || ( " + jet1_depth + " && jet1_L1trig_Matched == 1 )" # jet 1 condition
	one_jet_tagged_string_triggered += " || ( " + jet2_depth + " && jet2_L1trig_Matched == 1 )" # jet 2 condition
	one_jet_tagged_string_triggered += " || ( " + jet3_depth + " && jet3_L1trig_Matched == 1 )" # jet 3 condition
	one_jet_tagged_string_triggered += " || ( " + jet4_depth + " && jet4_L1trig_Matched == 1 )" # jet 4 condition
	one_jet_tagged_string_triggered += " || ( " + jet5_depth + " && jet5_L1trig_Matched == 1 ))" # jet 5 condition

	two_jet_tagged_string_triggered = " ( (   " + jet0_timing + " && jet0_L1trig_Matched == 1 )" # jet 0 condition
	two_jet_tagged_string_triggered += " || ( " + jet1_timing + " && jet1_L1trig_Matched == 1 )" # jet 1 condition
	two_jet_tagged_string_triggered += " || ( " + jet2_timing + " && jet2_L1trig_Matched == 1 )" # jet 2 condition
	two_jet_tagged_string_triggered += " || ( " + jet3_timing + " && jet3_L1trig_Matched == 1 )" # jet 3 condition
	two_jet_tagged_string_triggered += " || ( " + jet4_timing + " && jet4_L1trig_Matched == 1 )" # jet 4 condition
	two_jet_tagged_string_triggered += " || ( " + jet5_timing + " && jet5_L1trig_Matched == 1 ))" # jet 5 condition

	three_jet_tagged_string_triggered = " ( (   " + jet0_depth_timing + " && jet0_L1trig_Matched == 1 )" # jet 0 condition
	three_jet_tagged_string_triggered += " || ( " + jet1_depth_timing + " && jet1_L1trig_Matched == 1 )" # jet 1 condition
	three_jet_tagged_string_triggered += " || ( " + jet2_depth_timing + " && jet2_L1trig_Matched == 1 )" # jet 2 condition
	three_jet_tagged_string_triggered += " || ( " + jet3_depth_timing + " && jet3_L1trig_Matched == 1 )" # jet 3 condition
	three_jet_tagged_string_triggered += " || ( " + jet4_depth_timing + " && jet4_L1trig_Matched == 1 )" # jet 4 condition
	three_jet_tagged_string_triggered += " || ( " + jet5_depth_timing + " && jet5_L1trig_Matched == 1 ))" # jet 5 condition

	# defining single and double tag strings, require jet is matched to L1 LLP not-triggered jet
	jet_string_not_triggered = "  ( ( jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_L1trig_Matched == 0) || (jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26 && jet1_L1trig_Matched == 0)"
	jet_string_not_triggered += " || (jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26 && jet2_L1trig_Matched == 0) || (jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26 && jet3_L1trig_Matched == 0)"
	jet_string_not_triggered += " || (jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26 && jet4_L1trig_Matched == 0) || (jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26 && jet5_L1trig_Matched == 0))"

	jet_string_not_triggered_noFlag = "  ( ( jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_L1trig_Matched == 0 && (jet0_DepthTowers + jet0_TimingTowers == 0)) || (jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26 && jet1_L1trig_Matched == 0 && (jet1_DepthTowers + jet1_TimingTowers == 0))"
	jet_string_not_triggered_noFlag += " || (jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26 && jet2_L1trig_Matched == 0 && (jet2_DepthTowers + jet2_TimingTowers == 0)) || (jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26 && jet3_L1trig_Matched == 0 && (jet3_DepthTowers + jet3_TimingTowers == 0))"
	jet_string_not_triggered_noFlag += " || (jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26 && jet4_L1trig_Matched == 0 && (jet4_DepthTowers + jet4_TimingTowers == 0)) || (jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26 && jet5_L1trig_Matched == 0 && (jet5_DepthTowers + jet5_TimingTowers == 0)))"

	one_jet_tagged_string_not_triggered = " ( (  " + jet0_depth + " && jet0_L1trig_Matched == 0 )" # jet 0 condition
	one_jet_tagged_string_not_triggered += " || (" + jet1_depth + " && jet1_L1trig_Matched == 0 )" # jet 1 condition
	one_jet_tagged_string_not_triggered += " || (" + jet2_depth + " && jet2_L1trig_Matched == 0 )" # jet 2 condition
	one_jet_tagged_string_not_triggered += " || (" + jet3_depth + " && jet3_L1trig_Matched == 0 )" # jet 3 condition
	one_jet_tagged_string_not_triggered += " || (" + jet4_depth + " && jet4_L1trig_Matched == 0 )" # jet 4 condition
	one_jet_tagged_string_not_triggered += " || (" + jet5_depth + " && jet5_L1trig_Matched == 0 ))" # jet 5 condition

	two_jet_tagged_string_not_triggered = " ( (   " + jet0_timing + " && jet0_L1trig_Matched == 0 )" # jet 0 condition
	two_jet_tagged_string_not_triggered += " || ( " + jet1_timing + " && jet1_L1trig_Matched == 0 )" # jet 1 condition
	two_jet_tagged_string_not_triggered += " || ( " + jet2_timing + " && jet2_L1trig_Matched == 0 )" # jet 2 condition
	two_jet_tagged_string_not_triggered += " || ( " + jet3_timing + " && jet3_L1trig_Matched == 0 )" # jet 3 condition
	two_jet_tagged_string_not_triggered += " || ( " + jet4_timing + " && jet4_L1trig_Matched == 0 )" # jet 4 condition
	two_jet_tagged_string_not_triggered += " || ( " + jet5_timing + " && jet5_L1trig_Matched == 0 ))" # jet 5 condition

	three_jet_tagged_string_not_triggered = " ( (   " + jet0_depth_timing + " && jet0_L1trig_Matched == 0 )" # jet 0 condition
	three_jet_tagged_string_not_triggered += " || ( " + jet1_depth_timing + " && jet1_L1trig_Matched == 0 )" # jet 1 condition
	three_jet_tagged_string_not_triggered += " || ( " + jet2_depth_timing + " && jet2_L1trig_Matched == 0 )" # jet 2 condition
	three_jet_tagged_string_not_triggered += " || ( " + jet3_depth_timing + " && jet3_L1trig_Matched == 0 )" # jet 3 condition
	three_jet_tagged_string_not_triggered += " || ( " + jet4_depth_timing + " && jet4_L1trig_Matched == 0 )" # jet 4 condition
	three_jet_tagged_string_not_triggered += " || ( " + jet5_depth_timing + " && jet5_L1trig_Matched == 0 ))" # jet 5 condition

	# defining single and double tag strings, require jet is matched to L1 LLP jet, regardless of trigger result
	jet_string_matched = "  ( ( jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_L1trig_Matched >= 0) || (jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26 && jet1_L1trig_Matched >= 0)"
	jet_string_matched += " || (jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26 && jet2_L1trig_Matched >= 0) || (jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26 && jet3_L1trig_Matched >= 0)"
	jet_string_matched += " || (jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26 && jet4_L1trig_Matched >= 0) || (jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26 && jet5_L1trig_Matched >= 0))"

	jet_string_matched_noFlag = "  ( ( jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_L1trig_Matched >= 0 && (jet0_DepthTowers + jet0_TimingTowers == 0)) || (jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26 && jet1_L1trig_Matched >= 0 && (jet1_DepthTowers + jet1_TimingTowers == 0))"
	jet_string_matched_noFlag += " || (jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26 && jet2_L1trig_Matched >= 0 && (jet2_DepthTowers + jet2_TimingTowers == 0)) || (jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26 && jet3_L1trig_Matched >= 0 && (jet3_DepthTowers + jet3_TimingTowers == 0))"
	jet_string_matched_noFlag += " || (jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26 && jet4_L1trig_Matched >= 0 && (jet4_DepthTowers + jet4_TimingTowers == 0)) || (jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26 && jet5_L1trig_Matched >= 0 && (jet5_DepthTowers + jet5_TimingTowers == 0)))"

	one_jet_tagged_string_matched = " ( (  " + jet0_depth + " && jet0_L1trig_Matched >= 0 )" # jet 0 condition
	one_jet_tagged_string_matched += " || (" + jet1_depth + " && jet1_L1trig_Matched >= 0 )" # jet 1 condition
	one_jet_tagged_string_matched += " || (" + jet2_depth + " && jet2_L1trig_Matched >= 0 )" # jet 2 condition
	one_jet_tagged_string_matched += " || (" + jet3_depth + " && jet3_L1trig_Matched >= 0 )" # jet 3 condition
	one_jet_tagged_string_matched += " || (" + jet4_depth + " && jet4_L1trig_Matched >= 0 )" # jet 4 condition
	one_jet_tagged_string_matched += " || (" + jet5_depth + " && jet5_L1trig_Matched >= 0 ))" # jet 5 condition

	two_jet_tagged_string_matched = " ( (   " + jet0_timing + " && jet0_L1trig_Matched >= 0 )" # jet 0 condition
	two_jet_tagged_string_matched += " || ( " + jet1_timing + " && jet1_L1trig_Matched >= 0 )" # jet 1 condition
	two_jet_tagged_string_matched += " || ( " + jet2_timing + " && jet2_L1trig_Matched >= 0 )" # jet 2 condition
	two_jet_tagged_string_matched += " || ( " + jet3_timing + " && jet3_L1trig_Matched >= 0 )" # jet 3 condition
	two_jet_tagged_string_matched += " || ( " + jet4_timing + " && jet4_L1trig_Matched >= 0 )" # jet 4 condition
	two_jet_tagged_string_matched += " || ( " + jet5_timing + " && jet5_L1trig_Matched >= 0 ))" # jet 5 condition

	three_jet_tagged_string_matched = " ( (   " + jet0_depth_timing + " && jet0_L1trig_Matched >= 0 )" # jet 0 condition
	three_jet_tagged_string_matched += " || ( " + jet1_depth_timing + " && jet1_L1trig_Matched >= 0 )" # jet 1 condition
	three_jet_tagged_string_matched += " || ( " + jet2_depth_timing + " && jet2_L1trig_Matched >= 0 )" # jet 2 condition
	three_jet_tagged_string_matched += " || ( " + jet3_depth_timing + " && jet3_L1trig_Matched >= 0 )" # jet 3 condition
	three_jet_tagged_string_matched += " || ( " + jet4_depth_timing + " && jet4_L1trig_Matched >= 0 )" # jet 4 condition
	three_jet_tagged_string_matched += " || ( " + jet5_depth_timing + " && jet5_L1trig_Matched >= 0 ))" # jet 5 condition

	# defining single and double tag strings, require jet is matched to L1 LLP triggered jet, no cuts on DNN scores
	two_jet_string_triggered = " ( ( ( jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_L1trig_Matched == 1 ) && (jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26 && jet1_L1trig_Matched == 1 ))" # jet 0 AND 1 condition
	two_jet_string_triggered += " || ((jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_L1trig_Matched == 1 ) && (jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26 && jet2_L1trig_Matched == 1 ))" # jet 0 AND 2 condition
	two_jet_string_triggered += " || ((jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_L1trig_Matched == 1 ) && (jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26 && jet3_L1trig_Matched == 1 ))" # jet 0 AND 3 condition
	two_jet_string_triggered += " || ((jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_L1trig_Matched == 1 ) && (jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26 && jet4_L1trig_Matched == 1 ))" # jet 0 AND 4 condition
	two_jet_string_triggered += " || ((jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_L1trig_Matched == 1 ) && (jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26 && jet5_L1trig_Matched == 1 ))" # jet 0 AND 5 condition
	two_jet_string_triggered += " || ((jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26 && jet1_L1trig_Matched == 1 ) && (jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26 && jet2_L1trig_Matched == 1 ))" # jet 1 AND 2 condition
	two_jet_string_triggered += " || ((jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26 && jet1_L1trig_Matched == 1 ) && (jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26 && jet3_L1trig_Matched == 1 ))" # jet 1 AND 3 condition
	two_jet_string_triggered += " || ((jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26 && jet1_L1trig_Matched == 1 ) && (jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26 && jet4_L1trig_Matched == 1 ))" # jet 1 AND 4 condition
	two_jet_string_triggered += " || ((jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26 && jet1_L1trig_Matched == 1 ) && (jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26 && jet5_L1trig_Matched == 1 ))" # jet 1 AND 5 condition
	two_jet_string_triggered += " || ((jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26 && jet2_L1trig_Matched == 1 ) && (jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26 && jet3_L1trig_Matched == 1 ))" # jet 2 AND 3 condition
	two_jet_string_triggered += " || ((jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26 && jet2_L1trig_Matched == 1 ) && (jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26 && jet4_L1trig_Matched == 1 ))" # jet 2 AND 4 condition
	two_jet_string_triggered += " || ((jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26 && jet2_L1trig_Matched == 1 ) && (jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26 && jet5_L1trig_Matched == 1 ))" # jet 2 AND 5 condition
	two_jet_string_triggered += " || ((jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26 && jet3_L1trig_Matched == 1 ) && (jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26 && jet4_L1trig_Matched == 1 ))" # jet 3 AND 4 condition
	two_jet_string_triggered += " || ((jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26 && jet3_L1trig_Matched == 1 ) && (jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26 && jet5_L1trig_Matched == 1 ))" # jet 3 AND 5 condition
	two_jet_string_triggered += " || ((jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26 && jet4_L1trig_Matched == 1 ) && (jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26 && jet5_L1trig_Matched == 1 )))" # jet 4 AND 5 condition

	HLT_string = "(( HLT_HT200_L1SingleLLPJet_DisplacedDijet40_Inclusive1PtrkShortSig5 == 1 || HLT_HT240_L1SingleLLPJet_DisplacedDijet40_Inclusive1PtrkShortSig5 == 1 || HLT_HT280_L1SingleLLPJet_DisplacedDijet40_Inclusive1PtrkShortSig5 == 1)"
	HLT_string += "|| ( HLT_HT170_L1SingleLLPJet_DisplacedDijet40_DisplacedTrack == 1 || HLT_HT200_L1SingleLLPJet_DisplacedDijet40_DisplacedTrack == 1 || HLT_HT270_L1SingleLLPJet_DisplacedDijet40_DisplacedTrack == 1 || HLT_HT200_L1SingleLLPJet_DisplacedDijet60_DisplacedTrack == 1 )"
	HLT_string += "|| ( HLT_HT320_L1SingleLLPJet_DisplacedDijet60_Inclusive == 1 || HLT_HT420_L1SingleLLPJet_DisplacedDijet60_Inclusive == 1 )"
	HLT_string += "|| ( HLT_HT200_L1SingleLLPJet_DelayedJet40_DoubleDelay0p5nsTrackless == 1 || HLT_HT200_L1SingleLLPJet_DelayedJet40_DoubleDelay1nsInclusive == 1 || HLT_HT200_L1SingleLLPJet_DelayedJet40_SingleDelay1nsTrackless == 1 || HLT_HT200_L1SingleLLPJet_DelayedJet40_SingleDelay2nsInclusive == 1 )"
	HLT_string += "|| HLT_L1SingleLLPJet == 1)"

	if "LLP_MC" in file_path:
		# Cutflow table for background estimation: basic event selection
		print(" \n")
		print("Signal cutflow, with truth cuts, done per event")
		print(" \n")

		selection_list = [
			"All", 
			"LLP $r$ in HCAL depth 3 or 4", 
			"LLP $\\abs\\eta \\leq 1.26$", 
			"LLP $E > 60$~GeV", 
			"LLP 0 OR LLP 1",
			"LLP 0 AND LLP 1",
			"LLP OR, with Event HT $> 200$~GeV", 
			"LLP AND, with Event HT $> 200$~GeV", 
            "No tower flags",
            "Depth towers $\\geq 2$",
            "Timing towers $\\geq 2$",
            "1 timing, 1 depth tower",
		]
	
		selection_list_abbrev = [
			"All       ", 
			"LLP R d3-4", 
			"LLP eta   ", 
			"LLP pT    ", 
			"LLP OR	   ", 
			"LLP AND   ",
			"LLP OR with HTT", 
			"LLP AND with HTT",
            "No tower flags",
            "2+ depth passed",
            "2+ timing passed",
			"1 timing, 1 depth passed",
		]
	
		file = ROOT.TFile.Open(file_path)
		# tree = file.Get("Classification")
		tree = file.Get("NoSel")
		
		selection_string = ""
		selection1_string = ""
		total_selection_string = ""
		
		if print_latex:
			latex_setup(file_path)

		init = -1
		
		for i in range(len(selection_list)):
			selname = selection_list[i]
			selval  = -1
			selval1 = -1
			Nevents = -1

			if i == 0: 
				selval = tree.GetEntries()
				selval1 = tree.GetEntries()
				init = selval
			if i == 1: 
				selection_string += "(LLP0_DecayR >= 214.2 && LLP0_DecayR < 295)"
				selection1_string += "(LLP1_DecayR >= 214.2 && LLP1_DecayR < 295)"
			if i == 2: 
				selection_string += "&& abs(LLP0_Eta) <= 1.26"
				selection1_string += "&& abs(LLP1_Eta) <= 1.26"
			if i == 3: 
				selection_string += "&& LLP0_E > 60"
				selection1_string += "&& LLP1_E > 60"

			if i == 4: total_selection_string = "(" + selection_string + ") || (" + selection1_string + ")"
			if i == 5: total_selection_string = "(" + selection_string + " && " + selection1_string + ")"
			if i == 6: total_selection_string = "((" + selection_string + ") || (" + selection1_string + ")) && eventHT > 200"
			if i == 7: 
				both_LLP = "(" + selection_string + " && " + selection1_string + ") && eventHT > 200"
				total_selection_string = both_LLP

			if i == 8: total_selection_string = both_LLP + " && " + jet_string_noFlag
			if i == 9: total_selection_string = both_LLP + " && " + one_jet_tagged_string
			if i == 10: total_selection_string = both_LLP + " && " + two_jet_tagged_string
			if i == 11: total_selection_string = both_LLP + " && " + three_jet_tagged_string

			selval = tree.GetEntries(selection_string)
			selval1 = tree.GetEntries(selection1_string)

			if i >= 4: selval = tree.GetEntries(total_selection_string)
			if i == 8: all_events = selval
			if i == 9: one_plus_jets = selval
			if i == 10: two_plus_jets = selval
			if i == 11: three_plus_jets = selval

			Nevents = tree.GetEntries()

			if print_latex:
				if i < 4: print(selname+" &", round(selval, 4), "&", round(selval1, 4), "&", round(selval/init, 4), "&", round(selval1/init, 4), "\\\\")
				if i == 4: print("\\hline")
				if (i >= 4 and i < 8): print(selname+" & \\multicolumn{2}{l}{", round(selval, 4), "} & \\multicolumn{2}{l}{", round(selval/init, 4), "} \\\\ ")
				if i == 7: print("\\hline")
				if i == 8: print(selname+" & \\multicolumn{2}{l}{", round(all_events, 4), "} & \\multicolumn{2}{l}{", round((all_events)/init, 4), "} \\\\ ") # 0 bin is all events - events with at least 1
				if i == 9: print(selname+" & \\multicolumn{2}{l}{", round(one_plus_jets, 4), "} & \\multicolumn{2}{l}{", round((one_plus_jets)/init, 4), "} \\\\ ") # 1 bin is events with at least 1 - events with at least 2
				if i == 10: print(selname+" & \\multicolumn{2}{l}{", round(two_plus_jets, 4), "} & \\multicolumn{2}{l}{", round((two_plus_jets)/init, 4), "} \\\\ ") # 2 bin is events with at least 2
				if i == 11: 
					print(selname+" & \\multicolumn{2}{l}{", round(three_plus_jets, 4), "} & \\multicolumn{2}{l}{", round(three_plus_jets/init, 4), "} \\\\ ") # 2+ bin is events with at least 2
					latex_end(file_path)

			else:
				if (i < 4): print(selection_list_abbrev[i], "\t", Nevents, "\t", round(selval, 4), "\t", round(selval/init, 4), "LLP 1:", "\t", round(selval1, 4), "\t", round(selval1/init, 4))
				if (i >= 4): print(selection_list_abbrev[i], "\t", Nevents, "\t", round(selval, 4), "\t", round(selval/init, 4))

		# Cutflow table for background estimation: basic event selection
		print(" \n")
		print("Signal cutflow, with truth cuts (NO HT imposed), done per event")
		print(" \n")

		selection_list = [
			"All", 
			"LLP $r$ in HCAL", 
			"LLP $\\abs\\eta \\leq 1.26$", 
			"LLP $E > 40$~GeV", 
			"LLP 0 OR LLP 1",
            "Jet $\\geq "+jet_energy+"$~GeV $p_T$ and $\\abs\\eta \\leq 1.26$", 
            "No tower flags",
            "Depth towers $\\geq 2$",
            "Timing towers $\\geq 2$",
            "1 timing, 1 depth tower",
		]
	
		selection_list_abbrev = [
			"All       ", 
			"LLP R d1-4", 
			"LLP eta   ", 
			"LLP pT    ", 
			"LLP OR    ",
			"Jet pT and eta",
            "No tower flags",
            "2+ depth passed",
            "2+ timing passed",
			"1 timing, 1 depth passed",
		]
	
		file = ROOT.TFile.Open(file_path)
		# tree = file.Get("Classification")
		tree = file.Get("NoSel")
		
		if print_latex:
			latex_setup(file_path)

		init = -1
		
		for i in range(len(selection_list)):
			selname = selection_list[i]
			selval  = -1
			selval1 = -1
			Nevents = -1

			if i == 0: 
				init = tree.GetEntries()
				selection_string = ""
				selection1_string = ""
			if i == 1: 
				selection_string = "(LLP0_DecayR >= 177 && LLP0_DecayR < 295)"
				selection1_string = "(LLP1_DecayR >= 177 && LLP1_DecayR < 295)"
			if i == 2: 
				selection_string += "&& abs(LLP0_Eta) <= 1.26"
				selection1_string += "&& abs(LLP1_Eta) <= 1.26"
			if i == 3: 
				selection_string += "&& LLP0_E > 40"
				selection1_string += "&& LLP1_E > 40"

			selval0 = tree.GetEntries(selection_string)
			selval1 = tree.GetEntries(selection1_string)

			if i == 4: 
				either_LLP = "((" + selection_string + ") || (" + selection1_string + "))"
				total_selection_string = either_LLP
			if i == 5: total_selection_string = either_LLP + " && " + jet_string
			if i == 6: total_selection_string = either_LLP + " && " + jet_string_noFlag
			if i == 7: total_selection_string = either_LLP + " && " + one_jet_tagged_string
			if i == 8: total_selection_string = either_LLP + " && " + two_jet_tagged_string
			if i == 9: total_selection_string = either_LLP + " && " + three_jet_tagged_string

			if i >= 4: selval = tree.GetEntries(total_selection_string)
			if i == 6: all_events = selval
			if i == 7: one_plus_jets = selval
			if i == 8: two_plus_jets = selval
			if i == 9: three_plus_jets = selval

			Nevents = tree.GetEntries()

			if print_latex:
				if i < 4: print(selname+" &", round(selval0, 4), "&", round(selval1, 4), "&", round(selval0/init, 4), "&", round(selval1/init, 4), "\\\\")
				if i == 4 or i == 5: 
					print(selname+" & \\multicolumn{2}{l}{", round(selval, 4), "}& \\multicolumn{2}{l}{", round(selval/init, 4), "} \\\\")
					if i == 5: print("\\hline")
				if i == 6: print(selname+" & \\multicolumn{2}{l}{", round(all_events, 4), "} & \\multicolumn{2}{l}{", round((all_events)/init, 4), "} \\\\ ") # 0 bin is all events - events with at least 1
				if i == 7: print(selname+" & \\multicolumn{2}{l}{", round(one_plus_jets , 4), "} & \\multicolumn{2}{l}{", round((one_plus_jets)/init, 4), "} \\\\ ") # 1 bin is events with at least 1 - events with at least 2
				if i == 8: print(selname+" & \\multicolumn{2}{l}{", round(two_plus_jets, 4), "} & \\multicolumn{2}{l}{", round((two_plus_jets)/init, 4), "} \\\\ ") # 2 bin is events with at least 2 - events with at least 3
				if i == 9: 
					print(selname+" & \\multicolumn{2}{l}{", round(three_plus_jets, 4), "} & \\multicolumn{2}{l}{", round(three_plus_jets/init, 4), "} \\\\ ") # 3+ bin is events with at least 3
					latex_end(file_path)

			else:
				if (i < 4): print(selection_list_abbrev[i], "\t", Nevents, "\t", round(selval, 4), "\t", round(selval/init, 4), "LLP 1:", "\t", round(selval1, 4), "\t", round(selval1/init, 4))
				if (i >= 4): print(selection_list_abbrev[i], "\t", Nevents, "\t", round(selval, 4), "\t", round(selval/init, 4))

	if "LLPskim" or "LLP_MC" in file_path:
		# Cutflow table for background estimation: basic event selection
		print(" \n")
		print("Signal cutflow, without truth cuts, done per event")
		print(" \n")
		selection_list_noCut = [
			"All", 
            "Jet $\\geq "+jet_energy+"$~GeV $p_T$ and $\\abs\\eta \\leq 1.26$", # (one of 6 leading)", 
            "No tower flags",
            "Depth towers $\\geq 2$",
            "Timing towers $\\geq 2$",
            "1 timing, 1 depth tower",
			"LLP HLT passed",
            "HLT and no tower flags",
            "HLT and Depth towers $\\geq 2$",
            "HLT and Timing towers $\\geq 2$",
            "HLT and 1 timing, 1 depth tower",
			"Jet $\\geq "+jet_energy+"$~GeV $p_T$, $\\abs\\eta \\leq 1.26$, triggered",
			"LLP HLT passed",
            "HLT and no tower flags",
            "HLT and Depth towers $\\geq 2$",
            "HLT and Timing towers $\\geq 2$",
            "HLT and 1 timing, 1 depth tower",
			"Jet $\\geq "+jet_energy+"$~GeV $p_T$, $\\abs\\eta \\leq 1.26$, not triggered",
			"LLP HLT passed",
            "HLT and no tower flags",
            "HLT and Depth towers $\\geq 2$",
            "HLT and Timing towers $\\geq 2$",
            "HLT and 1 timing, 1 depth tower",
			"Jet $\\geq "+jet_energy+"$~GeV $p_T$, $\\abs\\eta \\leq 1.26$, L1 matched",
			"LLP HLT passed",
            "HLT and no tower flags",
            "HLT and Depth towers $\\geq 2$",
            "HLT and Timing towers $\\geq 2$",
            "HLT and 1 timing, 1 depth tower",
		]
	
		selection_list_abbrev_noCut = [
			"All       ",
            "Jet pT and eta", 
            "No tower flags",
            "2+ depth passed",
            "2+ timing passed",
			"HLT passed",
			"HLT and no tower flags",
            "HLT and 2+ depth passed",
            "HLT and 2+ timing passed",
			"Triggered", 
			"HLT passed",
			"HLT and no tower flags",
            "HLT and 2+ depth passed",
            "HLT and 2+ timing passed",
			"Not triggered", 
			"HLT passed",
			"HLT and no tower flags",
            "HLT and 2+ depth passed",
            "HLT and 2+ timing passed",
			"L1 matched", 
			"HLT passed",
			"HLT and no tower flags",
            "HLT and 2+ depth passed",
            "HLT and 2+ timing passed",
		]
		
		if print_latex:
			jetSplit_event_latex_setup(file_path)

		init = -1

		file = ROOT.TFile.Open(file_path)
		tree = file.Get("WPlusJets")

		for i in range(len(selection_list_noCut)):
			selname = selection_list_noCut[i]
			selval  = -1
			Nevents = -1

			if i == 0: 
				init = tree.GetEntries()
				total_selection_string = ""
			if i == 1 or i == 6: total_selection_string = jet_string
			if i == 2 or i == 7: total_selection_string = jet_string_noFlag
			if i == 3 or i == 8: total_selection_string = one_jet_tagged_string
			if i == 4 or i == 9: total_selection_string = two_jet_tagged_string
			if i == 5 or i == 10: total_selection_string = three_jet_tagged_string

			if i == 11 or i == 12: total_selection_string = jet_string_triggered
			if i == 13: total_selection_string = jet_string_triggered_noFlag
			if i == 14: total_selection_string = one_jet_tagged_string_triggered
			if i == 15: total_selection_string = two_jet_tagged_string_triggered
			if i == 16: total_selection_string = three_jet_tagged_string_triggered

			if i == 17 or i == 18: total_selection_string = jet_string_not_triggered
			if i == 19: total_selection_string = jet_string_not_triggered_noFlag
			if i == 20: total_selection_string = one_jet_tagged_string_not_triggered
			if i == 21: total_selection_string = two_jet_tagged_string_not_triggered
			if i == 22: total_selection_string = three_jet_tagged_string_not_triggered

			if i == 23 or i == 24: total_selection_string = jet_string_matched
			if i == 25: total_selection_string = jet_string_matched_noFlag
			if i == 26: total_selection_string = one_jet_tagged_string_matched
			if i == 27: total_selection_string = two_jet_tagged_string_matched
			if i == 28: total_selection_string = three_jet_tagged_string_matched

			if i >= 6 and i != 11 and i != 17 and i != 23: total_selection_string += " && " + HLT_string # i == 11, 17, 23 are for triggered / not, without HLT requirement

			selval = tree.GetEntries(total_selection_string)
			if i == 1 or i == 11 or i == 17 or i == 23: valid_events = selval # all valid events, triggered valid events, non-triggered valid events
			if (i % 5 == 2 and i <= 7) or i == 13 or i == 19 or i == 25: all_events = selval # i == 1, 6, 11, 17 # do comparison (denominator) to all LLP events and valid events both
			if (i % 5 == 3 and i <= 8) or i == 14 or i == 20 or i == 26: one_plus_jets = selval # i == 2, 7, 13, 19
			if (i % 5 == 4 and i <= 9) or i == 15 or i == 21 or i == 27: two_plus_jets = selval # i == 3
			if (i % 5 == 0 and i <= 10) or i == 16 or i == 22 or i == 28: three_plus_jets = selval # i == 3

			Nevents = tree.GetEntries()

			if print_latex:
				if i == 0: print(selname+" &", round(selval, 4), "&\\multicolumn{2}{l}{", round(selval/init, 4), "} \\\\ ")
				if (i % 5 == 1 and i <= 11) or i == 12 or i == 17 or i == 18 or i == 23 or i == 24: print(selname+" &", round(selval, 4), " &", round(selval/init, 4), " &", round(selval/valid_events, 4), " \\\\ ")
				if (i % 5 == 2 and i <= 7) or i == 13 or i == 19 or i == 25: print(selname+" & ", round(all_events, 4), " &", round((all_events)/init, 4), " &", round((all_events)/valid_events, 4), " \\\\ ") # 0 bin is all events - events with at least 1
				if (i % 5 == 3 and i <= 8) or i == 14 or i == 20 or i == 26: print(selname+" & ", round(one_plus_jets, 4), " &", round((one_plus_jets)/init, 4), " &", round((one_plus_jets)/valid_events, 4), " \\\\ ") # 1 bin is events with at least 1 - events with at least 2
				if (i % 5 == 4 and i <= 9) or i == 15 or i == 21 or i == 27: print(selname+" & ", round(two_plus_jets, 4), " &", round((two_plus_jets)/init, 4), " &", round((two_plus_jets)/valid_events, 4), " \\\\ ") # 2 bin events with at least 2
				if (i % 5 == 0 and i <= 10) or i == 16 or i == 22 or i == 28: 
					if i >= 4: print(selname+" & ", round(three_plus_jets, 4), " & ", round(three_plus_jets/init, 4), " & ", round(three_plus_jets/valid_events, 4), " \\\\ ") # 3+ bin is events with at least 3
					if i != 23 and i < 28: print("\\hline")
					if i == 28: latex_end(file_path)

			else:
				print(selection_list_abbrev_noCut[i], "\t", Nevents, "\t", round(selval, 4), "\t", round(selval/init, 4))

		# Cutflow table for fraction of events passing DNN that have HLT set
		print(" \n")
		print("Signal cutflow, HLT check, done per event")
		print(" \n")
		selection_list_noCut = [
			"All", 
            "Jet $\\geq "+jet_energy+"$~GeV $p_T$ and $\\abs\\eta \\leq 1.26$", 
			"Event HT $\\geq 200$ GeV",
            "No tower flags",
            "2+ depth towers passed",
			"2+ depth towers and HCAL based HLT passed",
            "2+ timing towers passed",
			"2+ timing towers and HCAL based HLT passed",		
            "1 timing, 1 depth tower passed",
			"1 timing, 1 depth tower and HCAL based HLT passed",		
		]

		selection_list_abbrev_noCut = [
			"All       ",
            "Jet pT and eta",
			"Event HT",
            "No tower flags",
            "2+ depth passed",
			"and HLT passed",
			"2+ timing passed",
			"and HLT passed",
			"1 timing, 1 depth passed",
			"and HLT passed",
		]
		
		if print_latex:
			event_latex_setup(file_path)

		init = -1
		
		for i in range(len(selection_list_noCut)):
			selname = selection_list_noCut[i]
			selval  = -1
			Nevents = -1

			if i == 0: 
				init = tree.GetEntries()
				total_selection_string = ""
			if i == 1: total_selection_string = jet_string
			if i == 2: total_selection_string += " && eventHT > 200" # slight difference caused by addition of this as compared to above HLT cutflow
			if i == 3: total_selection_string = jet_string_noFlag + " && eventHT > 200" # slight difference caused by addition of this as compared to above HLT cutflow
			if i == 4: 
				total_selection_string = jet_string + " && eventHT > 200"
				total_selection_string += " && " + one_jet_tagged_string
				two_selection_string = total_selection_string + " && " + two_jet_tagged_string
				three_selection_string = total_selection_string + " && " + three_jet_tagged_string
			if i == 5: total_selection_string += " && " + HLT_string
			if i == 7: total_selection_string = two_selection_string + " && " + HLT_string
			if i == 9: total_selection_string = three_selection_string + " && " + HLT_string

			selval = tree.GetEntries(total_selection_string)
			if i == 3: all_events = selval # but end up doing comparison (denominator) to all LLP events...
			if i == 4: 
				one_plus_jets = selval
				two_plus_jets = tree.GetEntries(two_selection_string)
				three_plus_jets = tree.GetEntries(three_selection_string)

			Nevents = tree.GetEntries()

			if print_latex:
				if i <= 2: 
					print(selname+" &", round(selval, 4), " &", round(selval/init, 4), " \\\\ ")
					if i == 0: print("\\hline")
				if i == 3: print(selname+" & ", round(all_events, 4), " &", round((all_events)/init, 4), " \\\\ ") # 0 bin is all events - events with at least 1
				if i == 4: print(selname+" & ", round(one_plus_jets, 4), " &", round((one_plus_jets)/init, 4), " \\\\ ") # 1 bin is events with at least 1 
				if i == 5: print(selname+" & ", round(selval, 4), " & ", round((selval)/init, 4), " \\\\ ") 
				if i == 6: print(selname+" & ", round(two_plus_jets, 4), " &", round((two_plus_jets)/init, 4), " \\\\ ")
				if i == 7: print(selname+" & ", round(selval, 4), " & ", round(selval/init, 4), " \\\\ ") 
				if i == 8: print(selname+" & ", round(three_plus_jets, 4), " &", round((three_plus_jets)/init, 4), " \\\\ ")
				if i == 9: 
					print(selname+" & ", round(selval, 4), " & ", round(selval/init, 4), " \\\\ ") 
					latex_end(file_path)

			else:
				print(selection_list_abbrev_noCut[i], "\t", Nevents, "\t", round(selval, 4), "\t", round(selval/init, 4))



	# not edited for 3+ yet! only working with LLP MC now

	if "Run2023" in file_path:
		# Cutflow table for background estimation: basic event selection
		print(" \n")
		print("Background (W+jets, then NoSel) cutflow, done per event")
		print(" \n")

		# Check how many events in bkg have 2 jets triggered 
		event_selection_list = [
            "All events (from skim)", 
			"Jet $\\geq "+jet_energy+"$~GeV $p_T$ and $\\abs\\eta \\leq 1.26$", # (one of 6 leading)", 
			"Jet $\\geq "+jet_energy+"$~GeV $p_T$, $\\abs\\eta \\leq 1.26$, triggered",
			"2 jets $\\geq "+jet_energy+"$~GeV $p_T$, $\\abs\\eta \\leq 1.26$, triggered"
        ]
        
		event_selection_list_abbrev = [
            "All       ", 
            "Jet pT and eta", 
            "Jet eta, pT (triggered)",
            "2 jets eta, pT (triggered)"
        ]

		# trees = ["Classification"]
		trees = ["NoSel"]
		for tree_sel in trees:
			print("\n Running for: " + tree_sel + " \n")
			file = ROOT.TFile.Open(file_path)
			tree = file.Get(tree_sel)
			
			selection_string = ""
			
			if print_latex:
				jetSplit_event_latex_setup(file_path)

			init = -1
			
			for i in range(len(event_selection_list)):
				selname = event_selection_list[i]
				selval  = -1
				Nevents = -1

				if i == 0: 
					selval = tree.GetEntries()
					init = selval
				if i == 1: selection_string = jet_string
				# triggered jet
				if i == 2: selection_string = jet_string_triggered
				if i == 3: selection_string = two_jet_string_triggered

				selval = tree.GetEntries(selection_string)
				if i == 1: all_events = selval # i = 1, 5, 9

				if print_latex:
					if i == 0: print(selname+" &", round(selval, 4), "&\\multicolumn{2}{l}{", round(selval/init, 4), "} \\\\ ")
					if i >= 1: print(selname+" &", round(selval, 4), "&", round(selval/init, 4), "&", round(selval/all_events, 4), " \\\\ ")
					if i == 3: 
						print("\\hline")
						latex_end(file_path)
						

		event_selection_list = [
            "All events (from skim)", 
			"Jet $\\geq "+jet_energy+"$~GeV $p_T$ and $\\abs\\eta \\leq 1.26$", # (one of 6 leading)", 
            "No towers flagged",
			"Depth towers $\\geq 2$",
            "Timing towers $\\geq 2$",
			"Jet $\\geq "+jet_energy+"$~GeV $p_T$, $\\abs\\eta \\leq 1.26$, triggered",
            "No towers flagged",
			"Depth towers $\\geq 2$",
            "Timing towers $\\geq 2$",
			"Jet $\\geq "+jet_energy+"$~GeV $p_T$, $\\abs\\eta \\leq 1.26$, not triggered",
            "No towers flagged",
			"Depth towers $\\geq 2$",
            "Timing towers $\\geq 2$",
			"Jet $\\geq "+jet_energy+"$~GeV $p_T$, $\\abs\\eta \\leq 1.26$, L1 matched",
            "No towers flagged",
			"Depth towers $\\geq 2$",
            "Timing towers $\\geq 2$",
        ]
        
		event_selection_list_abbrev = [
            "All       ", 
            "Jet pT and eta", 
            "No towers flagged",
            "2+ depth passed",
            "2+ timing passed",
            "Jet eta, pT (triggered)",
            "No towers flagged",
            "2+ depth passed",
            "2+ timing passed",
            "Jet eta, pT (not triggered)",
            "No towers flagged",
            "2+ depth passed",
            "2+ timing passed",
            "Jet eta, pT (L1 matched)",
            "No towers flagged",
            "2+ depth passed",
            "2+ timing passed",
        ]

		# trees = ["WPlusJets", "NoSel"]
		# trees = ["Classification"]
		trees = ["NoSel"]
		for tree_sel in trees:
			print("\n Running for: " + tree_sel + " \n")
			file = ROOT.TFile.Open(file_path)
			tree = file.Get(tree_sel)
			
			selection_string = ""
			
			if print_latex:
				jetSplit_event_latex_setup(file_path)

			init = -1
			
			for i in range(len(event_selection_list)):
				selname = event_selection_list[i]
				selval  = -1
				Nevents = -1

				if tree_sel == "Classification" and i >= 4 and i % 4 == 0: # do not unblind for data!! used to be NoSel
					if i == 12: latex_end(file_path)
					continue

				if i == 0: 
					selval = tree.GetEntries()
					init = selval
				if i == 1: selection_string = jet_string
				if i == 2: selection_string = jet_string_noFlag
				if i == 3: selection_string = one_jet_tagged_string
				if i == 4: selection_string = two_jet_tagged_string
				# triggered jet
				if i == 5: selection_string = jet_string_triggered
				if i == 6: selection_string = jet_string_triggered_noFlag
				if i == 7: selection_string = one_jet_tagged_string_triggered
				if i == 8: selection_string = two_jet_tagged_string_triggered
				# Non-triggered jet
				if i == 9: selection_string = jet_string_not_triggered
				if i == 10: selection_string = jet_string_not_triggered_noFlag
				if i == 11: selection_string = one_jet_tagged_string_not_triggered
				if i == 12: selection_string = two_jet_tagged_string_not_triggered
				# L1 matched jet
				if i == 13: selection_string = jet_string_matched
				if i == 14: selection_string = jet_string_matched_noFlag
				if i == 15: selection_string = one_jet_tagged_string_matched
				if i == 16: selection_string = two_jet_tagged_string_matched

				selval = tree.GetEntries(selection_string)
				if i % 4 == 1: all_events = selval # i = 1, 5, 9, 13
				if i % 4 == 2: all_events = selval
				if i % 4 == 3: one_plus_jets = selval 
				if i % 4 == 0: two_plus_jets = selval 
				Nevents = tree.GetEntries()

				if print_latex:
					if i == 0: print(selname+" &", round(selval, 4), "&\\multicolumn{2}{l}{", round(selval/init, 4), "} \\\\ ")
					if i % 4 == 1: print(selname+" &", round(selval, 4), "&", round(selval/init, 4), "&", round(selval/all_events, 4), " \\\\ ")
					if i % 4 == 2 and tree_sel != "Classification": print(selname+" &", round(all_events, 4), "&", round((all_events)/init, 4),  "&", round((all_events)/all_events, 4), " \\\\ ") # 0 bin is all events - events with at least 1
					if i % 4 == 3: print(selname+" &", round(one_plus_jets, 4), "&", round((one_plus_jets)/init, 4),  "&", round((one_plus_jets)/all_events, 4), " \\\\ ") # 1 bin is events with at least 1 - events with at least 2
					if i >= 4 and i % 4 == 0: 
						print(selname+" &", round(two_plus_jets, 4), "&", round(two_plus_jets/init, 4),  "&", round(two_plus_jets/all_events, 4), " \\\\ ") # 2+ bin is events with at least 2
						if i != 16: print("\\hline")
						if i == 16: latex_end(file_path)

				else:
					print(selection_list_abbrev[i], "\t", Nevents, "\t", round(selval, 4), "\t", round(selval/init, 4))


		print(" \n")
		print("Background single tag estimation, done per event")
		print(" \n")
		event_selection_list = [
            "All events (2023 LLP skim)", 
            "Jet $\\geq 40$~GeV $p_T$ (one of 6 valid jets)", 
            "Jet $\\abs\\eta \\leq 1.26$", 
            "One tagged jet prediction ($p=0.0014$)",
            "Jet $\\geq 60$~GeV $p_T$ (one of 6 valid jets)", 
            "Jet $\\abs\\eta \\leq 1.26$", 
            "One tagged jet prediction ($p=0.0012$)",
        ]
        
		event_selection_list_abbrev = [
            "All       ", 
            "Jet pT", 
            "Jet eta",
            "One jet prediction",
            "Jet pT", 
            "Jet eta",
            "One jet prediction",
        ]

		file = ROOT.TFile.Open(file_path)
		# tree = file.Get("Classification")
		tree = file.Get("WPlusJets")
        
		selection_string = ""
        
		if print_latex:
			event_latex_setup(file_path)

		init = -1
        
		for i in range(len(event_selection_list)):
			selname = event_selection_list[i]
			selval  = -1
			Nevents = -1

			if i == 0: 
				selval = tree.GetEntries()
				init = selval
			if i == 1: selection_string += "(jet0_Pt >= 40 || jet1_Pt >= 40 || jet2_Pt >= 40)"
			if i == 2: selection_string += "&& ((jet0_Pt >= 40 && abs(jet0_Eta) <= 1.26) || (jet1_Pt >= 40 && abs(jet1_Eta) <= 1.26) || (jet2_Pt >= 40 && abs(jet2_Eta) <= 1.26))"
			if i == 4: selection_string = "(jet0_Pt >= 60 || jet1_Pt >= 60 || jet2_Pt >= 60)"
			if i == 5: selection_string += "&& ((jet0_Pt >= 60 && abs(jet0_Eta) <= 1.26) || (jet1_Pt >= 60 && abs(jet1_Eta) <= 1.26) || (jet2_Pt >= 60 && abs(jet2_Eta) <= 1.26))"
								
			selval = tree.GetEntries(selection_string)
			if (i == 2 or i == 5): all_events = selval
			if i == 3: selval = all_events * 0.0004
			if i == 6: selval = all_events * 0.0003

			if print_latex:
				print(selname+" &", round(selval, 4), "&", round(selval/init, 4), " \\\\ ")
				if i == 0: print("\\hline")
				if i == 3: print("\\hline")
				if i == 6: latex_end(file_path)

if __name__ == '__main__':
	main()