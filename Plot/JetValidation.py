import ROOT
import math
from array import array

ROOT.gROOT.SetBatch(True)  # Disable canvas display
ROOT.gStyle.SetOptStat(0)  # remove stats box globally

# -------------------------------------------------
# Utility functions
# -------------------------------------------------
def deltaR(eta1, phi1, eta2, phi2):
    dphi = abs(phi1 - phi2)
    if dphi > math.pi:
        dphi = 2 * math.pi - dphi
    return math.sqrt((eta1 - eta2)**2 + dphi**2)

def CMS_label(latex, name=""):
    # CMS Label
    latex.SetNDC()
    latex.SetTextFont(62)
    latex.SetTextSize(0.05)
    latex.DrawLatex(0.12, 0.92, "CMS")
    latex.SetTextFont(52)
    latex.SetTextSize(0.045)
    latex.DrawLatex(0.22, 0.92, "Simulation")

    # Title
    latex.SetTextFont(42)
    latex.SetTextSize(0.045)
    latex.DrawLatex(0.45, 0.92, name)

# -------------------------------------------------
# Step 1 + 2: LLP–jet dR plot and efficiency
# -------------------------------------------------
def plot_llp_jet_dR(filename, tree_name="NoSel", name=""):
    f = ROOT.TFile.Open(filename)
    tree = f.Get(tree_name)

    h_dR = ROOT.TH1F("h_dR", ";#DeltaR(LLP, closest jet);Entries", 50, 0, 3.0)
    
    total_matched = 0
    matched_dRlt04 = 0

    max_events = 20000
    for i, ev in enumerate(tree):
        if i >= max_events: break
        for illp in [0, 1]:
            decayR = getattr(ev, f"LLP{illp}_DecayR")
            eta_llp = getattr(ev, f"LLP{illp}_Eta")
            phi_llp = getattr(ev, f"LLP{illp}_Phi")

            if not (177 < decayR < 295 and abs(eta_llp) < 1.26):
            # if not (decayR < 295 and abs(eta_llp) < 1.26):
                continue

            # Find the closest jet
            min_dR = 999.
            for ijet in range(0, 3):  # adjust jet multiplicity as needed
                try:
                    eta_jet = getattr(ev, f"jet{ijet}_Eta")
                    phi_jet = getattr(ev, f"jet{ijet}_Phi")
                    pt_jet = getattr(ev, f"jet{ijet}_Pt")
                except AttributeError:
                    continue

                if pt_jet < 40: continue
                if abs(eta_jet) > 1.26: continue

                dR = deltaR(eta_llp, phi_llp, eta_jet, phi_jet)
                if dR < min_dR:
                    min_dR = dR

            if min_dR < 999.:
                h_dR.Fill(min_dR)
                total_matched += 1
                if min_dR < 0.4:
                    matched_dRlt04 += 1

    eff = matched_dRlt04 / total_matched if total_matched > 0 else 0

    print(f"dR < 0.4 efficiency: {eff:.3f} ({matched_dRlt04}/{total_matched})")

    # Canvas and style
    c1 = ROOT.TCanvas("c1", "", 700, 600)
    c1.SetLeftMargin(0.12)
    c1.SetBottomMargin(0.12)
    h_dR.SetLineWidth(2)
    h_dR.SetLineColor(ROOT.kBlue+1)
    h_dR.Draw("HIST")

    # Title and CMS label
    latex = ROOT.TLatex()
    CMS_label(latex, name)

    # Number of entries
    entries = h_dR.GetEntries()
    latex.SetTextFont(42)
    latex.SetTextSize(0.04)
    latex.DrawLatex(0.65, 0.85, f"Entries = {int(entries)}")

    c1.Update()
    c1.SaveAs(f"llp_closestJet_dR_" + name + ".png")
    f.Close()

    return eff

# -------------------------------------------------
# Step 3: Compare tagging efficiencies
# -------------------------------------------------
def compute_tag_eff_rdf(filename, tree_name="NoSel"):
    rdf = ROOT.RDataFrame(tree_name, filename)

    # Preselection
    pre = (
        "(jet0_Pt > 60 && abs(jet0_Eta) < 1.26 && jet0_L1trig_Matched == 1) || "
        "(jet1_Pt > 60 && abs(jet1_Eta) < 1.26 && jet1_L1trig_Matched == 1)"
    )

    # Tag condition: opposite tags for lead/sublead
    tag_cond = "((jet0_InclTagCand && jet1_DepthTagCand) || (jet0_DepthTagCand && jet1_InclTagCand))"

    rdf_sel = rdf.Filter(pre, "Preselection")
    n_all = rdf_sel.Count()
    n_pass = rdf_sel.Filter(tag_cond, "Lead/sublead tag condition").Count()

    eff = float(n_pass.GetValue()) / float(n_all.GetValue()) if n_all.GetValue() > 0 else 0
    print(f"{filename}: efficiency = {eff:.3f} ({n_pass.GetValue()}/{n_all.GetValue()})")
    return eff


