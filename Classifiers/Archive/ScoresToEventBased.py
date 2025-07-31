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

perJet = False

CONSTANTS = pd.read_csv("norm_constants.csv") 
if perJet: CONSTANTS = pd.read_csv("perJet_norm_constants.csv") 

class DataProcessor:
    def __init__(self, num_classes=2, mode=None, sel=True): #counting from 0 
        self.return_value_bkg = num_classes
        self.return_sig_value = num_classes - 1
        self.mode = mode
        self.sel = sel
    
    def load_data(self, sig_files=None, bkg_files=None):
        print("Loading Data...")
        bkg_value = self.return_value_bkg
        sig_value = self.return_sig_value
        
        # applying selection to remove Null values/ maxed out values/ etc.
        def select_HCAL12(row):
            return 177 <= row['jet0_MatchedLLP_DecayR'] < 214.2 and abs(row['jet0_MatchedLLP_Eta']) < 1.26

        def select_HCAL34(row):
            return 214.2 <= row['jet0_MatchedLLP_DecayR'] < 295 and abs(row['jet0_MatchedLLP_Eta']) < 1.26

        def select_safety(row):
            return (row['jet0_Pt'] > 40 and abs(row['jet0_Eta']) < 1.26 and 
#             1 <= row['jet0_LeadingRechitD'] <= 4 and 
            0 <= row['jet0_EnergyFrac_Depth1'] <= 1 and 
            0 <= row['jet0_EnergyFrac_Depth2'] <= 1 and 
            0 <= row['jet0_EnergyFrac_Depth3'] <= 1 and 
            0 <= row['jet0_EnergyFrac_Depth4'] <= 1 and 
            0 < row['jet0_S_phiphi'] < 900 and 
            0 < row['jet0_LeadingRechitE'] < 900 and 
            0 < row['jet0_Track0Pt'] < 900 and 
            0 <= row['jet0_Track0dR'] < 1 and 
            0 < row['jet0_Track1Pt'] < 900 and 
            0 <= row['jet0_Track1dR'] < 1)

        def classify_bkg(row): 
            if (row['jet0_Pt'] > 40 and abs(row['jet0_Eta']) < 1.26 and 
#             1 <= row['jet0_LeadingRechitD'] <= 4 and 
            0 <= row['jet0_EnergyFrac_Depth1'] <= 1 and 
            0 <= row['jet0_EnergyFrac_Depth2'] <= 1 and 
            0 <= row['jet0_EnergyFrac_Depth3'] <= 1 and 
            0 <= row['jet0_EnergyFrac_Depth4'] <= 1 and 
            0 < row['jet0_S_phiphi'] < 900 and 
            0 < row['jet0_LeadingRechitE'] < 900 and 
            0 < row['jet0_Track0Pt'] < 900 and 
            0 <= row['jet0_Track0dR'] < 1 and 
            0 < row['jet0_Track1Pt'] < 900 and 
            0 <= row['jet0_Track1dR'] < 1):
                return bkg_value # 2 or 1 
            else:
                return -1

        def classify_sig(row):
            if select_safety(row) and select_HCAL12(row):
                return 0
            elif select_safety(row) and select_HCAL34(row):
                return sig_value # 1 or 0
            else:
                return -1
        
        filter_name = "*" # all branches in the tree
        filepath = '/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.8/'
    
        sig_fps = [filepath + filename for filename in sig_files] if sig_files is not None else []
        bkg_fps = [filepath + filename for filename in bkg_files] if bkg_files is not None else []
        sig_df = pd.DataFrame()
        bkg_df = pd.DataFrame()
        
        # aggregating all signal events
        for file in sig_fps:
            # sig = uproot.open(file)['PassedHLT']
            sig = uproot.open(file)['NoSel']
            sig = sig.arrays(filter_name=filter_name, library="pd")
            sig_df = pd.concat((sig_df, sig))
    
        # aggregating all background events
        for file in bkg_fps:
            bkg = uproot.open(file)['WPlusJets']
            bkg = bkg.arrays(filter_name=filter_name, library="pd")
            bkg_df = pd.concat((bkg_df, bkg))
        
        if sig_files is not None and self.sel:
            # applying selections cut to signal
            sig_df["classID"] = sig_df.apply(classify_sig, axis=1)
            sig_df = sig_df[sig_df["classID"] != -1] # removing junk
        
        if bkg_files is not None and self.sel:
            # applying selections cut to background
            bkg_df["classID"] = bkg_df.apply(classify_bkg, axis=1)
            bkg_df = bkg_df[bkg_df["classID"] != -1] # removing junk
    
        cumulative_df = pd.concat((sig_df, bkg_df)) # combining into one large dataset with labels
        
        self.cumulative_df = cumulative_df
        
        print("Loaded Data")
        
    def process_data(self, norm_type="MinMax"):
        
        print("Processing...")
        features = ['jet0_LeadingRechitD', 'jet0_E', 'jet0_Pt', 'jet0_Eta',
       'jet0_Phi', 'jet0_Mass', # 'jet0_Area',
       'jet0_S_phiphi', 'jet0_S_etaeta',
       'jet0_S_etaphi', 'jet0_Tracks_dR', 'jet0_Track0Pt',
       'jet0_Track0dR', 'jet0_Track0dEta', 'jet0_Track0dPhi',
       'jet0_Track0dzToPV', 'jet0_Track0dxyToBS', 'jet0_Track0dzOverErr',
       'jet0_Track0dxyOverErr', 'jet0_Track1Pt', 'jet0_Track1dR',
       'jet0_Track1dEta', 'jet0_Track1dPhi', 'jet0_Track1dzToPV',
       'jet0_Track1dxyToBS', 'jet0_Track1dzOverErr',
       'jet0_Track1dxyOverErr', 'jet0_Track2Pt', 'jet0_Track2dR',
       'jet0_Track2dEta', 'jet0_Track2dPhi',
       'jet0_EnergyFrac_Depth1',
       'jet0_EnergyFrac_Depth2', 'jet0_EnergyFrac_Depth3', 'jet0_EnergyFrac_Depth4', 'jet0_LeadingRechitE',
       'jet0_SubLeadingRechitE', 'jet0_SSubLeadingRechitE',
       'jet0_AllRechitE', 'jet0_NeutralHadEFrac', 'jet0_ChargedHadEFrac']
        
        labels = None 
        if not self.mode and self.sel:
            self.cumulative_df = self.cumulative_df.sample(frac=1).reset_index(drop=True) # shuffling
            labels = self.cumulative_df["classID"].values

        data = self.cumulative_df[features]
        # in data, would like to rename jetX to perJet to work with the input files
        # data is already a pandas DataFrame so try "rename" option
        if not perJet:
            data = data.rename(columns={'jet0_LeadingRechitD':'perJet_LeadingRechitD', 'jet0_E':'perJet_E', 'jet0_Pt':'perJet_Pt', 'jet0_Eta':'perJet_Eta',
            'jet0_Phi':'perJet_Phi', 'jet0_Mass':'perJet_Mass', # 'jet0_Area',
            'jet0_S_phiphi':'perJet_S_phiphi', 'jet0_S_etaeta':'perJet_S_etaeta',
            'jet0_S_etaphi':'perJet_S_etaphi', 'jet0_Tracks_dR':'perJet_Tracks_dR', 'jet0_Track0Pt':'perJet_Track0Pt',
            'jet0_Track0dR':'perJet_Track0dR', 'jet0_Track0dEta':'perJet_Track0dEta', 'jet0_Track0dPhi':'perJet_Track0dPhi',
            'jet0_Track0dzToPV':'perJet_Track0dzToPV', 'jet0_Track0dxyToBS':'perJet_Track0dxyToBS', 'jet0_Track0dzOverErr':'perJet_Track0dzOverErr',
            'jet0_Track0dxyOverErr':'perJet_Track0dxyOverErr', 'jet0_Track1Pt':'perJet_Track1Pt', 'jet0_Track1dR':'perJet_Track1dR',
            'jet0_Track1dEta':'perJet_Track1dEta', 'jet0_Track1dPhi':'perJet_Track1dPhi', 'jet0_Track1dzToPV':'perJet_Track1dzToPV',
            'jet0_Track1dxyToBS':'perJet_Track1dxyToBS', 'jet0_Track1dzOverErr':'perJet_Track1dzOverErr',
            'jet0_Track1dxyOverErr':'perJet_Track1dxyOverErr', 'jet0_Track2Pt':'perJet_Track2Pt', 'jet0_Track2dR':'perJet_Track2dR',
            'jet0_Track2dEta':'perJet_Track2dEta', 'jet0_Track2dPhi':'perJet_Track2dPhi',
            'jet0_EnergyFrac_Depth1':'perJet_EnergyFrac_Depth1',
            'jet0_EnergyFrac_Depth2':'perJet_EnergyFrac_Depth2', 'jet0_EnergyFrac_Depth3':'perJet_EnergyFrac_Depth3', 'jet0_EnergyFrac_Depth4':'perJet_EnergyFrac_Depth4', 'jet0_LeadingRechitE':'perJet_LeadingRechitE',
            'jet0_SubLeadingRechitE':'perJet_SubLeadingRechitE', 'jet0_SSubLeadingRechitE':'perJet_SSubLeadingRechitE',
            'jet0_AllRechitE':'perJet_AllRechitE', 'jet0_NeutralHadEFrac':'perJet_NeutralHadEFrac', 'jet0_ChargedHadEFrac':'perJet_ChargedHadEFrac'})
            # runs but worry the output isn't sensible yet, the means don't look like they used to, probably due to safety selections relying on the previous names

        print("data.describe() = ")
        print(data.describe())
        normed_data = pd.DataFrame()
        for feature in data.columns:
        # for feature, perJet_feature in zip(data.columns, perJet_data.columns):
            # need to handle CONSTANTS as perJet instead
            normed_data[feature] = (data[feature] - CONSTANTS[feature][0])/ CONSTANTS[feature][1]
        print(normed_data.describe()) 
        print("Processing Complete")
        print(self.cumulative_df.describe())
        
        print("Processing Complete")
        
        return normed_data, labels
    
    
    def write_to_root(self, scores, filename, labels=None):
        filename = f"{filename}_scores_nosels_v1.root"
        # only implemented for multiclass for now but easy to change
        dataframe = self.cumulative_df
        dataframe['scores12'] = scores[:, 0]
        dataframe['scores34'] = scores[:, 1]
        dataframe['scoresbkg'] = scores[:, 2]
        if labels is not None:
            dataframe['classID'] = labels
        with uproot.recreate(filename) as f:
            f['Classification'] = {key: dataframe[key] for key in dataframe.columns}
        print(f"Wrote to ROOT file: {filename}")
            
        
