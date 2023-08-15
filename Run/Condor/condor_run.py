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

max_jobs_per_submission = 24

header_cmd    = os.path.abspath("condor_header.cmd")
executable_sh = os.path.abspath("condor_executable.sh")

# Global path to executable (& other files to transfer if applicable) -- comma-separated list without spaces
transfer_input_files = os.path.abspath("../../DisplacedHcalJetAnalyzer/exe/DisplacedHcalJetAnalyzer") # this is made after compiling 

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
	parser.add_argument("-p", "--proxy",      action="store", help="Proxy path: generate via `voms-proxy-init -voms cms` and copy file from /tmp/ area", required=True) # TODO
	parser.add_argument("-s", "--setup_only", action="store_true", help="Setup jobs only") 
	parser.add_argument("-t", "--test",       action="store_true", help="Submit test job only") 
	parser.add_argument("-d", "--debug",      action="store_true", help="Debug") 

	args = parser.parse_args()

	return args

# -------------------------------------------------------------------------------------------------
def main():
 
	print( "Gathering Submission Info..." )

	# ----- Get Args ----- #

	args = parseArgs()

	input_file 	= args.input_file
	output_dir 	= args.output_dir + "_" + datetime_now
	proxy 		= os.path.abspath( args.proxy )
	setup_only 	= args.setup_only
	test 		= args.test
	debug 		= args.debug

	# ----- Get Inputs ----- #

	if not os.path.isfile(input_file): 
		print( "ERROR:", input_file, " does not exist. Exiting..." )
		quit()

	input_list = []
	with open(input_file) as f:
		for line in f.readlines():
			line_temp = line.replace(" ","").strip()
			if line_temp[0] == "#": continue

			infile_path = line_temp
			infile_tag  = line_temp.split("/")[-1].replace(".root","")
			input_list.append( [infile_tag, infile_path] )	

			if debug: print( "Reading in:", [infile_tag, infile_path] )

	# ----- Make Output Dir ----- #

	if os.path.exists(output_dir):
		print( "WARNING:", output_dir, "already exists! Files might be overwritten" )
	else: 
		os.mkdir( output_dir )

    # ----- Make Submission Dir ----- #

	submission_dir = "Jobs_"+datetime_now

	if os.path.exists(submission_dir):
                print( "ERROR:", submission_dir, "already exists. Please use another name. Check if your output directory will be overwritten! Exiting..." )
                quit()

	os.mkdir( submission_dir )
	os.chdir( submission_dir )

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
		os.system( "echo 'output_destination = root://eosuser.cern.ch/"+output_dir+"/' >> condor_submit.cmd")
		os.system( "echo 'transfer_input_files = "+transfer_input_files+" ' >> condor_submit.cmd" )

		n_jobs_submitted_temp = n_jobs_submitted
		for i in range(max_jobs_per_submission):

			if n_jobs_submitted >= len(input_list): break

			j = n_jobs_submitted_temp + i
			infile_tag = "job"+str(ii)+"_"+str(i)+"_"+input_list[j][0]
			infile_path = input_list[j][1]

			os.system("echo 'arguments = "+proxy+"   "+infile_tag+"   "+infile_path+"' >> condor_submit.cmd ")
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

