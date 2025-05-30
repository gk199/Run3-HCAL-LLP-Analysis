import numpy as np
import tensorflow as tf
from tensorflow.keras.callbacks import ModelCheckpoint
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense, Conv1D, Flatten, Dropout, MaxPooling1D, BatchNormalization, AveragePooling1D
from tensorflow.keras.optimizers import Nadam, Adam
import pandas as pd
import matplotlib.pyplot as plt

import uproot
import awkward as ak
import h5py
import gc
import fnmatch

from sklearn.model_selection import train_test_split
from sklearn.preprocessing import MinMaxScaler
from sklearn.utils import shuffle
from itertools import combinations
from sklearn.metrics import roc_curve, auc
import csv
from sklearn.feature_selection import mutual_info_regression
from tensorflow.keras.callbacks import EarlyStopping
from tensorflow.keras.initializers import GlorotUniform, GlorotNormal
from tensorflow.keras import layers, models

tf.random.set_seed(311)

CONSTANTS = pd.read_csv("norm_constants_v3.csv") # large negative values removed from mean / std dev computation 
      
FEATURES = ['perJet_Eta', 'perJet_Mass', 
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

debug_mode = False

class DataProcessor:
    def __init__(self, num_classes=2, mode=None, sel=True): # counting from 0
        # this function is passed num_classes - 1 = 1. value_bkg = 1, sig_value = 0
        self.return_value_bkg = num_classes
        self.return_sig_value = num_classes - 1
        self.num_classes = num_classes
        self.mode = mode
        self.sel = sel
    
    def load_data(self, sig_files=None, bkg_files=None, step_size=100000):
        # Load the signal and background data. Do this in batches, otherwise the large root files cause memory issues
        print("Loading Data in Batches with uproot.iterate...")
        print("Filtering and saving data to HDF5 for DNN training...")
        
        tree_name = "PerJet_NoSel"
        filepath = '/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.14/'
        sig_fps = [filepath + filename for filename in sig_files] if sig_files is not None else []
        bkg_fps = [filepath + filename for filename in bkg_files] if bkg_files is not None else []

        with uproot.open(f"{sig_fps[0]}:{tree_name}") as tree:
            all_branches = tree.keys()
        include_patterns = ["perJet_*", "Pass_WPlusJets", "Pass_HLTDisplacedJet", "Pass_InclTagCand", "Pass_DepthTagCand"]
        exclude_patterns = ["perJet_rechit*"] # these have type "awkward" which doesn't work with h5 format
        # Match includes
        included = set()
        for pattern in include_patterns:
            included.update(fnmatch.filter(all_branches, pattern))
        # Remove excluded matches
        for pattern in exclude_patterns:
            to_remove = fnmatch.filter(all_branches, pattern)
            included.difference_update(to_remove)
        # Convert to sorted list for consistency
        filter_name = sorted(included)

        # Initialize output HDF5 files
        with h5py.File("signal.h5", "w") as sig_h5f, h5py.File("background.h5", "w") as bkg_h5f:
            # These datasets will be extendable
            sig_data = None
            bkg_data = None

            total_signal = 0
            total_background = 0

            # Signal
            for chunk in uproot.iterate(
                [f"{file}:{tree_name}" for file in sig_fps], 
                filter_name=filter_name, 
                library="pd", 
                step_size=step_size,
            ):

                # Apply event-level filter here
                chunk_filtered = chunk[chunk["Pass_HLTDisplacedJet"]]
                print(f"Loaded signal chunk of size: {len(chunk)} entries, and {len(chunk_filtered)} entries passed HLT displaced jet selection")
                total_signal += len(chunk_filtered)
                if not chunk_filtered.empty:
                    if debug_mode: 
                        with pd.option_context('display.max_rows', None, 'display.max_columns', None): print(chunk_filtered.dtypes) # debugging which column is badly behaved
                    data = chunk_filtered.to_records(index=False)

                    if sig_data is None:
                        sig_data = sig_h5f.create_dataset(
                            "features", data=data, maxshape=(None,), chunks=True
                        )
                    else:
                        sig_data.resize((sig_data.shape[0] + len(data),))
                        sig_data[-len(data):] = data

                    del chunk_filtered, data
                    gc.collect()

                # sig_df.append(chunk_filtered)

        # self.sig_df = pd.concat(sig_df) if sig_df else pd.DataFrame()
        # print(f"Concatenated all signal entries, total length {total_signal}")

            # Background
            for chunk in uproot.iterate(
                [f"{file}:{tree_name}" for file in bkg_fps], 
                filter_name=filter_name, 
                library="pd", 
                step_size=step_size,
            ):
                # Apply event-level filter here
                chunk_filtered = chunk[chunk["Pass_WPlusJets"]] 
                print(f"Loaded background chunk of size: {len(chunk)} entries, and {len(chunk_filtered)} entries passed W+jets selection")
                total_background += len(chunk_filtered)
                if not chunk_filtered.empty:
                    data = chunk_filtered.to_records(index=False)

                    if bkg_data is None:
                        bkg_data = bkg_h5f.create_dataset(
                            "features", data=data, maxshape=(None,), chunks=True
                        )
                    else:
                        bkg_data.resize((bkg_data.shape[0] + len(data),))
                        bkg_data[-len(data):] = data

                    del chunk_filtered, data
                    gc.collect()
                # bkg_df.append(chunk_filtered)

        print(f"Saved {total_signal} signal and {total_background} background entries to HDF5 (combined number of samples is {total_signal + total_background})")
        print("Loaded Data")

    def apply_selections(self, inclusive=False):
        print("Applying Selections")
        bkg_value = self.return_value_bkg
        sig_value = self.return_sig_value
        
        # applying selection to remove Null values/ maxed out values/ etc.
        def select_LLPjet(row):
            return row['perJet_isTruthMatched'] == 1 # returns 1 if this jet is matched to an LLP or decay product, 0 otherwise
        def select_L1jet(row):
            return row['perJet_L1trig_Matched'] > 0 # returns 1 if jet matched to L1 hwQual-set jet, 0 if not (hwQual not set, or not matched to L1). Now included in depth(row) function instead

        def select_HCAL12(row):
            return 177 <= row['perJet_MatchedLLP_DecayR'] < 214.2 and abs(row['perJet_MatchedLLP_Eta']) < 1.26
        def select_HCAL34(row):
            return 214.2 <= row['perJet_MatchedLLP_DecayR'] < 295 and abs(row['perJet_MatchedLLP_Eta']) < 1.26

        # only consider events that pass the displaced jet HLT (Pass_HLTDisplacedJet) -- both the depth and inclusive tag canidates already require this
        # use "Pass_DepthTagCand" or "Pass_InclTagCand", but still require L1 trig matched for the background
        def depth(row):
            return (row['Pass_DepthTagCand'] and abs(row['perJet_Eta']) < 1.26 and row['perJet_Pt'] > 60)
            # returns 1 if this jet is a depth tag candidate (leading L1 LLP hwQual matched jet + kinematic requirements), 2+ depth emulated towers, eta < 1.26, and pT > 60
            # TODO need to add in that this must be a depth candidate in the CR -- sequential trainings. 
            # If scores are added, need to check the score of the one that Pass_InclTagCand -- not how per jet tree is setup though...
        def inclusive_bkg(row):
            return (row['Pass_DepthTagCand'] == 0 and row['Pass_HLTDisplacedJet']) # not depth tag candidate, but must pass HLT
        def inclusive_signal(row):
            return row['Pass_InclTagCand'] # returns 1 if this jet is a inclusive tag candidate (leading jet that is not a depth tag candidate + kinematic requirements)

        def WPlusJets(row):
            return row['Pass_WPlusJets']

        def select_safety(row):
            return (row['perJet_Pt'] > 40 and abs(row['perJet_Eta'] < 2.0) and
            0 <= row['perJet_EnergyFrac_Depth1'] <= 1 and 
            0 <= row['perJet_EnergyFrac_Depth2'] <= 1 and 
            0 <= row['perJet_EnergyFrac_Depth3'] <= 1 and 
            0 <= row['perJet_EnergyFrac_Depth4'] <= 1 and 
            0 < row['perJet_S_phiphi'] < 900 and 
            0 < row['perJet_LeadingRechitE'] < 900 and 
            0 <= row['perJet_Track0Pt'] < 900 and 
            0 <= row['perJet_Track0dR'] < 1 and 
            0 <= row['perJet_Track1Pt'] < 900 and 
            0 <= row['perJet_Track1dR'] < 1) # and
            # 0 <= row['jetIndex'] <= 1)                           # only consider the leading or subleading jet

        def classify_background(row): 
            if select_safety(row) and WPlusJets(row) and depth(row): # remove WPlusJets because this will be in the CR instead
                return bkg_value # 2 or 1 
            else:
                return -1

        def classify_bkg_inclusive(row): 
            if select_safety(row) and WPlusJets(row) and inclusive_bkg(row):
                return bkg_value 
            else:
                return -1

        def classify_signal(row):
            if select_safety(row) and select_HCAL12(row) and depth(row):
                return 0
            elif select_safety(row) and select_HCAL34(row) and depth(row):
                return sig_value # 1 or 0
            else:
                return -1
            
        def classify_sig_inclusive(row):
            if select_safety(row) and select_LLPjet(row) and inclusive_signal(row):
                return 0
            else:
                return -1
        
        classify_sig = classify_sig_inclusive if inclusive else classify_signal

        classify_bkg = classify_bkg_inclusive if inclusive else classify_background

        sig_df = pd.DataFrame()
        bkg_df = pd.DataFrame()
        
        # Load signal HDF5 data
        with h5py.File("signal.h5", "r") as sig_file:
            sig_data = sig_file["features"][:]
            self.sig_df = pd.DataFrame.from_records(sig_data)  # Convert structured array to DataFrame
        # Load background HDF5 data
        with h5py.File("background.h5", "r") as bkg_file:
            bkg_data = bkg_file["features"][:]
            self.bkg_df = pd.DataFrame.from_records(bkg_data)

        if not self.sig_df.empty and self.sel:
            sig_df = self.sig_df.copy(deep=True)
            # applying selections cut to signal
            sig_df["classID"] = sig_df.apply(classify_sig, axis=1) # add a column saying if passed signal selections (-1 or 0)
            sig_df = sig_df[sig_df["classID"] != -1].reset_index(drop=True) # removing junk
        
        if not self.bkg_df.empty and self.sel:
            bkg_df = self.bkg_df.copy(deep=True)
            # applying selections cut to background
            bkg_df["classID"] = bkg_df.apply(classify_bkg, axis=1) # add a column saying if passed signal selections (-1 or 1)
            bkg_df = bkg_df[bkg_df["classID"] != -1].reset_index(drop=True) # removing junk
            
        self.cumulative_df = pd.concat((sig_df, bkg_df))

        print("Length of signal after selections =", len(sig_df))
        print("Length of background after selections =", len(bkg_df))
        print("Length of signal + background after selections =", len(sig_df) + len(bkg_df))

        if debug_mode:
            print("-------------------Cumulative Data---------")
            print(self.cumulative_df.describe())

    def no_selections_concatenate(self):
        self.cumulative_df = pd.concat((self.sig_df, self.bkg_df))
        if debug_mode:
            print("-------------------All Data -- No Cuts applied---------")
            print(self.cumulative_df.describe())
        
    def process_data(self):
        print("Processing...")

        labels = None 
        if not self.mode and self.sel:
            self.cumulative_df = self.cumulative_df.sample(frac=1, random_state=42).reset_index(drop=True) # shuffling
            labels = self.cumulative_df["classID"].values
        
        perJet_Pt_series = self.cumulative_df['perJet_Pt']
        data = self.cumulative_df[FEATURES]
        normed_data = pd.DataFrame()
        for feature in data.columns:
            normed_data[feature] = (data[feature] - CONSTANTS[feature][0])/ CONSTANTS[feature][1]
        
        if debug_mode:
            print("Normed data:")
            print(normed_data.describe())
            print("Jet pT, 1000th digit will be used for random float later:")
            print(perJet_Pt_series.describe())
            print("Labels:")
            print(labels) # labels is 1 if signal and 0 if background

        print("Processing Complete")
        self.data = normed_data
        return normed_data, labels, perJet_Pt_series
    
    def write_to_root(self, scores, filename, labels=None):
        filename = f"{filename}_scores.root"
        dataframe = self.cumulative_df
        if self.num_classes == 2:
            dataframe['scores12'] = scores[:, 0]
            dataframe['scores34'] = scores[:, 1]
            dataframe['scoresbkg'] = scores[:, 2]
        elif self.num_classes == 1:
            dataframe['scores'] = scores[:, 0] # 0 is the signal class
        if labels is not None:
            dataframe['classID'] = labels
        with uproot.recreate(filename) as f:
            f['Classification'] = {key: dataframe[key] for key in dataframe.columns}
        print(f"Wrote to ROOT file: {filename}")

    def compute_norm_constants(self):
        # assumes that you have already loaded the files and applied safety selections
        constants_df = pd.DataFrame({'Mean': self.cumulative_df.mean().to_dict(), 'Standard Deviation': self.cumulative_df.std().to_dict()})
        # for manual input of other useful variables not included in safety selection:
        useful_variables = ['perJet_Track0dzToPV', 'perJet_Track0dzOverErr', 'perJet_Track0dxyToBS', 'perJet_Track0dxyOverErr',
                            'perJet_Track1dzToPV', 'perJet_Track1dzOverErr', 'perJet_Track1dxyToBS', 'perJet_Track1dxyOverErr', 
                            'perJet_Track2dzToPV', 'perJet_Track2dzOverErr', 'perJet_Track2dxyToBS', 'perJet_Track2dxyOverErr'] 
        for useful_variable in useful_variables:
            data = self.cumulative_df[[useful_variable]].copy()
            mask_condition = (data[useful_variable] <= -900) | (data[useful_variable] >= 900)
            data.loc[mask_condition, useful_variable] = 0  # Default to 0 for matching entries
            mean_value = data[useful_variable].mean()
            std_value = data[useful_variable].std()
            print(f"{useful_variable} mean: {mean_value}")
            print(f"{useful_variable} std: {std_value}")
            constants_df.loc[useful_variable, 'Mean'] = mean_value
            constants_df.loc[useful_variable, 'Standard Deviation'] = std_value
        constants_df.T.to_csv("norm_constants_v4.csv")

class ModelHandler:
    def __init__(self, num_classes=3, num_layers=2, optimizer="adam", lr=0.00027848106048644665, model_name="dense_model_v4.keras"):
        # this function is passed num_classes = 2

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
            
    def build_resnet(self):                 
        inputs = layers.Input(shape=(40,)) # 40 is number of features
        x = layers.Dense(64, activation="relu", kernel_initializer=GlorotUniform())(inputs)
        x = layers.BatchNormalization()(x)
        for _ in range(self.num_layers):
            shortcut = x
            x = layers.Dense(64, activation="relu", kernel_initializer=GlorotUniform())(x)
            x = layers.Dropout(0.2)(x)
            x = layers.Dense(128, activation="relu", kernel_initializer=GlorotUniform())(x)
            x = layers.Dense(64, activation="relu", kernel_initializer=GlorotUniform())(x)
            x = layers.BatchNormalization()(x)
            x = layers.add([x, shortcut])
            x = layers.Activation("relu")(x)
        x = layers.Dense(128, activation="relu")(x)
        outputs = layers.Dense(self.num_classes, activation="softmax", kernel_initializer=GlorotUniform())(x)

        # Create the model
        self.model = models.Model(inputs=inputs, outputs=outputs)
        
        #self.model = model
        self.model.compile(optimizer=self.optimizer, loss="sparse_categorical_crossentropy")
        print("Model summary:")
        print(self.model.summary())

    def build(self):                 
        model = Sequential()
        model.add(Dense(185, activation="sigmoid"))
        model.add(BatchNormalization())
        for _ in range(self.num_layers):
            model.add(Dense(159, activation="sigmoid"))
        model.add(BatchNormalization())
        model.add(Dense(181, activation="sigmoid"))
        model.add(Dense(self.num_classes, activation="softmax"))
        
        self.model = model
        
        self.model.compile(optimizer=self.optimizer, loss="sparse_categorical_crossentropy")
                  
    def train(self, X_train, y_train, epochs=200, batch_size=512, val=0.2):
        self.build() # similar to runner-v2
        name="best_model_v4.keras"
        early_stopping = EarlyStopping(monitor='val_loss', patience=10, restore_best_weights=True)
        checkpoint = ModelCheckpoint(name, monitor='val_loss', save_best_only=True, save_weights_only=True)
        history = self.model.fit(X_train, y_train, epochs=epochs, batch_size=512, validation_split=val, verbose=1, callbacks = [early_stopping, checkpoint])
        self.model.load_weights(name)
        if debug_mode: print("Total Train Samples: ", len(y_train))

        # plot loss and val_loss vs number of epochs 
        plt.figure()
        plt.plot(history.history['loss'])
        plt.plot(history.history['val_loss'])
        plt.title('Model Loss')
        plt.ylabel('Loss')
        plt.xlabel('Epoch')
        plt.legend(['train', 'val'], loc='upper left')
        plt.savefig("ModelLoss_v4.png")

        # Print summary of model
        print("Model summary:")
        print(self.model.summary())
        
    def get_model(self):
        return self.model
    
    def save(self):
        self.model.save(self.model_name)
        
    def load(self):
        self.model = tf.keras.models.load_model(self.model_name)
    
    def test(self, X_test, y_test):
        self.X_test = X_test
        preds = self.model.predict(X_test)
        predictions = np.argmax(preds, axis=1)
        accuracy = np.sum(predictions == y_test) / len(y_test)
        print("Total Test Samples: ", len(y_test))
        print("Accuracy: ", accuracy)
        
        self.roc_data = (y_test, preds)
        return self.roc_data
    
    def predict(self, predicting_data, labels=None):
        preds = self.model.predict(predicting_data)
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
            print("Wrote ROC data to file")
        
    def one_vs_one_roc(self):
        fig, ax = plt.subplots()
        custom_xlim = (0.00001,1)
        plt.setp(ax, xlim=custom_xlim)

        class_pairs = list(combinations(range(self.num_classes), 2))
        
        true_labels, predicted_scores = self.roc_data
        
        for (signal_class, bkg_class), color in zip(class_pairs, self.colors):
            # sklearn's roc_curve treats 1 labels as signal and 0 labels as bkg
            # Create a mask to only include signal and background classes
            mask = (true_labels == signal_class) | (true_labels == bkg_class)
        
            # Apply the mask to get the true labels for these classes
            masked_true_labels = true_labels[mask]
        
            # Re-label the true labels to binary (signal_class -> 1, bkg_class -> 0)
            binary_true_labels = np.where(masked_true_labels == signal_class, 1, 0)
            
            # Apply the mask to get the predicted scores for these classes
            masked_predicted_scores = predicted_scores[mask, signal_class]
            fpr, tpr, _ = roc_curve(binary_true_labels, masked_predicted_scores, drop_intermediate=False)
            roc_auc = auc(fpr, tpr)
            ax.semilogx(fpr, tpr, color=color, label=f"Class {self.names[signal_class]} vs Class {self.names[bkg_class]}, AUC: {roc_auc:.3f}")
        
        ax.set_xlabel('False Positive Rate (FPR)')
        ax.set_ylabel('True Positive Rate (TPR)')
        ax.set_title('One-vs-One ROC Curves')
        ax.legend(loc="lower right")
        ax.grid(True)
        fig.savefig("ROC1v1_v4.png")
            
    def one_vs_all_roc(self):       
        fig, ax = plt.subplots()
        custom_xlim = (0.00001,1)
        plt.setp(ax, xlim=custom_xlim)

        true_labels, predicted_scores = self.roc_data
        for signal_class, color in zip(range(self.num_classes -1), self.colors):
            masked_true_labels = np.where(true_labels == signal_class, 1, 0)
            masked_predicted_scores = predicted_scores[:, signal_class]
            fpr, tpr, _ = roc_curve(masked_true_labels, masked_predicted_scores, drop_intermediate=False)                
            roc_auc = auc(fpr, tpr)
            self.save_roc_data(fpr, tpr)
            ax.semilogx(fpr, tpr, color=color, label=f"Class {self.names[signal_class]} vs Rest, AUC: {roc_auc:.3f}")
            
        ax.set_xlabel('False Positive Rate (FPR)')
        ax.set_ylabel('True Positive Rate (TPR)')
        ax.set_title('One-vs-All ROC Curves')
        ax.legend(loc="lower right")
        ax.grid(True)  
        fig.savefig("ROC1vA_v4.png")
        print("-------ROC data-------")
        print("fpr shape ", fpr.shape)
        print("First non-zero TPR", tpr[fpr !=0][0])
        #index = np.where( fpr <= 1e-4)
        print("TPR: ", tpr[fpr <=1e-4][-1], "at FPR", fpr[fpr <=1e-4][-1])
        print("TPR: ", tpr[fpr <=1e-3][-1], "at FPR", fpr[fpr <=1e-3][-1])
        print("TPR: ", tpr[fpr <=1e-2][-1], "at FPR", fpr[fpr <=1e-2][-1])


    def compute_mutual_info(self):
        _, scores = self.roc_data 
        scores = scores[:, 0] # 0 is the signal
        features = self.X_test
        mutual_info = mutual_info_regression(features, scores)
        mi_results = pd.Series(mutual_info, index=FEATURES)
        mi_results_sorted = mi_results.sort_values(ascending=False)
        mi_results_sorted.to_csv("mutual_info_v4.csv")
        print("Computed mutual information")
        
        
class Runner:
    def __init__(self, sig_files=None, bkg_files=None, mode="train", num_classes=3, inclusive=False, load=True):
        self.mode = mode
        self.num_classes = num_classes
        self.inclusive = inclusive
        self.load = load
        self.sig = sig_files
        self.bkg = bkg_files
        self.model_name = "dense_model_v4.keras"
     
    def run_training(self):
        # modifying to do per-event splitting
        print("Training")
        if self.load:
            self.processor = DataProcessor(num_classes=self.num_classes - 1)
            self.processor.load_data(self.sig, self.bkg)
        self.processor.apply_selections(inclusive=self.inclusive)

        X, y, perJet_Pt_series = self.processor.process_data() # no longer processing train and test samples separately
        randFloat = (perJet_Pt_series * 1000).astype(int) % 10 # extract 1000th place. Trained on "train_mask = randFloat_values < 4"
        train_mask = randFloat < 4
        test_mask = ~train_mask  # or randFloat >= 4

        X_train, y_train = X[train_mask], y[train_mask]
        X_test, y_test = X[test_mask], y[test_mask]

        if debug_mode:
            print("Test and train masks based on the random float")
            print(type(X), type(y), type(randFloat)) # X is a Pandas DataFrame (features), y is a numpy array (labels), perJet_Pt_series is a Pandas DataFrame and randFloat is numpy array
            print(perJet_Pt_series)
            print(randFloat)
            print(train_mask)
            print(test_mask)

            print("Data and labels (signal or background) before train / test split")
            print(X)
            print(y)
            print(X_train)
            print(X_test)
        
        print("Total length of data (signal + background) = " + str(len(y)))
        print("Total length of training data (signal + background) = " + str(len(y_train)))
        print("Total length of testing data (signal + background) = " + str(len(y_test)))

        handler = ModelHandler(num_classes=self.num_classes, model_name=self.model_name)
        handler.train(X_train, y_train) # calls build()
        y_test, scores = handler.test(X_test, y_test)
        print("-------scores---------")
        print(scores[:10])
        handler.save()
        handler.one_vs_one_roc()
        handler.one_vs_all_roc()
        handler.compute_mutual_info()
        
        
    def run_evaluation(self):
        # this is when you don't want to rebuild and retrain the model -- just test it
        print("Evaluation")
        if self.load:
            # self.processor = DataProcessor(mode="eval", num_classes=self.num_classes - 1) # crashes with this because "labels" is none and therefore y_eval is none
            self.processor = DataProcessor(num_classes=self.num_classes - 1)
            self.processor.load_data(self.sig, self.bkg)
        self.processor.apply_selections(inclusive=self.inclusive)
        
        X_eval, y_eval, randFloat = self.processor.process_data()
        
        handler = ModelHandler(num_classes=self.num_classes, model_name=self.model_name)
        handler.load()
        handler.test(X_eval, y_eval)
        handler.one_vs_one_roc()
        handler.one_vs_all_roc()
        
        
    def run_file_evaluation(self):
        print("Evaluating Single File")
        if self.sig:
            # processes one file per run for now
            if self.load:
                self.processor = DataProcessor(num_classes=self.num_classes - 1, mode="filewrite", sel=False)
                self.processor.load_data(sig_files=self.sig)
            self.processor.no_selections_concatenate() # automatically inclusive
            self.fname = self.sig[0]
        elif self.bkg:
            if self.load:
                self.processor = DataProcessor(num_classes=self.num_classes - 1, mode="filewrite", sel=False)
                self.processor.load_data(bkg_files=self.bkg)
            self.processor.no_selections_concatenate()
            self.fname = self.bkg[0]
            
        predicting_data, labels, randFloat = self.processor.process_data()
        handler = ModelHandler(num_classes=self.num_classes, model_name=self.model_name)
        handler.load()
        preds = handler.predict(predicting_data, labels)
        self.processor.write_to_root(preds, self.fname, labels=None)

    def update_constants(self):
        print("Updating constants")
        if self.load:
            self.processor = DataProcessor(num_classes=self.num_classes - 1)
            self.processor.load_data(self.sig, self.bkg)
        self.processor.apply_selections(inclusive=self.inclusive)
        self.processor.compute_norm_constants()
        print("Constants updated")
        
    def run(self):
        if self.mode == "train":
            self.run_training()
        elif self.mode == "eval":
            self.run_evaluation()
        elif self.mode == "filewrite":
            self.run_file_evaluation()
        elif self.mode == "constants":
            self.update_constants()
        else:
            print("No Valid Mode Entered")
            
    def set_inclusive(self, inclusive=False):
        self.inclusive = inclusive
        return
    
    def set_load(self,load=True):
        self.load = load
    
    def set_model_name(self, model_name="dense_model_v4.keras"):
        self.model_name = model_name
        
        
def main():
    sig_files = [
        "minituple_HToSSTo4b_125_15_CTau10000.root",
        "minituple_HToSSTo4b_125_15_CTau1000.root",
        "minituple_HToSSTo4b_125_15_CTau3000.root",
        "minituple_HToSSTo4b_125_50_CTau10000.root",
        "minituple_HToSSTo4b_125_50_CTau3000.root",
        "minituple_HToSSTo4b_250_120_CTau10000.root",
        "minituple_HToSSTo4b_350_160_CTau10000.root",
        "minituple_HToSSTo4b_350_80_CTau500.root"
    ]

    # minituples_Zmu_2023Bv1.root
    # minituples_Zmu_2023Cv1.root
    # minituples_Zmu_2023Cv2.root
    # minituples_Zmu_2023Cv3.root
    # minituples_Zmu_2023Cv4.root
    # minituples_Zmu_2023Dv1.root
    # minituples_Zmu_2023Dv2.root
    
    bkg_files = [
        "minituple_LLPskim_2023Bv1.root",
        "minituple_LLPskim_2023Cv1.root",
        "minituple_LLPskim_2023Cv2.root",
        "minituple_LLPskim_2023Cv3.root",
        "minituple_LLPskim_2023Cv4.root",
        "minituple_LLPskim_2023Dv1.root",
        "minituple_LLPskim_2023Dv2.root"
    ]

    # this is for the depth training, the per event tree with inclusive scores appended
    sig_files_perEvent = sig_files
    bkg_files_perEvent = bkg_files
    
    mode = "train" # "train", "eval", "filewrite", "constants"
    
    # running the depth and inclusive tagger sequentially
    # print("Running Depth Tagger")
    # runner = Runner(sig_files=sig_files_perEvent[:], bkg_files=bkg_files_perEvent[:], mode=mode, num_classes=2, inclusive=False)
    # runner.set_model_name(model_name="depth_model_v4.keras")
    # runner.run()
    
    print("Running Inclusive Tagger")
    runner = Runner(sig_files=sig_files[:], bkg_files=bkg_files[:], mode=mode, num_classes=2, inclusive=True)
    runner.set_model_name(model_name="inclusive_model_v4.keras")
    runner.run()
      
if __name__ == "__main__":
    main()