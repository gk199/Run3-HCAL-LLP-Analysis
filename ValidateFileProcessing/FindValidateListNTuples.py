"""
Find, Validate, and List NTuples
================================

TODO:
- Split data by era

"""

import os
import argparse
#import yaml
#import ROOT
#import git # gitpython
import time
import tarfile
import uproot

import logging
logger = logging.getLogger(__name__)

from datetime import datetime
now = datetime.now()
timestamp = now.strftime("%Y-%m-%d_%H.%M.%S")

xrd = "root://cmsxrootd.fnal.gov//"

# Valid Users on Wisc Server
users = ["gkopp", "kikenned"] 
#users = ["kikenned"]

# Script version
SCRIPTVERSION = "0.1"

cwd = os.getcwd()

# ------------------------------------------------------------------------------
def parseArgs():
	""" Parse command-line arguments
	"""
	parser = argparse.ArgumentParser(
		add_help=True,
		description=''
	)

	# Required
	parser.add_argument("-v", "--version",      action="store",      required=True,  help="NTuple Version")
	#parser.add_argument("-u", "--user",        action="store",      required=True,  help="Your name")
	#parser.add_argument("-c", "--config",       action="store",      required=True,  help="Config file (.xml)")
	#parser.add_argument("-f", "--firmware",     action="store",      required=True,  help="Firmware image file loaded onto fpga via fpgaconfig")
	#parser.add_argument("-m", "--modules",      action="append",     required=True,  help="Module IDs (pass each module with -m)")

	# Optional
	#parser.add_argument("-v", "--high-voltage", action="store",      default=0,      help="High voltage value")
	#parser.add_argument("-t", "--temp",         action="store",      default="RT",   help="Tempterature (default = room temp)")

	# Commands
	#parser.add_argument("-r", "--reconfigure",  action="store_true", default=False, help="Passes '--reconfigure -b' command to ot_module_test")
	#parser.add_argument("-n", "--noise", 		action="store_true", default=False, help="Passes '-m -a -t' command to ot_module_test")
	#parser.add_argument("-x", "--execute",      action="store",      default=False, help="Passes argument as full command to ot_module_test (overrides everything else)")

	# Debug and Additional Info
	#parser.add_argument("-d", "--debug",        action="store_true", default=False, help="Debug mode")
	#parser.add_argument("--dry-run",            action="store_true", default=False, help="Test only, nothing executes")
	#parser.add_argument("--comment",            action="store",      default="",    help="User comment")

	args = parser.parse_args()

	return args

# ------------------------------------------------------------------------------
def checkArgs( args ):

	return True


# ------------------------------------------------------------------------------
def getDatasets():

	# datasets = ["DisplacedJet", "HToSSTo4B_MH125_MS15_CTau10000"]
	datasets = ["HToSSTo4B_MH125_MS50_CTau3000", "HToSSTo4B_MH250_MS120_CTau10000", "HToSSTo4B_MH350_MS160_CTau10000", "HToSSTo4B_MH350_MS80_CTau500"]
	return datasets


# ------------------------------------------------------------------------------
def getNTupleFileLists( dataset, tag_version ):

	filelists = {}
	filelists["ntuples"]   = []
	filelists["basepaths"] = []
	filelists["logs"]      = []

	#tag_version_mod = "NTupleV"+version

	for user in users:
		basepath = os.path.join( "/hdfs/store/user/",user,dataset )
		
		for root, dirs, files in os.walk(basepath):
			if tag_version not in root: continue

			for filename in files:
				if filename.endswith('.root'):
					filepath = os.path.join(root, filename)
					filelists["ntuples"].append(filepath)
					filelists["basepaths"].append(root)

				if filename.endswith('.tar.gz'):
					filepath = os.path.join(root, filename)
					filelists["logs"].append(filepath)

	for filelist in filelists:
		filelist = list(set(filelist))

	return filelists

# ------------------------------------------------------------------------------
def writeNTupleFileLists( dataset, filelists ):

	textfilepath = "NTuplesV4_"+dataset+".txt"

	with open(textfilepath, 'w') as output:
		for filepath in filelists["ntuples"]:
			output.write(filepath+"\n")
	
	print("NTuple list written to: ", textfilepath)

