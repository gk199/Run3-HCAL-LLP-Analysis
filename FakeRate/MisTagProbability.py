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
  cmsLabelExtra = "#scale[0.8]{#it{2023 LLP Skim}}"
  yearLumi = "#scale[0.85]{2023 (13.6 TeV)}"
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
def ProbabilityEst(infilepath):
    infile = ROOT.TFile.Open( infilepath )
    tree = infile.Get("WPlusJets")
    MisTagProbability(tree, "jet")

# ------------------------------------------------------------------------------
def BackgroundPrediction(infilepath):
    infile = ROOT.TFile.Open( infilepath )
    tree = infile.Get("NoSel")
    MisTagPrediction(tree, "jet")

# ------------------------------------------------------------------------------
def SignalDistribution(infilepath1, infilepath2, infilepath3, infilepath4, infilepath5, signal_names, selection, infilepath_bkg, selection_bkg):
    tree_list = []
    #for path in infilepath_list: # when tried this way, got error "AttributeError: 'CPyCppyy_NoneType' object has no attribute 'Draw'" so doing the slow messy way for now
    infile1 = ROOT.TFile.Open( infilepath1 )
    tree1 = infile1.Get( selection ) # selection is tree (NoSel for signal)
    infile2 = ROOT.TFile.Open( infilepath2 )
    tree2 = infile2.Get( selection )
    infile3 = ROOT.TFile.Open( infilepath3 )
    tree3 = infile3.Get( selection )
    infile4 = ROOT.TFile.Open( infilepath4 )
    tree4 = infile4.Get( selection )
    infile5 = ROOT.TFile.Open( infilepath5 )
    tree5 = infile5.Get( selection )

    infile_bkg = ROOT.TFile.Open( infilepath_bkg )
    tree_bkg = infile_bkg.Get( selection_bkg )
    SignalJetTagged(tree1, tree2, tree3, tree4, tree5, signal_names, tree_bkg, "jet")
    
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
    if (radius == "HCAL"):     selection_radius = radius_inHCAL
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
                xaxis = int(300)
                hname_temp = obj_type + var[0] + i
                LLP_dist[i] = ROOT.TH2F(hname_temp, "Jet " + i + " " + var[0] + " vs. " + var[1] + "  r=" + radius + "; " + var[1] + "; " + var[0], 100, 0, xaxis, int(100/2), 0, int(xaxis/6) ); 
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
def SignalJetTagged(tree1, tree2, tree3, tree4, tree5, signal_names, tree_bkg, obj_type):
    if (obj_type == "jet"):
        number = ["0", "1", "2", "3", "4", "5"]
        LLP_matching = ["L1trig_Matched"] # jet + number + var = full histogram name
        LLP_BDTscore = ["bdtscoreX_LLP350_MS80_perJet"]

        tree_list = [tree1, tree2, tree3, tree4, tree5]

        JetDist = {}
        JetDist_bkg = {}
        TagJets = {}
        N_tot = {}

        triggered = [-9999,0,1]

        for trig_matched in triggered:
            canv = ROOT.TCanvas()
            legend = ROOT.TLegend(0.6,0.2,0.77,0.2 + 0.07 * len(tree_list))

            for name in signal_names: 
                TagJets[name] = ROOT.TH1F("Tagged Jet " +  str(trig_matched) + name, "Tagged Jet, for L1 jet matching=" + str(trig_matched) + " ; pT order of tagged jet; Fraction of tagged jets", 6, 0, 6 ); 
                N_tot[name] = 0
            TagJets_bkg = ROOT.TH1F("Tagged Jet Background " +  str(trig_matched), "Tagged Jet, for L1 jet matching=" + str(trig_matched) + " ; pT order of tagged jet; Fraction of tagged jets", 6, 0, 6 ); 
            N_tot_bkg = 0

            for i in number:
                
                selection_region = GetCut(obj_type + i + "_" + LLP_matching[0], trig_matched) # require matching trigger set or not set
                pT_region = GetCut(obj_type + i + "_Pt", [40,1000])                         # require jet pT is over 40 GeV
                eta_region = GetCut(obj_type + i + "_Eta", [-1.26,1.26])                    # require jet eta is in HB 
                denom_cut = selection_region + pT_region + eta_region
                BDTcut_region = GetCut(obj_type + i + "_" + LLP_BDTscore[0], [0.9, 1.1])

                LLP_selection_region = MakeSelection(obj_type + i, "HCAL")                  # determine if this jet is matched to a LLP that decays in the region of interest (signal plots only)

                JetPt = obj_type + i + "_Pt" # use this as a distribution to draw, then integrate over

                JetDist[i] = {}
                N_flagged = {}
                for tree_sig, name in zip(tree_list, signal_names):
                    hname_temp = name + "Signal_" + obj_type + i + "_matched_" + str(trig_matched)
                    JetDist[i][name] = ROOT.TH1F(hname_temp, "Jet " + LLP_matching[0] +  "_" + str(trig_matched) + "; pT order of tagged jet; Fraction of tagged jets", 100, 0, 1000 ); 
                    tree_sig.Draw(JetPt +" >> "+hname_temp, denom_cut + BDTcut_region + LLP_selection_region, "", tree_sig.GetEntries(), 0 ) # require matching variable set + LLP pt is high enough

                    N_flagged[name] = JetDist[i][name].Integral()
                    N_tot[name] += N_flagged[name]

                hname_temp_bkg = "Background_" + obj_type + i + "_matched_" + str(trig_matched)
                JetDist_bkg[i] = ROOT.TH1F(hname_temp_bkg, "Jet " + LLP_matching[0] +  "_" + str(trig_matched) + "; pT order of tagged jet; Fraction of tagged jets", 100, 0, 1000 ); 
                tree_bkg.Draw(JetPt +" >> "+hname_temp_bkg, denom_cut + BDTcut_region, "", tree_bkg.GetEntries(), 0 ) # require matching variable set + LLP pt is high enough

                N_flagged_bkg = JetDist_bkg[i].Integral()
                N_tot_bkg += N_flagged_bkg

                for name in signal_names: 
                    TagJets[name].SetBinContent(int(i)+1, N_flagged[name]) # remember to fill first bin (not 0!)
                    if (N_flagged[name] > 0): TagJets[name].SetBinError(int(i)+1, sqrt(1/N_flagged[name]))
                TagJets_bkg.SetBinContent(int(i)+1, N_flagged_bkg)
                if (N_flagged_bkg > 0): TagJets_bkg.SetBinError(int(i)+1, sqrt(1/N_flagged_bkg))

            for name in signal_names: 
                TagJets[name].Scale(1/N_tot[name]) # fraction, not number
                legend.AddEntry(TagJets[name], name)

            TagJets_bkg.Scale(1/N_tot_bkg) # fraction, not number
            legend.AddEntry(TagJets_bkg, "W+Jets")

            canv.cd()
            iteration = 0

            mean_text = ROOT.TLatex()
            mean_text.SetNDC()
            mean_text.SetTextFont(42)
            mean_text.SetTextSize(0.036)
            for name in signal_names: 
                TagJets[name].GetYaxis().SetRangeUser(0, 1.05)
                if iteration > 0: TagJets[name].Draw("SAME HIST PLC E1")
                else: TagJets[name].Draw("HIST PLC E1")
                mean_text.DrawLatex( xpos+0.45, ypos - iteration*0.05, "Sig (" + name + ") mean = %.2f" %(TagJets[name].GetMean()))
                iteration += 1
            TagJets_bkg.SetFillStyle(3004)
            TagJets_bkg.Draw("SAME HIST PLC PFC E1")
            mean_text.DrawLatex( xpos+0.45, ypos-len(tree_list)*0.05, "Bkg mean = %.2f" %(TagJets_bkg.GetMean()))

            LegendLabel(legend)
            canv.SaveAs(folder + "/Tagged_" + obj_type +"_trigMatch" + str(trig_matched) + ".png")

