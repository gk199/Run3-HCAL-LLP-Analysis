jet_energy = "40"

jet_string = "(jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26)"
jet1_string = "(jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26)"
jet2_string = "(jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26)"
jet3_string = "(jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26)"
jet4_string = "(jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26)"
jet5_string = "(jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26)"

jet_string_six = jet_string + "||" + jet1_string + "||" + jet2_string + "||" + jet3_string + "||" + jet4_string + "||" + jet5_string

# Emulated flags 
jet_string_noFlag = "(jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && (jet0_DepthTowers + jet0_TimingTowers == 0))"

jet_string_six_noFlag = "((jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && (jet0_DepthTowers + jet0_TimingTowers == 0)) || (jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26 && (jet1_DepthTowers + jet1_TimingTowers == 0)) || (jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26 && (jet2_DepthTowers + jet2_TimingTowers == 0))"
jet_string_six_noFlag += "|| (jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26 && (jet3_DepthTowers + jet3_TimingTowers == 0)) || (jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26 && (jet4_DepthTowers + jet4_TimingTowers == 0)) || (jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26 && (jet5_DepthTowers + jet5_TimingTowers == 0)))"

jet0_depth0 = " ( jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_DepthTowers == 0)"
jet0_depth1 = " ( jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_DepthTowers == 1)"
jet0_depth = " ( jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_DepthTowers >= 2)"
jet1_depth = " ( jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26 && jet1_DepthTowers >= 2)"
jet2_depth = " ( jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26 && jet2_DepthTowers >= 2)"
jet3_depth = " ( jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26 && jet3_DepthTowers >= 2)"
jet4_depth = " ( jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26 && jet4_DepthTowers >= 2)"
jet5_depth = " ( jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26 && jet5_DepthTowers >= 2)"

jet0_timing0 = " ( jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_TimingTowers == 0)"
jet0_timing1 = " ( jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_TimingTowers == 1)"
jet0_timing = " ( jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_TimingTowers >= 2)"
jet1_timing = " ( jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26 && jet1_TimingTowers >= 2)"
jet2_timing = " ( jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26 && jet2_TimingTowers >= 2)"
jet3_timing = " ( jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26 && jet3_TimingTowers >= 2)"
jet4_timing = " ( jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26 && jet4_TimingTowers >= 2)"
jet5_timing = " ( jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26 && jet5_TimingTowers >= 2)"

jet0_depth_timing = " ( jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_DepthTowers == 1 && jet0_TimingTowers == 1)"
jet1_depth_timing = " ( jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26 && jet1_DepthTowers == 1 && jet1_TimingTowers == 1)"
jet2_depth_timing = " ( jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26 && jet2_DepthTowers == 1 && jet2_TimingTowers == 1)"
jet3_depth_timing = " ( jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26 && jet3_DepthTowers == 1 && jet3_TimingTowers == 1)"
jet4_depth_timing = " ( jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26 && jet4_DepthTowers == 1 && jet4_TimingTowers == 1)"
jet5_depth_timing = " ( jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26 && jet5_DepthTowers == 1 && jet5_TimingTowers == 1)"

# defining single and double tag strings, no trigger requirements
one_jet_tagged_string = jet0_depth
one_jet_tagged_string_six = " ( " + jet0_depth + " || " + jet1_depth + " || " + jet2_depth + " || " + jet3_depth + " || " + jet4_depth + " || " + jet5_depth + ")"

two_jet_tagged_string = jet0_timing
two_jet_tagged_string_six = " ( " + jet0_timing + " || " + jet1_timing + " || " + jet2_timing + " || " + jet3_timing + " || " + jet4_timing + " || " + jet5_timing + ")"

three_jet_tagged_string = jet0_depth_timing 
three_jet_tagged_string_six = " ( " + jet0_depth_timing + " || " + jet1_depth_timing + " || " + jet2_depth_timing + " || " + jet3_depth_timing + " || " + jet4_depth_timing + " || " + jet5_depth_timing + ")"

