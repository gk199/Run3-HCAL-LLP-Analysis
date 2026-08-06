import uproot
import ROOT
import numpy as np
import argparse, os
import json
import awkward as ak

parser = argparse.ArgumentParser(description="W+Jets Selection Data vs MC tagger score comparison.")
parser.add_argument("-m", "--mc", required=True, help="Path to W+Jets MC ROOT file")
parser.add_argument("--inclusive", action="store_true", help="Use inclusive tagger")
parser.add_argument("--depth", action="store_true", help="Use depth tagger")
args = parser.parse_args()

ROOT.gROOT.SetBatch(True)
ROOT.gStyle.SetOptStat(0)

era = "postBPix"
cmsLabel = "#scale[1.0]{#bf{CMS}} #scale[0.8]{#it{Work in Progress}}"

xpos = 0.16
ypos = 0.85

folder = "./outPlots/DataMCTagger/"
os.makedirs(folder, exist_ok=True)
tree_name = "NoSel"

if args.depth:
    tagger_name = "depth"
    score_var = "jet0_scores_depth_LLPanywhere_updated" 
    threshold = 0.
elif args.inclusive:
    tagger_name = "inclusive"
    score_var = "jet0_scores_inc_train80_updated" 
    threshold = 0. 
else:
    raise ValueError("Please specify either --depth or --inclusive")

print(f"Using tagger: {tagger_name}")

if era == "preBPix":
    data_file1 = uproot.open("/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.16/minituples_Zmu_2023Cv1_allscores_v7_cat12.root")
    data_file2 = uproot.open("/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.16/minituples_Zmu_2023Cv2_allscores_v7_cat12.root")
    data_file3 = uproot.open("/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.16/minituples_Zmu_2023Cv3_allscores_v7_cat12.root")
    data_file4 = uproot.open("/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.16/minituples_Zmu_2023Cv4_allscores_v7_cat12.root")
elif era == "postBPix":
    data_file5 = uproot.open("/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.16/minituples_Zmu_2023Dv1_allscores_v7_cat3.root")
    data_file6 = uproot.open("/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.16/minituples_Zmu_2023Dv2_allscores_v7_cat3.root")


mc_file   = uproot.open(args.mc) 

if era == "preBPix":
    data_tree1 = data_file1[tree_name]
    data_tree2 = data_file2[tree_name]
    data_tree3 = data_file3[tree_name]
    data_tree4 = data_file4[tree_name]
elif era == "postBPix":
    data_tree5 = data_file5[tree_name]
    data_tree6 = data_file6[tree_name]

mc_tree   = mc_file[tree_name]

if era == "preBPix": data_trees = [data_tree1, data_tree2, data_tree3, data_tree4]
elif era == "postBPix": data_trees = [data_tree5, data_tree6]

mc_score   = mc_tree[score_var].array(library="np")
mc_pass  = mc_tree["Pass_WPlusJets"].array(library="np")
mc_input_pass_1  = mc_tree["jet0_EleEFrac"].array(library="np")
mc_input_pass_2  = mc_tree["jet0_S_phiphi"].array(library="np")
mc_input_pass_3  = mc_tree["jet0_S_etaphi"].array(library="np")
mc_input_pass_4  = mc_tree["jet0_S_etaeta"].array(library="np")
mc_input_pass_5  = mc_tree["jet0_Pt"].array(library="np")
mc_input_pass_6  = mc_tree["jet0_Eta"].array(library="np")
#mc_input_pass_7  = mc_tree["jet0_ChargedHadEFrac"].array(library="np")

mc_input_inc = mc_tree["jet0_InclTagCand"].array(library="np")
mc_input_depth = mc_tree["jet0_DepthTagCand"].array(library="np")
bins = 10
h_data_comb = ROOT.TH1F("h_data_comb", f"{tagger_name} score;{tagger_name} score;Normalized entries", bins, 0, 1)

h_mc   = ROOT.TH1F("h_mc",   f"{tagger_name} score;{tagger_name} score;Normalized entries", bins, 0, 1)

h_mc.Sumw2()
h_data_comb.Sumw2()

