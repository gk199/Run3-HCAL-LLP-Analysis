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

from DefineCuts import *
from SetupLatex import *

ROOT.gROOT.SetBatch(1)
ROOT.gStyle.SetOptStat(0)
ROOT.gStyle.SetEndErrorSize(0)

# print_latex = False
print_latex = True

# ------------------------------------------------------------------------------
def LLP_Cutflow(file_path):
	# Cutflow table for signal estimation

	print(" \n")
	print("Signal cutflow, with truth cuts, done per event")
	print(" \n")

	selection_list = [
		"All", 
		"LLP $r$ in HCAL depth 3 or 4", 
		"LLP $\\abs\\eta \\leq 1.26$", 
		"LLP $E > 60$~GeV", 
		"LLP 0 OR LLP 1",
		"LLP OR, with Event HT $> 200$~GeV", 
		"LLP 0 AND LLP 1",
		"LLP AND, with Event HT $> 200$~GeV", 
		"Either LLP and triggered (any jet 0-5)",
		"Either LLP and depth towers $\\geq 2$ (any jet 0-5)",
		"Either LLP and timing towers $\\geq 2$ (any jet 0-5)",
		"Either LLP and 1 timing, 1 depth tower (any jet 0-5)",
		"Either LLP and depth towers $\\geq 2$ (any jet 0-1)",
		"Either LLP and timing towers $\\geq 2$ (any jet 0-1)",
		"Either LLP and 1 timing, 1 depth tower (any jet 0-1)",
		"Either LLP and depth towers $\\geq 2$ (jet 0)",
		"Either LLP and timing towers $\\geq 2$ (jet 0)",
		"Either LLP and 1 timing, 1 depth tower (jet 0)",
		"Either LLP and no tower flags (jet 0)",
	]

	selection_list_abbrev = [
		"All       ", 
		"LLP R d3-4", 
		"LLP eta   ", 
		"LLP pT    ", 
		"LLP OR	   ", 
		"LLP OR with HTT", 
		"LLP AND   ",
		"LLP AND with HTT",
		"Either LLP and triggered",
		"Either LLP and 2+ depth passed (6 jet)",
		"Either LLP and 2+ timing passed (6 jet)",
		"Either LLP and 1 timing, 1 depth passed (6 jet)",
		"Either LLP and 2+ depth passed (2 jet)",
		"Either LLP and 2+ timing passed (2 jet)",
		"Either LLP and 1 timing, 1 depth passed (2 jet)",
		"Either LLP and 2+ depth passed (leading jet)",
		"Either LLP and 2+ timing passed",
		"Either LLP and 1 timing, 1 depth passed",
		"Either LLP and no tower flags",
	]

	file = ROOT.TFile.Open(file_path)
	# tree = file.Get("Classification")
	# tree = file.Get("NoSel")
	tree = file.Get("PassedHLT")
	
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
			selection_string += "(LLP0_DecayR >= 214.2 && LLP0_DecayR < 295)" # 214.2 for depth 3,4. 177 for d 1-4
			selection1_string += "(LLP1_DecayR >= 214.2 && LLP1_DecayR < 295)"
		if i == 2: 
			selection_string += "&& abs(LLP0_Eta) <= 1.26"
			selection1_string += "&& abs(LLP1_Eta) <= 1.26"
		if i == 3: 
			selection_string += "&& LLP0_E > 60"
			selection1_string += "&& LLP1_E > 60"

		if i == 4: total_selection_string = "(" + selection_string + ") || (" + selection1_string + ")"
		if i == 5: 
			one_LLP = "((" + selection_string + ") || (" + selection1_string + ")) && eventHT > 200"
			total_selection_string = one_LLP
		if i == 6: total_selection_string = "(" + selection_string + " && " + selection1_string + ")"
		if i == 7: total_selection_string = "(" + selection_string + " && " + selection1_string + ") && eventHT > 200"

		# 6 jets
		if i == 8: total_selection_string = one_LLP + " && " + jet_string_six_triggered
		if i == 9: total_selection_string = one_LLP + " && (" + one_jet_tagged_string_triggered + " || " + one_jet1_tagged_string_triggered + " || " + one_jet2_tagged_string_triggered + " || " + one_jet3_tagged_string_triggered + " || " + one_jet4_tagged_string_triggered + " || " + one_jet5_tagged_string_triggered + ")"
		if i == 10: total_selection_string = one_LLP + " && (" + two_jet_tagged_string_triggered + " || " + two_jet1_tagged_string_triggered + " || " + two_jet2_tagged_string_triggered + " || " + two_jet3_tagged_string_triggered + " || " + two_jet4_tagged_string_triggered + " || " + two_jet5_tagged_string_triggered + ")"
		if i == 11: total_selection_string = one_LLP + " && (" + three_jet_tagged_string_triggered + " || " + three_jet1_tagged_string_triggered + " || " + three_jet2_tagged_string_triggered + " || " + three_jet3_tagged_string_triggered + " || " + three_jet4_tagged_string_triggered + " || " + three_jet5_tagged_string_triggered + ")"

		# 2 jets
		if i == 12: total_selection_string = one_LLP + " && (" + one_jet_tagged_string_triggered + " || " + one_jet1_tagged_string_triggered + ")"
		if i == 13: total_selection_string = one_LLP + " && (" + two_jet_tagged_string_triggered + " || " + two_jet1_tagged_string_triggered + ")"
		if i == 14: total_selection_string = one_LLP + " && (" + three_jet_tagged_string_triggered + " || " + three_jet1_tagged_string_triggered + ")"

		# leading jet
		if i == 15: total_selection_string = one_LLP + " && " + one_jet_tagged_string_triggered
		if i == 16: total_selection_string = one_LLP + " && " + two_jet_tagged_string_triggered
		if i == 17: total_selection_string = one_LLP + " && " + three_jet_tagged_string_triggered
		if i == 18: total_selection_string = one_LLP + " && " + jet_string_triggered_noFlag # remove "triggered" if want to see just if jet is emulated with these towers

		selval = tree.GetEntries(selection_string)
		selval1 = tree.GetEntries(selection1_string)

		if i >= 4: selval = tree.GetEntries(total_selection_string)

		Nevents = tree.GetEntries()

		if print_latex:
			if i < 4: print(selname+" &", round(selval, 4), "&", round(selval1, 4), "&", round(selval/init, 4), "&", round(selval1/init, 4), "\\\\")
			if i >= 4: print(selname+" & \\multicolumn{2}{l}{", round(selval, 4), "} & \\multicolumn{2}{l}{", round(selval/init, 4), "} \\\\ ")
			if i == 3 or i == 7 or i == 11 or i == 14: print("\\hline")
			if i == 18: latex_end(file_path)

		else:
			if (i < 4): print(selection_list_abbrev[i], "\t", Nevents, "\t", round(selval, 4), "\t", round(selval/init, 4), "LLP 1:", "\t", round(selval1, 4), "\t", round(selval1/init, 4))
			if (i >= 4): print(selection_list_abbrev[i], "\t", Nevents, "\t", round(selval, 4), "\t", round(selval/init, 4))

	# Cutflow table for signal estimation: basic event selection
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
	# tree = file.Get("NoSel")
	tree = file.Get("PassedHLT")
	
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

