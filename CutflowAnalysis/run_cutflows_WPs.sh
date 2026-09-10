#!/bin/bash
# =============================================================================
# Signal cutflows for the three candidate working points (mH=125, mS=50, ctau=3m)
# run separately on each of the three eras.
#
#   preEE     era1, lumi_frac 0.1282  -> cat12 DNN cuts
#   postEE    era2, lumi_frac 0.7165  -> cat12 DNN cuts
#   postBPix  era3, lumi_frac 0.1553  -> cat3  DNN cuts
#
# Working points (from Optimization/SR_optimization_summary_ANv8_Sept4.txt):
#   fullopt WP1   cat12 and cat3 optimized independently; best limit
#   fullopt WP2   ditto, tighter / lower background
#   sigmatch WP2  cat12 chosen to reproduce the cat3 SIGNAL efficiency
#
# -----------------------------------------------------------------------------
# WHY ONE SAMPLE PER ERA
#
# This replaces an earlier version that ran on the merged file
#   minituple_HToSSTo4B_MH125_MS50_CTau3000_22preBPix23_scores.root
# That file is Premix_2022preEE + PU60_2022postEE + PU70_2022postEE hadded
# (30249 + 31977 + 32715 = 94941 events). PU60 and PU70 are BOTH era2, both
# carry weight = 0.8131 and lumi_frac = 0.7165, i.e. each is normalised as if it
# alone represented the full era2 luminosity. Summing them DOUBLE COUNTS era2
# and inflated the preBPix signal yield by ~1.9x.
#
# This is the failure mode optimize_cuts.py documents: "requires ONE sample per
# era in each .txt list, otherwise an era is counted more than once". The
# optimization's own list (HToSSTo4B_MH125_MS50_CTau3000_22.txt) is correct --
# PU60 + Premix, one per era -- so only the cutflow was affected.
#
# Running per era, with one sample each, avoids the issue by construction: every
# file here has a single uniform lumi_frac, so the "x lumi_frac" yields are
# directly summable across eras.
#
# NB: PU70_2022postEE is an equally valid era2 sample. Use it INSTEAD of PU60 as
# a cross-check, never in addition. Set POSTEE_SAMPLE below to switch.
#
# -----------------------------------------------------------------------------
# Argument mapping for FinalAnalysisCutflow.py:
#     --depth       LJDC depth cut        --inc        LJDC inclusive cut
#     --depth-sjdc  SJDC depth cut        --inc-sjdc   SJDC inclusive cut
#
# Each configuration is run twice: once plain, once with --lumi-frac (which adds
# a final row scaling the yield by the per-event lumi_frac branch).
#
# "weight" assumes BR(H->SS->4b) = 1%, so a yield is the expected count at
# BR = 1%, and a limit mu converts as BR = mu * 0.01.
# =============================================================================

set -u

REPO="/afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis"
CUTFLOW="${REPO}/CutflowAnalysis/FinalAnalysisCutflow.py"
EOS="/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.6"

# era2 sample: PU60 (matches the optimization list). Swap to PU70 as a cross-check.
POSTEE_SAMPLE="PU60"

PREEE="${EOS}/minituple_HToSSTo4B_MH350_MS80_CTau500_Premix_2022preEE_scores.root"
POSTEE="${EOS}/minituple_HToSSTo4B_MH350_MS80_CTau500_${POSTEE_SAMPLE}_2022postEE_scores.root"
POSTBPIX="${EOS}/minituple_HToSSTo4B_350_80_CTau500_scores.root"

OUTDIR="${1:-${REPO}/CutflowAnalysis/cutflows_WPs}"
mkdir -p "${OUTDIR}"

for f in "${PREEE}" "${POSTEE}" "${POSTBPIX}"; do
    if [ ! -f "${f}" ]; then
        echo "ERROR: input file not found: ${f}" >&2
        exit 1
    fi
done

