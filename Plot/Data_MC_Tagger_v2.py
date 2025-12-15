import uproot
import ROOT
import numpy as np
import argparse, os
import json

parser = argparse.ArgumentParser(description="W+Jets Selection Data vs MC tagger score comparison.")
parser.add_argument("-d", "--data", required=True, help="Path to Zmumu data ROOT file")
parser.add_argument("-m", "--mc", required=True, help="Path to W+Jets MC ROOT file")
parser.add_argument("--inclusive", action="store_true", help="Use inclusive tagger")
parser.add_argument("--depth", action="store_true", help="Use depth tagger")
args = parser.parse_args()

ROOT.gROOT.SetBatch(True)
ROOT.gStyle.SetOptStat(0)

cmsLabel = "#scale[1.0]{#bf{CMS}} #scale[0.8]{#it{Work in Progress}}"

xpos = 0.16
ypos = 0.85

folder = "./outPlots/DataMCTagger/"
os.makedirs(folder, exist_ok=True)
tree_name = "NoSel"

if args.depth:
    tagger_name = "Depth"
    score_var = "jet0_scores_depth_LLPanywhere"
    score_var_holder = "jet0_scores_depth_anywhere" 
    threshold = 0.
elif args.inclusive:
    tagger_name = "Inclusive"
    score_var = "jet0_scores_inc_train80" 
    threshold = 0. 
else:
    raise ValueError("Please specify either --depth or --inclusive")

print(f"Using tagger: {tagger_name}")

data_file = uproot.open(args.data)
mc_file   = uproot.open(args.mc)

data_tree = data_file[tree_name]
mc_tree   = mc_file[tree_name]

if tagger_name == "Depth": data_score  = data_tree[score_var_holder].array(library="np")
else: data_score  = data_tree[score_var].array(library="np")
mc_score   = mc_tree[score_var].array(library="np")

data_pass = data_tree["Pass_WPlusJets"].array(library="np")
mc_pass   = mc_tree["Pass_WPlusJets"].array(library="np")

bins = 40
h_data = ROOT.TH1F("h_data", f"{tagger_name} score;{tagger_name} score;Normalized entries", bins, 0, 1)
h_mc   = ROOT.TH1F("h_mc",   f"{tagger_name} score;{tagger_name} score;Normalized entries", bins, 0, 1)
h_data.Sumw2()
h_mc.Sumw2()

for val in data_score[(data_pass == 1)]:h_data.Fill(float(val))
for val in mc_score[(mc_pass == 1)]:h_mc.Fill(float(val))

h_data_raw = h_data.Clone("h_data_raw")
h_mc_raw   = h_mc.Clone("h_mc_raw")

if h_data.Integral() > 0: h_data.Scale(1.0/h_data.Integral())
if h_mc.Integral() > 0: h_mc.Scale(1.0/h_mc.Integral())

ymax = max(h_data.GetMaximum(), h_mc.GetMaximum())
h_data.SetMaximum(1.3 * ymax)

c = ROOT.TCanvas("c", "", 800, 700)
ROOT.gStyle.SetOptStat(0)
pad1 = ROOT.TPad("pad1", "", 0, 0.3, 1, 1.0)
pad1.SetBottomMargin(0)
pad1.Draw()
pad1.SetLogy()
pad1.cd()

h_data.SetLineColor(ROOT.kBlack)
h_mc.SetLineColor(ROOT.kRed)
h_mc.SetLineStyle(2)
h_data.SetTitle("")
h_data.Draw("HIST")
h_mc.Draw("HIST SAME")

cutLabel = f"{tagger_name} DNN score"

legend = ROOT.TLegend(0.6, 0.7, 0.85, 0.85)
legend.AddEntry(h_data, "Data", "l")
legend.AddEntry(h_mc, "MC", "l")
legend.Draw()

stamp = ROOT.TLatex()
stamp.SetNDC()
stamp.SetTextFont(42)
stamp.SetTextSize(0.045)
stamp.DrawLatex(xpos, ypos, cmsLabel)

c.cd()
pad2 = ROOT.TPad("pad2", "", 0, 0.05, 1, 0.3)
pad2.SetTopMargin(0)
pad2.SetBottomMargin(0.3)
pad2.Draw()
pad2.cd()

h_ratio = h_data.Clone("h_ratio")
h_ratio.Divide(h_mc)

h_ratio.SetTitle("")
h_ratio.GetYaxis().SetTitle("Data/Bkg")
h_ratio.GetYaxis().SetTitleSize(0.09)
h_ratio.GetYaxis().SetLabelSize(0.08)
h_ratio.GetYaxis().SetTitleOffset(0.5)
h_ratio.GetXaxis().SetTitle(f"{tagger_name} tagger score")
h_ratio.GetXaxis().SetTitleSize(0.1)
h_ratio.GetXaxis().SetLabelSize(0.09)
h_ratio.GetYaxis().SetNdivisions(5)#404
h_ratio.GetXaxis().SetNdivisions(506)
h_ratio.SetMinimum(0.01)
h_ratio.SetMaximum(1.99)
h_ratio.Draw("E")
line = ROOT.TLine(0, 1.0, 1, 1.0)
line.SetLineColor(ROOT.kGray+2)
line.SetLineStyle(2)
line.SetLineWidth(2)
line.Draw("SAME")

pad2.Update()

outname = f"{folder}/{tagger_name}_Score_DataMC.png"
c.SaveAs(outname)
print(f"Saved score plot: {outname}")

sf = []
sf_err = []

for i in range(1, bins+1):
    d = h_data.GetBinContent(i)
    m = h_mc.GetBinContent(i)
    de = h_data.GetBinError(i)
    me = h_mc.GetBinError(i)

    if m > 0:
        sf_i = d/m
        sf_e = sf_i * np.sqrt((de/d)**2 + (me/m)**2) if d>0 else 0
    else:
        sf_i = 0
        sf_e = 0

    sf.append(sf_i)
    sf_err.append(sf_e)

json_file = f"{folder}/{tagger_name}_Score_SF.json"
with open(json_file, "w") as f:
    json.dump({"sf": sf, "sf_err": sf_err}, f, indent=2)

print(f"Saved SFs: {json_file}")

