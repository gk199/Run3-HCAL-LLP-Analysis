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
def main():
	file_path = sys.argv[1]
	#tree     = sys.argv[2]
	#weight   = sys.argv[3]	
	
	selection_list = [
		"All", 
		"LLP $r$ in HCAL depth 3 or 4", 
		"LLP $\\eta <= 1.26$", 
		"LLP $p_T > 60$~GeV", 
		"LLP 0 OR LLP 1",
		"LLP 0 AND LLP 1",
		"LLP OR, with Event HT $> 200$~GeV", 
		"LLP AND, with Event HT $> 200$~GeV", 
	]
	
	selection_list_abbrev = [
		"All       ", 
		"LLP R     ", 
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
		print("\\begin{table}[ht]")
		print("\\centering")
#		print("\\caption{\\label{}}")
		if "data" in file_path: 
			print("\\begin{tabular}{l|llll}")
			print("\\hline")
			print("\\textbf{Selection} & \\multicolumn{2}{l}{\\textbf{Number of Entries}} & \\multicolumn{2}{l}{\\textbf{Fraction of Entries}} \\\\ \\hline")
			print(" & \\textbf{LLP 0} & \\textbf{LLP 1} & \\textbf{LLP 0} & \\textbf{LLP 1} \\\\ \\hline")
			# print("\\textbf{Selection} & \\textbf{Number of Entries} & \\textbf{Fraction of Entries} \\\\ \\hline")
		else:
			print("\\begin{tabular}{l|llll}")
			print("\\hline")
			print("\\textbf{Selection} & \\multicolumn{2}{l}{\\textbf{Number of Weighted Entries}} & \\multicolumn{2}{l}{\\textbf{Fraction of Weighted Entries}} \\\\ \\hline")
			print(" & \\textbf{LLP 0} & \\textbf{LLP 1} & \\textbf{LLP 0} & \\textbf{LLP 1} \\\\ \\hline")
			# print("\\textbf{Selection} & \\textbf{Number of Weighted Entries} & \\textbf{Fraction of Weighted Entries} \\\\ \\hline")

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
			if i == 1: selection_string += "(LLP0_DecayR >= 214.2 && LLP0_DecayR < 295)"
			if i == 2: selection_string += "&& abs(LLP0_Eta) <= 1.26"
			if i == 3: selection_string += "&& LLP0_Pt > 60"

			if i == 1: selection1_string += "(LLP1_DecayR >= 214.2 && LLP1_DecayR < 295)"
			if i == 2: selection1_string += "&& abs(LLP1_Eta) <= 1.26"
			if i == 3: selection1_string += "&& LLP1_Pt > 60"

			if i == 4: total_selection_string = "(" + selection_string + ") || (" + selection1_string + ")"
			if i == 5: total_selection_string = "(" + selection_string + " && " + selection1_string + ")"
			if i == 6: total_selection_string = "((" + selection_string + ") || (" + selection1_string + ")) && eventHT > 200"
			if i == 7: total_selection_string = "(" + selection_string + " && " + selection1_string + ") && eventHT > 200"

			selval = tree.GetEntries(selection_string)
			selval1 = tree.GetEntries(selection1_string)

			if (i >= 4): selval = tree.GetEntries(total_selection_string)

			Nevents = tree.GetEntries()

		if print_latex:
			if (i < 4): print(selname+" &", round(selval, 4), "&", round(selval1, 4), "&", round(selval/init, 4), "&", round(selval1/init, 4), "\\\\")
			if (i == 4): print("\\hline")
			if (i >= 4): print(selname+" & \\multicolumn{2}{l}{", round(selval, 4), "} & \\multicolumn{2}{l}{", round(selval/init, 4), "} \\\\")
		else:
			if (i < 4): print(selection_list_abbrev[i], "\t", Nevents, "\t", round(selval, 4), "\t", round(selval/init, 4), "LLP 1:", "\t", round(selval1, 4), "\t", round(selval1/init, 4))
			if (i >= 4): print(selection_list_abbrev[i], "\t", Nevents, "\t", round(selval, 4), "\t", round(selval/init, 4))

	if print_latex:
		print("\\end{tabular}")
		print("\\caption{\\url{" + file_path + "}}") # to save which file this cutflow is from
		print("\\label{8-Table:CutFlow}")
		print("\\end{table}")

if __name__ == '__main__':
	main()