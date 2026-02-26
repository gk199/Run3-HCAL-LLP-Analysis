import numpy as np
import tensorflow as tf
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense, Conv1D, Flatten, Dropout, MaxPooling1D, BatchNormalization, AveragePooling1D
from tensorflow.keras.optimizers import Nadam, Adam
import pandas as pd
import matplotlib.pyplot as plt
import uproot

from sklearn.model_selection import train_test_split
from sklearn.preprocessing import MinMaxScaler
from sklearn.utils import shuffle
from itertools import combinations
from sklearn.metrics import roc_curve, auc
import csv
import fnmatch

import sys, os, argparse, time, errno
import os.path

import gc

perJet = False
num_jets = 1 if perJet else 4 # in v3.13 only 4 jets are saved! In earlier versions, 6 jets are saved

class DataProcessor:
    def __init__(self, num_classes=2, mode=None, sel=True, tree="NoSel", constants=None): #counting from 0 
        self.return_value_bkg = num_classes
        self.return_sig_value = num_classes - 1
        self.num_classes = num_classes
        self.mode = mode
        self.tree = tree
        self.sel = sel
        self.constants = constants
    
    def load_data(self, input_file=None):

        print("Loading Data (chunked)...")

        filter_name = "*"

        sig_tree = uproot.open(input_file)[self.tree]

        # Branch filtering (same as original code)
        include_patterns = ["*"]
        exclude_patterns = ["*_rechit_*", "HLT*"]

        all_branches = sig_tree.keys()

        included = set()
        for pattern in include_patterns:
            included.update(fnmatch.filter(all_branches, pattern))

        for pattern in exclude_patterns:
            included.difference_update(fnmatch.filter(all_branches, pattern))

        filter_name = sorted(included)

        df_chunks = []

        # Instead of loading full file, stream in chunks
        for chunk in uproot.iterate(
            f"{input_file}:{self.tree}",
            filter_name,
            library="pd",
            step_size=100000
        ):
            df_chunks.append(chunk)

        self.df = pd.concat(df_chunks)
        del df_chunks
        gc.collect()
        print("Loaded Data")

    def no_selections_concatenate(self):
        self.cumulative_df = self.df
        print("-------------------All Data // No Cuts applied---------")
        print(self.cumulative_df.describe())

    def process_data(self):
        
        print("Processing...")

        features = ['perJet_Eta', 'perJet_Mass', 
       'perJet_S_phiphi', 'perJet_S_etaeta', 'perJet_S_etaphi', 
       'perJet_Tracks_dR', 
       'perJet_Track0dR', 'perJet_Track0dEta', 'perJet_Track0dPhi', 
       'perJet_Track1dR', 'perJet_Track1dEta', 'perJet_Track1dPhi',
       'perJet_Track2dR', 'perJet_Track2dEta', 'perJet_Track2dPhi',
       'perJet_Frac_Track0Pt', 'perJet_Frac_Track1Pt', 'perJet_Frac_Track2Pt',
       'perJet_EnergyFrac_Depth1', 'perJet_EnergyFrac_Depth2', 'perJet_EnergyFrac_Depth3', 'perJet_EnergyFrac_Depth4', 
       'perJet_LeadingRechitD', 
       'perJet_Frac_LeadingRechitE', 'perJet_Frac_SubLeadingRechitE', 'perJet_Frac_SSubLeadingRechitE', 
       'perJet_AllRechitE', 
       'perJet_NeutralHadEFrac', 'perJet_ChargedHadEFrac', 'perJet_PhoEFrac', 'perJet_EleEFrac', 'perJet_MuonEFrac']

        all_normed_data = []
        all_labels = []
        all_jet_valid = []

        for jet in range(num_jets): 
            if not perJet:
                new_features = [i.replace('perJet','jet'+str(jet)) for i in features] # list comprehension 
                feature_dictionary = dict(zip(new_features, features))
            else: new_features = features
        
            labels = None 
            if not self.mode and self.sel:
                self.cumulative_df = self.cumulative_df.sample(frac=1, random_state=42).reset_index(drop=True) # shuffling
                labels = self.cumulative_df["classID"].values
            
            data = self.cumulative_df[new_features]
            print(data.describe())
            jet_valid = self.cumulative_df["jet"+str(jet)+"_E"].values
            jet_trained_on = self.cumulative_df
            # in data, would like to rename jetX to perJet to work with the input files
            if not perJet: data = data.rename(columns=feature_dictionary)

            normed_data = pd.DataFrame()
            for feature in data.columns:
                # giving data a nominal 0 value if bad, but scores are -9999 if jet is not valid
                mask_condition = (data[feature] <= -900) | (data[feature] >= 900)
                data.loc[mask_condition, feature] = 0  # Default to 0 for matching entries
                normed_data[feature] = (data[feature] - self.constants[feature][0])/ self.constants[feature][1]

            print(normed_data.describe())
        
            print("Processing Complete")

            all_normed_data.append(normed_data)
            all_labels.append(labels)
            all_jet_valid.append(jet_valid)
        
        # return normed data, labels, and a list of rows are ok (energy is positive) and which are not (energy is -9999.9)
        return all_normed_data, all_labels, all_jet_valid
    
    def default_variables(self, jet_index = 0):
        # assumes that you have already loaded the files and applied safety selections
        # for manual input of other useful variables not included in safety selection:
        useful_variables = ['perJet_Track0dzToPV', 'perJet_Track0dzOverErr', 'perJet_Track0dxyToBS', 'perJet_Track0dxyOverErr',
                            'perJet_Track1dzToPV', 'perJet_Track1dzOverErr', 'perJet_Track1dxyToBS', 'perJet_Track1dxyOverErr', 
                            'perJet_Track2dzToPV', 'perJet_Track2dzOverErr', 'perJet_Track2dxyToBS', 'perJet_Track2dxyOverErr']
        # rename to jet i 
        useful_variables = [i.replace('perJet','jet'+str(jet_index)) for i in useful_variables] # list comprehension
        for useful_variable in useful_variables:
            data = self.cumulative_df[[useful_variable]].copy()
            mask_condition = (data[useful_variable] <= -900) | (data[useful_variable] >= 900)
            data.loc[mask_condition, useful_variable] = 0  # Default to 0 for matching entries         
        
