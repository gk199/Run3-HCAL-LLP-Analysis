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

max_jobs_per_submission = 100

header_cmd    = os.path.abspath("condor_header.cmd")
executable_sh = os.path.abspath("condor_executable.sh")

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
    parser.add_argument("-n", "--nperjob",     action="store", default=1, help="Number of files to run per job")
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
    files_per_job = int(args.nperjob)

    # ----- Get Inputs ----- #

    if not os.path.isfile(input_file): 
        print( "ERROR:", input_file, " does not exist. Exiting..." )
        quit()

    infile_path_list = []

    i = 0
    input_list = []
    input_list_temp = []
    with open(input_file) as f:
        for line in f.readlines():
            line_temp = line.replace(" ","").strip()
            if line_temp[0] == "#": continue

            if i % files_per_job == 0:
                if i != 0: input_list.append( input_list_temp ) 
                input_list_temp = []

            input_list_temp.append( line_temp )
            i += 1

            if debug: print( "Reading in:", infile_path )

    input_list.append( input_list_temp )  

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
    add_scores = os.path.abspath( "../../../Classifiers/ScoresToEventBased-v3.py" )
    keras_depth = os.path.abspath( "../../../Classifiers/depth_model_v3_Oct15.keras" )
    keras_inclusive = os.path.abspath( "../../../Classifiers/inclusive_model_v3_Oct15.keras" )
    norm_constants = os.path.abspath( "../../../Classifiers/norm_constants_v3.csv" )
    transfer_input_files = Executable_DisplacedHcalJetAnalyzer + "," + add_scores + "," + keras_depth + "," + keras_inclusive + "," + norm_constants

    # ----- Submit Jobs ----- #

    print( "Begin submitting", len(input_list), "jobs...")

    n_jobs_submitted = 0
    ii = -1
    while n_jobs_submitted < len(input_list):

        ii += 1

        job_dir = "Job_"+str(ii)
        os.mkdir(job_dir)
        os.chdir(job_dir)

        os.system( "cp "+header_cmd+" ./condor_submit.cmd" )
        os.system( "cp "+executable_sh+" ./condor_executable.sh" )

        # Output
        os.system( "echo ' '  >> condor_submit.cmd")
        os.system( "echo '# Output'  >> condor_submit.cmd")
        os.system( "echo 'log    = job"+str(ii)+"_$(Process).local.log' >> condor_submit.cmd")
        os.system( "echo 'output = job"+str(ii)+"_$(Process).local.output' >> condor_submit.cmd")
        os.system( "echo 'error  = job"+str(ii)+"_$(Process).local.error' >> condor_submit.cmd")

        os.system( "echo ' '  >> condor_submit.cmd")
        os.system( "echo 'x509userproxy = "+proxy+"' >> condor_submit.cmd")
        os.system( "echo 'use_x509userproxy = True' >> condor_submit.cmd")
        os.system( "echo ' '  >> condor_submit.cmd")

        os.system( "echo 'output_destination = root://eosuser.cern.ch/"+output_dir+"/' >> condor_submit.cmd")
        os.system( "echo ' ' >> condor_submit.cmd") 
        os.system( "echo 'transfer_input_files = "+transfer_input_files+" ' >> condor_submit.cmd" )

        n_jobs_submitted_temp = n_jobs_submitted
        for i in range(max_jobs_per_submission):

            if n_jobs_submitted >= len(input_list): break

            j = n_jobs_submitted_temp + i
            infile_tag = "job"+str(ii)+"_"+str(i)+"_"+input_list[j][0]

            infile_paths_str = ""
            for path in input_list[j]: infile_paths_str += path + "   "

            os.system("echo 'arguments = "+proxy+"   "+infile_tag+"   "+infile_paths_str+"/' >> condor_submit.cmd ")
            os.system("echo queue >> condor_submit.cmd")
            os.system("echo >> condor_submit.cmd")

            n_jobs_submitted += 1

        print( "Submitting", n_jobs_submitted % max_jobs_per_submission, "jobs to output dir", job_dir )

        if setup_only: os.system("echo 'COMMAND: condor_submit condor_submit.cmd'")
        else:          os.system("condor_submit condor_submit.cmd")

        time.sleep(2.)
        os.chdir("../")

        if test:
            break

    print( "Done submitting", n_jobs_submitted, "jobs")

    quit()

# -------------------------------------------------------------------------------------------------
if __name__ == "__main__":
    main()

