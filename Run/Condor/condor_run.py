#!/usr/bin/env python
"""
HTCondor Wrapper
===========================
Author: Kiley Kennedy <kiley.elizabeth.kennedy@cern.ch>

Submits condor jobs on lxplus from list of input files

For usage: `python3 condor_run.py --help`
"""

import argparse, sys, os, time
from datetime import datetime

datetime_now = datetime.today().strftime('%Y_%m_%d')

start_time = time.time()
cwd = os.getcwd()

# -------------------------------------------------------------------------------------------------
# MODIFY ME! (Keep things global)

#max_jobs_per_submission = 500

header_cmd    = os.path.abspath("condor_header.cmd")
executable_sh = os.path.abspath("condor_executable.sh")

file_disk_usage = os.path.abspath("../Ntuples_v4/NTuplesV4_DiskUsage.txt")

# Global path to executable (& other files to transfer if applicable) -- comma-separated list without spaces
#transfer_input_files 
Executable_DisplacedHcalJetAnalyzer = os.path.abspath("../../DisplacedHcalJetAnalyzer/exe/DisplacedHcalJetAnalyzer") # this is made after compiling 

# ------------------------------------------------------------------------------
def parseArgs():
    """ Parse command-line arguments
    """
    parser = argparse.ArgumentParser(
        add_help=True,
        description=''
    )

    parser.add_argument("-i", "--input_file", action="store", help="Input text file containing ROOT inputs", required=True)
    parser.add_argument("-o", "--output_dir", action="store", default="./", help="Output directory path")
    # parser.add_argument("-b", "--bdt_zipfile",  action="store", default="./BDTWeightFiles.zip", help="Path to directory containing bdt files")
    # zip -r BDTWeightFiles.zip BDTWeightFiles
    parser.add_argument("-p", "--proxy",      action="store", help="Proxy path: generate via `voms-proxy-init -voms cms` and copy file from /tmp/ area", required=True) # TODO
    parser.add_argument("-s", "--setup_only", action="store_true", help="Setup jobs only") 
    parser.add_argument("-t", "--test",       action="store_true", help="Submit test job only") 
    parser.add_argument("-d", "--debug",      action="store_true", help="Debug") 
    parser.add_argument("-f", "--flag",       action="store", default="", help="Flag to append to Job_ directory")

    args = parser.parse_args()

    return args

