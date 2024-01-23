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
	if "data" in file_path: 
		print("\\begin{tabular}{l|llll}")
		print("\\hline")
		print("\\textbf{Selection} & \\multicolumn{2}{l}{\\textbf{Number of Entries}} & \\multicolumn{2}{l}{\\textbf{Fraction of Entries}} \\\\ \\hline")
		print(" & \\textbf{LLP 0} & \\textbf{LLP 1} & \\textbf{LLP 0} & \\textbf{LLP 1} \\\\ \\hline")
	else:
		print("\\begin{tabular}{l|llll}")
		print("\\hline")
		print("\\textbf{Selection} & \\multicolumn{2}{l}{\\textbf{Number of Weighted Entries}} & \\multicolumn{2}{l}{\\textbf{Fraction of Weighted Entries}} \\\\ \\hline")
		print(" & \\textbf{LLP 0} & \\textbf{LLP 1} & \\textbf{LLP 0} & \\textbf{LLP 1} \\\\ \\hline")

def latex_end(file_path):
	print("\\hline")
	print("\\end{tabular}")
	print("\\caption{\\url{" + file_path + "}}") # to save which file this cutflow is from
	print("\\label{8-Table:CutFlow}")
	print("\\end{table}")

# ------------------------------------------------------------------------------
def main():
	file_path = sys.argv[1]
	#tree     = sys.argv[2]
	#weight   = sys.argv[3]	
	
	selection_list = [
		"All", 
		"LLP $r$ in HCAL depth 3 or 4", 
		"LLP $\\eta \\leq 1.26$", 
		"LLP $E > 60$~GeV", 
		"LLP 0 OR LLP 1",
		"LLP 0 AND LLP 1",
		"LLP OR, with Event HT $> 200$~GeV", 
		"LLP AND, with Event HT $> 200$~GeV", 
		"L1 passed (monitoring path)",
		"HLT dijet, $\\leq$1 prompt track",
		"HLT dijet, 1 displaced track",
		"HLT dijet, $\\leq$2 prompt track",
		"HLT delayed jet, ECAL timing",
		"All", 
		"LLP $t > 6$~ns and $r < 295$~cm", 
		"LLP $\\eta \\leq 1.26$", 
		"LLP $E > 60$~GeV", 
		"LLP 0 OR LLP 1",
		"LLP 0 AND LLP 1",
		"LLP OR, with Event HT $> 200$~GeV", 
		"LLP AND, with Event HT $> 200$~GeV", 
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
		"HLT dijet, 1 prompt trk",
		"HLT dijet, displaced trk",
		"HLT dijet, 2 prompt trk",
		"HLT delayed jet, ECAL",
		"All       ", 
		"LLP time  ", 
		"LLP eta   ", 
		"LLP pT    ", 
		"LLP OR	   ", 
		"LLP AND   ",
		"LLP OR with HTT", 
		"LLP AND with HTT",
	]
	
	file = ROOT.TFile.Open(file_path)
	# hist = file.Get("Cutflow_Medium")
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

		if (i == 0 or i == 13): 
			selval = tree.GetEntries()
			selval1 = tree.GetEntries()
			init = selval
		else:
			if i == 1: selection_string += "(LLP0_DecayR >= 214.2 && LLP0_DecayR < 295)"
			if i == 2: selection_string += "&& abs(LLP0_Eta) <= 1.26"
			if i == 3: selection_string += "&& LLP0_E > 60"

			if i == 1: selection1_string += "(LLP1_DecayR >= 214.2 && LLP1_DecayR < 295)"
			if i == 2: selection1_string += "&& abs(LLP1_Eta) <= 1.26"
			if i == 3: selection1_string += "&& LLP1_E > 60"

			if i == 4: total_selection_string = "(" + selection_string + ") || (" + selection1_string + ")"
			if i == 5: total_selection_string = "(" + selection_string + " && " + selection1_string + ")"
			if i == 6: total_selection_string = "((" + selection_string + ") || (" + selection1_string + ")) && eventHT > 200"
			if i == 7: total_selection_string = "(" + selection_string + " && " + selection1_string + ") && eventHT > 200"

			if i == 8: total_selection_string = "( HLT_L1SingleLLPJet == 1 )"
			if i == 9: total_selection_string = "( HLT_HT200_L1SingleLLPJet_DisplacedDijet40_Inclusive1PtrkShortSig5 == 1 || HLT_HT240_L1SingleLLPJet_DisplacedDijet40_Inclusive1PtrkShortSig5 == 1 || HLT_HT280_L1SingleLLPJet_DisplacedDijet40_Inclusive1PtrkShortSig5 == 1)"
			if i == 10: total_selection_string = "( HLT_HT170_L1SingleLLPJet_DisplacedDijet40_DisplacedTrack == 1 || HLT_HT200_L1SingleLLPJet_DisplacedDijet40_DisplacedTrack == 1 || HLT_HT270_L1SingleLLPJet_DisplacedDijet40_DisplacedTrack == 1 || HLT_HT200_L1SingleLLPJet_DisplacedDijet60_DisplacedTrack == 1 )"
			if i == 11: total_selection_string = "( HLT_HT320_L1SingleLLPJet_DisplacedDijet60_Inclusive == 1 || HLT_HT420_L1SingleLLPJet_DisplacedDijet60_Inclusive == 1 )"
			if i == 12: total_selection_string = "( HLT_HT200_L1SingleLLPJet_DelayedJet40_DoubleDelay0p5nsTrackless == 1 || HLT_HT200_L1SingleLLPJet_DelayedJet40_DoubleDelay1nsInclusive == 1 || HLT_HT200_L1SingleLLPJet_DelayedJet40_SingleDelay1nsTrackless == 1 || HLT_HT200_L1SingleLLPJet_DelayedJet40_SingleDelay2nsInclusive == 1 )"

			if i == 14: selection_string = "(LLP0_TravelTime > 6 && LLP0_DecayR < 295)"
			if i == 15: selection_string += "&& abs(LLP0_Eta) <= 1.26"
			if i == 16: selection_string += "&& LLP0_E > 60"

			if i == 14: selection1_string = "(LLP1_TravelTime > 6 && LLP1_DecayR < 295)"
			if i == 15: selection1_string += "&& abs(LLP1_Eta) <= 1.26"
			if i == 16: selection1_string += "&& LLP1_E > 60"

			if i == 17: total_selection_string = "(" + selection_string + ") || (" + selection1_string + ")"
			if i == 18: total_selection_string = "(" + selection_string + " && " + selection1_string + ")"
			if i == 19: total_selection_string = "((" + selection_string + ") || (" + selection1_string + ")) && eventHT > 200"
			if i == 20: total_selection_string = "(" + selection_string + " && " + selection1_string + ") && eventHT > 200"

			selval = tree.GetEntries(selection_string)
			selval1 = tree.GetEntries(selection1_string)

			if ((i >= 4 and i <= 12) or i >= 17): selval = tree.GetEntries(total_selection_string)

			Nevents = tree.GetEntries()

		if print_latex:
			if (i < 4 or (i > 12 and i < 17)): print(selname+" &", round(selval, 4), "&", round(selval1, 4), "&", round(selval/init, 4), "&", round(selval1/init, 4), "\\\\")
			if (i == 4 or i == 17): print("\\hline")
			if ((i >= 4 and i <= 12 ) or i >= 17): print(selname+" & \\multicolumn{2}{l}{", round(selval, 4), "} & \\multicolumn{2}{l}{", round(selval/init, 4), "} \\\\ ")
			if (i == 7): print("\\hline")
			if (i == 12):
				latex_end(file_path)
				latex_setup(file_path)
			if (i == 20):
				latex_end(file_path)

		else:
			if (i < 4): print(selection_list_abbrev[i], "\t", Nevents, "\t", round(selval, 4), "\t", round(selval/init, 4), "LLP 1:", "\t", round(selval1, 4), "\t", round(selval1/init, 4))
			if (i >= 4): print(selection_list_abbrev[i], "\t", Nevents, "\t", round(selval, 4), "\t", round(selval/init, 4))

if __name__ == '__main__':
	main()