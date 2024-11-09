# Kat's Branch -- Classifiers

**1. runner.py**
Self-contained script that can train (and save the model), evaluate, and write classifier scores to root files (which also includes previous information in the branch). The script has a binary and multi-class mode. The normalization constants are stored in the norm_constants.csv file. For now, only the writing of multi-class scores to root files is implemented.

**2. requirements.txt**
List of all packages within the environment used in SWAN to train and test the models.

**3. keras models**
Uploaded some trained .keras models; dense_binary is for binary classification, dense_model should be for multi-classification. The runner.py script handles these models in the 'eval' and 'filewrite' modes.

**4. runner-V2.py**
No longer need separate train and test files. Added running of inclusive tagger within the same run of the script as the depth tagger. First run in train mode to obtain the models: see relevant section of main() and uncomment the relevant section. Remember to modify the name for each model. Eval and filewrite mode: need to add method of sequentially running this too (soon). 

**5. All other images**
Currently having issues with opening up plots on SWAN and Lxplus, so temporary solution is to visualise them on GitHub. This can be ignored.

**6. running.py**
This is the main script for training and evaluating the 3DCNN using the 3D jet images in the form of .h5 files. This code is originally ran on SWAN using a GPU. The code is designed to train and evaluate the 3DCNN models imported from the models.py script, and can plot metrics such as the ROC curve. To run, select the train option first so that the model is trained and saved in.keras format, and which can then be evaluated. If the file paths are changed, the code should run as it is -- in the current configuration to train a 3DCNN, save it, and plot the ROC curve.