class ModelHandler:
    def __init__(self, num_classes=3, num_layers=3, optimizer="adam", lr=0.00027848106048644665, model_name="model.keras"):
        
        self.num_classes = num_classes
        self.num_layers = num_layers
        self.model_name = model_name
        self.colors = ['red', 'blue', 'green']
        
        self.names = ["HCAL12", "HCAL34", "bkg"] if num_classes == 3 else ["HCAL", "bkg"]
    
        if optimizer == "adam":
            self.optimizer = tf.keras.optimizers.Adam(learning_rate=lr)
        elif optimizer == 'adamax':
            self.optimizer = tf.keras.optimizers.Adamax(learning_rate=lr)
        elif optimizer == 'nadam':
            self.optimizer = tf.keras.optimizers.Nadam(learning_rate=lr)
        else:
            self.optimizer= tf.keras.optimizers.SGD(learning_rate=lr)    
            
    def get_model(self):
        return self.model
        
    def load(self):
        self.model = tf.keras.models.load_model(self.model_name)
    
    def test(self, X_test, y_test):
        preds = self.model.predict(X_test)
        predictions = np.argmax(preds, axis=1)
        accuracy = np.sum(predictions == y_test) / len(y_test)
        print("Total Test Samples: ", len(y_test))
        print("Accuracy: ", accuracy)
        
        self.roc_data = (y_test, preds)
        return self.roc_data
    
    def predict(self, predicting_data, labels=None):
        preds = self.model.predict(predicting_data, batch_size=4096, verbose=0)
        if labels is not None:
            print("Accuracy: ", np.sum(np.argmax(preds, axis=1) == labels) / len(labels) )
        return preds
    
    def save_roc_data(self, fpr, tpr):
        roc_data = zip(fpr, tpr)
        with open("roc_binary_data.csv", "w", newline='') as f:
            writer = csv.writer(f)

            # Write the header
            writer.writerow(["FPR", "TPR"])

            # Write all rows at once
            writer.writerows(roc_data)
            print("wrote to file")  

