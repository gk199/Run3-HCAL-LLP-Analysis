# with old version

# python3 optimize_cuts.py -i HToSSTo4B_125_50_CTau3000 -s FullRange_100x100_Normal_v5.3 -c -p
# python3 optimize_cuts.py -i HToSSTo4B_250_120_CTau10000 -s FullRange_100x100_Normal_v5.3 -c -p
# python3 optimize_cuts.py -i HToSSTo4B_350_160_CTau10000 -s FullRange_100x100_Normal_v5.3 -c -p
# python3 optimize_cuts.py -i HToSSTo4B_350_80_CTau500 -s FullRange_100x100_Normal_v5.3 -c -p

# with version using different signal per year (pre and post BPix)

python3 optimize_cuts.py --sig22 HToSSTo4B_MH125_MS50_CTau3000_22.txt --sig23 HToSSTo4B_125_50_CTau3000 -s test -c -p
python3 optimize_cuts.py --sig22 HToSSTo4B_MH250_MS120_CTau10000_22.txt --sig23 HToSSTo4B_250_120_CTau10000 -s test -c -p
python3 optimize_cuts.py --sig22 HToSSTo4B_MH350_MS160_CTau10000_22.txt --sig23 HToSSTo4B_350_160_CTau10000 -s test -c -p
python3 optimize_cuts.py --sig22 HToSSTo4B_MH350_MS80_CTau500_22.txt --sig23 HToSSTo4B_350_80_CTau500 -s test -c -p
# python3 optimize_cuts.py --sig22 HToSSTo4B_MH250_MS120_CTau10000_PU60_2022postEE --sig23 HToSSTo4B_250_120_CTau10000 -s test -c -p