# defining single and double tag strings, require jet is matched to L1 LLP triggered jet
jet_string_triggered = "  ( jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_L1trig_Matched == 1) "
jet_string_six_triggered = "  ( ( jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_L1trig_Matched == 1) || (jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26 && jet1_L1trig_Matched == 1)"
jet_string_six_triggered += " || (jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26 && jet2_L1trig_Matched == 1) || (jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26 && jet3_L1trig_Matched == 1)"
jet_string_six_triggered += " || (jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26 && jet4_L1trig_Matched == 1) || (jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26 && jet5_L1trig_Matched == 1))"

jet_string_triggered_noFlag = "(jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_L1trig_Matched == 1 && (jet0_DepthTowers + jet0_TimingTowers == 0)) "
jet_string_six_triggered_noFlag = "((jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_L1trig_Matched == 1 && (jet0_DepthTowers + jet0_TimingTowers == 0)) || (jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26 && jet1_L1trig_Matched == 1 && (jet1_DepthTowers + jet1_TimingTowers == 0)) || (jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26 && jet2_L1trig_Matched == 1 && (jet2_DepthTowers + jet2_TimingTowers == 0))"
jet_string_six_triggered_noFlag += "|| (jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26 && jet3_L1trig_Matched == 1 && (jet3_DepthTowers + jet3_TimingTowers == 0)) || (jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26 && jet4_L1trig_Matched == 1 && (jet4_DepthTowers + jet4_TimingTowers == 0)) || (jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26 && jet5_L1trig_Matched == 1 && (jet5_DepthTowers + jet5_TimingTowers == 0)))"

one_jet_tagged_string_triggered = " (  " + jet0_depth + " && jet0_L1trig_Matched == 1 )" # jet 0 condition
one_jet1_tagged_string_triggered = " ( " + jet1_depth + " && jet1_L1trig_Matched == 1 )"
one_jet2_tagged_string_triggered = " ( " + jet2_depth + " && jet2_L1trig_Matched == 1 )"
one_jet3_tagged_string_triggered = " ( " + jet3_depth + " && jet3_L1trig_Matched == 1 )"
one_jet4_tagged_string_triggered = " ( " + jet4_depth + " && jet4_L1trig_Matched == 1 )"
one_jet5_tagged_string_triggered = " ( " + jet5_depth + " && jet5_L1trig_Matched == 1 )"
one_jet_tagged_string_six_triggered = "(" + one_jet_tagged_string_triggered + " || " + one_jet1_tagged_string_triggered + " || " + one_jet2_tagged_string_triggered + " || " + one_jet3_tagged_string_triggered + " || " + one_jet4_tagged_string_triggered + " || " + one_jet5_tagged_string_triggered + ")"

two_jet_tagged_string_triggered = " (  " + jet0_timing + " && jet0_L1trig_Matched == 1 )" # jet 0 condition
two_jet1_tagged_string_triggered = " ( " + jet1_timing + " && jet1_L1trig_Matched == 1 )"
two_jet2_tagged_string_triggered = " ( " + jet2_timing + " && jet2_L1trig_Matched == 1 )"
two_jet3_tagged_string_triggered = " ( " + jet3_timing + " && jet3_L1trig_Matched == 1 )"
two_jet4_tagged_string_triggered = " ( " + jet4_timing + " && jet4_L1trig_Matched == 1 )"
two_jet5_tagged_string_triggered = " ( " + jet5_timing + " && jet5_L1trig_Matched == 1 )"
two_jet_tagged_string_six_triggered = "(" + two_jet_tagged_string_triggered + " || " + two_jet1_tagged_string_triggered + " || " + two_jet2_tagged_string_triggered + " || " + two_jet3_tagged_string_triggered + " || " + two_jet4_tagged_string_triggered + " || " + two_jet5_tagged_string_triggered + ")"

