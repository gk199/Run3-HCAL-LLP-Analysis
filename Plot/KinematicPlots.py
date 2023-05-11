# To run: python KinematicPlots.py <path to output of ../DisplacedHcalJetAnalyzer/util/DisplacedHcalJetAnalyzer.C>

import ROOT
from ROOT import Form
from math import *
import numpy as np
import sys, os, argparse, time, errno
import matplotlib.pyplot as plt

debug = False
data = True
MC = False
if sys.argv[2] == "data": data = True
if sys.argv[2] == "MC": MC = True

# Set up for plots
ROOT.gROOT.SetBatch(1)
ROOT.gStyle.SetOptStat(0)
ROOT.gStyle.SetEndErrorSize(0)
# ROOT.gStyle.SetPalette(ROOT.kDeepSea)
# ROOT.gStyle.SetPalette(ROOT.kBlueGreenYellow)
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
  cmsLabel = "#scale[1.0]{#bf{CMS}} #scale[0.8]{#it{2022 13.6 TeV Collisions Data (JetMET)}}"; 
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

print("Starting plotting script")

categories = {"NoSel", "JetPt40", "PassedHLT"};
objects = {"jet", "ele", "muon", "pho"}
quantities = {"energy", "eta", "phi", "pt"}
jet_quantities = {"energyProfile", "rechitN", "etaSpread", "etaSpread_energy", "phiSpread", "phiSpread_energy"}
leading = [0,1,2]

print("Making plots!")
for obj in objects:
  OutputFolder(obj)
  if (obj == "jet"): quantities_loop = quantities | jet_quantities # these are sets -- use set union operator
  else: quantities_loop = quantities
  for quant in quantities_loop:
    if ((quant == "energyProfile" or quant == "rechitN") and obj != "jet"): continue # only have these plots for jets currently 
    for cat in categories:
      legend = ROOT.TLegend(0.65,0.65,0.8,0.8)
      for i in leading:
        canv.cd()
        hist = infile.Get(cat + "__" + obj + str(i) + "_" + quant)
        if (hist.GetEntries() > 0): 
          if (quant == "energyProfile" and hist.Integral() > 0): hist.Scale(1/hist.Integral()) # for ones that are already normalized (not by entry)
          else: hist.Scale(1/hist.GetEntries())
        hist.Draw("HIST PLC")
        canv.SaveAs(folder + obj + "/" + obj + str(i) + "_" + quant + "_" + cat + ".png")

        overlayCanv.cd()
        overlayCanv.SetLogy()
        if (quant == "energyProfile"): 
          overlayCanv.SetLogy(0)
          hist.GetYaxis().SetRangeUser(0,0.65)

        legend.AddEntry(hist,obj + str(i))
        if (quant == "energy"): hist.GetYaxis().SetRangeUser(0.001,0.6)
        if (quant == "eta"): hist.GetYaxis().SetRangeUser(0.001,0.4)
        if (quant == "phi"): hist.GetYaxis().SetRangeUser(0.00001,0.1)
        if (quant == "pt"): 
          hist.GetYaxis().SetRangeUser(0.001,1)
          hist.GetXaxis().SetRangeUser(0,400)
        if (quant[3:9] == "Spread"): hist.GetYaxis().SetRangeUser(0.0001,0.1)
        if (quant != "energyProfile"): hist.Rebin(5)

        # Overlay various leading / sub-leading quantities 
        if (i == 0):
          hist.Draw("HIST PLC")
        if (i > 0):
          hist.Draw("SAME HIST PLC")
        if (i == 2):
          legend.Draw()
          stamp_text.DrawLatex( xpos, ypos, cmsLabel)
          overlayCanv.SaveAs(folder + obj + "/Overlay_" + obj + str(i) + "_" + quant + "_" + cat + ".png")

