import ROOT
from ROOT import Form
from math import *
import numpy as np
import sys, os, argparse, time, errno
import matplotlib.pyplot as plt
from datetime import datetime

start = time.time()

debug = True
time_debug = True

# declare any variables or settings here
data = False
MC = False
if sys.argv[2] == "data": data = True
if sys.argv[2] == "MC": MC = True

datetime_now = datetime.today().strftime('%Y_%m_%d')

# set ROOT canvas information
ROOT.gROOT.SetBatch(1)
ROOT.gStyle.SetOptStat(0)
ROOT.gStyle.SetEndErrorSize(0)
ROOT.gStyle.SetPalette(ROOT.kCandy)

stamp_text = ROOT.TLatex()
stamp_text.SetNDC()
stamp_text.SetTextFont(42)
stamp_text.SetTextColor(ROOT.kBlack)
stamp_text.SetTextSize(0.045)
xpos = 0.45
ypos = 0.85

canv = ROOT.TCanvas("c","c",800,600)
overlayCanv = ROOT.TCanvas("c1","c1",800,600)
if (data): 
  cmsLabel = "#scale[1.0]{#bf{CMS}} #scale[0.8]{#it{2022 13.6 TeV Collisions Data (MET skim)}}"; 
  folder = "./outPlots/"
if (MC): 
  cmsLabel = "#scale[1.0]{#bf{CMS}} #scale[0.8]{#it{2023 LLP MC}}"; 
  folder = "./outPlots_MC/"
  xpos += 0.2

# Save output plots
def OutputFolder(name):
  try:
    os.makedirs(folder + name)
  except OSError:
    pass 

OutputFolder("")

path = sys.argv[1] # pass the location of the processed ntuple as an argument 
infile = ROOT.TFile.Open(path)

# ------------------------------------------------------------------------------
def GetTag( branch_name, branch_sel ):

	tag = ""

	if type(branch_sel) is int:
		tag = branch_name + str(branch_sel)

	elif type(branch_sel) is list:
		if len(branch_sel) == 0:
			print("WARNING <GetTag> : Entry error (length = 0). No tag included")
		elif len(branch_sel) == 1:
			tag = branch_name + str(branch_sel[0])
		else:
			if branch_sel[1] == branch_sel[0] + 1:
				tag = branch_name + str(branch_sel[0])
			else:
				tag = branch_name + str(branch_sel[0]) + "-" + str(branch_sel[1]-1)

		if len(branch_sel) > 2:
			print("WARNING <GetTag> : 'branch_sel' input has more than two entries. Only using the first two!")

	else: 
		print("WARNING <GetTag> : Type Error. No tag included")
	return tag 

# ------------------------------------------------------------------------------
def GetCut( branch_name, branch_sel ):

	selection = ""

	if type(branch_sel) is int:
		selection = branch_name + " == " + str(branch_sel) 

	elif type(branch_sel) is list:
		if len(branch_sel) == 0:
			print("WARNING <GetCut> : Entry error (length = 0). No cut implemented")
		elif len(branch_sel) == 1:
			selection = branch_name + " == " + str(branch_sel[0])
		elif len(branch_sel) == 2:
			selection = branch_name + " >= " + str(branch_sel[0]) + " && " + branch_name + " < " + str(branch_sel[1])

		else:
			if (branch_name != "bx"): print("WARNING <GetCut> : 'branch_sel' input has more than two entries. Only using the first two!")

	else: 
		print("WARNING <GetCut> : Type Error. No cut implemented")

	if debug: print(selection + "  in Get Cut ")
	return ROOT.TCut( selection + " " )

# ------------------------------------------------------------------------------
# select which iphi values to exclude, if this is empty, no cut is implemented 
def ExcludedCut( branch_name, branch_sel ):

    selection = ""

    if type(branch_sel) is int:
        selection = branch_name + " != " + str(branch_sel) 

    elif type(branch_sel) is list:
        if len(branch_sel) == 0:
            print("WARNING <ExcludedCut> : Entry error (length = 0). No cut implemented")
        elif len(branch_sel) == 1:
            selection = branch_name + " != " + str(branch_sel[0])
        else:
            for exclude in branch_sel[:-1]:
                selection += branch_name + " != " + str(exclude) + " && "
            selection += branch_name + " != " + str(branch_sel[-1])

    else: 
        print("WARNING <ExcludedCut> : Type Error. No cut implemented")

    if debug: print(selection + "  in Excluded Cut ")
    return ROOT.TCut( selection + " " )