# ------------------------------------------------------------------------------
def checkEventsProcessed( filelists ):

	eventcountreport = ""

	# Check File Count #

	NFiles_logs    = len(filelists["logs"])
	NFiles_ntuples = len(filelists["ntuples"])

	eventcountreport += "Checking file counts...\n"
	eventcountreport += "\tNFiles_logs    = "+str(NFiles_logs)+"\n"
	eventcountreport += "\tNFiles_ntuples = "+str(NFiles_ntuples)+"\n"

	if NFiles_logs != NFiles_ntuples:
		eventcountreport += "\tERROR: Number of files disagree!\n"

	# Check File Suffixes #

	filesuffixes_logs    = [x.split("_")[-1].split(".log")[0] for x in filelists["logs"]]
	filesuffixes_ntuples = [x.split("_")[-1].split(".root")[0] for x in filelists["ntuples"]]

	if filesuffixes_logs != filesuffixes_ntuples:

		eventcountreport += "\tERROR: Could not find ntuple files for the following logfiles:"
		for suf in filesuffixes_logs:
			if suf not in filesuffixes_ntuples:
				eventcountreport += " " + suf + ","
		eventcountreport = eventcountreport[:-1] + "\n"

		eventcountreport += "\tERROR: Could not find logfiles for the following ntuples:"
		for suf in filesuffixes_ntuples:
			if suf not in filesuffixes_logs:
				eventcountreport += " " + suf + ","
		eventcountreport = eventcountreport[:-1] + "\n"

	# Check Number of Events Processed #

	for logfilepath in filelists["logs"]:
		eventcountreport += "Checking "+logfilepath+"\n"

		# Logfile #

		NEvents_logs = -1

		#tarfilepath = "TEST/hdfs/store/user/gkopp/DisplacedJet/Run2023D-EXOLLPJetHCAL-PromptReco-v2_AOD_20231114_143510/231114_133516/0000/log/cmsRun_1-1.log.tar.gz"
		
		if not logfilepath.endswith("tar.gz"):
			continue

		tar = tarfile.open(logfilepath, "r:gz")
		members = tar.getmembers()

		if len(members) != 2:
			eventcountreport += "\tERROR: Number of files in tarball incorrect (="+str(len(members))+", should be 2)\n"

		for member in tar.getmembers():
			if not member.name.endswith('.log'): continue
			f = tar.extractfile(member)
			lines = f.readlines()

			for line in lines: #reversed(lines):
				line = line.decode().strip()
				#if "Failed to open file at URL" in line: 
				#print( line )
				if "Number of Events" in line:
					NEvents_logs = int(line.split(" ")[-1])
					#break
			break

		if NEvents_logs == -1:
			eventcountreport += "\tERROR: Could not identify number of events processed in logfile \n"

		# NTuple #

		NEvents_ntuple  = -1

		ntuplefilepath = os.path.join( logfilepath.split("log/cmsRun")[0], "output_"+logfilepath.split("_")[-1].split(".log")[0]+".root" )
		#ntuplefilepath = "TEST/hdfs/store/user/gkopp/DisplacedJet/Run2023D-EXOLLPJetHCAL-PromptReco-v2_AOD_20231114_143510/231114_133516/0000/output_1.root"
		
		eventcountreport += "Checking "+ntuplefilepath+"\n"

		if not os.path.isfile(ntuplefilepath):
			eventcountreport += "\tERROR: NTuple FileNotFound\n"
			continue

		# os.path.isfile(file_path)
		with uproot.open(ntuplefilepath) as file:
			tree = file["DisplacedHcalJets/Events"]
			NEvents_ntuple = tree.num_entries

		if NEvents_ntuple == -1:
			eventcountreport += "\tERROR: Could not identify number of events processed in ntuple\n"

		if NEvents_logs != NEvents_ntuple:
			eventcountreport += "ERROR: Mismatched number of events in the above log and ntuple files\n"

	#print( eventcountreport )
	return eventcountreport

# ------------------------------------------------------------------------------
def processDataset( dataset, tag_version ):

	# Write NTuple File Lists

	filelists = getNTupleFileLists( dataset, tag_version )
	writeNTupleFileLists( dataset, filelists )

	# Check Number of Events

	eventcountreport = checkEventsProcessed( filelists )

	if eventcountreport.find("ERROR"):
		#print( "ERROR ", dataset )

		errorfilepath = "NTuplesV4_"+dataset+".err"

		with open(errorfilepath, 'w') as output:
			output.write(eventcountreport)
			print("Errors written to:      ", errorfilepath)
	else:
		print( "No errors found" )

# ------------------------------------------------------------------------------
def main():

	# Parse Arguments

	logger.info("Parsing Arguments...")
	args = parseArgs()
	checkArgs( args )
	tag_version = args.version

	# Loop over datasets

	datasets = getDatasets()

	for dataset in datasets:
		print( "\n--- Running over", dataset, "---" )
		processDataset( dataset, tag_version )

	#os.chdir( cwd )

# ------------------------------------------------------------------------------
if __name__ == '__main__':
	main()