# ------------------------------------------------------------------------------
# def SetupNumeratorDenominator(tree, obj_type):

# ------------------------------------------------------------------------------
def MisTagProbability(tree, obj_type):
    
    if (obj_type == "jet"):
        # number = ["0", "1", "2", "3", "4", "5"]
        number = ["0"]

        LLP_matching = ["L1trig_Matched"] # jet + number + var = full histogram name
        LLP_BDTscore = ["bdtscoreX_LLP350_MS80_perJet"]
        jet_kinematics = ["Eta", "Phi", "Pt"]
        bin_num = [12, 6, 6]
        plot_x_range = [1.26, 3.2, 1]
        plot_y_range = [0.01, 0.007, 0.02]
        bin_widths = np.array([40, 60, 80, 120, 160, 240, 400], dtype='float64') 

        misTagJets = {}
        allJets = {}

        triggered = [-9999,0,1]

        outfile = ROOT.TFile.Open("outfile.root", "RECREATE")

        for trig_matched in triggered:
            counter = 0
            for var in jet_kinematics:
                canv = ROOT.TCanvas()
                legend = ROOT.TLegend(0.8,0.72,0.87,0.8)

                if var != "Pt":
                    misTagJets_6 = ROOT.TH1F("Numerator " + var, "Jet;" + var + " Efficiency by jet; Jet Tagging Efficiency", bin_num[counter], -plot_x_range[counter], plot_x_range[counter] ); 
                    allJets_6 = ROOT.TH1F("Denominator "+ var, "Jet;" + var + " Efficiency by jet; Jet Tagging Efficiency", bin_num[counter], -plot_x_range[counter], plot_x_range[counter] ); 
                else:
                    misTagJets_6 = ROOT.TH1F("Numerator " + var, "Jet;" + var + " Efficiency by jet; Jet Tagging Efficiency", bin_num[counter], bin_widths ); 
                    allJets_6 = ROOT.TH1F("Denominator "+ var, "Jet;" + var + " Efficiency by jet; Jet Tagging Efficiency", bin_num[counter], bin_widths ); 

                for i in number:
                    hname_temp = obj_type + i + "_" + var + "_highScore"
                    hname_denom = obj_type + i + "_" + var
                    if var != "Pt":
                        misTagJets[i] = ROOT.TH1F(hname_temp, "Jet " + LLP_matching[0] + "; " + var +" efficiency by jet; Jet Tagging Efficiency", bin_num[counter], -plot_x_range[counter], plot_x_range[counter] ); 
                        allJets[i] = ROOT.TH1F(hname_denom, "Jet " + LLP_matching[0] + "; " + var +" efficiency by jet; Jet Tagging Efficiency", bin_num[counter], -plot_x_range[counter], plot_x_range[counter] ); 
                    else:
                        misTagJets[i] = ROOT.TH1F(hname_temp, "Jet " + LLP_matching[0] + "; " + var +" efficiency by jet; Jet Tagging Efficiency", bin_num[counter], bin_widths ); 
                        allJets[i] = ROOT.TH1F(hname_denom, "Jet " + LLP_matching[0] + "; " + var +" efficiency by jet; Jet Tagging Efficiency", bin_num[counter], bin_widths ); 

                    selection_region = GetCut(obj_type + i + "_" + LLP_matching[0], trig_matched) # require matching trigger set or not set
                    pT_region = GetCut(obj_type + i + "_Pt", [40,1000])                         # require jet pT is over 40 GeV
                    eta_region = GetCut(obj_type + i + "_Eta", [-1.26,1.26])                    # require jet eta is in HB 
                    denom_cut = selection_region + pT_region + eta_region
                    BDTcut_region = GetCut(obj_type + i + "_" + LLP_BDTscore[0], [0.9, 1.1])

                    LLP_radius = obj_type + i + "_" + var
                    tree.Draw(LLP_radius +" >> "+hname_temp, denom_cut + BDTcut_region, "", tree.GetEntries(), 0 ) # require matching variable set + LLP pt is high enough
                    tree.Draw(LLP_radius +" >> "+hname_denom, denom_cut, "", tree.GetEntries(), 0 ) # require LLP pt is high enough and in HB
                    
                    misTagJets_6.Add(misTagJets[i])
                    allJets_6.Add(allJets[i])

                legend.AddEntry(misTagJets_6, obj_type + " " + str(len(number)-1))
                # check efficiency on the whole distribution now
                if(ROOT.TEfficiency.CheckConsistency(misTagJets_6, allJets_6)):
                    pEff = ROOT.TEfficiency(misTagJets_6,allJets_6)
                    canv.cd()
                    pEff.Draw()

                    ROOT.gPad.Update()
                    pEff.GetPaintedGraph().GetYaxis().SetRangeUser(0, plot_y_range[counter])
                    pEff.SetTitle("Mis-Tag Probability for L1 trigger matched = " + str(trig_matched))

                    mean_text = ROOT.TLatex()
                    mean_text.SetNDC()
                    mean_text.SetTextFont(42)
                    mean_text.SetTextSize(0.036)
                    mean_text.DrawLatex( xpos+0.5, ypos, "mean = %.2f" %(misTagJets_6.GetMean()))
                    mean_text.DrawLatex( xpos+0.5, ypos-0.05, "#sigma = %.2f" %(misTagJets_6.GetStdDev()))

                    LegendLabel(legend)
                    canv.SaveAs(folder + "/Efficiency_" + obj_type + "_" +var+"_trigMatch" + str(trig_matched) + ".png")
                    outfile.WriteObject(pEff, "Efficiency_" + obj_type + "_" +var+"_trigMatch" + str(trig_matched))
                counter += 1
        outfile.Close()

