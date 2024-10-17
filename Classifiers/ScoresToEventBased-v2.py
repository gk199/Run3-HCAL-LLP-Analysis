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
num_jets = 1 if perJet else 6

CONSTANTS = pd.read_csv("norm_constants_v3.csv")

class DataProcessor:
    def __init__(self, num_classes=2, mode=None, sel=True): #counting from 0 
        self.return_value_bkg = num_classes
        self.return_sig_value = num_classes - 1
        self.num_classes = num_classes
        self.mode = mode
        self.sel = sel
        #self.inclusive = inclusive
    
    def load_data(self, sig_files=None, bkg_files=None):
        
        # need a mode that doesn't pre-classify them but just loads the data and then you can get it to predict
        # and returns the dataframe unchanged so filenames are the same
        print("Loading Data...")
        
        filter_name = "*"
        filepath = '/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.9/'
    
        sig_fps = [filepath + filename for filename in sig_files] if sig_files is not None else []
        bkg_fps = [filepath + filename for filename in bkg_files] if bkg_files is not None else []
        sig_df = [] #pd.DataFrame()
        bkg_df = [] #pd.DataFrame()
        
        # aggregating all signal events
        for file in sig_fps:
            if perJet: sig = uproot.open(file)['PerJet_LLPmatched']
            else: 
                #sig = uproot.open(file)['NoSel']
                sig = uproot.open(file)['PassedHLT']
            print(f"Opened {file}")
            sig = sig.arrays(filter_name=filter_name, library="pd")
            #sig_df = pd.concat((sig_df, sig))
            sig_df.append(sig)
        self.sig_df = pd.concat(sig_df) if sig_df else pd.DataFrame()
        
        # aggregating all background events
        for file in bkg_fps:
            if perJet: bkg = uproot.open(file)['PerJet_WPlusJets']
            # else: bkg = uproot.open(file)['WPlusJets']
            else: bkg = uproot.open(file)['Zmumu']
            print(f"Opened {file}")
            bkg = bkg.arrays(filter_name=filter_name, library="pd")
            #bkg_df = pd.concat((bkg_df, bkg))
            bkg_df.append(bkg)
        self.bkg_df = pd.concat(bkg_df) if bkg_df else pd.DataFrame()
        
        print("Loaded Data")
        
    def no_selections_concatenate(self):
        self.cumulative_df = pd.concat((self.sig_df, self.bkg_df))
        print("-------------------All Data // No Cuts applied---------")
        print(self.cumulative_df.describe())

    def process_data(self):
        
        print("Processing...")
        features = ['perJet_Eta', 'perJet_Mass', 
       'perJet_S_phiphi', 'perJet_S_etaeta', 'perJet_S_etaphi', 
       'perJet_Tracks_dR', 
       # 'perJet_E', 'perJet_Pt', # remove when have fractional pT and E variables implemented! 
       'perJet_Track0dR', 'perJet_Track0dEta', 'perJet_Track0dPhi', # ideally track pT would be track pT / jet pT
       'perJet_Track1dR', 'perJet_Track1dEta', 'perJet_Track1dPhi',
       'perJet_Track2dR', 'perJet_Track2dEta', 'perJet_Track2dPhi',
       # 'perJet_Track0Pt', 'perJet_Track1Pt', 'perJet_Track2Pt',  
       'perJet_Frac_Track0Pt', 'perJet_Frac_Track1Pt', 'perJet_Frac_Track2Pt',
       'perJet_EnergyFrac_Depth1', 'perJet_EnergyFrac_Depth2', 'perJet_EnergyFrac_Depth3', 'perJet_EnergyFrac_Depth4', 
       'perJet_LeadingRechitD', 
       # 'perJet_LeadingRechitE', 'perJet_SubLeadingRechitE', 'perJet_SSubLeadingRechitE', # ideally rechit E would be rechit E / jet E
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
            # print(self.cumulative_df["jet"+str(jet)+"_E"].values)
            jet_valid = self.cumulative_df["jet"+str(jet)+"_E"].values
            # in data, would like to rename jetX to perJet to work with the input files
            # data is already a pandas DataFrame so try "rename" option
            if not perJet: data = data.rename(columns=feature_dictionary)

            normed_data = pd.DataFrame()
            for feature in data.columns:
                # giving data a nominal 0 value if bad, but scores are -9999 if jet is not valid
                mask_condition = (data[feature] <= -900) | (data[feature] >= 900)
                data.loc[mask_condition, feature] = 0  # Default to 0 for matching entries
                normed_data[feature] = (data[feature] - CONSTANTS[feature][0])/ CONSTANTS[feature][1]

            # print(data.describe())
            print(normed_data.describe())
        
            print("Processing Complete")

            all_normed_data.append(normed_data)
            all_labels.append(labels)
            all_jet_valid.append(jet_valid)
        
        # return normed data, labels,and a list of rows are ok (energy is positive) and which are not (energy is -9999.9)
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
    
    def write_to_root(self, scores, scores_inc, filename, labels=None):
        filename = f"{filename[:-5]}_scores.root" # remove .root from initial filename
        dataframe = self.cumulative_df
        if self.num_classes == 2:
            for i in range(num_jets):
                dataframe['jet'+str(i)+'_scores12'] = scores[i][:, 0]
                dataframe['jet'+str(i)+'_scores34'] = scores[i][:, 1]
                dataframe['jet'+str(i)+'_scoresbkg'] = scores[i][:, 2]
                dataframe['jet'+str(i)+'_scores12_inc'] = scores_inc[i][:, 0]
                dataframe['jet'+str(i)+'_scores34_inc'] = scores_inc[i][:, 1]
                dataframe['jet'+str(i)+'_scoresbkg_inc'] = scores_inc[i][:, 2]
        elif self.num_classes == 1:
            for i in range(num_jets):
                dataframe['jet'+str(i)+'_scores'] = scores[i][:, 0] # 0 is the signal class
                dataframe['jet'+str(i)+'_scores_inc'] = scores_inc[i][:, 0] # 0 is the signal class
        if labels is not None:
            dataframe['classID'] = labels
        with uproot.recreate(filename) as f:
            f['Classification'] = {key: dataframe[key] for key in dataframe.columns}
        print(f"Wrote to ROOT file: {filename}")
            
        
class ModelHandler:
    def __init__(self, num_classes=3, num_layers=3, optimizer="adam", lr=0.00027848106048644665, model_name="dense_model_v3.keras"):
        
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
            print("wrote to file")  

class Runner:
    def __init__(self, sig_files=None, bkg_files=None, mode="train", num_classes=3, inclusive=False, load=True):
        self.mode = mode
        self.num_classes = num_classes
        self.inclusive = inclusive
        self.load = load
        self.sig = sig_files
        self.bkg = bkg_files
        self.model_name = "dense_model_v3.keras"
    
    def evaluate_scores(self): # this code used to be in run_file_evaluation -- still testing
        print("Determining Scores")
        predicting_data, labels, jet_valid = self.processor.process_data()
        # depth
        # handler = ModelHandler(num_classes=self.num_classes, model_name=self.model_name)
        handler = ModelHandler(num_classes=self.num_classes, model_name="depth_model_v3_Oct15.keras")
        handler.load()
        preds = [ handler.predict(predicting_data[i], labels[i]) for i in range(num_jets) ]
        # inclusive
        handler_inc = ModelHandler(num_classes=self.num_classes, model_name="inclusive_model_v3_Oct15.keras")
        handler_inc.load()
        preds_inc = [ handler_inc.predict(predicting_data[i], labels[i]) for i in range(num_jets) ]
        # data is predicting_data, jet valid is jet_valid, scores are preds, all indexed by jet (6 total)
        # to print whole data table, predicting_data[0], to print just values in a list, predicting_data[0].values

        # check if predicting_data[i] has a valid jet. If so, keep DNN score. If not, set score = -9999
        for i in range(num_jets): # evaluate for all 6 jets
            for jet in range(len(predicting_data[i])): # evaluate for every jet, if valid
                if (jet_valid[i][jet] < -9000): # if jet E is -9999.9, replace score with -9999.9
                    preds[i][jet] = [-9999.9, -9999.9]
                    preds_inc[i][jet] = [-9999.9, -9999.9]

        self.processor.write_to_root(preds, preds_inc, self.fname, labels=None)

    def run_file_evaluation(self):
        print("Evaluating Single File")
        if self.sig:
            # processes one file per run for now
            for i in range(len(self.sig)):
                print("Loaded signal, #" + str(i))
                if self.load:
                    self.processor = DataProcessor(num_classes=self.num_classes - 1, mode="filewrite", sel=False)
                    self.processor.load_data(sig_files=self.sig)
                self.processor.no_selections_concatenate() # automatically inclusive
                self.fname = self.sig[i]
                self.evaluate_scores()
        elif self.bkg:
            for i in range(len(self.bkg)):
                print("Loaded background, #" + str(i))
                if self.load:
                    self.processor = DataProcessor(num_classes=self.num_classes - 1, mode="filewrite", sel=False)
                    self.processor.load_data(bkg_files=self.bkg)
                self.processor.no_selections_concatenate()
                self.fname = self.bkg[i]
                self.evaluate_scores()

        # for i in range(num_jets): self.processor.default_variables(jet_index = i) # testing writing variables to 0 to avoid model having "peaky" values
        
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
    
    def set_model_name(self, model_name="dense_model_v3.keras"):
        self.model_name = model_name
        
        
def main():
    sig_files = [
        # "minituple_v3.9_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-15_CTau1000_13p6TeV_2024_10_14_TRAIN.root", # no passed HLT tree 
        #"minituple_v3.9_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-50_CTau3000_13p6TeV_2024_10_14_batch1.root",
        #"minituple_v3.9_LLP_MC_ggH_HToSSTobbbb_MH-250_MS-120_CTau10000_13p6TeV_2024_10_14_batch1.root",
        #"minituple_v3.9_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-160_CTau10000_13p6TeV_2024_10_14_batch1.root",
        #"minituple_v3.9_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-80_CTau500_13p6TeV_2024_10_14_TRAIN.root",
        # # "minituple_v3.9_LLP_MC_ggH_HToSSTobbbb_MH-HADD_TRAIN-batch1.root",
        # "minituple_v3.9_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-15_CTau1000_13p6TeV_2024_10_14_TEST.root", # no passed HLT tree 
        #"minituple_v3.9_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-50_CTau3000_13p6TeV_2024_10_14_batch2.root",
        #"minituple_v3.9_LLP_MC_ggH_HToSSTobbbb_MH-250_MS-120_CTau10000_13p6TeV_2024_10_14_batch2.root",
        #"minituple_v3.9_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-160_CTau10000_13p6TeV_2024_10_14_batch2.root",
        #"minituple_v3.9_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-80_CTau500_13p6TeV_2024_10_14_TEST.root",
        # # "minituple_v3.9_LLP_MC_ggH_HToSSTobbbb_MH-HADD_TEST-batch2.root"
    ]
    
    bkg_files = [
        #"minituple_v3.9_LLPskim_Run2023Bv1_2024_10_14.root",
        #"minituple_v3.9_LLPskim_Run2023Cv1_2024_10_14.root",
        #"minituple_v3.9_LLPskim_Run2023Cv2_2024_10_14.root",
        #"minituple_v3.9_LLPskim_Run2023Cv3_2024_10_14.root",
        #"minituple_v3.9_LLPskim_Run2023Cv4_2024_10_14.root",
        #"minituple_v3.9_LLPskim_Run2023Dv1_2024_10_14.root",
        #"minituple_v3.9_LLPskim_Run2023Dv2_2024_10_14.root",
        "minituple_v3.9_Zmu_Run2023Bv1_2024_10_14.root",
        "minituple_v3.9_Zmu_Run2023Cv1_2024_10_14.root",
        "minituple_v3.9_Zmu_Run2023Cv2_2024_10_14.root",
        "minituple_v3.9_Zmu_Run2023Cv3_2024_10_14.root",
        ##"minituple_v3.9_Zmu_Run2023Cv4_2024_10_14.root",# not produced for 3.9
        "minituple_v3.9_Zmu_Run2023Dv1_2024_10_14.root",
        "minituple_v3.9_Zmu_Run2023Dv2_2024_10_14.root"
    ]

    
    mode = "filewrite" # "eval", "filewrite"
    
    # pass runner each signal file (as a list, using list slicing), and then each background file, such that scores are appended to each
    print("Running Depth and Inclusive Tagger over each file")
    for i in range(len(sig_files)):
        runner = Runner(sig_files=sig_files[i:i+1], bkg_files=None, mode=mode, num_classes=2, inclusive=False)
        runner.run()
    for i in range(len(bkg_files)):
        runner = Runner(sig_files=None, bkg_files=bkg_files[i:i+1], mode=mode, num_classes=2, inclusive=False)
        runner.run()    
    
if __name__ == "__main__":
    main()