class ModelHandler:
    def __init__(self, num_classes=3, num_layers=3, optimizer="adam", lr=0.00027848106048644665):
        
        self.num_classes = num_classes
        self.num_layers = num_layers
        self.model_name = "dense_model_v1.keras"
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
                  
    def train(self, X_train, y_train, epochs=50, batch_size=512, val=0.2):
        self.build()
        self.model.fit(X_train, y_train, epochs=epochs, batch_size=512, validation_split=val, verbose=1)
        
    def get_model(self):
        return self.model
    
    def save(self):
        self.model.save(self.model_name)
        
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
        preds = self.model.predict(predicting_data)
        if labels is not None:
            print("Accuracy: ", np.sum(np.argmax(preds, axis=1) == labels) / len(labels) )
        return preds
        
    def one_vs_one_roc(self):
        fig, ax = plt.subplots()
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
        fig.savefig("ROC1v1.png")
            
    def one_vs_all_roc(self):       
        fig, ax = plt.subplots()
        true_labels, predicted_scores = self.roc_data
        for signal_class, color in zip(range(self.num_classes -1), self.colors):
            masked_true_labels = np.where(true_labels == signal_class, 1, 0)
            masked_predicted_scores = predicted_scores[:, signal_class]
            fpr, tpr, _ = roc_curve(masked_true_labels, masked_predicted_scores, drop_intermediate=False)                
            roc_auc = auc(fpr, tpr)
            ax.semilogx(fpr, tpr, color=color, label=f"Class {self.names[signal_class]} vs Rest, AUC: {roc_auc:.3f}")
            
        ax.set_xlabel('False Positive Rate (FPR)')
        ax.set_ylabel('True Positive Rate (TPR)')
        ax.set_title('One-vs-All ROC Curves')
        ax.legend(loc="lower right")
        ax.grid(True)  
        fig.savefig("ROC1vA.png")
        
        
        
        