class Runner:
    def __init__(self, input_file=None, filepath=None, mode="train", num_classes=3, inclusive=False, load=True, tree="NoSel", depth_model_keras="depth_model.keras", incl_model_keras="inclusive_model.keras", constants=None):
        self.mode = mode
        self.num_classes = num_classes
        self.inclusive = inclusive
        self.load = load
        self.sig = input_file
        #self.path = filepath
        self.tree = tree

        self.depth_model_keras = depth_model_keras
        self.incl_model_keras  = incl_model_keras
        self.constants         = constants
    
    def run_file_evaluation(self):

        print("Evaluating Single File (streaming)...")

        self.processor = DataProcessor(
            num_classes=self.num_classes - 1,
            mode="filewrite",
            sel=False,
            tree=self.tree,
            constants=self.constants
        )

        # Load models once
        depth_handler = ModelHandler(
            num_classes=self.num_classes,
            model_name=self.depth_model_keras
        )
        depth_handler.load()

        incl_handler = ModelHandler(
            num_classes=self.num_classes,
            model_name=self.incl_model_keras
        )
        incl_handler.load()

        output_filename = f"{self.sig[:-5]}_{self.tree}_scores.root"

        first_write = True
        outfile = None
        outtree = None

        # Branch filtering
        with uproot.open(self.sig) as f:
            branches = f[self.tree].keys()

        include_patterns = ["*"]
        exclude_patterns = ["*_rechit_*", "HLT*"]

        included = set()
        for p in include_patterns:
            included.update(fnmatch.filter(branches, p))

        for p in exclude_patterns:
            included.difference_update(fnmatch.filter(branches, p))

        filter_name = sorted(included)

        # streaming looping over file in chunks, processing each chunk, and writing out scores immediately to avoid memory issues. Note that the model is loaded once at the beginning and then used for all chunks. Also note that the output file is created and schema is written on the first chunk, and then subsequent chunks just update the tree with new data. This way we never have to hold more than one chunk of data in memory at a time, along with the model itself.
        for batch in uproot.iterate(
            f"{self.sig}:{self.tree}",
            expressions=filter_name,
            library="pd",
            step_size=100000
        ):

            # Process + normalize
            self.processor.cumulative_df = batch
            predicting_data, labels, jet_valid = self.processor.process_data()

            preds = []
            preds_inc = []

            for i in range(num_jets):
                preds.append(
                    depth_handler.model.predict(predicting_data[i].values, batch_size=4096, verbose=0)
                )
                preds_inc.append(
                    incl_handler.model.predict(predicting_data[i].values, batch_size=4096, verbose=0)
                )

            dataframe = batch.copy()
            dataframe = dataframe.reset_index(drop=True)

            # Add scores
            for i in range(num_jets):
                dataframe[f'jet{i}_scores_depth_LLPanywhere'] = preds[i][:,0]
                dataframe[f'jet{i}_scores_inc_train80'] = preds_inc[i][:,0]

            # Convert to ROOT-safe numpy arrays
            output_dict = {}

            for col in dataframe.columns:

                arr = dataframe[col].to_numpy()

                # # # Convert to safe numeric types 
                if arr.dtype == object:
                    # check if this is a string like "era"
                    if len(arr) > 0 and isinstance(arr[0], str):
                        arr = arr.astype("U") # preserve root string branch
                    else:
                        arr = pd.to_numeric(arr, errors="coerce").astype("float32")

                output_dict[col] = arr

            # Write schema once
            if first_write:

                outfile = uproot.recreate(output_filename)

                branch_types = {}
                for k, v in output_dict.items():
                    if isinstance(v[0], str):
                        branch_types[k] = "string"
                    else:
                        branch_types[k] = np.asarray(v).dtype

                outtree = outfile.mktree(self.tree, branch_types)

                first_write = False

            outtree.extend(output_dict)

            # VERY IMPORTANT — force memory release
            del batch
            del dataframe
            del predicting_data
            del preds
            del preds_inc

        if outfile is not None:
            outfile.close()

        print("Wrote:", output_filename)
        
    def run(self):
        if self.mode == "train":
            self.run_training()
        elif self.mode == "eval":
            self.run_evaluation()
        elif self.mode == "filewrite":
            self.run_file_evaluation()
        else:
            print("No Valid Mode Entered")
            
    def set_inclusive(self, inclusive=False):
        self.inclusive = inclusive
        return
    
    def set_load(self,load=True):
        self.load = load
    
    #def set_model_name(self, model_name="dense_model_v4.keras"):
    #    self.model_name = model_name
        
def parseArgs():
    """ Parse command-line arguments
    """
    parser = argparse.ArgumentParser(
        add_help=True,
        description=''
    )

    parser.add_argument("-f", "--input_file", action="store", help="Input file (.root)", required=True) #nargs='+'
    parser.add_argument("-t", "--tree",       action="store", default="NoSel", help="Tree name")
    
    parser.add_argument("-d", "--depth",      action="store", default="depth_model_v4_LLPanywhere.keras", help="Depth model (.keras)")
    parser.add_argument("-i", "--inclusive",  action="store", default="inclusive_model_v4_train80.keras", help="Inclusive model (.keras)")
    parser.add_argument("-c", "--constants",  action="store", default="norm_constants_v4.csv", help="Norm constants file (.csv)")
    
    parser.add_argument("-m", "--mode",       action="store", default="filewrite", help="Running mode (train, eval, filewrite)")

    args = parser.parse_args()

    return args        
        
def main():

    print("Parsing Arguments...")

    args = parseArgs()

    input_file        = args.input_file
    input_tree        = args.tree
    depth_model_keras = args.depth
    incl_model_keras  = args.inclusive
    normconstants_csv = args.constants

    constants = pd.read_csv(normconstants_csv)

    #filepath = input_file

    mode = args.mode #"filewrite", "eval"
    
    print("Running Depth and Inclusive Tagger over each file")
    print("Infile(s):", input_file)


    with uproot.open(input_file) as file:
        # Access the tree
        tree = file[input_tree]
        # Get the number of entries
        num_entries = tree.num_entries
        print(input_tree)
        print(num_entries)
        if (num_entries > 0):
            runner = Runner(input_file=input_file, mode=mode, num_classes=2, inclusive=False, tree=input_tree, depth_model_keras=depth_model_keras, incl_model_keras=incl_model_keras, constants=constants)
            runner.run()
    
if __name__ == "__main__":
    main()
