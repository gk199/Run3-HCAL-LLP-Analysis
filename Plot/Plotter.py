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
xpos = 0.13
ypos = 0.85

canv = ROOT.TCanvas("c","c",800,600)
overlayCanv = ROOT.TCanvas("c1","c1",800,600)
cmsLabel = "#scale[1]{#bf{CMS} }"
if (data): 
  cmsLabelExtra = "#scale[0.8]{#it{HCAL LLP Skim}}"
  yearLumi = "#scale[0.85]{2023C (13.6 TeV)}"
  folder = "./outPlots"
if (MC): 
  cmsLabelExtra = "#scale[0.8]{#it{2023 LLP MC}}"
  yearLumi = "#scale[0.85]{#sqrt{s} = 13.6 TeV}"
  folder = "./outPlots_MC"

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
def GetCut( branch_name, branch_sel):

	selection = ""

	if type(branch_sel) is int:
		selection = branch_name + " == " + str(branch_sel) 

	elif type(branch_sel) is list:
		if len(branch_sel) == 0:
			print("WARNING <GetCut> : Entry error (length = 0). No cut implemented")
		elif len(branch_sel) == 1:
			selection = branch_name + " == " + str(branch_sel[0])
		else:
			selection = branch_name + " >= " + str(branch_sel[0]) + " && " + branch_name + " < " + str(branch_sel[1])

		if len(branch_sel) > 2:
			print("WARNING <GetCut> : 'branch_sel' input has more than two entries. Only using the first two!")

	else: 
		print("WARNING <GetCut> : Type Error. No cut implemented")

	return ROOT.TCut( selection + " " )

# ------------------------------------------------------------------------------
def GetCut_LLPmatch( variable, branch_name, branch_sel ):
    # variable = jetx_IsMatchedTo or LLPx. branch_name = _DecayR. branch_sel = radius for LLP

    # if just LLP, do below cuts
    if (variable[0:3] == "LLP"):
        selection = ""
        if type(branch_sel) is int:
		    selection = variable + branch_name + " == " + str(branch_sel)
        elif type(branch_sel) is list:
            if len(branch_sel) == 0:
			    print("WARNING <GetCut_LLPmatch> : Entry error (length = 0). No cut implemented")
            elif len(branch_sel) == 1:
			    selection = variable + branch_name + " == " + str(branch_sel[0])
            elif len(branch_sel) == 2:
		    	selection = variable + branch_name + " >= " + str(branch_sel[0]) + " && " + variable + branch_name + " < " + str(branch_sel[1])
            else:
	    		print("WARNING <GetCut_LLPmatch> : 'branch_sel' input has more than two entries. Only using the first two!")
        else: 
	    	print("WARNING <GetCut_LLPmatch> : Type Error. No cut implemented")
        
        if debug: print(selection + " in Get Cut (on LLP) ")
        return ROOT.TCut( selection + " " )

    # if jet, a bit more complicated. Which LLP does the jet in question match to? 
    if (variable[0:3] == "jet"):
        jet_match_selection_0 = variable + " == 0"
        jet_match_selection_1 = variable + " == 1"
    
        selection_0 = ""
        selection_1 = ""
    
        if type(branch_sel) is int:
		    selection_0 = "LLP0" + branch_name + " == " + str(branch_sel) + " && " + jet_match_selection_0
		    selection_1 = "LLP1" + branch_name + " == " + str(branch_sel) + " && " + jet_match_selection_1
    
        elif type(branch_sel) is list:
            if len(branch_sel) == 0:
                print("WARNING <GetCut_LLPmatch> : Entry error (length = 0). No cut implemented")
            elif len(branch_sel) == 1:
	    		selection_0 = "LLP0" + branch_name + " == " + str(branch_sel[0]) + " && " + jet_match_selection_0
		    	selection_1 = "LLP1" + branch_name + " == " + str(branch_sel[0]) + " && " + jet_match_selection_1
            elif len(branch_sel) == 2:
	    		selection_0 = "LLP0" + branch_name + " >= " + str(branch_sel[0]) + " && " + "LLP0" + branch_name + " < " + str(branch_sel[1]) + " && " + jet_match_selection_0
		    	selection_1 = "LLP1" + branch_name + " >= " + str(branch_sel[0]) + " && " + "LLP1" + branch_name + " < " + str(branch_sel[1]) + " && " + jet_match_selection_1
            else:
			    print("WARNING <GetCut_LLPmatch> : 'branch_sel' input has more than two entries. Only using the first two!")    
        else: 
		    print("WARNING <GetCut_LLPmatch> : Type Error. No cut implemented")
        
        if debug: print("(" + selection_0 + ") || (" + selection_1 + ")  in Get Cut (on jet) ")
        return ROOT.TCut( "(" + selection_0 + ") || (" + selection_1 + ") " )

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
def PlotSetup(infilepath):

    infile = ROOT.TFile.Open( infilepath )
    tree = infile.Get("NoSel")
    
    obj_type = ["LLP", "jet"]
    radius = ["all", "preECAL", "ECAL", "depth12", "depth34", "depth3", "depth4"]

    for obj in obj_type:
        print("Plotting for " + obj + " ----------------------------------------------------------------------------------:")
        for rad in radius:
            if (data and (rad != "all" or obj == "LLP")): continue;
            print("Plotting radius = " + rad + " ----------------------------------------------------------------------------- for " + obj + ":")
            #Plot1D(tree, obj, rad)
            #Plot2D(tree, obj, rad)

    LLP_MatchingEfficiency(tree, "LLP")