# run_wp <tag> <file> <depth_LJDC> <inc_LJDC> <depth_SJDC> <inc_SJDC>
run_wp() {
    local tag="$1" file="$2" depth="$3" inc="$4" depth_sjdc="$5" inc_sjdc="$6"

    echo ""
    echo "================================================================"
    echo " ${tag}"
    echo "   LJDC: depth > ${depth}   incl > ${inc}"
    echo "   SJDC: depth > ${depth_sjdc}   incl > ${inc_sjdc}"
    echo "   file: $(basename "${file}")"
    echo "================================================================"

    local variant out flag
    for variant in plain lumifrac; do
        if [ "${variant}" = "plain" ]; then
            out="${OUTDIR}/cutflow_${tag}.tex";          flag=""
            echo "--- without lumi_frac ---"
        else
            out="${OUTDIR}/cutflow_${tag}_lumifrac.tex"; flag="--lumi-frac"
            echo "--- with lumi_frac ---"
        fi
        python3 "${CUTFLOW}" \
            --file "${file}" --depth "${depth}" --inc "${inc}" \
            --depth-sjdc "${depth_sjdc}" --inc-sjdc "${inc_sjdc}" \
            ${flag} --latex 2>&1 | tee "${out}"
        if [ "${PIPESTATUS[0]}" -ne 0 ]; then
            echo "WARNING: cutflow failed for ${tag} (${variant})" >&2
        fi
    done
}

# ----------------------------------------------------------------------------
#                                       depth   incl   depth   incl
#                                       LJDC    LJDC   SJDC    SJDC
# ----------------------------------------------------------------------------
# ---- fullopt WP1 ----
run_wp fullopt_WP1_preEE      "${PREEE}"    0.905  0.785  0.925  0.955
run_wp fullopt_WP1_postEE     "${POSTEE}"   0.905  0.785  0.925  0.955
run_wp fullopt_WP1_postBPix   "${POSTBPIX}" 0.985  0.995  0.995  0.975

# ---- fullopt WP2 ----
run_wp fullopt_WP2_preEE      "${PREEE}"    0.985  0.955  0.925  0.955
run_wp fullopt_WP2_postEE     "${POSTEE}"   0.985  0.955  0.925  0.955
run_wp fullopt_WP2_postBPix   "${POSTBPIX}" 0.995  0.995  0.995  0.985

# ---- signal-efficiency-matched WP2 ----
run_wp sigmatch_WP2_preEE     "${PREEE}"    0.935  0.690  0.917  0.699
run_wp sigmatch_WP2_postEE    "${POSTEE}"   0.935  0.690  0.917  0.699
run_wp sigmatch_WP2_postBPix  "${POSTBPIX}" 0.995  0.995  0.995  0.995

# ----------------------------------------------------------------------------
# Summary, grouped by working point, with the era sum
# ----------------------------------------------------------------------------
get_raw()  { [ -f "$1" ] && grep -E '\(depth\)' "$1" | tail -1 | awk -F'&' '{gsub(/ /,"",$2); print $2}'; }
get_corr() { [ -f "$1" ] && grep 'final yield x lumi_frac' "$1" | awk -F'&' '{gsub(/ /,"",$2); print $2}'; }
get_lf()   { [ -f "$1" ] && grep -o 'lumi\\_frac = [0-9.]*' "$1" | awk '{print $NF}'; }

echo ""
echo "======================================================================"
echo " SUMMARY -- final selection yield @ BR = 1%"
echo "======================================================================"
printf "%-16s %-10s %10s %13s %13s\n" "working point" "era" "raw" "x lumi_frac" "eff lumi_frac"
for wp in fullopt_WP1 fullopt_WP2 sigmatch_WP2; do
    total=0
    for era in preEE postEE postBPix; do
        raw=$(get_raw  "${OUTDIR}/cutflow_${wp}_${era}.tex")
        corr=$(get_corr "${OUTDIR}/cutflow_${wp}_${era}_lumifrac.tex")
        lf=$(get_lf    "${OUTDIR}/cutflow_${wp}_${era}_lumifrac.tex")
        printf "%-16s %-10s %10s %13s %13s\n" "${wp}" "${era}" "${raw:-n/a}" "${corr:-n/a}" "${lf:-n/a}"
        if [ -n "${corr:-}" ]; then
            total=$(awk -v a="${total}" -v b="${corr}" 'BEGIN{printf "%.4f", a+b}')
        fi
    done
    printf "%-16s %-10s %10s %13s\n" "${wp}" "TOTAL" "" "${total}"
    echo "----------------------------------------------------------------------"
done

echo ""
echo "Outputs in: ${OUTDIR}"
echo "era2 sample in use: ${POSTEE_SAMPLE}_2022postEE"
echo ""
echo "The 'x lumi_frac' column is the era's true contribution and the three eras"
echo "are directly summable (TOTAL row) -- each file holds one sample with a"
echo "single uniform lumi_frac, so there is no double counting. Use TOTAL as the"
echo "signal yield at BR = 1% when computing limits."