class Runner:
    def __init__(self, sig_files=None, sig_files_test=None, bkg_files=None, bkg_files_test=None, mode="train", num_classes=3):
        self.mode = mode
        self.num_classes = num_classes
        if mode == "train":
            self.sig_train, self.sig_test = sig_files, sig_files_test
            self.bkg_train, self.bkg_test = bkg_files, bkg_files_test
        elif mode == "eval":
            self.sig_eval = sig_files
            self.bkg_eval = bkg_files
        elif mode == "filewrite":
            self.sig = sig_files
            self.bkg = bkg_files
     
    def run_training(self):
        print("Training")
        processor = DataProcessor(num_classes=self.num_classes - 1)
        processor.load_data(self.sig_train, self.bkg_train)
        X_train, y_train = processor.process_data()
        
        processor.load_data(self.sig_test, self.bkg_test)
        X_test, y_test = processor.process_data()
        
        handler = ModelHandler(num_classes=self.num_classes)
        handler.train(X_train, y_train)
        y_test, scores = handler.test(X_test, y_test)
        handler.save()
        handler.one_vs_one_roc()
        handler.one_vs_all_roc()
        
        
    def run_evaluation(self):
        # this is when you don't want to rebuild and retrain the model -- just test it
        print("Evaluation")
        processor = DataProcessor(num_classes=self.num_classes - 1)
        processor.load_data(self.sig_eval, self.bkg_eval)
        
        X_eval, y_eval = processor.process_data()
        
        handler = ModelHandler(num_classes=self.num_classes)
        handler.load()
        handler.test(X_eval, y_eval)
        handler.one_vs_one_roc()
        handler.one_vs_all_roc()
        
        
    def run_file_evaluation(self):
        print("Evaluating Single File")
        processor = DataProcessor(mode="filewrite", sel=False)
        if self.sig:
            processor.load_data(sig_files=self.sig)
            self.fname = self.sig[0]
        elif self.bkg:
            processor.load_data(bkg_files=self.bkg)
            self.fname = self.bkg[0]
            
        predicting_data, labels = processor.process_data()
        handler = ModelHandler(num_classes=self.num_classes)
        handler.load()
        preds = handler.predict(predicting_data, labels)
        processor.write_to_root(preds, self.fname, labels=None)
        
        
    def run(self):
        if self.mode == "train":
            self.run_training()
        elif self.mode == "eval":
            self.run_evaluation()
        elif self.mode == "filewrite":
            self.run_file_evaluation()
        else:
            print("No Valid Mode Entered")
        
        