# ------------------------------------------------------------------------------
def MisTagPrediction(tree, obj_type):
    if (obj_type == "jet"):
        # number = ["0", "1", "2", "3", "4", "5"]
        number = ["0"]

        LLP_matching = ["L1trig_Matched"] # jet + number + var = full histogram name
        LLP_BDTscore = ["bdtscoreX_LLP350_MS80_perJet"]
        jet_kinematics = ["Eta", "Phi", "Pt"]
        bin_num = [12, 6, 6]
        plot_x_range = [1.26, 3.2, 1]
        plot_y_range = [0.01, 0.007, 0.02]
        bin_widths = np.array([40, 60, 80, 120, 160, 240, 400], dtype='float64') 

        misTagJets = {}
        allJets = {}
        allJets_actual = {}

        triggered = [-9999,0,1]

        mistag_file = ROOT.TFile.Open("outfile.root")

        for trig_matched in triggered:
            counter = 0
            for var in jet_kinematics:
                canv = ROOT.TCanvas()
                canv1 = ROOT.TCanvas()
                legend = ROOT.TLegend(0.7,0.65,0.87,0.8)

                hs = ROOT.THStack( "hs_temp", "Predicted and Actual Mis-Tag Jets for L1 trigger matched = " + str(trig_matched) + "; Jet " + var + "; Number of Mis-tagged Jets ")
                if var != "Pt":
                    allJets_6_actual = ROOT.TH1F("Actual "+ var + str(trig_matched), "Predicted and Actual Mis-Tag Jets for L1 trigger matched = " + str(trig_matched) + "; Jet " + var + "; Number of Mis-tagged Jets", bin_num[counter], -plot_x_range[counter], plot_x_range[counter] ); 
                    allJets_6 = ROOT.TH1F("Predicted "+ var + str(trig_matched), "Predicted and Actual Mis-Tag Jets for L1 trigger matched = " + str(trig_matched) + "; Jet " + var + "; Number of Mis-tagged Jets", bin_num[counter], -plot_x_range[counter], plot_x_range[counter] ); 
                else:
                    allJets_6_actual = ROOT.TH1F("Actual "+ var + str(trig_matched), "Predicted and Actual Mis-Tag Jets for L1 trigger matched = " + str(trig_matched) + "; Jet " + var + "; Number of Mis-tagged Jets", bin_num[counter], bin_widths ); 
                    allJets_6 = ROOT.TH1F("Predicted "+ var + str(trig_matched), "Predicted and Actual Mis-Tag Jets for L1 trigger matched = " + str(trig_matched) + "; Jet " + var + "; Number of Mis-tagged Jets", bin_num[counter], bin_widths ); 

                for i in number:                    
                    hname_denom_actual = obj_type + i + "_" + var + "_actualData"
                    hname_denom = obj_type + i + "_" + var + "_forPrediction"
                    if var != "Pt":
                        allJets_actual[i] = ROOT.TH1F(hname_denom_actual, "Jet " + LLP_matching[0] + "; " + var +" efficiency by jet; Predicted and Actual Number of Mis-tagged Jets", bin_num[counter], -plot_x_range[counter], plot_x_range[counter] ); 
                        allJets[i] = ROOT.TH1F(hname_denom, "Jet " + LLP_matching[0] + "; " + var +" efficiency by jet; Predicted and Actual Number of Mis-tagged Jets", bin_num[counter], -plot_x_range[counter], plot_x_range[counter] ); 
                    else:
                        allJets_actual[i] = ROOT.TH1F(hname_denom_actual, "Jet " + LLP_matching[0] + "; " + var +" efficiency by jet; Predicted and Actual Number of Mis-tagged Jets", bin_num[counter], bin_widths ); 
                        allJets[i] = ROOT.TH1F(hname_denom, "Jet " + LLP_matching[0] + "; " + var +" efficiency by jet; Predicted and Actual Number of Mis-tagged Jets", bin_num[counter], bin_widths ); 

                    selection_region = GetCut(obj_type + i + "_" + LLP_matching[0], trig_matched) # require matching trigger set or not set
                    pT_region = GetCut(obj_type + i + "_Pt", [40,1000])                         # require jet pT is over 40 GeV
                    eta_region = GetCut(obj_type + i + "_Eta", [-1.26,1.26])                    # require jet eta is in HB 
                    BDTcut_region = GetCut(obj_type + i + "_" + LLP_BDTscore[0], [0.9, 1.1])
                    denom_cut = selection_region + pT_region + eta_region

                    LLP_radius = obj_type + i + "_" + var
                    tree.Draw(LLP_radius +" >> "+hname_denom, denom_cut, "", tree.GetEntries(), 0 ) # require LLP pt is high enough and in HB
                    tree.Draw(LLP_radius +" >> "+hname_denom_actual, denom_cut + BDTcut_region, "", tree.GetEntries(), 0 ) # require LLP pt is high enough and in HB
                    
                    allJets_6_actual.Add(allJets_actual[i])
                    allJets_6.Add(allJets[i])

                canv.cd()
                #ROOT.gStyle.SetPalette(ROOT.kOcean)
                MisTagEffDist = mistag_file.Get("Efficiency_" + obj_type + "_" +var+"_trigMatch" + str(trig_matched))  
                MisTagEff_Denom = MisTagEffDist.GetCopyTotalHisto() # need to get a TH1 from TEff object
                MisTagEff_Num = MisTagEffDist.GetCopyPassedHisto() # need to get a TH1 from TEff object
                allJets_6.Multiply(MisTagEff_Num)
                allJets_6.Divide(MisTagEff_Denom) # confirmed that actual and predicted on W+jets (redundant) gives same answer

                hs.Add(allJets_6) # predicted
                legend.AddEntry(allJets_6, "Predicted Mis-tag")
                hs.Add(allJets_6_actual)
                legend.AddEntry(allJets_6_actual, "Actual Mis-tag")
                allJets_6_actual.SetFillStyle(3004) # to keep consistent, data is filled, MC or predictions are lines # but THstack doesn't use different fill styles :( 
                allJets_6.SetFillStyle(0)
                hs.Draw("HIST NOSTACK PLC PFC E1")
                LegendLabel(legend)
                canv.SaveAs(folder + "/PredictedMisTag_" + obj_type + "_" +var+"_trigMatch" + str(trig_matched) + ".png")
                
                counter += 1

