import ROOT

ROOT.gROOT.SetBatch(True) # no GUI pop up windows
ROOT.gStyle.SetOptStat(0) # no stat box

# Open the ROOT file
file = ROOT.TFile.Open("/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.13/minituple_v3.13_LLPskim_Run2023Bv1.root")
tree = file.Get("NoSel")

# -------------------------------------------------------------------------------------------------
print("\nRechit multiplicity vs jet energy (leading jet only)")
# Create a canvas
c = ROOT.TCanvas("c", "Rechits E > 4 vs Jet0 Pt", 800, 600)

# Draw the 2D histogram using TTree::Draw
# Sum$ counts how many elements in the array satisfy the condition
tree.Draw("Sum$(jet0_rechit_E > 4):jet0_Pt >> jet0_mult(50, 0, 500, 40, 0, 40)", "", "COLZ")

# Access the histogram and customize labels
jet0_mult = ROOT.gDirectory.Get("jet0_mult")
jet0_mult.GetXaxis().SetTitle("jet0_Pt [GeV]")
jet0_mult.GetYaxis().SetTitle("Number of rechits with E > 4")
jet0_mult.SetTitle("HCAL rechits with ET > 4 vs. jet0 Pt")

# Draw and save
c.SetLogz(True)
c.Draw()
c.SaveAs("rechits_vs_pt_jet0.png")

# -------------------------------------------------------------------------------------------------
print("\nRechit multiplicity vs jet energy (all jets)")
# now do it for all jets in the event
# Create the output histogram
hist2D = ROOT.TH2F("rechits_vs_jetPt", "HCAL rechits with ET > 4 GeV vs. Jet Pt (all LLP flagged jets)",
                   50, 0, 500,  # X: Pt
                   40, 0, 40)  # Y: rechit count

# Loop over all 4 jets and fill the same histogram
for i in range(4):
    pt_var = f"jet{i}_Pt"
    rechit_var = f"jet{i}_rechit_E"
    # Sum$ counts how many rechits in jet i have E > 4
    expr = f"Sum$({rechit_var} > 4):{pt_var} >>+ rechits_vs_jetPt"
    tree.Draw(expr, "", "goff")  # ">>+" appends to the same hist

# Draw it
c1 = ROOT.TCanvas()
c1.SetLogz(True)
hist2D.GetXaxis().SetTitle("Jet Pt [GeV]")
hist2D.GetYaxis().SetTitle("Number of rechits with ET > 4")
hist2D.SetTitle("HCAL rechits with ET > 4 GeV vs. Jet Pt (all LLP flagged jets)")
hist2D.Draw("COLZ")

# Save the output
c1.SaveAs("rechits_vs_pt.png")

# -------------------------------------------------------------------------------------------------
print("\nRechit multiplicity vs event HT")
# Do this vs event HT
# Use tree.Draw to plot total number of rechits with E > 4 vs eventHT
# Sum rechits for all jets (jet0, jet1, jet2, jet3) with energy > 4
tree.Draw(
    "Sum$(jet0_rechit_E > 4) + Sum$(jet1_rechit_E > 4) + Sum$(jet2_rechit_E > 4) + Sum$(jet3_rechit_E > 4):eventHT >> eventHT_mult(50, 0, 2000, 60, 0, 60)", 
    "",  # no additional cuts
    "COLZ")

c2 = ROOT.TCanvas()
c2.SetLogz(True)
eventHT_mult = ROOT.gDirectory.Get("eventHT_mult")
eventHT_mult.GetXaxis().SetTitle("Event HT [GeV]")
eventHT_mult.GetYaxis().SetTitle("Number of rechits with ET > 4")
eventHT_mult.SetTitle("HCAL rechits with ET > 4 GeV vs Event HT")
eventHT_mult.Draw("COLZ")
c2.SaveAs("rechits_vs_HT.png")

# First approach -- very slow
# # Create a 2D histogram: X = jet0_Pt, Y = number of rechits > 4
# hist2D = ROOT.TH2F("rechit_count_vs_pt", "Rechits ET>4 vs Jet0 Pt",
#                    50, 0, 500,   # 50 bins for jet0_Pt from 0 to 500 (adjust as needed)
#                    50, 0, 100)   # 50 bins for rechit count from 0 to 100

# # Loop over entries
# for event in tree:
#     # Skip events with no rechits
#     if event.njet0_rechit_E == 0:
#         continue

#     count_above_4 = 0
#     for i in range(event.njet0_rechit_E):
#         if event.jet0_rechit_E[i] > 4.0:
#             count_above_4 += 1

#     hist2D.Fill(event.jet0_Pt, count_above_4)

# # Draw the result
# c = ROOT.TCanvas()
# hist2D.GetXaxis().SetTitle("jet0_Pt [GeV]")
# hist2D.GetYaxis().SetTitle("Number of rechits with E > 4")
# hist2D.Draw("COLZ")
# c.SaveAs("rechit_count_vs_pt.png")