def main():

    filenames = [
    "minituple_v3.8_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-15_CTau1000_13p6TeV_2024_06_03_TRAIN.root",
    # "minituple_v3.8_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-50_CTau3000_13p6TeV_2024_06_03_batch1.root",
    # "minituple_v3.8_LLP_MC_ggH_HToSSTobbbb_MH-250_MS-120_CTau10000_13p6TeV_2024_06_03_batch1.root",
    # "minituple_v3.8_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-160_CTau10000_13p6TeV_2024_06_03_batch1.root",
    # "minituple_v3.8_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-80_CTau500_13p6TeV_2024_06_03_TRAIN.root",
    # "minituple_v3.8_LLP_MC_ggH_HToSSTobbbb_MH-HADD_TRAIN-batch1.root"
    ]
    
    bkg_files = [
    "minituple_v3.8_LLPskim_Run2023Cv1_2024_06_03.root",
    # "minituple_v3.8_LLPskim_Run2023Cv3_2024_06_03.root",
    # "minituple_v3.8_LLPskim_Run2023Dv1_2024_06_03.root",
    # "minituple_v3.8_LLPskim_Run2023Bv1_2024_06_03.root",
    # "minituple_v3.8_LLPskim_Run2023Cv2_2024_06_03.root",
    # "minituple_v3.8_LLPskim_Run2023Cv4_2024_06_03.root",
    # "minituple_v3.8_LLPskim_Run2023Dv2_2024_06_03.root"
    ]

    filenames_test = [
    "minituple_v3.8_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-15_CTau1000_13p6TeV_2024_06_03_TEST.root",
    # "minituple_v3.8_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-50_CTau3000_13p6TeV_2024_06_03_batch2.root",
    # "minituple_v3.8_LLP_MC_ggH_HToSSTobbbb_MH-250_MS-120_CTau10000_13p6TeV_2024_06_03_batch2.root",
    # "minituple_v3.8_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-160_CTau10000_13p6TeV_2024_06_03_batch2.root",
    # "minituple_v3.8_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-80_CTau500_13p6TeV_2024_06_03_TEST.root",
    # "minituple_v3.8_LLP_MC_ggH_HToSSTobbbb_MH-HADD_TEST-batch2.root"
    ]
    
    bkg_files_test = [
    # "minituple_v3.8_LLPskim_Run2023Cv1_2024_06_03.root",
    "minituple_v3.8_LLPskim_Run2023Cv3_2024_06_03.root",
    # "minituple_v3.8_LLPskim_Run2023Dv1_2024_06_03.root",
    # "minituple_v3.8_LLPskim_Run2023Bv1_2024_06_03.root",
    # "minituple_v3.8_LLPskim_Run2023Cv2_2024_06_03.root",
    # "minituple_v3.8_LLPskim_Run2023Cv4_2024_06_03.root",
    # "minituple_v3.8_LLPskim_Run2023Dv2_2024_06_03.root"
    ]
        
    # DON'T TRAIN: this is to apply the per jet trained DNN to the per event trees! 
    mode = "filewrite" # "eval", # "filewrite"
    
    runner = Runner(sig_files=filenames[:1], sig_files_test=filenames_test[:1], bkg_files=bkg_files[:1], bkg_files_test=bkg_files_test[:1], mode=mode, num_classes=3)
    runner.run()
    
if __name__ == "__main__":
    main()
    
