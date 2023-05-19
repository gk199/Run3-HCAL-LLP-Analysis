import ROOT
from ROOT import Form
from math import *
import numpy as np
import sys, os, argparse, time, errno
import matplotlib.pyplot as plt
from datetime import datetime

start = time.time()

debug = True
time_debug = False

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
def PlotSetup(infilepath):

    infile = ROOT.TFile.Open( infilepath )
    tree = infile.Get("NoSel")
    
    cut_vars = ["LLP0_DecayR"]

    radius_range    = [50, 2000]
    radius_beforeHB = [153, 183.6]
    radius_depth1   = [183.6, 190.2]
    radius_depth2   = [190.2, 214.2]
    radius_depth3   = [214.2, 244.8]
    radius_depth4   = [244.8, 295]

    selection_region = GetCut(cut_vars[0], radius_range)

    Plot1D(tree, selection_region)
    Plot2D(tree, selection_region)

# ------------------------------------------------------------------------------
def Plot1D(tree, selection_region):

    object = ["LLP"]
    number = ["0", "1"]
    kinematic_vars = ["DecayR", "DecayX", "DecayY", "DecayZ", ]

    LLP_decay = {}
    
    canv = ROOT.TCanvas()
    canvTemp = ROOT.TCanvas()

    for obj in object:
        for var in kinematic_vars:
            legend = ROOT.TLegend(0.65,0.65,0.8,0.8)
            for i in number:
                hname_temp = obj + var + i
                LLP_decay[i] = ROOT.TH1F(hname_temp, "LLP Decay Position " + var[-1] +" ; " + var +" [cm]; Number of Entries", 200, 1000, 1000 ); # this can be made into LLP_Rdecay[]
                legend.AddEntry(LLP_decay[i], obj + i)

                dist = obj + i + "_" + var
                canvTemp.cd()
                tree.Draw(dist +" >> "+hname_temp, selection_region, "", tree.GetEntries(), 0 )
                
                canv.cd()
                if i == "0": LLP_decay[i].Draw("HIST PLC")
                else: LLP_decay[i].Draw("HIST PLC SAME")

            legend.Draw()
            stamp_text = ROOT.TLatex()
            stamp_text.SetNDC()
            stamp_text.DrawLatex( xpos, ypos, cmsLabel)
            canv.SaveAs(obj + "_" +var+".png")

# ------------------------------------------------------------------------------
def Plot2D(tree, selection_region):

    object = ["LLP", "jet"]
    number = ["0", "1", "2"]
    kinematic_vars = ["_energyFrac_depth"]

    energy_profile, energy_depth_profile = {}, {}

    canvTemp = ROOT.TCanvas()
    canvDepth = ROOT.TCanvas()

    for obj in object:
        legend = ROOT.TLegend(0.65,0.65,0.8,0.8)
        for i in number:
            if (i == "2" and obj != "jet"): continue
            hname = obj + i + "_energy_depth_profile"
            energy_depth_profile[i] = ROOT.TH1F(hname, obj + " Depth Energy Profile ; Depth (HB); Fraction of Energy", 6,0,6 ); 

            for depth in range(4):
                depth += 1
                var = obj + i + kinematic_vars[0] + str(depth)
                hname_temp = obj+i+"_energy_profile_d"+str(depth)
                energy_profile[depth] = ROOT.TH1F(hname_temp, obj+" Depth Energy Frac in Depth "+str(depth)+"; Depth (HB); Fraction of Energy", 100, 0, 1 ); # replicate the initial plots first

                canvTemp.cd()
                if (time_debug): print("Drawing tree, with time = " + str(time.time() - start))
                tree.Draw(var +" >> "+hname_temp, selection_region, "", tree.GetEntries(), 0 )
                AverageEnergyFrac = energy_profile[depth].GetMean()
                AverageEnergyFrac_error = energy_profile[depth].GetMeanError()

                energy_depth_profile[i].SetBinContent(depth+1, AverageEnergyFrac)
                energy_depth_profile[i].SetBinError(depth+1, AverageEnergyFrac_error)

            canvDepth.cd()
            legend.AddEntry(energy_depth_profile[i], obj + i)
            energy_depth_profile[i].GetYaxis().SetRangeUser(0,0.7)
            if i == "0": energy_depth_profile[i].Draw("HIST PLC")
            else: energy_depth_profile[i].Draw("SAME HIST PLC")

        if (time_debug): print("Plotting results, with time = " + str(time.time() - start))
        legend.Draw()
        stamp_text = ROOT.TLatex()
        stamp_text.SetNDC()
        stamp_text.DrawLatex( xpos, ypos, cmsLabel)
        canvDepth.SaveAs(obj+"_energyProfile.png")

# ------------------------------------------------------------------------------
def main():

	infilepath = "../Run/hists_test.root"

	if len(sys.argv) > 1: infilepath = sys.argv[1]

	PlotSetup(infilepath)

if __name__ == '__main__':
	main()