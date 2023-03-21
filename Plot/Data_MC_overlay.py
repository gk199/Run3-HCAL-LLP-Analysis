# To run: python Data_MC_overlay.py <path to data output of ../DisplacedHcalJetAnalyzer/util/DisplacedHcalJetAnalyzer.C> <path to MC output> 

import ROOT
from ROOT import Form
from math import *
import numpy as np
import sys, os, argparse, time, errno
import matplotlib.pyplot as plt

debug = False

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
cmsLabel = "#scale[1.0]{#bf{CMS}} #scale[0.8]{#it{Run 3 MC vs. Data}}"; 
print("Starting plotting script")

# Save output plots
folder = "./outPlots/DataMC/"
try:
  os.makedirs(folder)
except OSError:
  pass 

data = sys.argv[1] # pass the location of the processed ntuple as an argument 
MC = sys.argv[2] 

infile_data = ROOT.TFile.Open( data )
infile_MC = ROOT.TFile.Open( MC )

categories = {"NoSel", "JetPt40", "PassedHLT"};
objects = {"jet", "ele", "muon", "pho"}
quantities = {"energy", "eta", "phi", "pt"}
leading = [0,1,2]

print("Making plots!")
for obj in objects:
  try:
    os.makedirs(folder + obj)
  except OSError:
    pass
  for quant in quantities:
    for cat in categories:
      legend = ROOT.TLegend(xpos+0.2,0.65,xpos+0.35,0.8)
      for i in leading:
        canv.cd()
        canv.SetLogy()

        hist_data = infile_data.Get(cat + "__" + obj + str(i) + "_" + quant)
        hist_MC = infile_MC.Get(cat + "__" + obj + str(i) + "_" + quant)
        if (hist_data.GetEntries() > 0): hist_data.Scale(1/hist_data.GetEntries())
        if (hist_MC.GetEntries() > 0): hist_MC.Scale(1/hist_MC.GetEntries())
        hist_data.Draw("HIST PLC")
        hist_MC.Draw("SAME HIST PLC")
        legend.AddEntry(hist_data,obj + str(i) + ", data")
        legend.AddEntry(hist_MC,obj + str(i) + ", MC")

        if (quant == "energy"): hist_data.GetYaxis().SetRangeUser(0.001,0.6)
        if (quant == "eta"): hist_data.GetYaxis().SetRangeUser(0.001,0.4)
        if (quant == "phi"): hist_data.GetYaxis().SetRangeUser(0.00001,0.1)
        if (quant == "pt"): hist_data.GetYaxis().SetRangeUser(0.001,1)
        if (quant == "pt"): hist_data.GetXaxis().SetRangeUser(0,400)
        hist_data.Rebin(5)
        hist_MC.Rebin(5)

        legend.Draw()
        stamp_text.DrawLatex( xpos+0.15, ypos, cmsLabel)
        canv.SaveAs(folder + obj + "/" + obj + str(i) + "_" + quant + "_" + cat + ".png")