# ------------------------------------------------------------------------------
def SixJetCheck(file_path, tree):
	# Cutflow table for background estimation

	print(" \n")
	print("Data cutflow, done per event")
	print(" \n")

	selection_list = [
		"All", 
		"Valid jet, ($p_T,\\eta$, any jet 0-5)",
		"Triggered (any jet 0-5)",
		"Triggered and depth towers $\\geq 2$ (any jet 0-5)",
		"Triggered and timing towers $\\geq 2$ (any jet 0-5)",
		"Triggered and 1 timing, 1 depth tower (any jet 0-5)",
		"Triggered and depth towers $\\geq 2$ (any jet 0-1)",
		"Triggered and timing towers $\\geq 2$ (any jet 0-1)",
		"Triggered and 1 timing, 1 depth tower (any jet 0-1)",
		"Triggered and depth towers $\\geq 2$ (jet 0)",
		"Triggered and timing towers $\\geq 2$ (jet 0)",
		"Triggered and 1 timing, 1 depth tower (jet 0)",
		"Triggered and no tower flags (jet 0)",
	]

	selection_list_abbrev = [
		"All       ", 
		"Valid jet",
		"Valid jet, triggered",
		"2+ depth passed (6 jet)",
		"2+ timing passed (6 jet)",
		"1 timing, 1 depth passed (6 jet)",
		"2+ depth passed (2 jet)",
		"2+ timing passed (2 jet)",
		"1 timing, 1 depth passed (2 jet)",
		"2+ depth passed (leading jet)",
		"2+ timing passed",
		"1 timing, 1 depth passed",
		"No tower flags",
	]

	file = ROOT.TFile.Open(file_path)
	tree = file.Get(tree)
	
	total_selection_string = ""
	
	if print_latex:
		event_latex_setup(file_path)

	init = -1
	
	for i in range(len(selection_list)):
		selname = selection_list[i]
		selval  = -1
		Nevents = -1
		
		# 6 jets
		if i == 1: total_selection_string = jet_string_six
		if i == 2: total_selection_string = jet_string_six_triggered
		if i == 3: total_selection_string = "(" + one_jet_tagged_string_triggered + " || " + one_jet1_tagged_string_triggered + " || " + one_jet2_tagged_string_triggered + " || " + one_jet3_tagged_string_triggered + " || " + one_jet4_tagged_string_triggered + " || " + one_jet5_tagged_string_triggered + ")"
		if i == 4: total_selection_string = "(" + two_jet_tagged_string_triggered + " || " + two_jet1_tagged_string_triggered + " || " + two_jet2_tagged_string_triggered + " || " + two_jet3_tagged_string_triggered + " || " + two_jet4_tagged_string_triggered + " || " + two_jet5_tagged_string_triggered + ")"
		if i == 5: total_selection_string = "(" + three_jet_tagged_string_triggered + " || " + three_jet1_tagged_string_triggered + " || " + three_jet2_tagged_string_triggered + " || " + three_jet3_tagged_string_triggered + " || " + three_jet4_tagged_string_triggered + " || " + three_jet5_tagged_string_triggered + ")"

		# 2 jets
		if i == 6: total_selection_string = "(" + one_jet_tagged_string_triggered + " || " + one_jet1_tagged_string_triggered + ")"
		if i == 7: total_selection_string = "(" + two_jet_tagged_string_triggered + " || " + two_jet1_tagged_string_triggered + ")"
		if i == 8: total_selection_string = "(" + three_jet_tagged_string_triggered + " || " + three_jet1_tagged_string_triggered + ")"

		# leading jet
		if i == 9: total_selection_string = one_jet_tagged_string_triggered
		if i == 10: total_selection_string = two_jet_tagged_string_triggered
		if i == 11: total_selection_string = three_jet_tagged_string_triggered
		if i == 12: total_selection_string = jet_string_triggered_noFlag # remove "triggered" if want to see just if jet is emulated with these towers

		selval = tree.GetEntries(total_selection_string)
		if i == 0: init = selval

		Nevents = tree.GetEntries()

		if print_latex:
			print(selname+" & ", round(selval, 4), " & ", round((selval)/init, 4), " \\\\ ") 
			if i == 2 or i == 5 or i == 8: print("\\hline")
			if i == 12: latex_end(file_path)

		else:
			print(selection_list_abbrev[i], "\t", Nevents, "\t", round(selval, 4), "\t", round(selval/init, 4))

