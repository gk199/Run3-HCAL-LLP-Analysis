import ROOT
from ROOT import Form
from math import *
import numpy as np
import sys, os, argparse, time, errno
import matplotlib.pyplot as plt
from datetime import datetime

start = time.time()

debug = False
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
    
    obj_type = ["LLP", "jet"]

    for obj in obj_type:
        Plot1D(tree, obj)
        Plot2D(tree, obj)

# ------------------------------------------------------------------------------
def MakeSelection(variable):
   
    radius_preECAL  = [0, 161.6]
    radius_ECAL     = [161.6, 183.6] # 22cm of ECAL
    radius_depth1   = [183.6, 190.2]
    radius_depth2   = [190.2, 214.2]
    radius_depth3   = [214.2, 244.8]
    radius_depth4   = [244.8, 295]

    radius_all      = [0,9999]
    radius_inHCAL   = [183.6, 295]
    radius_depth12  = [183.6, 214.2]
    radius_depth34  = [214.2, 295]

    selection_radius = radius_all

    # variable = LLP0 or LLP1. Would do jet - LLP matching requrements here, this is a placeholder
    if (variable == "jet0"): variable = "LLP0"
    if (variable == "jet1"): variable = "LLP1"
    if (variable == "jet2"): variable = "LLP1"
    cut_vars = variable + "_DecayR"

    selection_region = GetCut(cut_vars, selection_radius)

    return selection_region

# ------------------------------------------------------------------------------
def ResetRange(hist):
    minXbin = hist.FindFirstBinAbove()
    maxXbin = hist.FindLastBinAbove()

    hist.GetXaxis().SetRange(minXbin, maxXbin)

# ------------------------------------------------------------------------------
def Normalize(hist):
    hist.Scale(1/hist.GetEntries())

# ------------------------------------------------------------------------------
def Plot1D(tree, obj_type):

    canv = ROOT.TCanvas()
    canvTemp = ROOT.TCanvas()
        
    # LLP kinematic / decay plotting
    if (obj_type == "LLP"):
        number = ["0", "1"]
        kinematic_vars = ["DecayR", "DecayX", "DecayY", "DecayZ", "DecayD", "DecayT", "DecayCtau", "rechitN"]

        LLP_decay = {}
    
        for var in kinematic_vars:
            legend = ROOT.TLegend(0.65,0.65,0.75,0.75)
            for i in number:
                hname_temp = obj_type + i + var
                if (var == "DecayT"): LLP_decay[i] = ROOT.TH1F(hname_temp, "LLP " + var +" ; " + var +" [cm]; Number of Entries", 3000, 0, 300 ); 
                else: LLP_decay[i] = ROOT.TH1F(hname_temp, "LLP " + var +" ; " + var +" [cm]; Number of Entries", 3000, -3000, 3000 ); 

                dist = obj_type + i + "_" + var
                canvTemp.cd()
                if (time_debug): print("1D LLP loop, drawing tree, with time = " + str(time.time() - start))
                selection_region = MakeSelection(obj_type + i)
                tree.Draw(dist +" >> "+hname_temp, selection_region, "", tree.GetEntries(), 0 )
                
                if i == "1": 
                    ResetRange(LLP_decay[i])
                    legend.AddEntry(LLP_decay[i], obj_type)
                    LLP_decay[i].Add(LLP_decay["0"])
                    canv.cd()
                    LLP_decay[i].Draw("HIST PLC")

            legend.Draw()
            stamp_text = ROOT.TLatex()
            stamp_text.SetNDC()
            stamp_text.DrawLatex( xpos, ypos, cmsLabel)
            canv.SaveAs(folder + obj_type + "_" +var+".png")

    # jet kinematic plotting 
    if (obj_type == "jet"):
        number = ["0", "1", "2"]
        kinematic_vars = ["rechitN", "Eta", "Phi", "etaSpread", "phiSpread", "etaSpread_energy", "phiSpread_energy", "energy", "pt"]
        jet_group = [["etaSpread", "phiSpread"], ["etaSpread_energy", "phiSpread_energy"]]

        jet_kinematic = {}
        for var in kinematic_vars:
            legend = ROOT.TLegend(0.65,0.65,0.8,0.8)
            hs = ROOT.THStack("hs",  "Jet " + var +" ; " + var +";Normalized Number of Entries")
            minXbin = 0
            maxXbin = 0
            for i in number:
                hname_temp = obj_type + i + var
                if (var[1:3] == "ta" or var[1:3] == "hi"): jet_kinematic[i] = ROOT.TH1F(hname_temp, "Jet " + var +" ; " + var +"; Number of Entries", 1000, -4, 4 ); # eta phi range
                else: jet_kinematic[i] = ROOT.TH1F(hname_temp, "Jet " + var +" ; " + var +"; Number of Entries", 300, -100, 500 ); 
                legend.AddEntry(jet_kinematic[i], obj_type + i)

                dist = obj_type + i + "_" + var
                canvTemp.cd()
                if (time_debug): print("1D jet loop, drawing tree, with time = " + str(time.time() - start))
                selection_region = MakeSelection(obj_type + i)
                tree.Draw(dist +" >> "+hname_temp, selection_region, "", tree.GetEntries(), 0 )
                
                Normalize(jet_kinematic[i])
                if (jet_kinematic[i].FindFirstBinAbove() > minXbin): minXbin = jet_kinematic[i].FindFirstBinAbove()
                if (jet_kinematic[i].FindLastBinAbove() > maxXbin): maxXbin = jet_kinematic[i].FindLastBinAbove()
                hs.Add(jet_kinematic[i])

            canv.cd()
            hs.Draw("HIST PLC nostack")
            hs.GetXaxis().SetRange(minXbin, maxXbin)
            legend.Draw()
            stamp_text = ROOT.TLatex()
            stamp_text.SetNDC()
            stamp_text.DrawLatex( xpos, ypos, cmsLabel)
            canv.SaveAs(folder + obj_type + "_" +var+".png")

        # jet 2D distributions
        jet_dist = {}
        for var in jet_group:
            for i in number:
                legend = ROOT.TLegend(0.65,0.65,0.75,0.75)
                xaxis = 0.25
                if var[0] == "etaSpread": xaxis = 0.35
                hname_temp = obj_type + var[0] + i
                jet_dist[i] = ROOT.TH2F(hname_temp, "Jet " + i + " " + var[0] + " vs. " + var[1] + " ; " + var[0] + "; " + var[1], 100, 0, xaxis, 100, 0, xaxis ); 
                legend.AddEntry(jet_dist[i], obj_type + i)

                dist1 = obj_type + i + "_" + var[0] 
                dist2 = obj_type + i + "_" + var[1] 
                canvTemp.cd()
                selection_region = MakeSelection(obj_type + i)
                tree.Draw(dist1 + ":" + dist2 +" >> "+hname_temp, selection_region, "", tree.GetEntries(), 0 )
                canv.cd()
                jet_dist[i].Draw("COLZ PLC")

                legend.Draw()
                stamp_text = ROOT.TLatex()
                stamp_text.SetNDC()
                stamp_text.DrawLatex( xpos, ypos, cmsLabel)
                canv.SaveAs(folder + obj_type + i + "_" +var[0]+ "_" +var[1]+ ".png")

