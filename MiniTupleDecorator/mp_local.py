#!/usr/bin/env python3

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
executable = "root -b -q -l 'AddBDTScoreBranches.C("
print( "EXECUTABLE =", executable )

# ------------------------------------------------------------------------------
def execute( arglist ):

	infile_path_and_tree = arglist[0]

	command = "{} {} {}".format( executable, infile_path_and_tree, " )' ") # root -b -q -l 'AddBDTScoreBranches.C("job2_8_output_97", { "NoSel", "WPlusJets" })'

	print( "Running: ", command )
	os.system( command )

# ------------------------------------------------------------------------------
def multiprocess():
       
	# Using readlines()
	file = open(input_file_path, 'r')
	lines = file.readlines()

	arglist = []
	for line in lines:
		if line[0] == "#": continue

		infile_path_and_tree = line

		arglist.append( [infile_path_and_tree] )	

	NProcesses = NCORES_MAX
	if len(arglist) < NProcesses: 
		NProcesses = len(arglist)

	print("Running over files...")

	pool = mp.Pool(processes=int(NProcesses))   
	pool.map(execute, arglist)

	pool.terminate()

	os.chdir(cwd)

# -------------------------------------------------------------------------------------------------
if __name__ == "__main__":
	multiprocess()
