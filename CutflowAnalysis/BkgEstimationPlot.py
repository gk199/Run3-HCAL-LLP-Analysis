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
inputs = ["W+Jets", "LLP MC", "LLP MC passed HLT"] # currently for 350, 80

zero_bin = [21906/21907, 0.9315, 0.0498]
one_bin = [1/21907, 0.067, 0.037]
two_bin = [0, 0.0015, 0.0012]

stack = False
if stack: hs = ROOT.THStack("hs",  "Fraction of events with BDT tagged jets (mH = 350, mS = 80 GeV); Fraction of events with jets with BDT discriminator > 0.9999;Fraction of Events")

JetsPerCategory = {}
legend = ROOT.TLegend(0.68,0.65,0.88,0.8)
canv.cd()

i = 0
for type in inputs:
    hname = type
    JetsPerCategory[hname] = ROOT.TH1F(hname, "Fraction of events with BDT tagged jets (mH = 350, mS = 80 GeV);Fraction of events with jets with BDT discriminator > 0.9999;Fraction of Events", 3, 0, 3) 
    legend.AddEntry(JetsPerCategory[hname], type)

    JetsPerCategory[hname].SetBinContent(1, zero_bin[i]) 
    JetsPerCategory[hname].SetBinContent(2, one_bin[i]) 
    JetsPerCategory[hname].SetBinContent(3, two_bin[i]) 
    JetsPerCategory[hname].SetLineWidth(3)
    JetsPerCategory[hname].GetXaxis().SetBinLabel(1,"0 tagged jets")
    JetsPerCategory[hname].GetXaxis().SetBinLabel(2,"1 tagged jet")
    JetsPerCategory[hname].GetXaxis().SetBinLabel(3,"2+ tagged jets")
    if "LLP" not in type:
        JetsPerCategory[hname].SetFillStyle(3004)
    if not stack:
        if i == 0: JetsPerCategory[hname].Draw("PLC PFC")
        else: JetsPerCategory[hname].Draw("SAME PLC")
    i += 1
    if stack: hs.Add(JetsPerCategory[hname])

ROOT.gPad.SetLogy()
if stack: hs.Draw("HIST PLC nostack")
stamp_text.DrawLatex(xpos, ypos, cmsLabel)
#stamp_text.DrawLatex(xpos, ypos-0.04, cmsLabel2)
legend.Draw()
canv.SaveAs("NumberTagged.png")
canv.SaveAs("NumberTagged.pdf")