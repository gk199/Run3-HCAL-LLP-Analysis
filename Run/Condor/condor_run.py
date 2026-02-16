#!/usr/bin/env python
"""
HTCondor Wrapper
===========================
Author: Kiley Kennedy <kiley.elizabeth.kennedy@cern.ch>

Submits condor jobs on lxplus from list of input files

For usage: `python3 condor_run.py --help`
"""

import argparse, sys, os, time
import zipfile
from datetime import datetime

datetime_now = datetime.today().strftime('%Y_%m_%d')

start_time = time.time()
cwd = os.getcwd()

# -------------------------------------------------------------------------------------------------
# MODIFY ME! 

#max_jobs_per_submission = 500

BASE_DIR = cwd.replace("/Run/Condor", "")

header_cmd      = os.path.abspath("condor_header.cmd")
executable_sh   = os.path.abspath("condor_executable.sh")
file_disk_usage = os.path.abspath("../Ntuples_v4/NTuplesV4_DiskUsage.txt")

# Global path to executable (& other files to transfer if applicable) -- comma-separated list without spaces
#transfer_input_files 
Executable_DisplacedHcalJetAnalyzer = os.path.abspath("../../DisplacedHcalJetAnalyzer") #/exe/DisplacedHcalJetAnalyzer") # this is made after compiling 
Executable_ClassifierInputs         = os.path.abspath("../../Classifiers/Evaluate")

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
    parser.add_argument("-u", "--uncert",     action="store", default="Nominal", help="Systematic uncertainty (options: Nominal, None, JER_up, JER_down)")
    parser.add_argument("-p", "--proxy",      action="store", help="Proxy path: generate via `voms-proxy-init -voms cms` and copy file from /tmp/ area", required=True) # TODO
    parser.add_argument("-s", "--setup_only", action="store_true", help="Setup jobs only") 
    parser.add_argument("-t", "--test",       action="store_true", help="Submit test job only") 
    parser.add_argument("-d", "--debug",      action="store_true", help="Debug") 
    parser.add_argument("-f", "--flag",       action="store", default="", help="Flag to append to Job_ directory")

    args = parser.parse_args()

    return args

# -------------------------------------------------------------------------------------------------
def zip_dir(src_path, zip_path):

    if os.path.exists(zip_path): 
        print("Zip file", zip_path, "already exists. Using this (not re-zipping!)" )
        return

    src_path = os.path.abspath(src_path)
    base_name = os.path.basename(src_path)

    with zipfile.ZipFile(zip_path, 'w', zipfile.ZIP_DEFLATED) as zipf:
        for root, dirs, files in os.walk(src_path):
            for file in files:
                full_path = os.path.join(root, file)
                arcname = os.path.join(base_name, os.path.relpath(full_path, src_path))
                zipf.write(full_path, arcname)

# -------------------------------------------------------------------------------------------------
def main():
 
    print( "Gathering Submission Info..." )

    # ----- Get Args ----- #

    args = parseArgs()

    input_file  = args.input_file
    output_dir  = args.output_dir #+ "_" + datetime_now
    # bdt_zipfile = os.path.abspath( args.bdt_zipfile )
    uncert     = args.uncert
    proxy       = os.path.abspath( args.proxy )
    setup_only  = args.setup_only
    test        = args.test
    debug       = args.debug
    flag        = args.flag

    flavor="microcentury"
    #if "LLPskim_2023Cv4" in flag: flavor="workday"
    #if "LLPskim_2023Cv1" in flag: flavor="longlunch"
    #if "LLPskim_2023Vv1" in flag: flavor="longlunch"
    #if "HToSSTo4b" in flag: flavor="longlunch" #microcentury" #espresso"

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
                infile_paths_sizes += 1

            if infile_paths_sizes >= 1:
                input_list.append( infile_paths_temp ) 

                # Reset 
                infile_paths_temp  = []
                infile_paths_sizes = 0

    # ----- Make Output Dir ----- #

    if os.path.exists(output_dir):
        print( "WARNING:", output_dir, "already exists! Files might be overwritten" )
    else: 
        os.mkdir( output_dir )

    # ----- Transfer Input Files ----- #

    Jobs_macro_dir = "Jobs_"+datetime_now
    if not os.path.exists( Jobs_macro_dir ): 
        os.mkdir(Jobs_macro_dir )
    os.chdir( Jobs_macro_dir )

    #os.system( "ln -s " + bdt_zipfile + " BDTWeightFiles.zip" )
    #bdt_zipfile_new = os.path.abspath( "BDTWeightFiles.zip" )

    print("Zipping DisplacedHcalJetAnalyzer input...")
    ZIP_DisplacedHcalJetAnalyzer = os.path.join( cwd, "DisplacedHcalJetAnalyzer.zip")
    zip_dir(Executable_DisplacedHcalJetAnalyzer, ZIP_DisplacedHcalJetAnalyzer )

    print("Zipping Classifiers input...")
    ZIP_Classifier = os.path.join( cwd, "Classifiers.zip")
    zip_dir(Executable_ClassifierInputs, ZIP_Classifier )    

    #transfer_input_files = Executable_DisplacedHcalJetAnalyzer + "," + add_scores + "," + keras_depth + "," + keras_inclusive + "," + norm_constants
    transfer_input_files = ZIP_DisplacedHcalJetAnalyzer  + "," + ZIP_Classifier

    # ----- Make Submission Dir ----- #

    #DirFlag = ""
    #if (str(flag) != ""): DirFlag = str(flag) + "_"
    submission_dir = "Job_" + str(flag)

    if os.path.exists(submission_dir):
        print( "ERROR:", submission_dir, "already exists. Please use another name. Check if your output directory will be overwritten! Exiting..." )
        quit()

    os.mkdir( submission_dir )
    os.chdir( submission_dir )

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

        infile_tag = "job"+str(i)
        infile_paths = " ".join(input_list[i])

        os.system("echo 'arguments = "+BASE_DIR+"   "+proxy+"    "+infile_tag+"   "+uncert+"   "+infile_paths+" ' >> condor_submit.cmd ")
        os.system("echo queue >> condor_submit.cmd")
        os.system("echo >> condor_submit.cmd")

        #print( "Submitting", n_jobs_submitted % max_jobs_per_submission, "jobs to output dir", job_dir )
        if test:
            break

    if setup_only: os.system("echo 'COMMAND: condor_submit condor_submit.cmd'")
    else:          os.system("condor_submit condor_submit.cmd")

    #time.sleep(2.)
    #os.chdir("../")

    os.chdir( cwd )
    print( "Done submitting", len(input_list), "jobs")
    time.sleep(2.)

    quit()

# -------------------------------------------------------------------------------------------------
if __name__ == "__main__":
    main()