three_jet_tagged_string_triggered = " (  " + jet0_depth_timing + " && jet0_L1trig_Matched == 1 )" # jet 0 condition
three_jet1_tagged_string_triggered = " ( jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26 && jet1_DepthTowers == 1 && jet1_TimingTowers == 1 && jet1_L1trig_Matched == 1 )"
three_jet2_tagged_string_triggered = " ( jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26 && jet2_DepthTowers == 1 && jet2_TimingTowers == 1 && jet2_L1trig_Matched == 1 )"
three_jet3_tagged_string_triggered = " ( jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26 && jet3_DepthTowers == 1 && jet3_TimingTowers == 1 && jet3_L1trig_Matched == 1 )"
three_jet4_tagged_string_triggered = " ( jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26 && jet4_DepthTowers == 1 && jet4_TimingTowers == 1 && jet4_L1trig_Matched == 1 )"
three_jet5_tagged_string_triggered = " ( jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26 && jet5_DepthTowers == 1 && jet5_TimingTowers == 1 && jet5_L1trig_Matched == 1 )"
three_jet_tagged_string_six_triggered = "(" + three_jet_tagged_string_triggered + " || " + three_jet1_tagged_string_triggered + " || " + three_jet2_tagged_string_triggered + " || " + three_jet3_tagged_string_triggered + " || " + three_jet4_tagged_string_triggered + " || " + three_jet5_tagged_string_triggered + ")"

# defining single and double tag strings, require jet is matched to L1 LLP not-triggered jet
jet_string_not_triggered = "  ( jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_L1trig_Matched == 0) "

jet_string_not_triggered_noFlag = "  ( jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_L1trig_Matched == 0 && (jet0_DepthTowers + jet0_TimingTowers == 0)) "

one_jet_tagged_string_not_triggered = " ( " + jet0_depth + " && jet0_L1trig_Matched == 0 )" # jet 0 condition

two_jet_tagged_string_not_triggered = " ( " + jet0_timing + " && jet0_L1trig_Matched == 0 )" # jet 0 condition

three_jet_tagged_string_not_triggered = " (  " + jet0_depth_timing + " && jet0_L1trig_Matched == 0 )" # jet 0 condition

# same thing, but for 6 jets
jet_string_six_not_triggered = "  ( ( jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_L1trig_Matched == 0) || (jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26 && jet1_L1trig_Matched == 0)"
jet_string_six_not_triggered += " || (jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26 && jet2_L1trig_Matched == 0) || (jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26 && jet3_L1trig_Matched == 0)"
jet_string_six_not_triggered += " || (jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26 && jet4_L1trig_Matched == 0) || (jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26 && jet5_L1trig_Matched == 0))"

jet_string_six_not_triggered_noFlag = "  ( ( jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_L1trig_Matched == 0 && (jet0_DepthTowers + jet0_TimingTowers == 0)) || (jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26 && jet1_L1trig_Matched == 0 && (jet1_DepthTowers + jet1_TimingTowers == 0))"
jet_string_six_not_triggered_noFlag += " || (jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26 && jet2_L1trig_Matched == 0 && (jet2_DepthTowers + jet2_TimingTowers == 0)) || (jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26 && jet3_L1trig_Matched == 0 && (jet3_DepthTowers + jet3_TimingTowers == 0))"
jet_string_six_not_triggered_noFlag += " || (jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26 && jet4_L1trig_Matched == 0 && (jet4_DepthTowers + jet4_TimingTowers == 0)) || (jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26 && jet5_L1trig_Matched == 0 && (jet5_DepthTowers + jet5_TimingTowers == 0)))"

one_jet_tagged_string_six_not_triggered = " ( (  " + jet0_depth + " && jet0_L1trig_Matched == 0 )" # jet 0 condition
one_jet_tagged_string_six_not_triggered += " || (" + jet1_depth + " && jet1_L1trig_Matched == 0 )" # jet 1 condition
one_jet_tagged_string_six_not_triggered += " || (" + jet2_depth + " && jet2_L1trig_Matched == 0 )" # jet 2 condition
one_jet_tagged_string_six_not_triggered += " || (" + jet3_depth + " && jet3_L1trig_Matched == 0 )" # jet 3 condition
one_jet_tagged_string_six_not_triggered += " || (" + jet4_depth + " && jet4_L1trig_Matched == 0 )" # jet 4 condition
one_jet_tagged_string_six_not_triggered += " || (" + jet5_depth + " && jet5_L1trig_Matched == 0 ))" # jet 5 condition

