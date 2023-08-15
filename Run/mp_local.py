#!/usr/bin/env python

# -------------------------------------------------------------------------------------------------
# Purpose: 
#   Run over input files in parallel. Input file contains paths to files to run over
#
# Usage:
#   $ ./mp_local.py <path_to_input_file>
# -------------------------------------------------------------------------------------------------

import sys, os, time
from datetime import datetime
import multiprocessing as mp
start_time = time.time()

NCORES = mp.cpu_count()
NCORES_MAX = NCORES - 2

input_file_path = sys.argv[1]

if len(sys.argv) > 2:
	NCORES_MAX = sys.argv[2]

print("NCORES =", NCORES)
print("SETTING NCORES_MAX =", NCORES_MAX)

cwd = os.getcwd()
executable = os.path.join( cwd, "./../DisplacedHcalJetAnalyzer/exe/DisplacedHcalJetAnalyzer" )
print( "EXECUTABLE =", executable )

# ------------------------------------------------------------------------------
def execute( arglist ):

	infile_tag = arglist[0]
	infile_path = arglist[1]

	command = "{} {} {}".format( executable, infile_tag, infile_path)

	print( "Running: ", command )
	os.system( command )

# ------------------------------------------------------------------------------
def multiprocess():
       
	# Using readlines()
	file = open(input_file_path, 'r')
	lines = file.readlines()

	arglist = []
	for line in lines:
		line_temp = line.replace(" ","").strip()
		if line_temp[0] == "#": continue

		infile_path = line_temp
		infile_tag  = line_temp.split("/")[-1].replace(".root","")

		arglist.append( [infile_tag, infile_path] )	

	NProcesses = NCORES_MAX
	if len(arglist) < NProcesses: 
		NProcesses = len(arglist)

	# Output dir

	datetime_now = datetime.today().strftime('%Y_%m_%d') #_%H-%M-%S')
	output_dir = "/eos/user/g/gkopp/LLP_Analysis/output_minituples_"+datetime_now+"_"+input_file_path.split("/")[-1].split(".")[0]

	try: os.mkdir( output_dir )
	except OSError: print( "NOTE: Overwriting files in", output_dir )

	os.chdir( output_dir )

	print("Running over files...")

	pool = mp.Pool(processes=int(NProcesses))   
	pool.map(execute, arglist)

	pool.terminate()

	os.chdir(cwd)

# -------------------------------------------------------------------------------------------------
if __name__ == "__main__":
	multiprocess()
