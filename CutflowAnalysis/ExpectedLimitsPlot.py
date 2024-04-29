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
inputs = ["mH=125, mS=15", "mH=125, mS=50", "mH=250, mS=120", "mH=350, mS=80", "mH=350, mS=160"]

overall_acceptance = [2/292437, 382/2943719, 487/2937728, 293/299110, 1043/2946400]

ctau_pt5 = [-1, -1, -1, 293/299110, -1]
ctau_1 = [2/292437, -1, -1, -1, -1]
ctau_3 = [-1, 382/2943719, -1, -1, -1]
ctau_10 = [-1, -1, 487/2937728, -1, 1043/2946400]

n_pass = [2, 382, 487, 293, 1043]
n_total = [292437, 2943719, 2937728, 299110, 2946400]

# Full calculation results below
#ctau_pt5 = [-1, -1, -1, 0.00094, -1]
#ctau_1 = [0.135, -1, -1, -1, -1]
#ctau_3 = [-1, 0.0071, -1, -1, -1]
#ctau_10 = [-1, -1, 0.0056, -1, 0.0026]

stack = False
if stack: hs = ROOT.THStack("hs",  "Expected Limits based on BDT double jet tag; LLP c#tau;Branching Ratio")

JetsPerCategory = {}
legend = ROOT.TLegend(0.68,0.65,0.88,0.8)
canv.cd()

i = 0
for type in inputs:
    hname = type
    bin_num = 4 # 20 # 4
    JetsPerCategory[hname] = ROOT.TH1F(hname, "Expected Limits based on BDT double jet tag;LLP c#tau (m);Branching Ratio", bin_num, 0, bin_num) 
    legend.AddEntry(JetsPerCategory[hname], type)

    if bin_num == 4:
        bin1 = 1
        bin2 = 2
        bin3 = 3
        bin4 = 4
    if bin_num == 20:
        bin1 = 2
        bin2 = 3
        bin3 = 7
        bin4 = 19

    JetsPerCategory[hname].SetBinContent(bin1, 3 / (67 * ctau_pt5[i] * 48520)) 
    if (ctau_pt5[i] > 0): JetsPerCategory[hname].SetBinError(bin1, 3 / (67 * ctau_pt5[i] * 48520) * sqrt(ctau_pt5[i] * (1-ctau_pt5[i])/n_total[i]) * 1/ctau_pt5[i] ) # binomial errors 
    JetsPerCategory[hname].SetBinContent(bin2, 3 / (67 * ctau_1[i] * 48520)) 
    if (ctau_1[i] > 0): JetsPerCategory[hname].SetBinError(bin2, 3 / (67 * ctau_1[i] * 48520) * sqrt(ctau_1[i] * (1-ctau_1[i])/n_total[i]) * 1/ctau_1[i] ) 
    JetsPerCategory[hname].SetBinContent(bin3, 3 / (67 * ctau_3[i] * 48520)) 
    if (ctau_3[i] > 0): JetsPerCategory[hname].SetBinError(bin3, 3 / (67 * ctau_3[i] * 48520) * sqrt(ctau_3[i] * (1-ctau_3[i])/n_total[i]) * 1/ctau_3[i] ) 
    JetsPerCategory[hname].SetBinContent(bin4, 3 / (67 * ctau_10[i] * 48520)) 
    if (ctau_10[i] > 0): JetsPerCategory[hname].SetBinError(bin4, 3 / (67 * ctau_10[i] * 48520) * sqrt(ctau_10[i] * (1-ctau_10[i])/n_total[i]) * 1/ctau_10[i] ) 
        # print(3 / (67 * ctau_10[i] * 48520) * sqrt(ctau_10[i] * (1-ctau_10[i])/n_total[i]) * 1/ctau_10[i]) # binomial
        # print(3 / (67 * ctau_10[i] * 48520) * sqrt(1/n_total[i]) * 1/ctau_10[i]) # poisson
    JetsPerCategory[hname].SetMarkerSize(1.5)
    JetsPerCategory[hname].SetMarkerStyle(21)
    JetsPerCategory[hname].GetYaxis().SetRangeUser(0.00005, 1)
    if bin_num == 4:
        JetsPerCategory[hname].GetXaxis().SetBinLabel(bin1,"c#tau = 0.5 m")
        JetsPerCategory[hname].GetXaxis().SetBinLabel(bin2,"c#tau = 1 m")
        JetsPerCategory[hname].GetXaxis().SetBinLabel(bin3,"c#tau = 3 m")
        JetsPerCategory[hname].GetXaxis().SetBinLabel(bin4,"c#tau = 10 m")
    if "LLP" not in type:
        JetsPerCategory[hname].SetFillStyle(3004)
    if not stack:
        if i == 0: JetsPerCategory[hname].Draw("P PMC PLC E1")
        else: JetsPerCategory[hname].Draw("SAME P PMC PLC E1")
    i += 1
    if stack: hs.Add(JetsPerCategory[hname])

ROOT.gPad.SetLogy()
if stack: hs.Draw("HIST PLC nostack")
stamp_text.DrawLatex(xpos, ypos, cmsLabel)
legend.Draw()
if bin_num == 4: 
    canv.SaveAs("ExpectedLimits_labeled.png")
    canv.SaveAs("ExpectedLimits_labeled.pdf")
else: 
    canv.SaveAs("ExpectedLimits.png")
    canv.SaveAs("ExpectedLimits.pdf")