# ------------------------------------------------------------------------------
def MakeSelection(variable, radius):
    # variable could be jet or LLP. If jet, account for which LLP it is matched to. If LLP, cut directly on LLP
    if (variable[0:3] == "jet"): variable = variable + "_isMatchedTo"
    cut_vars = "_DecayR"
   
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
    if (radius == "all"):      selection_radius = radius_all
    if (radius == "preECAL"):  selection_radius = radius_preECAL
    if (radius == "ECAL"):     selection_radius = radius_ECAL
    if (radius == "depth12"):  selection_radius = radius_depth12
    if (radius == "depth34"):  selection_radius = radius_depth34
    if (radius == "depth3"):   selection_radius = radius_depth3
    if (radius == "depth4"):   selection_radius = radius_depth4
    selection_region = GetCut_LLPmatch(variable, cut_vars, selection_radius)

    return selection_region

# ------------------------------------------------------------------------------
def ResetRange(hist):
    minXbin = hist.FindFirstBinAbove()
    maxXbin = hist.FindLastBinAbove()

    hist.GetXaxis().SetRange(minXbin, maxXbin)

# ------------------------------------------------------------------------------
def Normalize(hist):
    if (hist.GetEntries() > 0): hist.Scale(1/hist.GetEntries())

# ------------------------------------------------------------------------------
def LegendLabel(legend):
    legend.Draw()
    stamp_text = ROOT.TLatex()
    stamp_text.SetNDC()
    stamp_text.SetTextFont(42)
    stamp_text.SetTextSize(0.036)
    stamp_text.DrawLatex( xpos, ypos, cmsLabel)
    stamp_text.DrawLatex( xpos+0.06, ypos, cmsLabelExtra)
    stamp_text.DrawLatex( 0.75, 0.91, yearLumi)

