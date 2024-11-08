import re
import subprocess
import os

# to be run on lxplus
def NTuples_to_Root_sigfiles(input_text_file):
    
    print("here")
    mass_tag_pattern = r"/([A-Za-z0-9_]+_MH[0-9]+_MS[0-9]+_CTau[0-9]+)"
    output_tag_pattern = r"(output_\d+)\.root"
    
    with open(input_text_file, "r") as f: 
        for root_file in f:
            root_file = root_file.strip()  # Removes any leading/trailing whitespace/newlines
            match_1 = re.search(mass_tag_pattern, root_file)
            match_2 = re.search(output_tag_pattern, root_file)
            
            if match_1 and match_2:  # Only proceed if both patterns are found
                m_tag = match_1.group(1)
                out_tag = match_2.group(1)
                print("here 2")
                command = [
                    "root", "-q", "-b", "-l",
                    f'NTupleEventToJetTree2.C("{root_file}", "../Train/Inputs/temp/jetntuple_ggH_HToSSTobbbb_{m_tag}_{out_tag}.root") '
                ]
                
                result = subprocess.run(command, capture_output=True, text=True)
                
                # You might want to check if the command was successful
                if result.returncode != 0:
                    print(f"Command failed for file {root_file} with error: {result.stderr}")
            else:
                print(f"Pattern not found in file: {root_file}")

def NTuples_to_Root_bkgfiles(input_text_file):
    with open(input_text_file, "r") as f: 
        exo_tag = r"\/([A-Za-z0-9\-]+-EXOLLPJetHCAL-PromptReco-v2)"
        #exo_tag = r"\/([A-Za-z0-9\-]+-EXOLLPJetHCAL-PromptReco-v1)"
        output_tag2 = r"(output_\d+)\.root"
        output_tag1 = r"(output_\d+-\d+)\.root"
        
        for root_file in f:
            root_file = root_file.strip()  # remove any surrounding whitespace/newlines
            match_1 = re.search(exo_tag, root_file)
            match_2 = re.search(output_tag1, root_file)
            match_3 = re.search(output_tag2, root_file)
            
            if match_1:
                x_tag = match_1.group(1)
            if match_2:
                out_tag = match_2.group(1)
            elif match_3:
                out_tag = match_3.group(1)
                
            command = [
                "root", "-q", "-b", "-l",
                f'NTupleEventToJetTree2.C("{root_file}", "../Train/Inputs/bkg_temp/jets_{x_tag}_{out_tag}.root")'
            ]
            
            print("here 2")
            result = subprocess.run(command, capture_output=True, text=True)
            
            print("mhm")              
                
def hadd_all_files(output_file, directory):
    # List all files in the directory
    all_files = os.listdir(directory)
    
    # Filter out only ROOT files
    root_files = [os.path.join(directory, f) for f in all_files if f.endswith(".root")]
    
    # Create the hadd command
    command = ["hadd", output_file] + root_files
    
    # Run the command
    result = subprocess.run(command, capture_output=True, text=True)
    
    # Check for success
    if result.returncode != 0:
        print(f"Failed to hadd files: {result.stderr}")
    else:
        print(f"Successfully created {output_file}")
                        
                
if __name__ == "__main__":
    input_text_file = "background.txt"
    NTuples_to_Root_bkgfiles(input_text_file)
    #output_file = "../Train/Inputs/hadded/HADD_Run2023D-EXOLLPJetHCAL-PromptReco-v2.root"
    #output_file = "../Train/Inputs/hadded/HADD_jetntuple_ggH_HToSSTo4B_MH350_MS160_CTau500.root"
    #input_dir = "../Train/Inputs/bkg_temp/"
    #hadd_all_files(output_file, input_dir)
    # then convert to h5 just the hadded file in the command line 
    
