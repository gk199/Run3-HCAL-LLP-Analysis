# Basic
universe                = vanilla
should_transfer_files   = YES
when_to_transfer_output = ON_EXIT_OR_EVICT
notification   		    = never

#request_gpus            = 1

# Espresso: batch 1, microcentury: batch 2, longlunch: displaced jet LLP dataset, workday: 2023Bv1
# +JobFlavour = "espresso" 
# +JobFlavour = "microcentury"
#+JobFlavour = "longlunch"
#+JobFlavour = "workday"

# Output
log                     = job$(Process).local.log
output                  = job$(Process).local.out
error                   = job$(Process).local.error

#stream_output = True
#stream_error  = True

# Executables
executable              = condor_executable.sh

# Things to Add: #########################################################
#transfer_input_files   = 
#output_destination    	= 
#arguments		        = PROXY INFILE_NAME OUTFILE_NAME
#input                  = data_file.root
#queue
##########################################################################

