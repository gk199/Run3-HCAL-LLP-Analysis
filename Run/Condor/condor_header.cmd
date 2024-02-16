# Basic
universe                = vanilla
should_transfer_files   = YES
when_to_transfer_output = ON_EXIT_OR_EVICT
notification   		= never

+JobFlavour = "espresso"

# Output
log                     = job$(Process).local.log
output                  = job$(Process).local.out
error                   = job$(Process).local.error

# Executables
executable              = condor_executable.sh

# Things to Add: #########################################################
#transfer_input_files   = 
#output_destination    	= 
#arguments		        = PROXY INFILE_NAME OUTFILE_NAME
#input                  = data_file.root
#queue
##########################################################################

