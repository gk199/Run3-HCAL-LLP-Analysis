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
		print("\\textbf{Selection} & \\multicolumn{2}{l}{\\textbf{Number of Weighted Entries}} & \\multicolumn{2}{l}{\\textbf{Fraction of Weighted Entries}} \\\\ \\hline")
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
		print("\\textbf{Selection} & \\textbf{Number of Weighted Entries} & \\multicolumn{2}{l}{\\textbf{Fraction of Weighted Entries}} \\\\ \\hline")
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
		print("\\textbf{Selection} & \\textbf{Number of Weighted Entries} & \\textbf{Fraction of Weighted Entries} \\\\ \\hline \\hline")

def latex_end(file_path):
    print("\\hline")
    print("\\end{tabular}")
    print("\\caption{\\protect\\url{" + file_path + "}}") # to save which file this cutflow is from
    print("\\label{9-Table:BkgEst}")
    print("\\end{table}")

# ------------------------------------------------------------------------------
def main():
	file_path = sys.argv[1]
	#tree     = sys.argv[2]
	#weight   = sys.argv[3]	

	jet_energy = sys.argv[2]
	
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
            "0 jets with BDT score $\\geq 0.99$",
            "1 jet with BDT score $\\geq 0.99$",
            "2 jets with BDT scores $\\geq 0.99$",
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
            "No jet BDT passed",
            "Jet BDT passed",
            "Two jet BDT passed",
		]
	
		file = ROOT.TFile.Open(file_path)
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
			else:
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
				if i == 7: total_selection_string = "(" + selection_string + " && " + selection1_string + ") && eventHT > 200"

				if i == 8: 
					total_selection_string += "&& (( jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_bdtscoreX_LLP350_MS80_perJet >= 0.99)" # jet 0 condition
					total_selection_string += " || ( jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26 && jet1_bdtscoreX_LLP350_MS80_perJet >= 0.99)" # jet 1 condition
					total_selection_string += " || ( jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26 && jet2_bdtscoreX_LLP350_MS80_perJet >= 0.99)" # jet 2 condition
					total_selection_string += " || ( jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26 && jet3_bdtscoreX_LLP350_MS80_perJet >= 0.99)" # jet 3 condition
					total_selection_string += " || ( jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26 && jet4_bdtscoreX_LLP350_MS80_perJet >= 0.99)" # jet 4 condition
					total_selection_string += " || ( jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26 && jet5_bdtscoreX_LLP350_MS80_perJet >= 0.99))" # jet 5 condition
				if i == 9: 
					total_selection_string += "&& ((( jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_bdtscoreX_LLP350_MS80_perJet >= 0.99) && ( jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26 && jet1_bdtscoreX_LLP350_MS80_perJet >= 0.99))" # jet 0 AND 1 condition
					total_selection_string += " || (( jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_bdtscoreX_LLP350_MS80_perJet >= 0.99) && ( jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26 && jet2_bdtscoreX_LLP350_MS80_perJet >= 0.99))" # jet 0 AND 2 condition
					total_selection_string += " || (( jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_bdtscoreX_LLP350_MS80_perJet >= 0.99) && ( jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26 && jet3_bdtscoreX_LLP350_MS80_perJet >= 0.99))" # jet 0 AND 3 condition
					total_selection_string += " || (( jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_bdtscoreX_LLP350_MS80_perJet >= 0.99) && ( jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26 && jet4_bdtscoreX_LLP350_MS80_perJet >= 0.99))" # jet 0 AND 4 condition
					total_selection_string += " || (( jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_bdtscoreX_LLP350_MS80_perJet >= 0.99) && ( jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26 && jet5_bdtscoreX_LLP350_MS80_perJet >= 0.99))" # jet 0 AND 5 condition
					total_selection_string += " || (( jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26 && jet1_bdtscoreX_LLP350_MS80_perJet >= 0.99) && ( jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26 && jet2_bdtscoreX_LLP350_MS80_perJet >= 0.99))" # jet 1 AND 2 condition
					total_selection_string += " || (( jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26 && jet1_bdtscoreX_LLP350_MS80_perJet >= 0.99) && ( jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26 && jet3_bdtscoreX_LLP350_MS80_perJet >= 0.99))"
					total_selection_string += " || (( jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26 && jet1_bdtscoreX_LLP350_MS80_perJet >= 0.99) && ( jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26 && jet4_bdtscoreX_LLP350_MS80_perJet >= 0.99))"	
					total_selection_string += " || (( jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26 && jet1_bdtscoreX_LLP350_MS80_perJet >= 0.99) && ( jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26 && jet5_bdtscoreX_LLP350_MS80_perJet >= 0.99))"				
					total_selection_string += " || (( jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26 && jet2_bdtscoreX_LLP350_MS80_perJet >= 0.99) && ( jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26 && jet3_bdtscoreX_LLP350_MS80_perJet >= 0.99))" # jet 2 AND 3 condition
					total_selection_string += " || (( jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26 && jet2_bdtscoreX_LLP350_MS80_perJet >= 0.99) && ( jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26 && jet4_bdtscoreX_LLP350_MS80_perJet >= 0.99))"
					total_selection_string += " || (( jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26 && jet2_bdtscoreX_LLP350_MS80_perJet >= 0.99) && ( jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26 && jet5_bdtscoreX_LLP350_MS80_perJet >= 0.99))"
					total_selection_string += " || (( jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26 && jet3_bdtscoreX_LLP350_MS80_perJet >= 0.99) && ( jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26 && jet4_bdtscoreX_LLP350_MS80_perJet >= 0.99))" # jet 3 AND 4 condition
					total_selection_string += " || (( jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26 && jet3_bdtscoreX_LLP350_MS80_perJet >= 0.99) && ( jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26 && jet5_bdtscoreX_LLP350_MS80_perJet >= 0.99))"
					total_selection_string += " || (( jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26 && jet4_bdtscoreX_LLP350_MS80_perJet >= 0.99) && ( jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26 && jet5_bdtscoreX_LLP350_MS80_perJet >= 0.99)))" # jet 4 AND 5 condition

				selval = tree.GetEntries(selection_string)
				selval1 = tree.GetEntries(selection1_string)

				if i >= 4: selval = tree.GetEntries(total_selection_string)
				if i == 7: all_events = selval
				if i == 8: one_plus_jets = selval
				if i == 9: two_plus_jets = selval

				Nevents = tree.GetEntries()

			if print_latex:
				if i < 4: print(selname+" &", round(selval, 4), "&", round(selval1, 4), "&", round(selval/init, 4), "&", round(selval1/init, 4), "\\\\")
				if i == 4: print("\\hline")
				if (i >= 4 and i < 8): print(selname+" & \\multicolumn{2}{l}{", round(selval, 4), "} & \\multicolumn{2}{l}{", round(selval/init, 4), "} \\\\ ")
				if i == 7: print("\\hline")
				if i == 8: print(selname+" & \\multicolumn{2}{l}{", round(all_events - one_plus_jets, 4), "} & \\multicolumn{2}{l}{", round((all_events - one_plus_jets)/init, 4), "} \\\\ ") # 0 bin is all events - events with at least 1
				if i == 9: print(selname+" & \\multicolumn{2}{l}{", round(one_plus_jets - two_plus_jets, 4), "} & \\multicolumn{2}{l}{", round((one_plus_jets - two_plus_jets)/init, 4), "} \\\\ ") # 1 bin is events with at least 1 - events with at least 2
				if i == 10: 
					print(selname+" & \\multicolumn{2}{l}{", round(two_plus_jets, 4), "} & \\multicolumn{2}{l}{", round(two_plus_jets/init, 4), "} \\\\ ") # 2+ bin is events with at least 2
					latex_end(file_path)

			else:
				if (i < 4): print(selection_list_abbrev[i], "\t", Nevents, "\t", round(selval, 4), "\t", round(selval/init, 4), "LLP 1:", "\t", round(selval1, 4), "\t", round(selval1/init, 4))
				if (i >= 4): print(selection_list_abbrev[i], "\t", Nevents, "\t", round(selval, 4), "\t", round(selval/init, 4))

		# Cutflow table for background estimation: basic event selection
		print(" \n")
		print("Signal cutflow, without truth cuts, done per event")
		print(" \n")
		selection_list_noCut = [
			"All", 
            "Jet $\\geq "+jet_energy+"$~GeV $p_T$ (one of 3 leading)", 
            "Jet $\\abs\\eta \\leq 1.26$", 
            "0 jets with BDT score $\\geq 0.99$",
            "1 jet with BDT score $\\geq 0.99$",
            "2 jets with BDT scores $\\geq 0.99$",
		]
	
		selection_list_abbrev_noCut = [
			"All       ",
            "Jet pT", 
            "Jet eta",
            "No jet BDT passed",
            "Jet BDT passed",
            "Two jet BDT passed",
		]
		
		if print_latex:
			event_latex_setup(file_path)

		init = -1
		
		for i in range(len(selection_list_noCut)):
			selname = selection_list_noCut[i]
			selval  = -1
			Nevents = -1

			if i == 0: 
				selval = tree.GetEntries()
				init = selval
			else:
				if i == 1: total_selection_string = "(jet0_Pt >= "+jet_energy+" || jet1_Pt >= "+jet_energy+" || jet2_Pt >= "+jet_energy+")"
				if i == 2: total_selection_string += "&& ((jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26) || (jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26) || (jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26))"
				if i == 3: 
					total_selection_string += "&& (( jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_bdtscoreX_LLP350_MS80_perJet >= 0.99)" # jet 0 condition
					total_selection_string += " || ( jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26 && jet1_bdtscoreX_LLP350_MS80_perJet >= 0.99)" # jet 1 condition
					total_selection_string += " || ( jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26 && jet2_bdtscoreX_LLP350_MS80_perJet >= 0.99)" # jet 2 condition
					total_selection_string += " || ( jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26 && jet3_bdtscoreX_LLP350_MS80_perJet >= 0.99)" # jet 3 condition
					total_selection_string += " || ( jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26 && jet4_bdtscoreX_LLP350_MS80_perJet >= 0.99)" # jet 4 condition
					total_selection_string += " || ( jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26 && jet5_bdtscoreX_LLP350_MS80_perJet >= 0.99))" # jet 5 condition
				if i == 4: 
					total_selection_string += "&& ((( jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_bdtscoreX_LLP350_MS80_perJet >= 0.99) && ( jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26 && jet1_bdtscoreX_LLP350_MS80_perJet >= 0.99))" # jet 0 AND 1 condition
					total_selection_string += " || (( jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_bdtscoreX_LLP350_MS80_perJet >= 0.99) && ( jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26 && jet2_bdtscoreX_LLP350_MS80_perJet >= 0.99))" # jet 0 AND 2 condition
					total_selection_string += " || (( jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_bdtscoreX_LLP350_MS80_perJet >= 0.99) && ( jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26 && jet3_bdtscoreX_LLP350_MS80_perJet >= 0.99))" # jet 0 AND 3 condition
					total_selection_string += " || (( jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_bdtscoreX_LLP350_MS80_perJet >= 0.99) && ( jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26 && jet4_bdtscoreX_LLP350_MS80_perJet >= 0.99))" # jet 0 AND 4 condition
					total_selection_string += " || (( jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_bdtscoreX_LLP350_MS80_perJet >= 0.99) && ( jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26 && jet5_bdtscoreX_LLP350_MS80_perJet >= 0.99))" # jet 0 AND 5 condition
					total_selection_string += " || (( jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26 && jet1_bdtscoreX_LLP350_MS80_perJet >= 0.99) && ( jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26 && jet2_bdtscoreX_LLP350_MS80_perJet >= 0.99))" # jet 1 AND 2 condition
					total_selection_string += " || (( jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26 && jet1_bdtscoreX_LLP350_MS80_perJet >= 0.99) && ( jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26 && jet3_bdtscoreX_LLP350_MS80_perJet >= 0.99))"
					total_selection_string += " || (( jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26 && jet1_bdtscoreX_LLP350_MS80_perJet >= 0.99) && ( jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26 && jet4_bdtscoreX_LLP350_MS80_perJet >= 0.99))"	
					total_selection_string += " || (( jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26 && jet1_bdtscoreX_LLP350_MS80_perJet >= 0.99) && ( jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26 && jet5_bdtscoreX_LLP350_MS80_perJet >= 0.99))"				
					total_selection_string += " || (( jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26 && jet2_bdtscoreX_LLP350_MS80_perJet >= 0.99) && ( jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26 && jet3_bdtscoreX_LLP350_MS80_perJet >= 0.99))" # jet 2 AND 3 condition
					total_selection_string += " || (( jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26 && jet2_bdtscoreX_LLP350_MS80_perJet >= 0.99) && ( jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26 && jet4_bdtscoreX_LLP350_MS80_perJet >= 0.99))"
					total_selection_string += " || (( jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26 && jet2_bdtscoreX_LLP350_MS80_perJet >= 0.99) && ( jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26 && jet5_bdtscoreX_LLP350_MS80_perJet >= 0.99))"
					total_selection_string += " || (( jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26 && jet3_bdtscoreX_LLP350_MS80_perJet >= 0.99) && ( jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26 && jet4_bdtscoreX_LLP350_MS80_perJet >= 0.99))" # jet 3 AND 4 condition
					total_selection_string += " || (( jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26 && jet3_bdtscoreX_LLP350_MS80_perJet >= 0.99) && ( jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26 && jet5_bdtscoreX_LLP350_MS80_perJet >= 0.99))"
					total_selection_string += " || (( jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26 && jet4_bdtscoreX_LLP350_MS80_perJet >= 0.99) && ( jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26 && jet5_bdtscoreX_LLP350_MS80_perJet >= 0.99)))" # jet 4 AND 5 condition

				selval = tree.GetEntries(total_selection_string)
				if i == 2: all_events = selval # but end up doing comparison (denominator) to all LLP events...
				if i == 3: one_plus_jets = selval
				if i == 4: two_plus_jets = selval

				Nevents = tree.GetEntries()

			if print_latex:
				if i <= 2: 
					print(selname+" &", round(selval, 4), " &", round(selval/init, 4), " \\\\ ")
					if i == 0: print("\\hline")
				if i == 3: print(selname+" & ", round(all_events - one_plus_jets, 4), " &", round((all_events - one_plus_jets)/init, 4), " \\\\ ") # 0 bin is all events - events with at least 1
				if i == 4: print(selname+" & ", round(one_plus_jets - two_plus_jets, 4), " &", round((one_plus_jets - two_plus_jets)/init, 4), " \\\\ ") # 1 bin is events with at least 1 - events with at least 2
				if i == 5: 
					print(selname+" & ", round(two_plus_jets, 4), " & ", round(two_plus_jets/init, 4), " \\\\ ") # 2+ bin is events with at least 2
					latex_end(file_path)

			else:
				print(selection_list_abbrev_noCut[i], "\t", Nevents, "\t", round(selval, 4), "\t", round(selval/init, 4))

		# Cutflow table for fraction of events passing BDT that have HLT set
		print(" \n")
		print("Signal cutflow, HLT check, done per event")
		print(" \n")
		selection_list_noCut = [
			"All", 
            "Jet $\\geq "+jet_energy+"$~GeV $p_T$ (one of 6 valid)", 
            "Jet $\\abs\\eta \\leq 1.26$", 
			"Event HT > 200 GeV",
            "0 jets with BDT score $\\geq 0.99$",
            "1+ jet with BDT score $\\geq 0.99$",
			"HCAL based HLT passed",
		]
	
		selection_list_abbrev_noCut = [
			"All       ",
            "Jet pT", 
            "Jet eta",
			"Event HT",
            "No jet BDT passed",
            "Jet BDT passed",
			"HLT passed",
		]
		
		if print_latex:
			event_latex_setup(file_path)

		init = -1
		
		for i in range(len(selection_list_noCut)):
			selname = selection_list_noCut[i]
			selval  = -1
			Nevents = -1

			if i == 0: 
				selval = tree.GetEntries()
				init = selval
			else:
				if i == 1: 
					total_selection_string = "(jet0_Pt >= "+jet_energy+" || jet1_Pt >= "+jet_energy+" || jet2_Pt >= "+jet_energy+" || "
					total_selection_string += "jet3_Pt >= "+jet_energy+" || jet4_Pt >= "+jet_energy+" || jet5_Pt >= "+jet_energy+")"
				if i == 2: 
					total_selection_string = "((jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26) || (jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26) || (jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26)"
					total_selection_string += " || (jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26) || (jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26) || (jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26))"
				if i == 3: total_selection_string += " && eventHT > 200"
				if i == 4: 
					total_selection_string =  " ( (  jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_bdtscoreX_LLP350_MS80_perJet >= 0.99)" # jet 0 condition
					total_selection_string += " || ( jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26 && jet1_bdtscoreX_LLP350_MS80_perJet >= 0.99)" # jet 1 condition
					total_selection_string += " || ( jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26 && jet2_bdtscoreX_LLP350_MS80_perJet >= 0.99)" # jet 2 condition
					total_selection_string += " || ( jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26 && jet3_bdtscoreX_LLP350_MS80_perJet >= 0.99)" # jet 3 condition
					total_selection_string += " || ( jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26 && jet4_bdtscoreX_LLP350_MS80_perJet >= 0.99)" # jet 4 condition
					total_selection_string += " || ( jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26 && jet5_bdtscoreX_LLP350_MS80_perJet >= 0.99))" # jet 5 condition
					total_selection_string += " && eventHT > 200"
				if i == 6: 
					total_selection_string += "&& (( HLT_HT200_L1SingleLLPJet_DisplacedDijet40_Inclusive1PtrkShortSig5 == 1 || HLT_HT240_L1SingleLLPJet_DisplacedDijet40_Inclusive1PtrkShortSig5 == 1 || HLT_HT280_L1SingleLLPJet_DisplacedDijet40_Inclusive1PtrkShortSig5 == 1)"
					total_selection_string += "|| ( HLT_HT170_L1SingleLLPJet_DisplacedDijet40_DisplacedTrack == 1 || HLT_HT200_L1SingleLLPJet_DisplacedDijet40_DisplacedTrack == 1 || HLT_HT270_L1SingleLLPJet_DisplacedDijet40_DisplacedTrack == 1 || HLT_HT200_L1SingleLLPJet_DisplacedDijet60_DisplacedTrack == 1 )"
					total_selection_string += "|| ( HLT_HT320_L1SingleLLPJet_DisplacedDijet60_Inclusive == 1 || HLT_HT420_L1SingleLLPJet_DisplacedDijet60_Inclusive == 1 )"
					total_selection_string += "|| ( HLT_HT200_L1SingleLLPJet_DelayedJet40_DoubleDelay0p5nsTrackless == 1 || HLT_HT200_L1SingleLLPJet_DelayedJet40_DoubleDelay1nsInclusive == 1 || HLT_HT200_L1SingleLLPJet_DelayedJet40_SingleDelay1nsTrackless == 1 || HLT_HT200_L1SingleLLPJet_DelayedJet40_SingleDelay2nsInclusive == 1 )"
					total_selection_string += "|| HLT_L1SingleLLPJet == 1)"

				selval = tree.GetEntries(total_selection_string)
				if i == 3: all_events = selval # but end up doing comparison (denominator) to all LLP events...
				if i == 4: one_plus_jets = selval

				Nevents = tree.GetEntries()

			if print_latex:
				if i <= 3: 
					print(selname+" &", round(selval, 4), " &", round(selval/init, 4), " \\\\ ")
					if i == 0: print("\\hline")
				if i == 4: print(selname+" & ", round(all_events - one_plus_jets, 4), " &", round((all_events - one_plus_jets)/init, 4), " \\\\ ") # 0 bin is all events - events with at least 1
				if i == 5: print(selname+" & ", round(one_plus_jets, 4), " &", round((one_plus_jets)/init, 4), " \\\\ ") # 1 bin is events with at least 1 
				if i == 6: 
					print(selname+" & ", round(selval, 4), " & ", round(selval/init, 4), " \\\\ ") 
					latex_end(file_path)

			else:
				print(selection_list_abbrev_noCut[i], "\t", Nevents, "\t", round(selval, 4), "\t", round(selval/init, 4))


	if "Run2023" in file_path:
		# Cutflow table for background estimation: basic event selection
		print(" \n")
		print("Background (W+jets, then NoSel) cutflow, done per event")
		print(" \n")

		BDT_score = "0.998"

		event_selection_list = [
            "All events (from skim)", 
            "Jet $\\geq "+jet_energy+"$~GeV $p_T$ (one of 6 leading)", 
            "Jet $\\abs\\eta \\leq 1.26$", 
            "0 jets with BDT score $\\geq "+BDT_score+"$",
            "1 jet with BDT score $\\geq "+BDT_score+"$",
            "2 jets with BDT scores $\\geq "+BDT_score+"$",
			"Jet $\\geq "+jet_energy+"$~GeV $p_T$, $\\abs\\eta \\leq 1.26$, triggered",
			"0 jets with BDT score $\\geq "+BDT_score+"$",
            "1 jet with BDT score $\\geq "+BDT_score+"$",
            "2 jets with BDT scores $\\geq "+BDT_score+"$",
			"Jet $\\geq "+jet_energy+"$~GeV $p_T$, $\\abs\\eta \\leq 1.26$, not triggered",
			"0 jets with BDT score $\\geq "+BDT_score+"$",
            "1 jet with BDT score $\\geq "+BDT_score+"$",
            "2 jets with BDT scores $\\geq "+BDT_score+"$",
        ]
        
		event_selection_list_abbrev = [
            "All       ", 
            "Jet pT", 
            "Jet eta",
            "No jet BDT passed",
            "Jet BDT passed",
            "Two jet BDT passed",
            "Jet eta, pT (triggered)",
            "No jet BDT passed",
            "Jet BDT passed",
            "Two jet BDT passed",
            "Jet eta, pT (not triggered)",
            "No jet BDT passed",
            "Jet BDT passed",
            "Two jet BDT passed",
        ]

		# trees = ["WPlusJets", "NoSel"]
		trees = ["WPlusJets"]
		for tree_sel in trees:
			print("\n Running fo: " + tree_sel + " \n")
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
				else:
					if i == 1: 
						selection_string = "(jet0_Pt >= "+jet_energy+" || jet1_Pt >= "+jet_energy+" || jet2_Pt >= "+jet_energy+" || jet3_Pt >= "+jet_energy+" || jet4_Pt >= "+jet_energy+" || jet5_Pt >= "+jet_energy+")"
					if i == 2: 
						selection_string = "((jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26) || (jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26) || (jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26)"
						selection_string += " || (jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26) || (jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26) || (jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26))"
					if i == 3: 
						selection_string = " ( (  jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+")" # jet 0 condition
						selection_string += " || (jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26 && jet1_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+")" # jet 1 condition
						selection_string += " || (jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26 && jet2_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+")" # jet 2 condition
						selection_string += " || (jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26 && jet3_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+")" # jet 3 condition
						selection_string += " || (jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26 && jet4_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+")" # jet 4 condition
						selection_string += " || (jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26 && jet5_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+"))" # jet 5 condition
					if i == 4: 
						selection_string = " ( ( ( jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+") && (jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26 && jet1_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+"))" # jet 0 AND 1 condition
						selection_string += " || ((jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+") && (jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26 && jet2_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+"))" # jet 0 AND 2 condition
						selection_string += " || ((jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+") && (jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26 && jet3_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+"))" # jet 0 AND 3 condition
						selection_string += " || ((jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+") && (jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26 && jet4_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+"))" # jet 0 AND 4 condition
						selection_string += " || ((jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+") && (jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26 && jet5_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+"))" # jet 0 AND 5 condition
						selection_string += " || ((jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26 && jet1_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+") && (jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26 && jet2_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+"))" # jet 1 AND 2 condition
						selection_string += " || ((jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26 && jet1_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+") && (jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26 && jet3_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+"))" # jet 1 AND 3 condition
						selection_string += " || ((jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26 && jet1_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+") && (jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26 && jet4_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+"))" # jet 1 AND 4 condition
						selection_string += " || ((jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26 && jet1_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+") && (jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26 && jet5_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+"))" # jet 1 AND 5 condition
						selection_string += " || ((jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26 && jet2_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+") && (jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26 && jet3_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+"))" # jet 2 AND 3 condition
						selection_string += " || ((jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26 && jet2_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+") && (jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26 && jet4_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+"))" # jet 2 AND 4 condition
						selection_string += " || ((jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26 && jet2_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+") && (jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26 && jet5_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+"))" # jet 2 AND 5 condition
						selection_string += " || ((jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26 && jet3_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+") && (jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26 && jet4_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+"))" # jet 3 AND 4 condition
						selection_string += " || ((jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26 && jet3_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+") && (jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26 && jet5_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+"))" # jet 3 AND 5 condition
						selection_string += " || ((jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26 && jet4_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+") && (jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26 && jet5_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+")))" # jet 4 AND 5 condition
					# triggered jet
					if i == 6: 
						selection_string = "  ( ( jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_L1trig_Matched == 1) || (jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26 && jet1_L1trig_Matched == 1)"
						selection_string += " || (jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26 && jet2_L1trig_Matched == 1) || (jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26 && jet3_L1trig_Matched == 1)"
						selection_string += " || (jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26 && jet4_L1trig_Matched == 1) || (jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26 && jet5_L1trig_Matched == 1))"
					if i == 7: 
						selection_string = " ( (  jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_L1trig_Matched == 1 && jet0_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+")" # jet 0 condition
						selection_string += " || (jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26 && jet1_L1trig_Matched == 1 && jet1_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+")" # jet 1 condition
						selection_string += " || (jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26 && jet2_L1trig_Matched == 1 && jet2_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+")" # jet 2 condition
						selection_string += " || (jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26 && jet3_L1trig_Matched == 1 && jet3_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+")" # jet 3 condition
						selection_string += " || (jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26 && jet4_L1trig_Matched == 1 && jet4_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+")" # jet 4 condition
						selection_string += " || (jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26 && jet5_L1trig_Matched == 1 && jet5_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+"))" # jet 5 condition
					if i == 8: 
						selection_string = " ( ( ( jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_L1trig_Matched == 1 && jet0_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+") && (jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26 && jet1_L1trig_Matched == 1 && jet1_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+"))" # jet 0 AND 1 condition
						selection_string += " || ((jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_L1trig_Matched == 1 && jet0_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+") && (jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26 && jet2_L1trig_Matched == 1 && jet2_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+"))" # jet 0 AND 2 condition
						selection_string += " || ((jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_L1trig_Matched == 1 && jet0_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+") && (jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26 && jet3_L1trig_Matched == 1 && jet3_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+"))" # jet 0 AND 3 condition
						selection_string += " || ((jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_L1trig_Matched == 1 && jet0_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+") && (jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26 && jet4_L1trig_Matched == 1 && jet4_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+"))" # jet 0 AND 4 condition
						selection_string += " || ((jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_L1trig_Matched == 1 && jet0_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+") && (jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26 && jet5_L1trig_Matched == 1 && jet5_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+"))" # jet 0 AND 5 condition
						selection_string += " || ((jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26 && jet1_L1trig_Matched == 1 && jet1_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+") && (jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26 && jet2_L1trig_Matched == 1 && jet2_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+"))" # jet 1 AND 2 condition
						selection_string += " || ((jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26 && jet1_L1trig_Matched == 1 && jet1_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+") && (jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26 && jet3_L1trig_Matched == 1 && jet3_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+"))" # jet 1 AND 3 condition
						selection_string += " || ((jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26 && jet1_L1trig_Matched == 1 && jet1_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+") && (jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26 && jet4_L1trig_Matched == 1 && jet4_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+"))" # jet 1 AND 4 condition
						selection_string += " || ((jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26 && jet1_L1trig_Matched == 1 && jet1_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+") && (jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26 && jet5_L1trig_Matched == 1 && jet5_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+"))" # jet 1 AND 5 condition
						selection_string += " || ((jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26 && jet2_L1trig_Matched == 1 && jet2_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+") && (jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26 && jet3_L1trig_Matched == 1 && jet3_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+"))" # jet 2 AND 3 condition
						selection_string += " || ((jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26 && jet2_L1trig_Matched == 1 && jet2_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+") && (jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26 && jet4_L1trig_Matched == 1 && jet4_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+"))" # jet 2 AND 4 condition
						selection_string += " || ((jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26 && jet2_L1trig_Matched == 1 && jet2_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+") && (jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26 && jet5_L1trig_Matched == 1 && jet5_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+"))" # jet 2 AND 5 condition
						selection_string += " || ((jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26 && jet3_L1trig_Matched == 1 && jet3_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+") && (jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26 && jet4_L1trig_Matched == 1 && jet4_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+"))" # jet 3 AND 4 condition
						selection_string += " || ((jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26 && jet3_L1trig_Matched == 1 && jet3_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+") && (jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26 && jet5_L1trig_Matched == 1 && jet5_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+"))" # jet 3 AND 5 condition
						selection_string += " || ((jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26 && jet4_L1trig_Matched == 1 && jet4_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+") && (jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26 && jet5_L1trig_Matched == 1 && jet5_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+")))" # jet 4 AND 5 condition
					# Non-triggered jet
					if i == 10: 
						selection_string = "  ( ( jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_L1trig_Matched == 0) || (jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26 && jet1_L1trig_Matched == 0)"
						selection_string += " || (jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26 && jet2_L1trig_Matched == 0) || (jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26 && jet3_L1trig_Matched == 0)"
						selection_string += " || (jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26 && jet4_L1trig_Matched == 0) || (jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26 && jet5_L1trig_Matched == 0))"
					if i == 11: 
						selection_string = " ( (  jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_L1trig_Matched == 0 && jet0_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+")" # jet 0 condition
						selection_string += " || (jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26 && jet1_L1trig_Matched == 0 && jet1_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+")" # jet 1 condition
						selection_string += " || (jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26 && jet2_L1trig_Matched == 0 && jet2_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+")" # jet 2 condition
						selection_string += " || (jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26 && jet3_L1trig_Matched == 0 && jet3_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+")" # jet 3 condition
						selection_string += " || (jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26 && jet4_L1trig_Matched == 0 && jet4_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+")" # jet 4 condition
						selection_string += " || (jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26 && jet5_L1trig_Matched == 0 && jet5_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+"))" # jet 5 condition
					if i == 12: 
						selection_string = " ( ( ( jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_L1trig_Matched == 0 && jet0_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+") && (jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26 && jet1_L1trig_Matched == 0 && jet1_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+"))" # jet 0 AND 1 condition
						selection_string += " || ((jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_L1trig_Matched == 0 && jet0_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+") && (jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26 && jet2_L1trig_Matched == 0 && jet2_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+"))" # jet 0 AND 2 condition
						selection_string += " || ((jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_L1trig_Matched == 0 && jet0_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+") && (jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26 && jet3_L1trig_Matched == 0 && jet3_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+"))" # jet 0 AND 3 condition
						selection_string += " || ((jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_L1trig_Matched == 0 && jet0_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+") && (jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26 && jet4_L1trig_Matched == 0 && jet4_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+"))" # jet 0 AND 4 condition
						selection_string += " || ((jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_L1trig_Matched == 0 && jet0_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+") && (jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26 && jet5_L1trig_Matched == 0 && jet5_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+"))" # jet 0 AND 5 condition
						selection_string += " || ((jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26 && jet1_L1trig_Matched == 0 && jet1_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+") && (jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26 && jet2_L1trig_Matched == 0 && jet2_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+"))" # jet 1 AND 2 condition
						selection_string += " || ((jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26 && jet1_L1trig_Matched == 0 && jet1_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+") && (jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26 && jet3_L1trig_Matched == 0 && jet3_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+"))" # jet 1 AND 3 condition
						selection_string += " || ((jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26 && jet1_L1trig_Matched == 0 && jet1_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+") && (jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26 && jet4_L1trig_Matched == 0 && jet4_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+"))" # jet 1 AND 4 condition
						selection_string += " || ((jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26 && jet1_L1trig_Matched == 0 && jet1_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+") && (jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26 && jet5_L1trig_Matched == 0 && jet5_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+"))" # jet 1 AND 5 condition
						selection_string += " || ((jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26 && jet2_L1trig_Matched == 0 && jet2_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+") && (jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26 && jet3_L1trig_Matched == 0 && jet3_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+"))" # jet 2 AND 3 condition
						selection_string += " || ((jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26 && jet2_L1trig_Matched == 0 && jet2_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+") && (jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26 && jet4_L1trig_Matched == 0 && jet4_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+"))" # jet 2 AND 4 condition
						selection_string += " || ((jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26 && jet2_L1trig_Matched == 0 && jet2_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+") && (jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26 && jet5_L1trig_Matched == 0 && jet5_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+"))" # jet 2 AND 5 condition
						selection_string += " || ((jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26 && jet3_L1trig_Matched == 0 && jet3_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+") && (jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26 && jet4_L1trig_Matched == 0 && jet4_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+"))" # jet 3 AND 4 condition
						selection_string += " || ((jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26 && jet3_L1trig_Matched == 0 && jet3_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+") && (jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26 && jet5_L1trig_Matched == 0 && jet5_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+"))" # jet 3 AND 5 condition
						selection_string += " || ((jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26 && jet4_L1trig_Matched == 0 && jet4_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+") && (jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26 && jet5_L1trig_Matched == 0 && jet5_bdtscoreX_LLP350_MS80_perJet >= "+BDT_score+")))" # jet 4 AND 5 condition


					selval = tree.GetEntries(selection_string)
					if i % 4 == 2: all_events = selval # i = 2, 6, 10
					if i % 4 == 3: one_plus_jets = selval # i = 3, 7, 11
					if i % 4 == 0: two_plus_jets = selval # i = 4, 8, 12
					Nevents = tree.GetEntries()

				if print_latex:
					if i < 2: print(selname+" &", round(selval, 4), "&\\multicolumn{2}{l}{", round(selval/init, 4), "} \\\\ ")
					if i % 4 == 2: print(selname+" &", round(selval, 4), "&", round(selval/init, 4), "&", round(selval/all_events, 4), " \\\\ ")
					if i % 4 == 3: print(selname+" &", round(all_events - one_plus_jets, 4), "&", round((all_events - one_plus_jets)/init, 4),  "&", round((all_events - one_plus_jets)/all_events, 4), " \\\\ ") # 0 bin is all events - events with at least 1
					if i > 0 and i % 4 == 0: print(selname+" &", round(one_plus_jets - two_plus_jets, 4), "&", round((one_plus_jets - two_plus_jets)/init, 4),  "&", round((one_plus_jets - two_plus_jets)/all_events, 4), " \\\\ ") # 1 bin is events with at least 1 - events with at least 2
					if i >= 5 and i % 4 == 1: 
						print(selname+" &", round(two_plus_jets, 4), "&", round(two_plus_jets/init, 4),  "&", round(two_plus_jets/all_events, 4), " \\\\ ") # 2+ bin is events with at least 2
						if i != 13: print("\\hline")
						if i == 13: latex_end(file_path)

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
		tree = file.Get("NoSel")
        
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
			else:
				if i == 1: selection_string += "(jet0_Pt >= 40 || jet1_Pt >= 40 || jet2_Pt >= 40)"
				if i == 2: selection_string += "&& ((jet0_Pt >= 40 && abs(jet0_Eta) <= 1.26) || (jet1_Pt >= 40 && abs(jet1_Eta) <= 1.26) || (jet2_Pt >= 40 && abs(jet2_Eta) <= 1.26))"
				if i == 4: selection_string = "(jet0_Pt >= 60 || jet1_Pt >= 60 || jet2_Pt >= 60)"
				if i == 5: selection_string += "&& ((jet0_Pt >= 60 && abs(jet0_Eta) <= 1.26) || (jet1_Pt >= 60 && abs(jet1_Eta) <= 1.26) || (jet2_Pt >= 60 && abs(jet2_Eta) <= 1.26))"
								
				selval = tree.GetEntries(selection_string)
				if (i == 2 or i == 5): all_events = selval
				if i == 3: selval = all_events * 0.0014
				if i == 6: selval = all_events * 0.0012

			if print_latex:
				print(selname+" &", round(selval, 4), "&", round(selval/init, 4), " \\\\ ")
				if i == 0: print("\\hline")
				if i == 3: print("\\hline")
				if i == 6: latex_end(file_path)

if __name__ == '__main__':
	main()