# ------------------------------------------------------------------------------
def Plot2D(tree, obj_type):

    # LLP and jet are essentially treated the same here
    # fraction of energy in each depth
    number = ["0", "1", "2"]
    kinematic_vars = ["_energyFrac_depth"]

    energy_profile, energy_depth_profile = {}, {}

    canvTemp = ROOT.TCanvas()
    canv = ROOT.TCanvas()
    canvDepth = ROOT.TCanvas()

    legend = ROOT.TLegend(0.65,0.65,0.8,0.8)
    for i in number:
        if (i == "2" and obj_type != "jet"): continue
        hname = obj_type + i + "_energy_depth_profile"
        energy_depth_profile[i] = ROOT.TH1F(hname, obj_type + " Depth Energy Profile ; Depth (HB); Fraction of Energy", 6,0,6 ); 

        hs = ROOT.THStack("hs",  obj_type + i + " Energy Fraction in HCAL Depths; Fraction of HCAL Rechit Energy; Normalized Number of Entries")
        legend_depth = ROOT.TLegend(0.65,0.65,0.8,0.8)
        for depth in range(4):
            depth += 1
            var = obj_type + i + kinematic_vars[0] + str(depth)
            hname_temp = obj_type + i + "_energy_profile_d"+str(depth)
            energy_profile[(depth,i)] = ROOT.TH1F(hname_temp, obj_type+" Depth Energy Frac in Depth "+str(depth)+"; Depth (HB); Fraction of Energy", 100, 0, 1 ); # replicate the initial plots first

            canvTemp.cd()
            if (time_debug): print("2D loop, drawing tree, with time = " + str(time.time() - start))
            selection_region = MakeSelection(obj_type + i)
            tree.Draw(var +" >> "+hname_temp, selection_region, "", tree.GetEntries(), 0 )

            # for LLP, combine distributions
            if obj_type == "LLP":
                if i == "0": continue
                if i == "1": energy_profile[(depth,i)].Add(energy_profile[(depth,"0")]) # determine averages for summed LLP1 and LLP2
                
            Normalize(energy_profile[(depth,i)])
            legend_depth.AddEntry(energy_profile[(depth,i)], "depth = " + str(depth))
            hs.Add(energy_profile[(depth,i)])

            AverageEnergyFrac = energy_profile[(depth,i)].GetMean()
            AverageEnergyFrac_error = energy_profile[(depth,i)].GetMeanError()
            
            energy_depth_profile[i].SetBinContent(depth+1, AverageEnergyFrac)
            energy_depth_profile[i].SetBinError(depth+1, AverageEnergyFrac_error)

        if obj_type == "LLP" and i == "0": continue
        canv.cd()
        hs.Draw("HIST PLC nostack")
        legend_depth.Draw()
        stamp_text = ROOT.TLatex()
        stamp_text.SetNDC()
        stamp_text.DrawLatex( xpos, ypos, cmsLabel)
        canv.SaveAs(folder + obj_type + i + "_energyFractionOverlay.png")

        # average energy fraction vs depth
        if i == "0" and obj_type == "LLP": continue
        canvDepth.cd()
        legend.AddEntry(energy_depth_profile[i], obj_type + i)
        energy_depth_profile[i].GetYaxis().SetRangeUser(0,0.7)
        if i == "0": energy_depth_profile[i].Draw("HIST E1 PLC")
        else: energy_depth_profile[i].Draw("SAME HIST E1 PLC")

    if (time_debug): print("Plotting results, with time = " + str(time.time() - start))
    legend.Draw()
    stamp_text = ROOT.TLatex()
    stamp_text.SetNDC()
    stamp_text.DrawLatex( xpos, ypos, cmsLabel)
    canvDepth.SaveAs(folder + obj_type + "_energyProfile.png")

# ------------------------------------------------------------------------------
def main():

	infilepath = "../Run/hists_test.root"

	if len(sys.argv) > 1: infilepath = sys.argv[1]

	PlotSetup(infilepath)

if __name__ == '__main__':
	main()