# ------------------------------------------------------------------------------
def Plot1D(tree, obj_type, radius):

    canv = ROOT.TCanvas()
    canvTemp = ROOT.TCanvas()
        
    # LLP kinematic / decay plotting
    if (obj_type == "LLP"):
        number = ["0", "1"]
        kinematic_vars = ["DecayR", "DecayX", "DecayY", "DecayZ", "DecayD", "DecayT", "DecayCtau", "RechitN", "dR_b"]
        LLP_group = [["RechitN", "DecayR"]]
        LLP_decay = {}

        for var in kinematic_vars:
            legend = ROOT.TLegend(0.65,0.65,0.75,0.75)
            for i in number:
                hname_temp = obj_type + i + var
                if (var == "DecayT" or var == "dR_b"): 
                    if (var == "DecayT"): LLP_decay[i] = ROOT.TH1F(hname_temp, "LLP " + var +"  r=" + radius + "; " + var +" [cm]; Number of Entries", 3000, 0, 300 ); 
                    if (var == "dR_b"): LLP_decay[i] = ROOT.TH1F(hname_temp, "#Delta R (LLP, b),  r=" + radius + "; #Delta R; Number of Entries", 100, 0, 1.6 ); 
                else: LLP_decay[i] = ROOT.TH1F(hname_temp, "LLP " + var +", r=" + radius + "; " + var +" [cm]; Number of Entries", 3000, -3000, 3000 ); 

                dist = obj_type + i + "_" + var
                canvTemp.cd()
                if (time_debug): print("1D LLP loop, drawing tree, with time = " + str(time.time() - start))
                selection_region = MakeSelection(obj_type + i, radius)
                tree.Draw(dist +" >> "+hname_temp, selection_region, "", tree.GetEntries(), 0 )
                
                if i == "1": 
                    ResetRange(LLP_decay[i])
                    legend.AddEntry(LLP_decay[i], obj_type)
                    LLP_decay[i].Add(LLP_decay["0"])
                    canv.cd()
                    LLP_decay[i].Draw("HIST PLC")
                    mean_text = ROOT.TLatex()
                    mean_text.SetNDC()
                    mean_text.SetTextFont(42)
                    mean_text.SetTextSize(0.036)
                    mean_text.DrawLatex( xpos+0.4, ypos, "mean = %.2f" %(LLP_decay[i].GetMean()))
                    mean_text.DrawLatex( xpos+0.4, ypos-0.05, "#sigma = %.2f" %(LLP_decay[i].GetStdDev()))

            LegendLabel(legend)
            OutputFolder("_" + radius)
            canv.SaveAs(folder + "_" + radius + "/" + obj_type + "_" +var+".png")

        # LLP 2D distributions
        LLP_dist = {}
        for var in LLP_group:
            for i in number:
                legend = ROOT.TLegend(0.65,0.65,0.75,0.75)
                xaxis = 300
                hname_temp = obj_type + var[0] + i
                LLP_dist[i] = ROOT.TH2F(hname_temp, "Jet " + i + " " + var[0] + " vs. " + var[1] + "  r=" + radius + "; " + var[1] + "; " + var[0], 100, 0, xaxis, 100/2, 0, xaxis/6 ); 
                legend.AddEntry(LLP_dist[i], obj_type + i)

                dist1 = obj_type + i + "_" + var[0] 
                dist2 = obj_type + i + "_" + var[1] 
                canvTemp.cd()
                selection_region = MakeSelection(obj_type + i, radius)
                tree.Draw(dist1 + ":" + dist2 +" >> "+hname_temp, selection_region, "", tree.GetEntries(), 0 )
                if i == "1": 
                    LLP_dist[i].Add(LLP_dist["0"])
                    canv.cd()
                    LLP_dist[i].Draw("COLZ PLC")
                    LegendLabel(legend)
                    canv.SaveAs(folder + "_" + radius + "/" + obj_type + i + "_" +var[0]+ "_" +var[1]+ ".png")

    # jet kinematic plotting 
    if (obj_type == "jet"):
        number = ["0", "1", "2"]
        kinematic_vars = ["RechitN", "Eta", "Phi", "EtaSpread", "PhiSpread", "EtaSpread_energy", "PhiSpread_energy", 
                            "E", "Pt", 
                            "Track0Pt", "Track1Pt", "Track2Pt", 
                            "Track0dzToPV", "Track1dzToPV", "Track2dzToPV", "Track0dxyToBS", "Track1dxyToBS", "Track2dxyToBS",
                            "Track0dzOverErr", "Track1dzOverErr", "Track2dzOverErr", "Track0dxyOverErr", "Track1dxyOverErr", "Track2dxyOverErr",
                            "TDCavg", "TDCavg_energyWeight", "TDCnDelayed"]
        jet_group = [[ "PhiSpread", "EtaSpread"], ["PhiSpread_energy", "EtaSpread_energy"]]

        jet_kinematic = {}
        for var in kinematic_vars:
            legend = ROOT.TLegend(0.65,0.65,0.8,0.8)
            hs = ROOT.THStack("hs",  "Jet " + var +", r=" + radius + "; " + var +";Normalized Number of Entries")
            minXbin = 0
            maxXbin = 0
            for i in number:
                hname_temp = obj_type + i + var + radius
                nBins = 600
                x_min = -100
                x_max = 500
                if (var[0:3] == "Eta" or var[0:3] == "Phi"): # eta phi range
                    nBins = 100
                    x_min = -4
                    x_max = 4
                    if (var[3:9] == "Spread"):
                        nBins = 600
                if (var[0:5] == "Track"):
                    nBins = 100
                    x_min = 0
                    x_max = 50
                if (var[6:12] == "dzToPV" or var[6:13] == "dxyToBS" ):
                    nBins = 80
                    x_min = -40
                    x_max = 40
                    if (var[6:13] == "dxyToBS"):
                        canv.SetLogy()
                        x_min = -10
                        x_max = 10
                if (var[9:16] == "OverErr"):
                    canv.SetLogy()
                    x_max = 25
                if (var[0:3] == "TDC" or var[0:12] == "energyWeight"):
                    canv.SetLogy()
                    nBins = 3000
                jet_kinematic[i] = ROOT.TH1F(hname_temp, "Jet " + var +"  r=" + radius + "; " + var +"; Number of Entries", nBins, x_min, x_max ); 
                legend.AddEntry(jet_kinematic[i], obj_type + i)

                dist = obj_type + i + "_" + var
                canvTemp.cd()
                if (time_debug): print("1D jet loop, drawing tree, with time = " + str(time.time() - start))
                if (MC): selection_region = MakeSelection(obj_type + i, radius)
                else: selection_region = ""
                tree.Draw(dist +" >> "+hname_temp, selection_region, "", tree.GetEntries(), 0 )
                
                Normalize(jet_kinematic[i])
                if (jet_kinematic[i].FindFirstBinAbove() > minXbin): minXbin = jet_kinematic[i].FindFirstBinAbove()
                if (jet_kinematic[i].FindLastBinAbove() > maxXbin): maxXbin = jet_kinematic[i].FindLastBinAbove()
                hs.Add(jet_kinematic[i])

            canv.cd()
            hs.Draw("HIST PLC nostack")
            hs.GetXaxis().SetRange(minXbin, maxXbin)
            LegendLabel(legend)
            # write mean and sigma for jet 0 to easily compare plots
            mean_text = ROOT.TLatex()
            mean_text.SetNDC()
            mean_text.SetTextFont(42)
            mean_text.SetTextSize(0.036)
            mean_text.DrawLatex( xpos+0.4, ypos, "jet 0 mean = %.2f" %(jet_kinematic["0"].GetMean()))
            mean_text.DrawLatex( xpos+0.4, ypos-0.05, "#sigma = %.2f" %(jet_kinematic["0"].GetStdDev()))
            OutputFolder("_" + radius)
            canv.SaveAs(folder + "_" + radius + "/" + obj_type + "_" +var+".png")

        # jet 2D distributions
        jet_dist = {}
        for var in jet_group:
            for i in number:
                legend = ROOT.TLegend(0.65,0.65,0.75,0.75)
                xaxis = 0.25
                if var[0] == "EtaSpread": xaxis = 0.35
                hname_temp = obj_type + var[0] + i
                jet_dist[i] = ROOT.TH2F(hname_temp, "Jet " + i + " " + var[0] + " vs. " + var[1] + "  r=" + radius + "; " + var[1] + "; " + var[0], 100, 0, xaxis, 100, 0, xaxis ); 
                legend.AddEntry(jet_dist[i], obj_type + i)

                dist1 = obj_type + i + "_" + var[0] 
                dist2 = obj_type + i + "_" + var[1] 
                canvTemp.cd()
                if (MC): selection_region = MakeSelection(obj_type + i, radius)
                else: selection_region = ""
                tree.Draw(dist1 + ":" + dist2 +" >> "+hname_temp, selection_region, "", tree.GetEntries(), 0 )
                canv.cd()
                jet_dist[i].Draw("COLZ PLC")

                LegendLabel(legend)
                canv.SetLogy(0) # linear                
                canv.SaveAs(folder + "_" + radius + "/"  + obj_type + i + "_" +var[0]+ "_" +var[1]+ ".png")