for tree in data_trees:
    scores = tree[score_var].array(library="np")
    passed = tree["Pass_WPlusJets"].array(library="np")
    input_passed_1 = tree["jet0_EleEFrac"].array(library="np")
    input_passed_2 = tree["jet0_S_phiphi"].array(library="np")
    input_passed_3 = tree["jet0_S_etaphi"].array(library="np")
    input_passed_4 = tree["jet0_S_etaeta"].array(library="np")
    input_passed_5 = tree["jet0_Pt"].array(library="np")
    input_passed_6 = tree["jet0_Eta"].array(library="np")
    #input_passed_7 = tree["jet0_ChargedHadEFrac"].array(library="np")

    input_passed_inc = tree["jet0_InclTagCand"].array(library="np")
    input_passed_depth = tree["jet0_DepthTagCand"].array(library="np")

    mask_data = (passed == 1) & (input_passed_5 > 40) & (input_passed_1 < 0.7) & (input_passed_3 < 0.05) & (input_passed_2 < 0.05) 

    for val in scores[mask_data]:
        h_data_comb.Fill(float(val))

mask_mc = (mc_pass == 1) & (mc_input_pass_5 > 40)  & (mc_input_pass_1 < 0.7) #& (mc_input_pass_2 < 0.05)  & (mc_input_pass_3 < 0.05) & (mc_input_pass_4 < 0.05) 

for val in mc_score[mask_mc]:
    h_mc.Fill(float(val))

if h_data_comb.Integral() > 0: h_data_comb.Scale(1.0/h_data_comb.Integral())
if h_mc.Integral() > 0: h_mc.Scale(1.0/h_mc.Integral())

h_data_comb = h_data_comb.GetCumulative(False)
h_mc   = h_mc.GetCumulative(False)

#for h in [h_data_comb, h_mc]:
#    for i in range(0, h.GetNbinsX()+2):   # includes underflow (0) and overflow (N+1)
#        if h.GetBinContent(i) <= 0:
#            h.SetBinContent(i, 1e-10)
#h_data_comb.SetMinimum(1e-10)

h_mc_band = h_mc.Clone("h_mc_band")
h_mc_band.SetDirectory(0)
h_mc_band.SetFillColorAlpha(ROOT.kGray+1, 0.35)
h_mc_band.SetLineColor(ROOT.kGray+1)
h_mc_band.SetMarkerSize(0)

h_ratio_band = h_mc.Clone("h_ratio_band")
h_ratio_band.SetDirectory(0)
h_ratio_band.Reset("ICES")

for ibin in range(1, h_mc.GetNbinsX() + 1):
    mc_val = h_mc.GetBinContent(ibin)
    mc_err = h_mc.GetBinError(ibin)

    h_ratio_band.SetBinContent(ibin, 1.0)
    if mc_val > 0:
        h_ratio_band.SetBinError(ibin, mc_err / mc_val)
    else:
        h_ratio_band.SetBinError(ibin, 0.0)

h_ratio_band.SetFillColorAlpha(ROOT.kGray+1, 0.35)
h_ratio_band.SetLineColor(ROOT.kGray+1)
#h_ratio_band.SetMarkerSize(0)

ymax = max(h_mc.GetMaximum(), h_mc.GetMaximum())
#h_mc.SetMinimum(0.0)
h_mc.SetMaximum(1000 * ymax)

h_ratio_comb = h_data_comb.Clone("h_ratio_comb")
for ibin in range(1, h_ratio_comb.GetNbinsX() + 1):
    mc_val = h_mc.GetBinContent(ibin)
    data_val = h_data_comb.GetBinContent(ibin)
    data_err = h_data_comb.GetBinError(ibin)
    if mc_val > 0:
        h_ratio_comb.SetBinContent(ibin, data_val / mc_val)
        h_ratio_comb.SetBinError(ibin, data_err / mc_val)
    else:
        h_ratio_comb.SetBinContent(ibin, 0.0)
        h_ratio_comb.SetBinError(ibin, 0.0)

