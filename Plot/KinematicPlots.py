# To run: python KinematicPlots.py <path to output of ../DisplacedHcalJetAnalyzer/util/DisplacedHcalJetAnalyzer.C>import ROOT

import ROOT
from ROOT import Form
from math import *
import numpy as np
import sys, os, argparse, time, errno
import matplotlib.pyplot as plt

debug = False
data = True
MC = False

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
if (data): cmsLabel = "#scale[1.0]{#bf{CMS}} #scale[0.8]{#it{2022 13.6 TeV Collisions Data}}"; 
if (MC): cmsLabel = "#scale[1.0]{#bf{CMS}} #scale[0.8]{#it{2023 LLP MC}}"; 
print("Starting plotting script")

# Save output plots
folder = "./outPlots/"
try:
  os.makedirs(folder)
except OSError:
  pass 

path = sys.argv[1] # pass the location of the processed ntuple as an argument 

infile = ROOT.TFile.Open( path)

categories = {"NoSel", "JetPt40"};
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
        hist = infile.Get(cat + "__" + obj + str(i) + "_" + quant)
        if (hist.GetEntries() > 0): hist.Scale(1/hist.GetEntries())
        hist.Draw("HIST PLC")
        canv.SaveAs(folder + obj + "/" + obj + str(i) + "_" + quant + "_" + cat + ".png")

        overlayCanv.cd()
        overlayCanv.SetLogy()
        legend.AddEntry(hist,obj + str(i))
        if (quant == "energy"): hist.GetYaxis().SetRangeUser(0.001,0.6)
        if (quant == "eta"): hist.GetYaxis().SetRangeUser(0.001,0.4)
        if (quant == "phi"): hist.GetYaxis().SetRangeUser(0.00001,0.1)
        if (quant == "pt"): hist.GetYaxis().SetRangeUser(0.001,1)
        if (quant == "pt"): hist.GetXaxis().SetRangeUser(0,400)
        hist.Rebin(5)

        # Overlay various leading / sub-leading quantities 
        if (i == 0):
          hist.Draw("HIST PLC")
        if (i > 0):
          hist.Draw("SAME HIST PLC")
        if (i == 2):
          legend.Draw()
          stamp_text.DrawLatex( xpos, ypos, cmsLabel)
          overlayCanv.SaveAs(folder + obj + "/Overlay_" + obj + str(i) + "_" + quant + "_" + cat + ".png")

jet_energy = infile.Get("JetPt40__jet0_energy")
jet_energy.Draw("HIST")
canv.SaveAs(folder + "LeadingJet_energy.png")