print ("2D histogram for eta and phi spread in a jet")
obj = "jet"
plots = {"spreadEtaPhi", "spreadEtaPhi_energy"}
for cat in categories:  
  for plot in plots:
    for i in leading:
      legend = ROOT.TLegend(0.65,0.65,0.8,0.8)
      canv.cd()
      hist = infile.Get(cat + "__" + obj + str(i) + "_" + plot)
      canv.SetLogy(0)
      canv.SetLogz()
      legend.AddEntry(hist,obj + str(i))
      hist.Draw("COLZ")
      legend.Draw()
      stamp_text.DrawLatex(xpos, ypos, cmsLabel)
      canv.SaveAs(folder + obj + "/" + plot + "_" + obj + str(i) + "_" + cat + ".png")

# =========================================================================================================== #
if (MC):
  plots = {"gen_Ddecay", "gen_Rdecay", "gen_Xdecay", "gen_Ydecay", "gen_Zdecay", "gen_cTau", "gen_deltaT"};

  print ("Plots for generator LLP positional information now run: ")
  for plot in plots:
    print (plot)
    legend = ROOT.TLegend(0.65,0.65,0.8,0.8)
    canv.cd()
    hist = infile.Get(plot)
    if (hist.GetEntries() > 0): hist.Scale(1/hist.GetEntries())
    if (plot == "gen_deltaT"): canv.SetLogy()
    else: canv.SetLogy(0)

    if (plot == "gen_cTau"):
      expFit = hist.Fit("expo","S")
      hist.Draw("pe")
      latex = ROOT.TLatex()
      latex.SetNDC()
      latex.SetTextSize(0.03)
      constant = expFit.Parameter(0)
      decay = expFit.Parameter(1)
      print(constant)
      print(decay)
      ctau = -1/decay
      latex.DrawText(0.7, 0.75, "Constant = %.3f"%constant)
      latex.DrawText(0.7, 0.7, "Decay = %.3f"%decay)
      latex.DrawText(0.7, 0.65, "ctau = %.3f"%ctau)
    else:
      hist.Draw("HIST PLC")
    stamp_text.DrawLatex( xpos+0.2, ypos, cmsLabel)

    OutputFolder("genLLP")
    canv.SaveAs(folder + "genLLP" + "/" + plot + ".png")

  print ("Plots for generator LLP rechit multiplicity and energy profiles now run: ")
  plots = {"gen_rechitNpt2", "gen_rechitNpt4", "gen_rechitNpt6", "gen_energyP"}
  leading = [0,1,2]
  for plot in plots:
    legend = ROOT.TLegend(0.65,0.65,0.8,0.8)
    for i in leading:
      type = "unknown"
      if (i == 0): type = " LLP"
      if (i == 1): type = " daughter 1"
      if (i == 2): type = " daughter 2"
      canv.cd()
      hist = infile.Get(plot + "_" + str(i))
      if (hist.GetEntries() > 0): hist.Scale(1/hist.Integral(0,5))
      if (plot[0:10] == "gen_rechit"): canv.SetLogy()
      else: canv.SetLogy(0)
      legend.AddEntry(hist,plot[4:0] + type)

      # Overlay quantities now
      if (i == 0): hist.Draw("HIST PLC")
      if (i > 0 ): hist.Draw("SAME HIST PLC")
      if (i == 2): 
          legend.Draw()
          stamp_text.DrawLatex(xpos, ypos, cmsLabel)
          canv.SaveAs(folder + "genLLP" + "/Overlay_" + plot + ".png")

  print ("2D histogram for rechit vs. LLP depth; fractional energy vs. HCAL depth")
  plots = {"gen_depth_energyP", "gen_decay_rechitN"}  
  leading = [0,1,2]
  for plot in plots:
    for i in leading:
      legend = ROOT.TLegend(0.65,0.65,0.8,0.8)
      type = "unknown"
      if (i == 0): type = " LLP"
      if (i == 1): type = " daughter 1"
      if (i == 2): type = " daughter 2"
      canv.cd()
      hist = infile.Get(plot + "_" + str(i))
      canv.SetLogy(0)
      canv.SetLogz()
      legend.AddEntry(hist,plot[10:0] + type)
      hist.Draw("COLZ")
      legend.Draw()
      stamp_text.DrawLatex(xpos, ypos, cmsLabel)
      canv.SaveAs(folder + "genLLP" + "/" + plot + "_" + type[1:-2] + type[-1] + ".png")