two_jet_tagged_string_six_not_triggered = " ( (   " + jet0_timing + " && jet0_L1trig_Matched == 0 )" # jet 0 condition
two_jet_tagged_string_six_not_triggered += " || ( " + jet1_timing + " && jet1_L1trig_Matched == 0 )" # jet 1 condition
two_jet_tagged_string_six_not_triggered += " || ( " + jet2_timing + " && jet2_L1trig_Matched == 0 )" # jet 2 condition
two_jet_tagged_string_six_not_triggered += " || ( " + jet3_timing + " && jet3_L1trig_Matched == 0 )" # jet 3 condition
two_jet_tagged_string_six_not_triggered += " || ( " + jet4_timing + " && jet4_L1trig_Matched == 0 )" # jet 4 condition
two_jet_tagged_string_six_not_triggered += " || ( " + jet5_timing + " && jet5_L1trig_Matched == 0 ))" # jet 5 condition

three_jet_tagged_string_six_not_triggered = " ( (   " + jet0_depth_timing + " && jet0_L1trig_Matched == 0 )" # jet 0 condition
three_jet_tagged_string_six_not_triggered += " || ( " + jet1_depth_timing + " && jet1_L1trig_Matched == 0 )" # jet 1 condition
three_jet_tagged_string_six_not_triggered += " || ( " + jet2_depth_timing + " && jet2_L1trig_Matched == 0 )" # jet 2 condition
three_jet_tagged_string_six_not_triggered += " || ( " + jet3_depth_timing + " && jet3_L1trig_Matched == 0 )" # jet 3 condition
three_jet_tagged_string_six_not_triggered += " || ( " + jet4_depth_timing + " && jet4_L1trig_Matched == 0 )" # jet 4 condition
three_jet_tagged_string_six_not_triggered += " || ( " + jet5_depth_timing + " && jet5_L1trig_Matched == 0 ))" # jet 5 condition

# defining single and double tag strings, require jet is matched to L1 LLP jet, regardless of trigger result
jet_string_matched = "  ( jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_L1trig_Matched >= 0) "

jet_string_matched_noFlag = "  ( jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_L1trig_Matched >= 0 && (jet0_DepthTowers + jet0_TimingTowers == 0)) "

one_jet_tagged_string_matched = " ( " + jet0_depth + " && jet0_L1trig_Matched >= 0 )" # jet 0 condition

two_jet_tagged_string_matched = " ( " + jet0_timing + " && jet0_L1trig_Matched >= 0 )" # jet 0 condition

three_jet_tagged_string_matched = " ( " + jet0_depth_timing + " && jet0_L1trig_Matched >= 0 )" # jet 0 condition

# same thing but for 6 jets
jet_string_six_matched = "  ( ( jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_L1trig_Matched >= 0) || (jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26 && jet1_L1trig_Matched >= 0)"
jet_string_six_matched += " || (jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26 && jet2_L1trig_Matched >= 0) || (jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26 && jet3_L1trig_Matched >= 0)"
jet_string_six_matched += " || (jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26 && jet4_L1trig_Matched >= 0) || (jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26 && jet5_L1trig_Matched >= 0))"

jet_string_six_matched_noFlag = "  ( ( jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_L1trig_Matched >= 0 && (jet0_DepthTowers + jet0_TimingTowers == 0)) || (jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26 && jet1_L1trig_Matched >= 0 && (jet1_DepthTowers + jet1_TimingTowers == 0))"
jet_string_six_matched_noFlag += " || (jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26 && jet2_L1trig_Matched >= 0 && (jet2_DepthTowers + jet2_TimingTowers == 0)) || (jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26 && jet3_L1trig_Matched >= 0 && (jet3_DepthTowers + jet3_TimingTowers == 0))"
jet_string_six_matched_noFlag += " || (jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26 && jet4_L1trig_Matched >= 0 && (jet4_DepthTowers + jet4_TimingTowers == 0)) || (jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26 && jet5_L1trig_Matched >= 0 && (jet5_DepthTowers + jet5_TimingTowers == 0)))"