def compare_tag_eff(file1, file2):
    eff1 = compute_tag_eff_rdf(file1)
    eff2 = compute_tag_eff_rdf(file2)

# -------------------------------------------------
# LLP matching to jets
# -------------------------------------------------
def plot_llp_match_fraction(filename, name="Sample", tree_name="NoSel"):
    f = ROOT.TFile.Open(filename)
    tree = f.Get(tree_name)

    # Define decay length bins
    bins = [0, 10, 129, 177, 295]
    nbins = len(bins) - 1

    # Histograms for total LLPs and matched ones
    h_total = ROOT.TH1F("h_total",  ";LLP decay radius [cm];Fraction of matched LLPs", nbins, array('d', bins))
    h_match = ROOT.TH1F("h_match",  "", nbins, array('d', bins))
    h_match40 = ROOT.TH1F("h_match40", "", nbins, array('d', bins))
    h_match60 = ROOT.TH1F("h_match60", "", nbins, array('d', bins))
    h_match60_L1 = ROOT.TH1F("h_match60_L1", "", nbins, array('d', bins))

    # Fill histograms
    max_events = 20000
    for i, ev in enumerate(tree):
        if i >= max_events: break
        L1_pass0 = getattr(ev, "jet0_L1trig_Matched")
        L1_pass1 = getattr(ev, "jet1_L1trig_Matched")
        L1 = getattr(ev, "Pass_L1SingleLLPJet")
        HLT = getattr(ev, "Pass_HLTDisplacedJet")
        for illp in [0, 1]:
            try:
                decayR = getattr(ev, f"LLP{illp}_DecayR")
                LLPeta = getattr(ev, f"LLP{illp}_Eta")
                LLPpt = getattr(ev, f"LLP{illp}_Pt")
                match = getattr(ev, f"LLP{illp}_isTruthMatched")
                match40 = getattr(ev, f"LLP{illp}_isTruthMatched_Jet40")
                match60 = getattr(ev, f"LLP{illp}_isTruthMatched_Jet60")

            except AttributeError:
                print("did not find needed branches")
                continue

            # add pre-selections about LLP eta and LLP pT 
            
            if (abs(LLPeta) > 1.26 or LLPpt < 40): continue
            if not L1: continue
            if not HLT: continue
            h_total.Fill(decayR)
            if match:
                h_match.Fill(decayR)
            if match40:
                h_match40.Fill(decayR)
            if match60:
                h_match60.Fill(decayR)
            if match60 and (L1_pass0 or L1_pass1):
                h_match60_L1.Fill(decayR)

    # Compute efficiencies per bin
    h_frac = h_match.Clone("h_frac")
    h_frac.Divide(h_match, h_total, 1, 1, "B")  # binomial errors

    h_frac40 = h_match40.Clone("h_frac40")
    h_frac40.Divide(h_match40, h_total, 1, 1, "B")

    h_frac60 = h_match60.Clone("h_frac60")
    h_frac60.Divide(h_match60, h_total, 1, 1, "B")

    h_frac60_L1 = h_match60_L1.Clone("h_frac60_L1")
    h_frac60_L1.Divide(h_match60_L1, h_total, 1, 1, "B")

    # --- Draw ---
    c1 = ROOT.TCanvas("c1", "", 700, 600)
    c1.SetLeftMargin(0.12)
    c1.SetBottomMargin(0.12)
    ROOT.gPad.SetTicks(1,1)

    h_frac.SetLineColor(ROOT.kAzure+1)
    h_frac.SetLineWidth(2)
    h_frac.GetYaxis().SetRangeUser(0, 1.05)
    h_frac.GetYaxis().SetTitle("Fraction of matched LLPs")
    h_frac.GetXaxis().SetTitle("LLP decay radius [cm]")
    h_frac.Draw("HIST E1")

    h_frac40.SetLineColor(ROOT.kRed+1)
    h_frac40.SetLineWidth(2)
    h_frac40.Draw("HIST E1 SAME")

    h_frac60.SetLineColor(ROOT.kGreen+1)
    h_frac60.SetLineWidth(2)
    h_frac60.Draw("HIST E1 SAME")

    h_frac60_L1.SetLineColor(ROOT.kViolet+1)
    h_frac60_L1.SetLineWidth(2)
    h_frac60_L1.Draw("HIST E1 SAME")

    h_frac.SetDirectory(0) 
    h_frac40.SetDirectory(0) 
    h_frac60.SetDirectory(0)
    h_frac60_L1.SetDirectory(0) # keep in memory for ratio plots

    # Legend
    leg = ROOT.TLegend(0.15, 0.7, 0.55, 0.88)
    leg.SetBorderSize(0)
    leg.SetFillStyle(0)
    leg.AddEntry(h_frac, "LLP matched to a jet", "l")
    leg.AddEntry(h_frac40, "LLP matched to a jet, with jet>40", "l")
    leg.AddEntry(h_frac60, "LLP matched to a jet, with jet>60", "l")
    leg.AddEntry(h_frac60_L1, "LLP matched to a jet, with jet>60, L1 triggered", "l")
    leg.Draw()

    latex = ROOT.TLatex()
    CMS_label(latex, name)

    # Entries
    total_llps = int(h_total.GetEntries())
    latex.SetTextSize(0.04)
    latex.DrawLatex(0.65, 0.85, f"Entries = {total_llps}")

    c1.Update()
    outname = f"llp_match_fraction_{name.replace(' ', '_')}.png"
    c1.SaveAs(outname)
    f.Close()
    return h_frac, h_frac40, h_frac60, h_frac60_L1