# ------------------------------------------------------------------------------
def DNN_Passing(file_path):
	# Cutflow table for fraction of events passing DNN for jet 0 and jet 1
	print(" \n")
	print("DNN cutflow, done per event")
	print(" \n")
	selection_list_noCut = [
		"All", 
		"Jet 0 $\\geq "+jet_energy+"$~GeV $p_T$ and $\\abs\\eta \\leq 1.26$", 
		"Jet 1 $\\geq "+jet_energy+"$~GeV $p_T$ and $\\abs\\eta \\leq 1.26$", 
		"Jet 0 or 1 is L1 triggered",
		"Triggered jet, 2+ depth towers passed",
		"Triggered jet, 2+ depth towers and both DNN $\\geq 0.9$",
		"Triggered jet, 2+ timing towers passed",
		"Triggered jet, 2+ timing towers and both DNN $\\geq 0.9$",		
		"Triggered jet, 1 timing, 1 depth tower passed",
		"Triggered jet, 1 timing, 1 depth tower and both DNN $\\geq 0.9$",		
		"Jet 0 is L1 triggered",
		"Leading jet, 2+ depth towers passed",
		"Leading jet, 2+ depth towers and both DNN $\\geq 0.9$",
		"Leading jet, 2+ timing towers passed",
		"Leading jet, 2+ timing towers and both DNN $\\geq 0.9$",		
		"Leading jet, 1 timing, 1 depth tower passed",
		"Leading jet, 1 timing, 1 depth tower and both DNN $\\geq 0.9$",			]

	selection_list_abbrev_noCut = [
		"All       ",
		"Jet 0 pT and eta",
		"Jet 1 pT and eta",
		"Jet 0 or 1 triggered",
		"2+ depth passed",
		"and DNNs passed",
		"2+ timing passed",
		"and DNNs passed",
		"1 timing, 1 depth passed",
		"and DNNs passed",
		"Jet 0 triggered",
		"2+ depth passed (jet0)",
		"and DNNs passed",
		"2+ timing passed (jet0)",
		"and DNNs passed",
		"1 timing, 1 depth passed (jet0)",
		"and DNNs passed",
	]
	
	if print_latex:
		event_latex_setup(file_path)

	init = -1
	file = ROOT.TFile.Open(file_path)
	if "LLPskim" or "Zmu" in file_path: tree = file.Get("WPlusJets")
	if "LLP_MC" in file_path: tree = file.Get("PassedHLT")
	
	for i in range(len(selection_list_noCut)):
		selname = selection_list_noCut[i]
		selval  = -1
		Nevents = -1

		if i == 0: 
			init = tree.GetEntries()
			total_selection_string = ""
		if i == 1: total_selection_string = jet_string
		if i == 2: total_selection_string += " && " + jet1_string
		# either jet 1 or jet 2 is triggered
		if i == 3: total_selection_string += " && (jet0_L1trig_Matched == 1 || jet1_L1trig_Matched == 1)"
		if i == 4: total_selection_string = "(" + one_jet_tagged_string_triggered + "||" + one_jet1_tagged_string_triggered + ") && ( " + jet_string + " && " + jet1_string + ")"
		if i == 5: total_selection_string += " && ((jet0_L1trig_Matched == 1 && jet0_scores >= 0.9 && jet1_scores_inc >= 0.9) || (jet0_scores_inc >= 0.9 && jet1_scores >= 0.9 && jet1_L1trig_Matched == 1))"
		if i == 6: total_selection_string = "(" + two_jet_tagged_string_triggered + "||" + two_jet1_tagged_string_triggered + ") && ( " + jet_string + " && " + jet1_string + ")"
		if i == 7: total_selection_string += " && ((jet0_L1trig_Matched == 1 && jet0_scores >= 0.9 && jet1_scores_inc >= 0.9) || (jet0_scores_inc >= 0.9 && jet1_scores >= 0.9 && jet1_L1trig_Matched == 1))"
		if i == 8: total_selection_string = "(" + three_jet_tagged_string_triggered + "||" + three_jet1_tagged_string_triggered + ") &&  ( " + jet_string + " && " + jet1_string + ")"
		if i == 9: total_selection_string += " && ((jet0_L1trig_Matched == 1 && jet0_scores >= 0.9 && jet1_scores_inc >= 0.9) || (jet0_scores_inc >= 0.9 && jet1_scores >= 0.9 && jet1_L1trig_Matched == 1))"

		# jet 1 is triggered
		if i == 10: total_selection_string = jet_string_triggered + " && " + jet1_string
		if i == 11: total_selection_string = one_jet_tagged_string_triggered + " && " + jet1_string
		if i == 12: total_selection_string += " && jet0_scores >= 0.9 && jet1_scores_inc >= 0.9"
		if i == 13: total_selection_string = two_jet_tagged_string_triggered + " && " + jet1_string
		if i == 14: total_selection_string += " && jet0_scores >= 0.9 && jet1_scores_inc >= 0.9"
		if i == 15: total_selection_string = three_jet_tagged_string_triggered + " && " + jet1_string
		if i == 16: total_selection_string += " && jet0_scores >= 0.9 && jet1_scores_inc >= 0.9"

		# if i == 10: total_selection_string = jet_string + " && " + jet1_string + "&& jet1_DepthTowers >= 2 && jet1_L1trig_Matched == 1"
		# if i == 11: total_selection_string += " && jet0_scores_inc >= 0.9 && jet1_scores >= 0.9"
		# if i == 12: total_selection_string = jet_string + " && " + jet1_string + "&& jet1_TimingTowers >= 2 && jet1_L1trig_Matched == 1"
		# if i == 13: total_selection_string += " && jet0_scores_inc >= 0.9 && jet1_scores >= 0.9"
		# if i == 14: total_selection_string = jet_string + " && " + jet1_string + "&& jet1_TimingTowers == 2 && jet1_DepthTowers == 1 && jet1_L1trig_Matched == 1"
		# if i == 15: total_selection_string += " && jet0_scores_inc >= 0.9 && jet1_scores >= 0.9"

		selval = tree.GetEntries(total_selection_string)

		Nevents = tree.GetEntries()

		if print_latex:
			print(selname+" & ", round(selval, 4), " & ", round((selval)/init, 4), " \\\\ ") 
			if i == 0 or i == 2 or i == 9: print("\\hline")
			if i == 16: latex_end(file_path)

		else:
			print(selection_list_abbrev_noCut[i], "\t", Nevents, "\t", round(selval, 4), "\t", round(selval/init, 4))