# ------------------------------------------------------------------------------



# ------------------------------------------------------------------------------
def Plot(infilepath):

    kinematic_vars = ["LLP0_DecayR", "LLP0_DecayX", "LLP0_DecayY", "LLP0_DecayZ", ]

    cut_vars = ["LLP0_DecayR"]
    
    infile = ROOT.TFile.Open( infilepath )
    tree = infile.Get("NoSel")

    radius_range    = [50, 2000]
    radius_beforeHB = [153, 183.6]
    radius_depth1   = [183.6, 190.2]
    radius_depth2   = [190.2, 214.2]
    radius_depth3   = [214.2, 244.8]
    radius_depth4   = [244.8, 295]

    selection_region = GetCut(cut_vars[0], radius_range)

    LLP_Rdecay = {}
    hname_temp = "h_LLP_radius"
    LLP_Rdecay = ROOT.TH1F(hname_temp, "LLP Decay Radius ; Radius [cm]; Number of Entries", 100, 0, 2000 ); # this can be made into LLP_Rdecay[]
    
    canv = ROOT.TCanvas()

    if (time_debug): print("Drawing tree, with time = " + str(time.time() - start))
    for var in kinematic_vars:
        tree.Draw(var +" >> "+hname_temp, selection_region, "", tree.GetEntries(), 0 )

        LLP_Rdecay.Draw()
        stamp_text = ROOT.TLatex()
        stamp_text.SetNDC()

        if (time_debug): print("Plotting results, with time = " + str(time.time() - start))
        canv.SaveAs("plotter_test_hist_"+var+".png")


# ------------------------------------------------------------------------------
def Plot2D(infilepath):

    kinematic_vars = ["LLP0_energyFrac_depth"]

    cut_vars = ["LLP0_DecayR"]
    
    infile = ROOT.TFile.Open( infilepath )
    tree = infile.Get("NoSel")

    radius_range    = [50, 2000]
    radius_beforeHB = [153, 183.6]
    radius_depth1   = [183.6, 190.2]
    radius_depth2   = [190.2, 214.2]
    radius_depth3   = [214.2, 244.8]
    radius_depth4   = [244.8, 295]

    selection_region = GetCut(cut_vars[0], radius_range)

    LLP_energy_profile = {}
    LLP_energy_depth_profile = {}
    hname = "h_LLP_energy_depth_profile"
    LLP_energy_depth_profile = ROOT.TH1F(hname, "LLP Depth Energy Profile ; Depth (HB); Fraction of Energy", 6,0,6 ); 

    canv = ROOT.TCanvas()
    canvTemp = ROOT.TCanvas()
    canvDepth = ROOT.TCanvas()

    if (time_debug): print("Drawing tree, with time = " + str(time.time() - start))
    for depth in range(4):
        depth += 1
        var = kinematic_vars[0]+str(depth)
        print(var)

        hname_temp = "h_LLP_energy_profile_d"+str(depth)
        LLP_energy_profile[depth] = ROOT.TH1F(hname_temp, "LLP Depth Energy Frac in Depth "+str(depth)+"; Depth (HB); Fraction of Energy", 100, 0, 1 ); # replicate the initial plots first

        canvTemp.cd()
        tree.Draw(var +" >> "+hname_temp, selection_region, "", tree.GetEntries(), 0 )
        AverageEnergyFrac = LLP_energy_profile[depth].GetMean()
        AverageEnergyFrac_error = LLP_energy_profile[depth].GetMeanError()
        print (str(depth) + " = depth, with energy profile mean = " + str(AverageEnergyFrac))

        canvDepth.cd()
        LLP_energy_depth_profile.SetBinContent(depth+1, AverageEnergyFrac)
        LLP_energy_depth_profile.SetBinError(depth+1, AverageEnergyFrac_error)
        stamp_text = ROOT.TLatex()
        stamp_text.SetNDC()
        LLP_energy_depth_profile.Draw()

    if (time_debug): print("Plotting results, with time = " + str(time.time() - start))
    canvTemp.SaveAs("plotter_test_hist_"+kinematic_vars[0]+str(depth)+".png")
    canvDepth.SaveAs("plotter_test_hist_"+kinematic_vars[0]+".png")
# ------------------------------------------------------------------------------
def main():

	infilepath = "../Run/hists_test.root"

	if len(sys.argv) > 1: infilepath = sys.argv[1]

	Plot(infilepath)
	Plot2D(infilepath)

if __name__ == '__main__':
	main()