def plot_llp_match_fraction_ratio(puppi_hists, chs_hists, labels, name="Puppi/CHS"):
    """
    Draw ratios (Puppi / CHS) for multiple histogram pairs.
    puppi_hists: list of TH1 histograms from Puppi sample
    chs_hists:   list of TH1 histograms from CHS sample (same binning)
    labels:      list of legend labels (same length as puppi_hists)
    """

    if not (len(puppi_hists) == len(chs_hists) == len(labels)):
        raise ValueError("Input lists must have the same length")

    # Style setup
    c = ROOT.TCanvas("c_ratio", "", 700, 600)
    c.SetLeftMargin(0.12)
    c.SetBottomMargin(0.12)
    ROOT.gPad.SetTicks(1, 1)

    colors = [ROOT.kAzure+1, ROOT.kRed+1, ROOT.kGreen+2, ROOT.kViolet+1, ROOT.kOrange+1]
    ratios = []

    # Create ratio histograms
    for i, (h_puppi, h_chs) in enumerate(zip(puppi_hists, chs_hists)):
        h_ratio = h_puppi.Clone(f"h_ratio_{i}")
        h_ratio.Divide(h_puppi, h_chs, 1, 1, "B")
        color = colors[i % len(colors)]
        h_ratio.SetLineColor(color)
        h_ratio.SetLineWidth(2)
        ratios.append(h_ratio)

    # Draw all ratios
    first = True
    for i, h_ratio in enumerate(ratios):
        if first:
            h_ratio.GetYaxis().SetRangeUser(0, 2.0)
            h_ratio.GetYaxis().SetTitle("Puppi / CHS")
            h_ratio.GetXaxis().SetTitle("LLP decay radius [cm]")
            h_ratio.Draw("HIST E1")
            first = False
        else:
            h_ratio.Draw("HIST E1 SAME")

    # Reference line at y = 1
    line = ROOT.TLine(ratios[0].GetXaxis().GetXmin(), 1.0, ratios[0].GetXaxis().GetXmax(), 1.0)
    line.SetLineColor(ROOT.kGray+2)
    line.SetLineStyle(2)
    line.Draw("SAME")

    # Legend
    leg = ROOT.TLegend(0.15, 0.7, 0.55, 0.88)
    leg.SetBorderSize(0)
    leg.SetFillStyle(0)
    for h_ratio, label in zip(ratios, labels):
        leg.AddEntry(h_ratio, label, "l")
    leg.Draw()

    latex = ROOT.TLatex()
    CMS_label(latex, name)

    c.Update()
    outname = f"llp_match_fraction_ratio_{name.replace(' ', '_')}.png"
    c.SaveAs(outname)
    print(f"Saved ratio plot: {outname}")

    return ratios

# -------------------------------------------------
# Main execution
# -------------------------------------------------
if __name__ == "__main__":
    file1 = "/afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis/Run/minituple_PUPPIjets_2023C_LLPskim.root"
    file2 = "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v4.1/minituple_LLPskim_2023Cv2_allscores.root"
    file3 = "/afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis/Run/minituple_PUPPIjets_LLP_MC_eta2.root"
    file4 = "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v4.1/minituple_HToSSTo4B_125_50_CTau3000_scores.root"

    print("Running LLP–jet dR plot and efficiency:")
    eff_dR = plot_llp_jet_dR(file3, "NoSel", "PUPPI")
    eff_dR = plot_llp_jet_dR(file4, "NoSel", "CHS")

    print("\nRunning tagging efficiency comparison for LLP MC:")
    compare_tag_eff(file3, file4)

    print("\nRunning tagging efficiency comparison for LLP skim (data):")
    compare_tag_eff(file1, file2)

    print("\nRunning LLP matching efficiency as a function of decay R:")
    h_frac_puppi, h_frac40_puppi, h_frac60_puppi, h_frac60_L1_puppi = plot_llp_match_fraction(file3, "LLP MC, Puppi jets")    
    h_frac_chs, h_frac40_chs, h_frac60_chs, h_frac60_L1_chs = plot_llp_match_fraction(file4, "LLP MC, CHS jets")
    plot_llp_match_fraction_ratio(puppi_hists = [h_frac_puppi, h_frac40_puppi, h_frac60_puppi, h_frac60_L1_puppi],
                                    chs_hists = [h_frac_chs, h_frac40_chs, h_frac60_chs, h_frac60_L1_chs],
                                    labels = ["LLP matched to jet", "Jet pT > 40", "Jet pT > 60", "Leading jet is L1 matched"],
                                    name = "LLP MC Puppi vs CHS")