# ------------------------------------------------------------------------------
def Plot2D(tree, obj_type, radius):

    # LLP and jet are essentially treated the same here
    # fraction of energy in each depth
    number = ["0", "1", "2"]
    kinematic_vars = ["_EnergyFrac_Depth", "_EnergyFracLLP_Depth"]

    jet_et_cuts = [""]
    if (obj_type == "jet"): jet_et_cuts = [[0,2000000], [0,200], [200,2000000]]

    for cut in jet_et_cuts:
        for var in kinematic_vars:
            energy_profile, energy_depth_profile = {}, {}

            canvTemp = ROOT.TCanvas()
            canv = ROOT.TCanvas()
            canvDepth = ROOT.TCanvas()
            legend = ROOT.TLegend(0.65,0.65,0.8,0.8)

            if (obj_type == "jet" and var == "_EnergyFracLLP_Depth"): continue # only have this for LLP
            for i in number:
                if (i == "2" and obj_type != "jet"): continue
                hname = obj_type + var + i + "_energy_depth_profile"
                energy_depth_profile[i] = ROOT.TH1F(hname, obj_type + " Depth Energy Profile r=" + radius + "; Depth (HB); Fraction of Energy", 6,0,6 ); 

                hs = ROOT.THStack("hs",  obj_type + i + " Energy Fraction in HCAL Depths, r=" + radius + "; Fraction of HCAL Rechit Energy; Normalized Number of Entries")
                legend_depth = ROOT.TLegend(0.65,0.65,0.8,0.8)
                for depth in range(4):
                    depth += 1
                    var_type = obj_type + i + var + str(depth)
                    hname_temp = var_type + "_energy_profile_d"+str(depth) + "_" + radius + str(cut)
                    energy_profile[(depth,i)] = ROOT.TH1F(hname_temp, obj_type+" Depth Energy Frac in Depth "+str(depth)+ ", r=" + radius + "; Depth (HB); Fraction of Energy", 100, 0, 1 );

                    canvTemp.cd()
                    if (time_debug): print("2D loop, drawing tree, with time = " + str(time.time() - start))
                    if (MC):
                        selection_region = MakeSelection(obj_type + i, radius)
                        if (obj_type == "jet"): selection_region += GetCut(obj_type + i + "_Pt", cut)  # [0, 200]
                        if (debug): print (selection_region)
                    else: selection_region = ""
                    tree.Draw(var_type +" >> "+hname_temp, selection_region, "", tree.GetEntries(), 0 )

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
                canv.SetLogy()
                hs.Draw("HIST PLC nostack")
                LegendLabel(legend_depth)
                canv.SaveAs(folder + "_" + radius + "/" + obj_type + i + var + "_energyFractionOverlay" + str(cut) + ".png")

                # average energy fraction vs depth
                if i == "0" and obj_type == "LLP": continue
                canvDepth.cd()
                legend.AddEntry(energy_depth_profile[i], obj_type + i)
                energy_depth_profile[i].GetYaxis().SetRangeUser(0,0.7)
                if i == "0": energy_depth_profile[i].Draw("HIST E1 PLC")
                else: energy_depth_profile[i].Draw("SAME HIST E1 PLC")

            if (time_debug): print("Plotting results, with time = " + str(time.time() - start))
            LegendLabel(legend)
            canvDepth.SaveAs(folder + "_" + radius + "/" + obj_type + var + "_energyProfile" + str(cut) + ".png")
            
