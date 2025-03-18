import sys, os, argparse, time, errno
import numpy as np
from datetime import datetime
import matplotlib.pyplot as plt
import re


#from array import array
#import matplotlib.pyplot as plt

debug = False


# ------------------------------------------------------------------------------
def process_log_files( directory ):

	run_times = []

	# Walk through the directory structure
	for root, _, files in os.walk( directory ):

		for file in files:
			if file.endswith(".log"):
				log_file = os.path.join(root, file)

				#print(f"Processing: {log_file}")
				
				# Example: Read and print first line of each log file
				try:
					with open(log_file, 'r') as f:
						time_start = 0
						time_stop = 0
						for line in f:
							match_start = re.search(r"(\d{2}/\d{2}) (\d{2}:\d{2}:\d{2}) Job executing on host", line)
							match_stop  = re.search(r"(\d{2}/\d{2}) (\d{2}:\d{2}:\d{2}) Job terminated", line)

							if match_start:
								#print(f"Execution Timestamp: {match_start.group(1)}")
								time_start = exec_time = datetime.strptime(match_start.group(1) + " " + match_start.group(2), "%m/%d %H:%M:%S")
	

							if match_stop:
								time_stop  = datetime.strptime(match_stop.group(1) + " " + match_stop.group(2), "%m/%d %H:%M:%S")

							
						time_diff = (time_stop - time_start).total_seconds() / 60
						print(f"Run time: {time_diff:.2f} min for", log_file)

						run_times.append(time_diff)

				except Exception as e:
					print(f"Error reading {log_file}: {e}")

	return run_times

# ------------------------------------------------------------------------------
def main():

	run_times_all = []

	for directory in sys.argv[1:]:
		run_times = process_log_files(directory)
		
		for run_time in run_times:
			run_times_all.append(run_time)



	plt.figure(figsize=(8, 6))
	plt.hist(run_times) #, bins=bins, edgecolor='black', alpha=0.7)
	plt.title('Minituple Job Run Time')
	plt.xlabel('Run time [min]')
	plt.ylabel('Number of Jobs')
	#plt.grid(axis='y', linestyle='--', alpha=0.7)
	plt.show()



# ------------------------------------------------------------------------------
if __name__ == '__main__':
	main()