one_jet_tagged_string_six_matched = " ( (  " + jet0_depth + " && jet0_L1trig_Matched >= 0 )" # jet 0 condition
one_jet_tagged_string_six_matched += " || (" + jet1_depth + " && jet1_L1trig_Matched >= 0 )" # jet 1 condition
one_jet_tagged_string_six_matched += " || (" + jet2_depth + " && jet2_L1trig_Matched >= 0 )" # jet 2 condition
one_jet_tagged_string_six_matched += " || (" + jet3_depth + " && jet3_L1trig_Matched >= 0 )" # jet 3 condition
one_jet_tagged_string_six_matched += " || (" + jet4_depth + " && jet4_L1trig_Matched >= 0 )" # jet 4 condition
one_jet_tagged_string_six_matched += " || (" + jet5_depth + " && jet5_L1trig_Matched >= 0 ))" # jet 5 condition

two_jet_tagged_string_six_matched = " ( (   " + jet0_timing + " && jet0_L1trig_Matched >= 0 )" # jet 0 condition
two_jet_tagged_string_six_matched += " || ( " + jet1_timing + " && jet1_L1trig_Matched >= 0 )" # jet 1 condition
two_jet_tagged_string_six_matched += " || ( " + jet2_timing + " && jet2_L1trig_Matched >= 0 )" # jet 2 condition
two_jet_tagged_string_six_matched += " || ( " + jet3_timing + " && jet3_L1trig_Matched >= 0 )" # jet 3 condition
two_jet_tagged_string_six_matched += " || ( " + jet4_timing + " && jet4_L1trig_Matched >= 0 )" # jet 4 condition
two_jet_tagged_string_six_matched += " || ( " + jet5_timing + " && jet5_L1trig_Matched >= 0 ))" # jet 5 condition

three_jet_tagged_string_six_matched = " ( (   " + jet0_depth_timing + " && jet0_L1trig_Matched >= 0 )" # jet 0 condition
three_jet_tagged_string_six_matched += " || ( " + jet1_depth_timing + " && jet1_L1trig_Matched >= 0 )" # jet 1 condition
three_jet_tagged_string_six_matched += " || ( " + jet2_depth_timing + " && jet2_L1trig_Matched >= 0 )" # jet 2 condition
three_jet_tagged_string_six_matched += " || ( " + jet3_depth_timing + " && jet3_L1trig_Matched >= 0 )" # jet 3 condition
three_jet_tagged_string_six_matched += " || ( " + jet4_depth_timing + " && jet4_L1trig_Matched >= 0 )" # jet 4 condition
three_jet_tagged_string_six_matched += " || ( " + jet5_depth_timing + " && jet5_L1trig_Matched >= 0 ))" # jet 5 condition

