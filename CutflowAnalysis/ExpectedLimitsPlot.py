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
xpos = 0.7
ypos = 0.85

canv = ROOT.TCanvas("c","c",800,600)

cmsLabel = "#scale[1.0]{#bf{CMS} #it{Internal}}"
cmsLabel2 = "#scale[0.8]{#sqrt{s} = 13.6 TeV}"

# list inputs and number of events in each bin
inputs = ["LLP 125, 15", "LLP 125, 50", "LLP 250, 120", "LLP 350, 50", "LLP 350, 160"]

ctau_pt5 = [-1, -1, -1, 0.094, -1]
ctau_1 = [13.5, -1, -1, -1, -1]
ctau_3 = [-1, 0.71, -1, -1, -1]
ctau_10 = [-1, -1, 0.56, -1, 0.26]

stack = False
if stack: hs = ROOT.THStack("hs",  "Expected Limits; LLP c#tau;Branching Ratio")

JetsPerCategory = {}
legend = ROOT.TLegend(0.68,0.65,0.88,0.8)
canv.cd()

i = 0
for type in inputs:
    hname = type
    JetsPerCategory[hname] = ROOT.TH1F(hname, "Expected Limits;LLP c#tau;Branching Ratio", 4, 0, 4) 
    legend.AddEntry(JetsPerCategory[hname], type)

    JetsPerCategory[hname].SetBinContent(1, ctau_pt5[i]) 
    JetsPerCategory[hname].SetBinContent(2, ctau_1[i]) 
    JetsPerCategory[hname].SetBinContent(3, ctau_3[i]) 
    JetsPerCategory[hname].SetBinContent(4, ctau_10[i]) 
    JetsPerCategory[hname].SetMarkerSize(1.5)
    JetsPerCategory[hname].SetMarkerStyle(21)
    JetsPerCategory[hname].GetXaxis().SetBinLabel(1,"c#tau = 0.5 m")
    JetsPerCategory[hname].GetXaxis().SetBinLabel(2,"c#tau = 1 m")
    JetsPerCategory[hname].GetXaxis().SetBinLabel(3,"c#tau = 3 m")
    JetsPerCategory[hname].GetXaxis().SetBinLabel(4,"c#tau = 10 m")
    if "LLP" not in type:
        JetsPerCategory[hname].SetFillStyle(3004)
    if not stack:
        if i == 0: JetsPerCategory[hname].Draw("P PMC")
        else: JetsPerCategory[hname].Draw("SAME P PMC")
    i += 1
    if stack: hs.Add(JetsPerCategory[hname])

ROOT.gPad.SetLogy()
if stack: hs.Draw("HIST PLC nostack")
stamp_text.DrawLatex(xpos, ypos, cmsLabel)
legend.Draw()
canv.SaveAs("ExpectedLimits.png")