# -------------------------------------------------------------------------------------------------
def main():
 
    print( "Gathering Submission Info..." )

    # ----- Get Args ----- #

    args = parseArgs()

    input_file  = args.input_file
    output_dir  = args.output_dir + "_" + datetime_now
    # bdt_zipfile = os.path.abspath( args.bdt_zipfile )
    proxy       = os.path.abspath( args.proxy )
    setup_only  = args.setup_only
    test        = args.test
    debug       = args.debug
    flag        = args.flag

    flavor="longlunch" #workday" #microcentury"
    #if "LLPskim_2023Cv4" in flag: flavor="workday"
    #if "LLPskim_2023Cv1" in flag: flavor="longlunch"
    #if "LLPskim_2023Vv1" in flag: flavor="longlunch"
    if "HToSSTo4b" in flag: flavor="longlunch" #microcentury" #espresso"
    # ----- Get Inputs ----- #

    if not os.path.isfile(input_file): 
        print( "ERROR:", input_file, " does not exist. Exiting..." )
        quit()

    file_sizes = {}
    with open(file_disk_usage) as f:
        for line in f.readlines():
            line_temp      = line.replace(" ","").strip()
            file_size_temp = line_temp.split("\t")[0]
            file_temp      = line_temp.split("store")[-1]

            if "K" in file_size_temp: 
                file_size_temp = float(file_size_temp.replace("K", ""))*1e-3*1e-3
            elif "M" in file_size_temp: 
                file_size_temp = float(file_size_temp.replace("M", ""))*1e-3
            elif "G" in file_size_temp: 
                file_size_temp = float(file_size_temp.replace("G", ""))
            elif "T" in file_size_temp:
                file_size_temp = float(file_size_temp.replace("T", ""))*1e3
            else: 
                file_size_temp = 0

            file_sizes[file_temp] = file_size_temp

    input_list = []

    infile_paths_temp = []
    infile_paths_sizes = 0
    j = 0

    with open(input_file) as f:
        for line in f.readlines():
            line_temp = line.replace(" ","").strip()
            if line_temp[0] == "#": continue

            if debug: print( "Reading in:", line_temp )

            #infile_tag  = line_temp.split("/")[-1].replace(".root","")
            infile_paths_temp.append( line_temp )

            if line_temp.split("store")[-1] in file_sizes: 
                infile_paths_sizes += file_sizes[line_temp.split("store")[-1]]
            else: 
                infile_paths_sizes += 5

            if infile_paths_sizes >= 5:
                input_list.append( infile_paths_temp ) 

	#os.system( "ln -s " + bdt_zipfile + " BDTWeightFiles.zip" )
	#bdt_zipfile_new = os.path.abspath( "BDTWeightFiles.zip" )

	# ----- DNN input files ----- #
	add_scores = os.path.abspath( "../../../Classifiers/ScoresToEventBased-v4.py" )
	keras_depth = os.path.abspath( "../../../Classifiers/depth_model_v4.keras" )
	keras_inclusive = os.path.abspath( "../../../Classifiers/inclusive_model_v4.keras" )
	norm_constants = os.path.abspath( "../../../Classifiers/norm_constants_v3.csv" )

	# ----- Jet veto map files ----- #
	# these are now hardcoded in the DisplacedHcalJetAnalyzer.C util file
	jetVeto_2022 = os.path.abspath( "../../../../CMSSW_13_2_0/src/cms_lpc_llp/Run3-HCAL-LLP-NTupler/data/JEC_JER/JECDatabase/jet_veto_maps/Summer22_23Sep2023/Summer22_23Sep2023_RunCD_v1.root" )	
	jetVeto_2022EE = os.path.abspath( "../../../../CMSSW_13_2_0/src/cms_lpc_llp/Run3-HCAL-LLP-NTupler/data/JEC_JER/JECDatabase/jet_veto_maps/Summer22EE_23Sep2023/Summer22EE_23Sep2023_RunEFG_v1.root" )
	jetVeto_2023 = os.path.abspath( "../../../../CMSSW_13_2_0/src/cms_lpc_llp/Run3-HCAL-LLP-NTupler/data/JEC_JER/JECDatabase/jet_veto_maps/Summer23Prompt23/Summer23Prompt23_RunC_v1.root" )	
	jetVeto_2023BPix = os.path.abspath( "../../../../CMSSW_13_2_0/src/cms_lpc_llp/Run3-HCAL-LLP-NTupler/data/JEC_JER/JECDatabase/jet_veto_maps/Summer23BPixPrompt23/Summer23BPixPrompt23_RunD_v1.root" )	

	# ----- Transfer Input Files ----- #
	transfer_input_files = Executable_DisplacedHcalJetAnalyzer + "," + add_scores + "," + keras_depth + "," + keras_inclusive + "," + norm_constants + "," + jetVeto_2022 + "," + jetVeto_2022EE + "," + jetVeto_2023 + "," + jetVeto_2023BPix

    # ----- Make Output Dir ----- #

    if os.path.exists(output_dir):
        print( "WARNING:", output_dir, "already exists! Files might be overwritten" )
    else: 
        os.mkdir( output_dir )

    # ----- Make Submission Dir ----- #

    DirFlag = ""
    if (str(flag) != ""): DirFlag = str(flag) + "_"
    submission_dir = "Jobs_"+DirFlag+datetime_now

    if os.path.exists(submission_dir):
                print( "ERROR:", submission_dir, "already exists. Please use another name. Check if your output directory will be overwritten! Exiting..." )
                quit()

    os.mkdir( submission_dir )
    os.chdir( submission_dir )

    # ----- BDT Weight Files ----- #

    #os.system( "ln -s " + bdt_zipfile + " BDTWeightFiles.zip" )
    #bdt_zipfile_new = os.path.abspath( "BDTWeightFiles.zip" )
    add_scores = os.path.abspath( "../../../Classifiers/ScoresToEventBased-v4.py" )
    keras_depth = os.path.abspath( "../../../Classifiers/inclusive_model_v4_train40.keras" )
    keras_inclusive = os.path.abspath( "../../../Classifiers/inclusive_model_v4_train80.keras" )
    norm_constants = os.path.abspath( "../../../Classifiers/norm_constants_v3.csv" )
    transfer_input_files = Executable_DisplacedHcalJetAnalyzer + "," + add_scores + "," + keras_depth + "," + keras_inclusive + "," + norm_constants

    # ----- Submit Jobs ----- #

    print( "Begin submitting", len(input_list), "jobs...")

    #job_dir = "Job_"+str(ii)
    #os.mkdir(job_dir)
    #os.chdir(job_dir)

    os.system( "cp "+header_cmd+" ./condor_submit.cmd" )
    os.system( "cp "+executable_sh+" ./condor_executable.sh" )
    os.system( "echo '+JobFlavour = \""+flavor+"\"' >> condor_submit.cmd")
    os.system( "echo 'output_destination = root://eosuser.cern.ch/"+output_dir+"/' >> condor_submit.cmd")
    os.system( "echo 'transfer_input_files = "+transfer_input_files+" ' >> condor_submit.cmd" )

    for i in range( len(input_list) ):

        infile_tag = "job"+str(j)
        infile_paths = " ".join(input_list[i])

        os.system("echo 'arguments = "+proxy+"   "+infile_tag+"   "+infile_paths+" ' >> condor_submit.cmd ")
        os.system("echo queue >> condor_submit.cmd")
        os.system("echo >> condor_submit.cmd")

        #print( "Submitting", n_jobs_submitted % max_jobs_per_submission, "jobs to output dir", job_dir )
        if test:
            break

    if setup_only: os.system("echo 'COMMAND: condor_submit condor_submit.cmd'")
    else:          os.system("condor_submit condor_submit.cmd")

    #time.sleep(2.)
    #os.chdir("../")

    print( "Done submitting", len(input_list), "jobs")
    time.sleep(2.)

    quit()

# -------------------------------------------------------------------------------------------------
if __name__ == "__main__":
    main()

