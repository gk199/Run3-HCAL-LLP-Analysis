#!/bin/bash
# Check what fraction of minituple output files are empty (<100KB) per sample directory.
# Covers central signal MC, private signal MC, and data in the same EOS base dir.
# Usage: ./check_empty_output.sh [eos_base_dir] [output_txt]

EOS_BASE=${1:-/eos/home-g/gkopp/LLP_Analysis/MiniTuples/v5.6}
OUTFILE=${2:-empty_output_report.txt}
THRESHOLD=100000   # bytes; empty ROOT files are ~44KB, good ones are ~1MB+
WARN_PCT=10        # flag as problematic if % empty exceeds this

classify_sample() {
    local name=$1
    if [[ "$name" == DisplacedJet* ]]; then
        echo "Data"
    elif [[ "$name" == *L1triggers* ]]; then
        echo "PrivMC"
    elif [[ "$name" == HToSSTo4B* ]] && [[ "$name" != *Premix* ]] && [[ "$name" != *PU[0-9]* ]]; then
        echo "PrivMC"
    elif [[ "$name" == HToSSTo4B* ]]; then
        echo "Signal"
    else
        echo "Other"
    fi
}

check_dir() {
    local dir=$1
    [ -d "$dir" ] || return
    local sample
    sample=$(basename "$dir")

    local total=0 empty=0 latest_mtime=0 latest_date=""
    for f in "$dir"/minituple_job*.root; do
        [[ "$f" == *_scores* ]] && continue
        [ -f "$f" ] || continue
        local size mtime
        size=$(stat -c%s "$f" 2>/dev/null) || continue
        mtime=$(stat -c%Y "$f" 2>/dev/null) || continue
        (( total++ ))
        (( size < THRESHOLD )) && (( empty++ ))
        if (( mtime > latest_mtime )); then
            latest_mtime=$mtime
            latest_date=$(stat -c%y "$f" 2>/dev/null | cut -d' ' -f1,2 | cut -d'.' -f1)
        fi
    done

    [ "$total" -eq 0 ] && return

    local good=$(( total - empty ))
    local pct=$(( empty * 100 / total ))

    local scores=0
    for f in "$dir"/minituple_job*_scores*.root; do
        [ -f "$f" ] || continue
        local size
        size=$(stat -c%s "$f" 2>/dev/null) || continue
        (( size >= THRESHOLD )) && (( scores++ ))
    done
    local scores_str="${scores}/${good}"

    local status="ok"
    (( pct >= WARN_PCT )) && status="RERUN NEEDED"
    (( good > 0 && scores == 0 )) && status="${status:+${status}, }NO SCORES"
    (( good > 0 && scores > 0 && scores < good )) && status="${status:+${status}, }SCORES PARTIAL"

    local type
    type=$(classify_sample "$sample")
    printf "%-8s %-70s %6d %6d %6d%% %8s  %-20s %s\n" \
        "$type" "$sample" "$good" "$empty" "$pct" "$scores_str" "$latest_date" "$status"
}

{
printf "%-8s %-70s %6s %6s %7s %8s  %-20s %s\n" "Type" "Sample" "Good" "Empty" "Empty%" "Scores" "LastModified" "Status"
printf -- "%.0s-" {1..140}; echo

current_section=""
for dir in "$EOS_BASE"/*/; do
    dir="${dir%/}"
    [ -d "$dir" ] || continue
    sample=$(basename "$dir")
    section=$(classify_sample "$sample")

    if [[ "$section" != "$current_section" ]]; then
        [ -n "$current_section" ] && echo
        printf "=== %s ===\n" "$section"
        current_section="$section"
    fi

    check_dir "$dir"
done
} | tee "$OUTFILE"

echo
echo "Report saved to: $OUTFILE"
