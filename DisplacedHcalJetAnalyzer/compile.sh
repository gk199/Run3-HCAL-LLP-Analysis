echo "Compiling DisplacedHcalJetAnalyzer..."
g++ -std=c++11 -I/Minuit2/inc util/DisplacedHcalJetAnalyzer.C -o exe/DisplacedHcalJetAnalyzer  `root-config --cflags --glibs` -lTMVA
echo "Done"