# ------------------------------------------------------------------------------
def Emulated_Towers(file_path):
	# Cutflow table for background estimation: basic event selection

	print(" \n")
	print("Signal cutflow, without truth cuts, done per event")
	print(" \n")
	selection_list_noCut = [
		"All", 
		"Jet $\\geq "+jet_energy+"$~GeV $p_T$ and $\\abs\\eta \\leq 1.26$", # (one of 6 leading)", 
		"0 depth towers",
		"1 depth towers",
		"Depth towers $\\geq 2$",
		"0 timing towers",
		"1 timing towers",
		"Timing towers $\\geq 2$",
		"1 timing, 1 depth tower",
		"LLP HLT passed",
		"HLT and 0 depth towers",
		"HLT and 1 depth towers",
		"HLT and Depth towers $\\geq 2$",
		"HLT and 0 timing towers",
		"HLT and 1 timing towers",
		"HLT and Timing towers $\\geq 2$",
		"HLT and 1 timing, 1 depth tower",
		"Jet $\\geq "+jet_energy+"$~GeV $p_T$, $\\abs\\eta \\leq 1.26$, triggered",
		"LLP HLT passed",
		"HLT and no tower flags",
		"HLT and 0 depth towers",
		"HLT and 1 depth towers",
		"HLT and Depth towers $\\geq 2$",
		"HLT and 0 timing towers",
		"HLT and 1 timing towers",
		"HLT and Timing towers $\\geq 2$",
		"HLT and 1 timing, 1 depth tower",
		"Jet $\\geq "+jet_energy+"$~GeV $p_T$, $\\abs\\eta \\leq 1.26$, not triggered",
		"LLP HLT passed",
		"HLT and no tower flags",
		"HLT and 0 depth towers",
		"HLT and 1 depth towers",
		"HLT and Depth towers $\\geq 2$",
		"HLT and 0 timing towers",
		"HLT and 1 timing towers",
		"HLT and Timing towers $\\geq 2$",
		"HLT and 1 timing, 1 depth tower",
		"Jet $\\geq "+jet_energy+"$~GeV $p_T$, $\\abs\\eta \\leq 1.26$, L1 matched",
		"LLP HLT passed",
		"HLT and no tower flags",
		"HLT and 0 depth towers",
		"HLT and 1 depth towers",
		"HLT and Depth towers $\\geq 2$",
		"HLT and 0 timing towers",
		"HLT and 1 timing towers",
		"HLT and Timing towers $\\geq 2$",
		"HLT and 1 timing, 1 depth tower",
	]

	selection_list_abbrev_noCut = [
		"All       ",
		"Jet pT and eta", 
		"0 depth passed",
		"1 depth passed",
		"2+ depth passed",
		"0 timing passed",
		"1 timing passed",
		"2+ timing passed",
		"HLT passed",
		"HLT and 0 depth passed",
		"HLT and 1 depth passed",
		"HLT and 2+ depth passed",
		"HLT and 0 timing passed",
		"HLT and 1 timing passed",
		"HLT and 2+ timing passed",
		"Triggered", 
		"HLT passed",
		"HLT and 0 depth passed",
		"HLT and 1 depth passed",
		"HLT and 2+ depth passed",
		"HLT and 0 timing passed",
		"HLT and 1 timing passed",
		"HLT and 2+ timing passed",
		"Not triggered", 
		"HLT passed",
		"HLT and 0 depth passed",
		"HLT and 1 depth passed",
		"HLT and 2+ depth passed",
		"HLT and 0 timing passed",
		"HLT and 1 timing passed",
		"HLT and 2+ timing passed",
		"L1 matched", 
		"HLT passed",
		"HLT and 0 depth passed",
		"HLT and 1 depth passed",
		"HLT and 2+ depth passed",
		"HLT and 2+ timing passed",
		"HLT and 0 timing passed",
		"HLT and 1 timing passed",
	]
	
	if print_latex:
		jetSplit_event_latex_setup(file_path)

	init = -1

	file = ROOT.TFile.Open(file_path)
	if "LLPskim" or "Zmu" in file_path: tree = file.Get("WPlusJets")
	if "LLP_MC" in file_path: tree = file.Get("PassedHLT")

	for i in range(len(selection_list_noCut)):
		selname = selection_list_noCut[i]
		selval  = -1
		Nevents = -1

		if i == 0: 
			init = tree.GetEntries()
			total_selection_string = ""
		# 1 - 16
		if i % 8 == 1: total_selection_string = jet_string
		if i % 8 == 2: total_selection_string = jet0_depth0
		if i % 8 == 3: total_selection_string = jet0_depth1
		if i % 8 == 4: total_selection_string = one_jet_tagged_string
		if i % 8 == 5: total_selection_string = jet0_timing0
		if i % 8 == 6: total_selection_string = jet0_timing1
		if i % 8 == 7: total_selection_string = two_jet_tagged_string
		if i % 8 == 0: total_selection_string = three_jet_tagged_string

		if i >= 17 and i < 25: total_selection_string += " && jet0_L1trig_Matched == 1 "

		if i >= 25 and i < 33: total_selection_string += " && jet0_L1trig_Matched == 0 "

		if i >= 33 and i < 41: total_selection_string += " && jet0_L1trig_Matched >= 0 "

		selval = tree.GetEntries(total_selection_string)
		if i % 16 == 1: valid_events = selval # all valid events, triggered valid events, non-triggered valid events

		Nevents = tree.GetEntries()

		if print_latex:
			if i == 0: print(selname+" &", round(selval, 4), "&\\multicolumn{2}{l}{", round(selval/init, 4), "} \\\\ ")
			elif (i % 16 == 1): print(selname+" &", round(selval, 4), " &", round(selval/init, 4), " &", round(selval/valid_events, 4), " \\\\ ")
			else: print(selname+" & ", round(selval, 4), " &", round((selval)/init, 4), " &", round((selval)/valid_events, 4), " \\\\ ") # 0 bin is all events - events with at least 1
			if i % 16 == 0: print("\\hline")
			if i == 40: latex_end(file_path)

		else:
			print(selection_list_abbrev_noCut[i], "\t", Nevents, "\t", round(selval, 4), "\t", round(selval/init, 4))

	# Cutflow table for fraction of events passing emulation that have HLT set
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
			# if i == 3: print(selname+" & ", round(all_events, 4), " &", round((all_events)/init, 4), " \\\\ ") # 0 bin is all events - events with at least 1
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

# ------------------------------------------------------------------------------
def Bkg_Estimation(file_path):
	# not edited for 3+ yet! only working with LLP MC now

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

# ------------------------------------------------------------------------------
def main():
	
	file_path = sys.argv[1]
	
	# Signal truth cuts, evalute acceptance and then 6, 2, 1 jets
	#if "LLP_MC" in file_path: LLP_Cutflow(file_path) 

	# Similar to the first table from LLP_Cutflow, but on data or LLP, using jet/analysis cuts
	if "Zmu" or "LLPskim" in file_path: SixJetCheck(file_path, "WPlusJets") # evaluate 6, 2, and 1 jets that are triggered and pass emulation
	# if "LLP_MC" in file_path: SixJetCheck(file_path, "PassedHLT")

	# DNN check
	DNN_Passing(file_path) 

	# Emulated towers check
	# if "LLPskim" or "LLP_MC" in file_path: Emulated_Towers(file_path)

	# Set up for background estimation now (needs update)
	# if "Run2023" in file_path: Bkg_Estimation(file_path)

if __name__ == '__main__':
	main()