#h_ratio1 = h_data1.Clone("h_ratio1")
#h_ratio1.Divide(h_mc)
#
#h_ratio2 = h_data2.Clone("h_ratio2")
#h_ratio2.Divide(h_mc)
#
#h_ratio3 = h_data3.Clone("h_ratio3")
#h_ratio3.Divide(h_mc)
#
#h_ratio4 = h_data4.Clone("h_ratio4")
#h_ratio4.Divide(h_mc)
#
#h_ratio5 = h_data5.Clone("h_ratio5")
#h_ratio5.Divide(h_mc)
#
#h_ratio6 = h_data6.Clone("h_ratio6")
#h_ratio6.Divide(h_mc)

c = ROOT.TCanvas("c", "", 800, 700)
ROOT.gStyle.SetOptStat(0)
pad1 = ROOT.TPad("pad1", "", 0, 0.3, 1, 1.0)
pad1.SetBottomMargin(0)
pad1.Draw()
pad1.SetLogy()
pad1.cd()
colors = [
    ROOT.kBlack,
    ROOT.kBlue,
    ROOT.kCyan,
    ROOT.kGreen+2,
    ROOT.kMagenta,
    ROOT.kOrange+1
]

markers = [20, 21, 22, 23, 33, 34]
#data_hists  = [h_data1, h_data2, h_data3, h_data4, h_data5, h_data6]
#ratio_hists = [h_ratio1, h_ratio2, h_ratio3, h_ratio4, h_ratio5, h_ratio6]
#for i in range(len(data_hists)):
#    data_hists[i].SetLineColor(colors[i])
#    data_hists[i].SetMarkerColor(colors[i])
#    data_hists[i].SetMarkerStyle(markers[i])
#    data_hists[i].SetLineWidth(2)

h_data_comb.SetLineColor(colors[0])
h_data_comb.SetMarkerColor(colors[0])
h_data_comb.SetMarkerStyle(markers[0])
h_data_comb.SetLineWidth(2)

h_mc.SetLineColor(ROOT.kRed)
h_mc.SetLineWidth(2)
h_mc.SetTitle("")

h_mc.Draw("HIST")
h_mc_band.Draw("E2 SAME")   # <- error band
h_mc.Draw("HIST SAME")      # redraw line on top

h_data_comb.Draw("E1 SAME")
#h_data1.Draw("E SAME")
#h_data2.Draw("E SAME")
#h_data3.Draw("E SAME")
#h_data4.Draw("E SAME")
#h_data5.Draw("E SAME")
#h_data6.Draw("E SAME")

cutLabel = f"{tagger_name} score;{tagger_name} score;CDF"#f"{tagger_name} DNN score"

legend = ROOT.TLegend(0.5, 0.55, 0.9, 0.9)
legend.SetTextSize(0.045)

#legend.AddEntry(h_data1, "Run 2023Cv1", "l")
#legend.AddEntry(h_data2, "Run 2023Cv2", "l")
#legend.AddEntry(h_data3, "Run 2023Cv3", "l")
#legend.AddEntry(h_data4, "Run 2023Cv4", "l")
#legend.AddEntry(h_data5, "Run 2023Dv1", "l")
#legend.AddEntry(h_data6, "Run 2023Dv2", "l")

legend.AddEntry(h_mc, "W+Jets MC", "l")
if era == "preBPix": legend.AddEntry(h_data_comb, "Combined Zmu Run 2023C", "lep")
elif era == "postBPix": legend.AddEntry(h_data_comb, "Combined Zmu Run 2023D", "lep")

legend.Draw()

latex = ROOT.TLatex()
latex.SetNDC()
latex.SetTextSize(0.035)

latex.DrawLatex(0.22, 0.77, "Pass W+jets selection");
latex.DrawLatex(0.22, 0.72, "p_{T} (leading jet) > 40 GeV");
#latex.DrawLatex(0.22, 0.67, "Not Depth Candidate");
#latex.DrawLatex(0.22, 0.67, "|#eta (leading jet)| < 1.26");
latex.DrawLatex(0.22, 0.67, "EleEFrac (leading jet) < 0.7");
#latex.DrawLatex(0.22, 0.57, "S_{#phi#phi} < 0.05");
#latex.DrawLatex(0.22, 0.52, "S_{#eta#phi} < 0.05");
#latex.DrawLatex(0.22, 0.47, "S_{#eta#eta} < 0.05");

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

