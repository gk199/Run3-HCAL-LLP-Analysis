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

print_latex = False

# ------------------------------------------------------------------------------
def main():
	file_path = sys.argv[1]
	#tree     = sys.argv[2]
	#weight   = sys.argv[3]	

	selection_list = [
		"All", 
		"LLP radius in HCAL depth 3 or 4", 
		"LLP $\\eta$ <= 1.26", 
		"LLP $p_T$ > 60~GeV", 
		"Event HT > 200~GeV", 
	]

	selection_list_abbrev = [
		"All       ", 
		"LLP R     ", 
		"LLP eta   ", 
		"LLP pT    ", 
		"HTT       ", 
	]

	file = ROOT.TFile.Open(file_path)
	# hist = file.Get("Cutflow_Medium")
	tree = file.Get("NoSel")

	selection_string = ""

	if print_latex:
		print("\\begin{table}[h]")
		print("\\caption{\\label{}}")
		if "data" in file_path: 
			print("\\begin{tabular}{l|ll}")
			print("\\hline")
			print("Selection & Number of entries & Fraction entries \\\\ \\hline")
		else:
			print("\\begin{tabular}{l|ll}")
			print("\\hline")
			print("Selection & Number of weighted entries & Fraction of weighted entries \\\\ \\hline")

	init = -1
	
	for i in range(len(selection_list)):
		selname = selection_list[i]
		selval  = -1
		Nevents = -1

		if i == 0: 
			selval = tree.GetEntries()
			init = selval
		else:
			if i == 1: selection_string += "(LLP0_DecayR >= 214.2 && LLP0_DecayR < 295)"
			if i == 2: selection_string += "&& abs(LLP0_Eta) <= 1.26"
			if i == 3: selection_string += "&& LLP0_Pt > 60"
			if i == 4: selection_string += "&& eventHT > 200"

			selval = tree.GetEntries(selection_string)

			Nevents = tree.GetEntries()

		if print_latex:
			print(selname+" &", round(selval, 4), "&", round(selval/init, 4), "\\\\")
		else:

			print(selection_list_abbrev[i], "\t", Nevents, "\t", round(selval, 4), "\t", round(selval/init, 4))

	if print_latex:
		print("\\end{tabular}")
		print("\\end{table}")

if __name__ == '__main__':
	main()