# ------------------------------------------------------------------------------
def main():

    infilepath = "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.7.1/minituple_v3.7_LLPskim_Run2023_HADD.root"

    if len(sys.argv) > 1: infilepath = sys.argv[1]

    LLP1 = "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.7.1/minituple_v3.7_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-15_CTau1000_13p6TeV_2024_03_14_TEST.root"
    LLP2 = "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.7.1/minituple_v3.7_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-80_CTau500_13p6TeV_2024_03_14_TEST.root"
    LLP3 = "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.7.1/minituple_v3.7_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-50_CTau3000_13p6TeV_2024_03_14_batch2.root"
    LLP4 = "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.7.1/minituple_v3.7_LLP_MC_ggH_HToSSTobbbb_MH-250_MS-120_CTau10000_13p6TeV_2024_03_14_batch2.root"
    LLP5 = "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.7.1/minituple_v3.7_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-160_CTau10000_13p6TeV_2024_03_14_batch2.root"
    LLP_list = {LLP1, LLP2, LLP3, LLP4, LLP5}
    LLP_names = ["LLP 350,80", "LLP 125,15", "LLP 125,50", "LLP 250,120", "LLP 350,160"]

    SignalDistribution(LLP1, LLP2, LLP3, LLP4, LLP5, LLP_names, "NoSel", infilepath, "WPlusJets")

    ProbabilityEst(infilepath)
    BackgroundPrediction(infilepath)

if __name__ == '__main__':
	main()