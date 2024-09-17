# Kat's Branch -- Classifiers

**1. runner.py**
Self-contained script that can train (and save the model), evaluate, and write classifier scores to root files (which also includes previous information in the branch). The script has a binary and multi-class mode. The normalization constants are stored in the norm_constants.csv file. For now, only the writing of multi-class scores to root files is implemented.

**2. requirements.txt**
List of all packages within the environment used in SWAN to train and test the models.

**3. keras models**
Uploaded some trained .keras models; dense_binary is for binary classification, dense_model should be for multi-classification. The runner.py script handles these models in the 'eval' and 'filewrite' modes.
