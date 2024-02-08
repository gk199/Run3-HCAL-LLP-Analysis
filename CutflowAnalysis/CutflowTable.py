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
		print(" & \\textbf{LLP 0} & \\textbf{LLP 1} & \\textbf{LLP 0} & \\textbf{LLP 1} \\\\ \\hline")
	else:
		print("\\begin{tabular}{l|llll}")
		print("\\hline")
		print("\\textbf{Selection} & \\multicolumn{2}{l}{\\textbf{Number of Weighted Entries}} & \\multicolumn{2}{l}{\\textbf{Fraction of Weighted Entries}} \\\\ \\hline")
		print(" & \\textbf{LLP 0} & \\textbf{LLP 1} & \\textbf{LLP 0} & \\textbf{LLP 1} \\\\ \\hline")

def bdt_latex_setup(file_path):
	print("\\begin{table}[ht]")
	print("\\centering")
	if "Run2023" in file_path: 
		print("\\begin{tabular}{l|lll}")
		print("\\hline")
		print("\\textbf{Selection} & \\textbf{Number of Entries} & \\multicolumn{2}{l}{\\textbf{Fraction of Entries}} \\\\ \\hline")
		print(" &  & Of all entries & Of jet matched entries \\\\ \\hline")
	else:
		print("\\begin{tabular}{l|lll}")
		print("\\hline")
		print("\\textbf{Selection} & \\textbf{Number of Weighted Entries} & \\multicolumn{2}{l}{\\textbf{Fraction of Weighted Entries}} \\\\ \\hline")
		print(" &  & Of all entries & Of jet matched entries \\\\ \\hline")

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
	
	# Cutflow table, for depth based and timing based, on LLP MC samples
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
		"L1 + Leading jet neutral hadron frac $>70\%$",
		"L1 + Leading jet neutral hadron frac $>80\%$",
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
		"L1 monitoring",
		"L1 + NHF 70",
		"L1 + NHF 80",
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
	
	if "Run2023" not in file_path:
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

			if (i == 0 or i == 15): 
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
				if i == 9: total_selection_string += "&& ( jet0_NeutralHadEFrac > 0.7 )"
				if i == 10: total_selection_string += "&& ( jet0_NeutralHadEFrac > 0.8 )"
				if i == 11: total_selection_string = "( HLT_HT200_L1SingleLLPJet_DisplacedDijet40_Inclusive1PtrkShortSig5 == 1 || HLT_HT240_L1SingleLLPJet_DisplacedDijet40_Inclusive1PtrkShortSig5 == 1 || HLT_HT280_L1SingleLLPJet_DisplacedDijet40_Inclusive1PtrkShortSig5 == 1)"
				if i == 12: total_selection_string = "( HLT_HT170_L1SingleLLPJet_DisplacedDijet40_DisplacedTrack == 1 || HLT_HT200_L1SingleLLPJet_DisplacedDijet40_DisplacedTrack == 1 || HLT_HT270_L1SingleLLPJet_DisplacedDijet40_DisplacedTrack == 1 || HLT_HT200_L1SingleLLPJet_DisplacedDijet60_DisplacedTrack == 1 )"
				if i == 13: total_selection_string = "( HLT_HT320_L1SingleLLPJet_DisplacedDijet60_Inclusive == 1 || HLT_HT420_L1SingleLLPJet_DisplacedDijet60_Inclusive == 1 )"
				if i == 14: total_selection_string = "( HLT_HT200_L1SingleLLPJet_DelayedJet40_DoubleDelay0p5nsTrackless == 1 || HLT_HT200_L1SingleLLPJet_DelayedJet40_DoubleDelay1nsInclusive == 1 || HLT_HT200_L1SingleLLPJet_DelayedJet40_SingleDelay1nsTrackless == 1 || HLT_HT200_L1SingleLLPJet_DelayedJet40_SingleDelay2nsInclusive == 1 )"

				# second table, travel time focused
				if i == 16: selection_string = "(LLP0_TravelTime > 6 && LLP0_DecayR < 295)"
				if i == 17: selection_string += "&& abs(LLP0_Eta) <= 1.26"
				if i == 18: selection_string += "&& LLP0_E > 60"

				if i == 16: selection1_string = "(LLP1_TravelTime > 6 && LLP1_DecayR < 295)"
				if i == 17: selection1_string += "&& abs(LLP1_Eta) <= 1.26"
				if i == 18: selection1_string += "&& LLP1_E > 60"

				if i == 19: total_selection_string = "(" + selection_string + ") || (" + selection1_string + ")"
				if i == 20: total_selection_string = "(" + selection_string + " && " + selection1_string + ")"
				if i == 21: total_selection_string = "((" + selection_string + ") || (" + selection1_string + ")) && eventHT > 200"
				if i == 22: total_selection_string = "(" + selection_string + " && " + selection1_string + ") && eventHT > 200"

				selval = tree.GetEntries(selection_string)
				selval1 = tree.GetEntries(selection1_string)

				if ((i >= 4 and i <= 14) or i >= 19): selval = tree.GetEntries(total_selection_string)

				Nevents = tree.GetEntries()

			if print_latex:
				if (i < 4 or (i > 14 and i < 19)): print(selname+" &", round(selval, 4), "&", round(selval1, 4), "&", round(selval/init, 4), "&", round(selval1/init, 4), "\\\\")
				if (i == 4 or i == 19): print("\\hline")
				if ((i >= 4 and i <= 14 ) or i >= 19): print(selname+" & \\multicolumn{2}{l}{", round(selval, 4), "} & \\multicolumn{2}{l}{", round(selval/init, 4), "} \\\\ ")
				if (i == 7): print("\\hline")
				if (i == 14):
					latex_end(file_path)
					latex_setup(file_path)
				if (i == 22):
					latex_end(file_path)

			else:
				if (i < 4): print(selection_list_abbrev[i], "\t", Nevents, "\t", round(selval, 4), "\t", round(selval/init, 4), "LLP 1:", "\t", round(selval1, 4), "\t", round(selval1/init, 4))
				if (i >= 4): print(selection_list_abbrev[i], "\t", Nevents, "\t", round(selval, 4), "\t", round(selval/init, 4))

	# Cutflow table for comparison with BDT results
	print(" \n")
	print("BDT cutflow")
	print(" \n")
	BDT_selection_list = [
		"All", 
		"Jet $\\eta \\leq 1.26$ and $p_T \\geq 40$", 
		"Jet matched to LLP", 
		"Neutral hadron fraction $>80\%$", 
		"Charged hadron fraction $<10\%$", 
		"$S_{\\phi\\phi} < 0.02$",
		"Track $\\abs{\\Delta \\eta} > 0.05$",
		"Track $\\abs{\\Delta \\phi} > 0.05$",
		"Leading rechit energy $> 8$~GeV",
	]
	
	BDT_selection_list_abbrev = [
		"All       ", 
		"Jet position",
		"Jet matched",
		"NHF",
		"CHF",
		"S_phiphi",
		"Track dEta",
		"Track dPhi",
		"RechitE",
	]
	
	file = ROOT.TFile.Open(file_path)
	tree = file.Get("NoSel")
	if "Run2023" in file_path: tree = file.Get("WPlusJets")
	
	selection_string = ""
	
	if print_latex:
		bdt_latex_setup(file_path)

	init = -1
	comp = -1
	
	for i in range(len(BDT_selection_list)):
		selname = BDT_selection_list[i]
		selval  = -1
		selval1 = -1
		Nevents = -1

		if (i == 0): 
			selval = tree.GetEntries()
			init = selval
		else:
			if i == 1: selection_string += "(abs(jet0_Eta) <= 1.26 && jet0_Pt > 40)"
			if i == 2 and "Run2023" not in file_path: selection_string += "&& ((LLP0_DecayR >= 183.6 && LLP0_DecayR < 295 && abs(LLP0_Eta) <= 1.26 && jet0_isMatchedTo == 0) || (LLP1_DecayR >= 183.6 && LLP1_DecayR < 295 && abs(LLP1_Eta) <= 1.26 && jet0_isMatchedTo == 1))"
			if i == 3: selection_string += "&& ( jet0_NeutralHadEFrac > 0.8 )"
			if i == 4: selection_string += "&& ( jet0_ChargedHadEFrac < 0.1 )"
			if i == 5: selection_string += "&& ( jet0_S_phiphi < 0.02 ) "
			if i == 6: selection_string += "&& ( abs(jet0_Track0dEta) > 0.05 )"
			if i == 7: selection_string += "&& ( abs(jet0_Track0dPhi) > 0.05 )"
			if i == 8: selection_string += "&& ( jet0_LeadingRechitE > 8 )"

			selval = tree.GetEntries(selection_string)
			if (i == 2): comp = selval
			Nevents = tree.GetEntries()

		if print_latex:
			if (i <= 2): print(selname+" &", round(selval, 4), "&", round(selval/init, 4), " & \\\\ ")
			if (i == 2): print("\\hline")
			if (i > 2): print(selname+" &", round(selval, 4), "&", round(selval/init, 4), " &", round(selval/comp, 4), " \\\\ ")
			if (i == 8):
				latex_end(file_path)

		else:
			if (i < 2): print(selection_list_abbrev[i], "\t", Nevents, "\t", round(selval, 4), "\t", round(selval/init, 4))
			if (i >= 2): print(selection_list_abbrev[i], "\t", Nevents, "\t", round(selval, 4), "\t", round(selval/init, 4))


	# Cutflow table for comparison with BDT results -- per jet
	print(" \n")
	print("BDT cutflow, done per jet")
	print(" \n")
	BDT_selection_list = [
		"All (jet $\\eta \\leq 1.26$ and $p_T \\geq 40$)", 
		"Jet matched to LLP in HCAL", 
		"Neutral hadron fraction $>80\%$", 
		"Charged hadron fraction $<10\%$", 
		"$S_{\\phi\\phi} < 0.02$",
		"Track $\\abs{\\Delta \\eta} > 0.05$",
		"Track $\\abs{\\Delta \\phi} > 0.05$",
		"Leading rechit energy $> 8$~GeV",
	]
	
	BDT_selection_list_abbrev = [
		"All       ", 
		"LLP in HCAL", 
		"NHF",
		"CHF",
		"S_phiphi",
		"Track dEta",
		"Track dPhi",
		"RechitE",
	]
	
	file = ROOT.TFile.Open(file_path)
	tree = file.Get("PerJet_LLPmatched")
	if "Run2023" in file_path: tree = file.Get("PerJet_WPlusJets")
	
	selection_string = ""
	
	if print_latex:
		bdt_latex_setup(file_path)

	init = -1
	comp = -1
	
	for i in range(len(BDT_selection_list)):
		selname = BDT_selection_list[i]
		selval  = -1
		Nevents = -1

		if (i == 0): 
			selval = tree.GetEntries()
			init = selval
		else:
			if i == 1 and "Run2023" not in file_path: selection_string += "(perJet_MatchedLLP_DecayR >= 183.6 && perJet_MatchedLLP_DecayR < 295 && abs(perJet_MatchedLLP_Eta) < 1.26)"
			if i == 2: 
				if "Run2023" not in file_path: selection_string += "&& "
				selection_string += "( perJet_NeutralHadEFrac > 0.8 )"
			if i == 3: selection_string += "&& ( perJet_ChargedHadEFrac < 0.1 )"
			if i == 4: selection_string += "&& ( perJet_S_phiphi < 0.02 ) "
			if i == 5: selection_string += "&& ( abs(perJet_Track0dEta) > 0.05 )"
			if i == 6: selection_string += "&& ( abs(perJet_Track0dPhi) > 0.05 )"
			if i == 7: selection_string += "&& ( perJet_LeadingRechitE > 8 )"

			selval = tree.GetEntries(selection_string)
			if (i == 1): comp = selval
			Nevents = tree.GetEntries()

		if print_latex:
			if (i <= 1): print(selname+" &", round(selval, 4), "&", round(selval/init, 4), " & \\\\ ")
			if (i == 1): print("\\hline")
			if (i > 1): print(selname+" &", round(selval, 4), "&", round(selval/init, 4), " &", round(selval/comp, 4), " \\\\ ")
			if (i == 7):
				latex_end(file_path)

		else:
			if (i < 2): print(selection_list_abbrev[i], "\t", Nevents, "\t", round(selval, 4), "\t", round(selval/init, 4))
			if (i >= 2): print(selection_list_abbrev[i], "\t", Nevents, "\t", round(selval, 4), "\t", round(selval/init, 4))


if __name__ == '__main__':
	main()