# defining single and double tag strings, require jet is matched to L1 LLP triggered jet, no cuts on DNN scores
two_jet_string_six_triggered = " ( ( ( jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_L1trig_Matched == 1 ) && (jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26 && jet1_L1trig_Matched == 1 ))" # jet 0 AND 1 condition
two_jet_string_six_triggered += " || ((jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_L1trig_Matched == 1 ) && (jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26 && jet2_L1trig_Matched == 1 ))" # jet 0 AND 2 condition
two_jet_string_six_triggered += " || ((jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_L1trig_Matched == 1 ) && (jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26 && jet3_L1trig_Matched == 1 ))" # jet 0 AND 3 condition
two_jet_string_six_triggered += " || ((jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_L1trig_Matched == 1 ) && (jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26 && jet4_L1trig_Matched == 1 ))" # jet 0 AND 4 condition
two_jet_string_six_triggered += " || ((jet0_Pt >= "+jet_energy+" && abs(jet0_Eta) <= 1.26 && jet0_L1trig_Matched == 1 ) && (jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26 && jet5_L1trig_Matched == 1 ))" # jet 0 AND 5 condition
two_jet_string_six_triggered += " || ((jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26 && jet1_L1trig_Matched == 1 ) && (jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26 && jet2_L1trig_Matched == 1 ))" # jet 1 AND 2 condition
two_jet_string_six_triggered += " || ((jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26 && jet1_L1trig_Matched == 1 ) && (jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26 && jet3_L1trig_Matched == 1 ))" # jet 1 AND 3 condition
two_jet_string_six_triggered += " || ((jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26 && jet1_L1trig_Matched == 1 ) && (jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26 && jet4_L1trig_Matched == 1 ))" # jet 1 AND 4 condition
two_jet_string_six_triggered += " || ((jet1_Pt >= "+jet_energy+" && abs(jet1_Eta) <= 1.26 && jet1_L1trig_Matched == 1 ) && (jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26 && jet5_L1trig_Matched == 1 ))" # jet 1 AND 5 condition
two_jet_string_six_triggered += " || ((jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26 && jet2_L1trig_Matched == 1 ) && (jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26 && jet3_L1trig_Matched == 1 ))" # jet 2 AND 3 condition
two_jet_string_six_triggered += " || ((jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26 && jet2_L1trig_Matched == 1 ) && (jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26 && jet4_L1trig_Matched == 1 ))" # jet 2 AND 4 condition
two_jet_string_six_triggered += " || ((jet2_Pt >= "+jet_energy+" && abs(jet2_Eta) <= 1.26 && jet2_L1trig_Matched == 1 ) && (jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26 && jet5_L1trig_Matched == 1 ))" # jet 2 AND 5 condition
two_jet_string_six_triggered += " || ((jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26 && jet3_L1trig_Matched == 1 ) && (jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26 && jet4_L1trig_Matched == 1 ))" # jet 3 AND 4 condition
two_jet_string_six_triggered += " || ((jet3_Pt >= "+jet_energy+" && abs(jet3_Eta) <= 1.26 && jet3_L1trig_Matched == 1 ) && (jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26 && jet5_L1trig_Matched == 1 ))" # jet 3 AND 5 condition
two_jet_string_six_triggered += " || ((jet4_Pt >= "+jet_energy+" && abs(jet4_Eta) <= 1.26 && jet4_L1trig_Matched == 1 ) && (jet5_Pt >= "+jet_energy+" && abs(jet5_Eta) <= 1.26 && jet5_L1trig_Matched == 1 )))" # jet 4 AND 5 condition

# HLT
HLT_string = "(( HLT_HT200_L1SingleLLPJet_DisplacedDijet40_Inclusive1PtrkShortSig5 == 1 || HLT_HT240_L1SingleLLPJet_DisplacedDijet40_Inclusive1PtrkShortSig5 == 1 || HLT_HT280_L1SingleLLPJet_DisplacedDijet40_Inclusive1PtrkShortSig5 == 1)"
HLT_string += "|| ( HLT_HT170_L1SingleLLPJet_DisplacedDijet40_DisplacedTrack == 1 || HLT_HT200_L1SingleLLPJet_DisplacedDijet40_DisplacedTrack == 1 || HLT_HT270_L1SingleLLPJet_DisplacedDijet40_DisplacedTrack == 1 || HLT_HT200_L1SingleLLPJet_DisplacedDijet60_DisplacedTrack == 1 )"
HLT_string += "|| ( HLT_HT320_L1SingleLLPJet_DisplacedDijet60_Inclusive == 1 || HLT_HT420_L1SingleLLPJet_DisplacedDijet60_Inclusive == 1 )"
HLT_string += "|| ( HLT_HT200_L1SingleLLPJet_DelayedJet40_DoubleDelay0p5nsTrackless == 1 || HLT_HT200_L1SingleLLPJet_DelayedJet40_DoubleDelay1nsInclusive == 1 || HLT_HT200_L1SingleLLPJet_DelayedJet40_SingleDelay1nsTrackless == 1 || HLT_HT200_L1SingleLLPJet_DelayedJet40_SingleDelay2nsInclusive == 1 )"
HLT_string += "|| HLT_L1SingleLLPJet == 1)"