# ------------------------------------------------------------------------------
def LLP_MatchingEfficiency(tree, obj_type):
    # for an LLP, determine efficiency for it's matching to a rechit cluster; and determine efficiency for its matching to a L1 jet
    # plot the efficiency as a function of decay R (or other variables -- start with radius for now)

    # LLP0Decay_RechitEnergy(20Gev)
    # LLP0_RechitEnergy(20GeV)
    # LLP0_isTruthMatched
    # if these are true (or if these pass a cut), then determine number of LLP with this over all LLP

    canv = ROOT.TCanvas()
    canvTemp = ROOT.TCanvas()
    
    if (obj_type == "LLP"):
        number = ["0", "1"]
        LLP_matching = ["Decay_RechitEnergy20GeV", "_RechitEnergy20GeV", "_isTruthMatched", "_isTruthMatched_Jet40"] # LLP + 0 or 1 + kinematic_var = full histogram name
        LLP_denominator = ["DecayR"]

        LLP_effs = {}
        LLP_denom = {}
    
        for var in LLP_matching:
            if (data): continue
            legend = ROOT.TLegend(0.8,0.72,0.87,0.8)
            for i in number:
                hname_temp = obj_type + i + var
                LLP_effs[i] = ROOT.TH1F(hname_temp, "LLP " + var + "; " + var +" efficiency by LLP decayR [cm]; Matching Efficiency", 300, 0, 300 ); 
                hname_denom = obj_type + i + var + "_" + LLP_denominator[0]
                LLP_denom[i] = ROOT.TH1F(hname_denom, "LLP " + var + "; " + var +" efficiency by LLP decayR [cm]; Matching Efficiency", 300, 0, 300 ); 

                selection_region = GetCut(obj_type + i + var, 1)        # require matching variable set
                pT_region = GetCut(obj_type + i + "_Pt", [40,90000])    # require LLP pT is over 40 GeV
                total_cut = selection_region + pT_region
                canvTemp.cd()

                LLP_radius = obj_type + i + "_" + LLP_denominator[0]
                tree.Draw(LLP_radius +" >> "+hname_temp, total_cut, "", tree.GetEntries(), 0 ) # require matching variable set + LLP pt is high enough
                tree.Draw(LLP_radius +" >> "+hname_denom, pT_region, "", tree.GetEntries(), 0 ) # require LLP pt is high enough
                
                if i == "1": 
                    ResetRange(LLP_effs[i])
                    legend.AddEntry(LLP_effs[i], obj_type)
                    LLP_effs[i].Add(LLP_effs["0"])
                    LLP_denom[i].Add(LLP_denom["0"])
                    # check efficiency on the whole distribution now
                    if(ROOT.TEfficiency.CheckConsistency(LLP_effs[i],LLP_denom[i])):
                        pEff = ROOT.TEfficiency(LLP_effs[i],LLP_denom[i])
                        canv.cd()
                        pEff.Draw()
                        mean_text = ROOT.TLatex()
                        mean_text.SetNDC()
                        mean_text.SetTextFont(42)
                        mean_text.SetTextSize(0.036)
                        if (var[1] == "i"):
                            mean_text.DrawLatex( xpos, ypos-.6, "mean = %.2f" %(LLP_effs[i].GetMean()))
                            mean_text.DrawLatex( xpos, ypos-0.65, "#sigma = %.2f" %(LLP_effs[i].GetStdDev()))
                        else:
                            mean_text.DrawLatex( xpos+0.5, ypos, "mean = %.2f" %(LLP_effs[i].GetMean()))
                            mean_text.DrawLatex( xpos+0.5, ypos-0.05, "#sigma = %.2f" %(LLP_effs[i].GetStdDev()))

            LegendLabel(legend)
            canv.SaveAs(folder + "/Efficiency_" + obj_type + "_" +var+".png")

# ------------------------------------------------------------------------------
def main():

	infilepath = "../Run/hists_test.root"

	if len(sys.argv) > 1: infilepath = sys.argv[1]

	PlotSetup(infilepath)

if __name__ == '__main__':
	main()