#for i in range(len(ratio_hists)):
#    ratio_hists[i].SetLineColor(colors[i])
#    ratio_hists[i].SetMarkerColor(colors[i])
#    ratio_hists[i].SetMarkerStyle(markers[i])
#    ratio_hists[i].SetLineWidth(2)

h_ratio_comb.SetTitle("")

h_ratio_comb.GetYaxis().SetTitle("Data / MC")
h_ratio_comb.GetYaxis().SetTitleSize(0.11)
h_ratio_comb.GetYaxis().SetLabelSize(0.095)
h_ratio_comb.GetYaxis().SetTitleOffset(0.42)
h_ratio_comb.GetYaxis().CenterTitle(True)
h_ratio_comb.GetYaxis().SetNdivisions(505)

h_ratio_comb.GetXaxis().SetTitle(f"Leading jet {tagger_name} score")
h_ratio_comb.GetXaxis().SetTitleSize(0.12)
h_ratio_comb.GetXaxis().SetLabelSize(0.10)
h_ratio_comb.GetXaxis().SetTitleOffset(1.0)
h_ratio_comb.GetXaxis().SetNdivisions(506)

h_ratio_comb.GetXaxis().SetTickLength(0.08)
h_ratio_comb.GetYaxis().SetTickLength(0.04)

h_ratio_comb.SetMinimum(0.01)
h_ratio_comb.SetMaximum(3.2)#(2.99)


#h_ratio1.SetTitle("")
#
#h_ratio1.GetYaxis().SetTitle("Data / MC")
#h_ratio1.GetYaxis().SetTitleSize(0.11)
#h_ratio1.GetYaxis().SetLabelSize(0.095)
#h_ratio1.GetYaxis().SetTitleOffset(0.42)
#h_ratio1.GetYaxis().CenterTitle(True)
#h_ratio1.GetYaxis().SetNdivisions(505)
#
#h_ratio1.GetXaxis().SetTitle(f"{tagger_name} score")
#h_ratio1.GetXaxis().SetTitleSize(0.12)
#h_ratio1.GetXaxis().SetLabelSize(0.10)
#h_ratio1.GetXaxis().SetTitleOffset(1.0)
#h_ratio1.GetXaxis().SetNdivisions(506)
#
#h_ratio1.GetXaxis().SetTickLength(0.08)
#h_ratio1.GetYaxis().SetTickLength(0.04)
#
#h_ratio1.SetMinimum(0.01)
#h_ratio1.SetMaximum(1.99)

h_ratio_comb.Draw("E")
h_ratio_band.Draw("E2 SAME")     # <- ratio uncertainty band around 1

#h_ratio1.Draw("E SAME")
#h_ratio2.Draw("E SAME")
#h_ratio3.Draw("E SAME")
#h_ratio4.Draw("E SAME")
#h_ratio5.Draw("E SAME")
#h_ratio6.Draw("E SAME")

line = ROOT.TLine(0, 1.0, 1, 1.0)
line.SetLineColor(ROOT.kGray+2)
line.SetLineStyle(2)
line.SetLineWidth(2)
line.Draw("SAME")

pad2.Update()

outname = f"{folder}/{tagger_name}_Score_DataMC_"+era+"_CDF_cut.png"
c.SaveAs(outname)
print(f"Saved score plot: {outname}")

sf = []
sf_err = []

for i in range(1, bins+1):
    d = h_data_comb.GetBinContent(i)
    m = h_mc.GetBinContent(i)
    de = h_data_comb.GetBinError(i)
    me = h_mc.GetBinError(i)

    if m > 0:
        sf_i = d/m
        sf_e = sf_i * np.sqrt((de/d)**2 + (me/m)**2) if d>0 else 0
    else:
        sf_i = 0
        sf_e = 0

    sf.append(sf_i)
    sf_err.append(sf_e)

json_file = f"{folder}/{tagger_name}_Score_SF_{era}_cut.json"
with open(json_file, "w") as f:
    json.dump({"sf": sf, "sf_err": sf_err}, f, indent=2)

print(f"Saved SFs: {json_file}")

