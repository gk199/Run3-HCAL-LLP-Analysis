import os
import numpy as np
import matplotlib.pyplot as plt


from timeit import default_timer as timer
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import MinMaxScaler

import tensorflow as tf
import keras
from keras import initializers
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense, Conv1D, Flatten, Dropout, MaxPooling1D, BatchNormalization, AveragePooling1D
from tensorflow.keras.optimizers import Nadam, Adam, Adamax

import neptune
from neptune.integrations.tensorflow_keras import NeptuneCallback
import optuna
import neptune.integrations.optuna as npt_utils
import uproot
import pandas as pd


print("Num GPUs Available: ", len(tf.config.list_physical_devices('GPU')))

# Initialize Neptune
run = neptune.init_run(
    project    = "katx711/Assignment3",
    api_token  = "eyJhcGlfYWRkcmVzcyI6Imh0dHBzOi8vYXBwLm5lcHR1bmUuYWkiLCJhcGlfdXJsIjoiaHR0cHM6Ly9hcHAubmVwdHVuZS5haSIsImFwaV9rZXkiOiJjMWQ4NWUxNC0xZjgwLTQxZjgtYTRjOS0zODA0Mzc5ZTM4ZGEifQ==",
)  



# ----------- Prepare what is fed into the NN -----------


CONSTANTS = pd.read_csv("norm_constants.csv")
FEATURES = ['perJet_LeadingRechitD', 'perJet_E', 'perJet_Pt', 'perJet_Eta',
       'perJet_Phi', 'perJet_Mass', 'perJet_Area',
 'perJet_S_phiphi', 'perJet_S_etaeta',
       'perJet_S_etaphi', 'perJet_Tracks_dR', 'perJet_Track0Pt',
       'perJet_Track0dR', 'perJet_Track0dEta', 'perJet_Track0dPhi',
       'perJet_Track0dzToPV', 'perJet_Track0dxyToBS', 'perJet_Track0dzOverErr',
       'perJet_Track0dxyOverErr', 'perJet_Track1Pt', 'perJet_Track1dR',
       'perJet_Track1dEta', 'perJet_Track1dPhi', 'perJet_Track1dzToPV',
       'perJet_Track1dxyToBS', 'perJet_Track1dzOverErr',
       'perJet_Track1dxyOverErr', 'perJet_Track2Pt', 'perJet_Track2dR',
       'perJet_Track2dEta', 'perJet_Track2dPhi',
       'perJet_EnergyFrac_Depth1',
       'perJet_EnergyFrac_Depth2', 'perJet_EnergyFrac_Depth3', 'perJet_EnergyFrac_Depth4', 'perJet_LeadingRechitE',
       'perJet_SubLeadingRechitE', 'perJet_SSubLeadingRechitE',
       'perJet_AllRechitE', 'perJet_NeutralHadEFrac', 'perJet_ChargedHadEFrac']

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
        
        filter_name = "perJet_*"
        filepath = '/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.6/'
    
        sig_fps = [filepath + filename for filename in sig_files] if sig_files is not None else []
        bkg_fps = [filepath + filename for filename in bkg_files] if bkg_files is not None else []
        sig_df = [] #pd.DataFrame()
        bkg_df = [] #pd.DataFrame()
        
        # aggregating all signal events
        for file in sig_fps:
            sig = uproot.open(file)['PerJet_LLPmatched']
            print(f"Opened {file}")
            sig = sig.arrays(filter_name=filter_name, library="pd")
            #sig_df = pd.concat((sig_df, sig))
            sig_df.append(sig)
        self.sig_df = pd.concat(sig_df) if sig_df else pd.DataFrame()
        
        # aggregating all background events
        for file in bkg_fps:
            bkg = uproot.open(file)['PerJet_WPlusJets']
            print(f"Opened {file}")
            bkg = bkg.arrays(filter_name=filter_name, library="pd")
            #bkg_df = pd.concat((bkg_df, bkg))
            bkg_df.append(bkg)
        self.bkg_df = pd.concat(bkg_df) if bkg_df else pd.DataFrame()
        

    
        #cumulative_df = pd.concat((sig_df, bkg_df)) # combining into one large dataset with labels
        
        #self.cumulative_df = cumulative_df
        
        print("Loaded Data")
        
        
    def apply_selections(self, inclusive=False):
        print("Applying Selections")
        bkg_value = self.return_value_bkg
        sig_value = self.return_sig_value
        
        # applying selection to remove Null values/ maxed out values/ etc.
        def select_HCAL12(row):
            return 177 <= row['perJet_MatchedLLP_DecayR'] < 214.2 and abs(row['perJet_MatchedLLP_Eta']) < 1.26

        def select_HCAL34(row):
            return 214.2 <= row['perJet_MatchedLLP_DecayR'] < 295 and abs(row['perJet_MatchedLLP_Eta']) < 1.26

        def select_safety(row):
            return (row['perJet_Pt'] > 40 and abs(row['perJet_Eta']) < 1.26 and 
            0 <= row['perJet_EnergyFrac_Depth1'] <= 1 and 
            0 <= row['perJet_EnergyFrac_Depth2'] <= 1 and 
            0 <= row['perJet_EnergyFrac_Depth3'] <= 1 and 
            0 <= row['perJet_EnergyFrac_Depth4'] <= 1 and 
            0 < row['perJet_S_phiphi'] < 900 and 
            0 < row['perJet_LeadingRechitE'] < 900 and 
            0 <= row['perJet_Track0Pt'] < 900 and 
            0 <= row['perJet_Track0dR'] < 1 and 
            0 < row['perJet_Track1Pt'] < 900 and 
            0 <= row['perJet_Track1dR'] < 1)

        def classify_bkg(row): 
            if (row['perJet_Pt'] > 40 and abs(row['perJet_Eta']) < 1.26 and 
            0 <= row['perJet_EnergyFrac_Depth1'] <= 1 and 
            0 <= row['perJet_EnergyFrac_Depth2'] <= 1 and 
            0 <= row['perJet_EnergyFrac_Depth3'] <= 1 and 
            0 <= row['perJet_EnergyFrac_Depth4'] <= 1 and 
            0 < row['perJet_S_phiphi'] < 900 and 
            0 < row['perJet_LeadingRechitE'] < 900 and 
            0 <= row['perJet_Track0Pt'] < 900 and 
            0 <= row['perJet_Track0dR'] < 1 and 
            0 < row['perJet_Track1Pt'] < 900 and 
            0 <= row['perJet_Track1dR'] < 1):
                return bkg_value # 2 or 1 
            else:
                return -1

        def classify_signal(row):
            if select_safety(row) and select_HCAL12(row):
                return 0
            elif select_safety(row) and select_HCAL34(row):
                return sig_value # 1 or 0
            else:
                return -1
            
        def classify_sig_inclusive(row):
            if select_safety(row):
                return 0
            else:
                return -1
        
        classify_sig = classify_sig_inclusive if inclusive else classify_signal

        sig_df = pd.DataFrame()
        bkg_df = pd.DataFrame()
        
        if not self.sig_df.empty and self.sel:
            sig_df = self.sig_df.copy(deep=True)
            # applying selections cut to signal
            sig_df["classID"] = sig_df.apply(classify_sig, axis=1)
            sig_df = sig_df[sig_df["classID"] != -1].reset_index(drop=True) # removing junk
        
        if not self.bkg_df.empty and self.sel:
            bkg_df = self.bkg_df.copy(deep=True)
            # applying selections cut to background
            bkg_df["classID"] = bkg_df.apply(classify_bkg, axis=1)
            bkg_df = bkg_df[bkg_df["classID"] != -1].reset_index(drop=True) # removing junk
            
        self.cumulative_df = pd.concat((sig_df, bkg_df))
        print("-------------------Cumulative Data---------")
        print(self.cumulative_df.describe())


    def no_selections_concatenate(self):
        self.cumulative_df = pd.concat((self.sig_df, self.bkg_df))
        print("-------------------All Data // No Cuts applied---------")
        print(self.cumulative_df.describe())
            
        
    def process_data(self):
        
        print("Processing...")
        features = ['perJet_LeadingRechitD', 'perJet_E', 'perJet_Pt', 'perJet_Eta',
       'perJet_Phi', 'perJet_Mass', 'perJet_Area',
 'perJet_S_phiphi', 'perJet_S_etaeta',
       'perJet_S_etaphi', 'perJet_Tracks_dR', 'perJet_Track0Pt',
       'perJet_Track0dR', 'perJet_Track0dEta', 'perJet_Track0dPhi',
       'perJet_Track0dzToPV', 'perJet_Track0dxyToBS', 'perJet_Track0dzOverErr',
       'perJet_Track0dxyOverErr', 'perJet_Track1Pt', 'perJet_Track1dR',
       'perJet_Track1dEta', 'perJet_Track1dPhi', 'perJet_Track1dzToPV',
       'perJet_Track1dxyToBS', 'perJet_Track1dzOverErr',
       'perJet_Track1dxyOverErr', 'perJet_Track2Pt', 'perJet_Track2dR',
       'perJet_Track2dEta', 'perJet_Track2dPhi',
       'perJet_EnergyFrac_Depth1',
       'perJet_EnergyFrac_Depth2', 'perJet_EnergyFrac_Depth3', 'perJet_EnergyFrac_Depth4', 'perJet_LeadingRechitE',
       'perJet_SubLeadingRechitE', 'perJet_SSubLeadingRechitE',
       'perJet_AllRechitE', 'perJet_NeutralHadEFrac', 'perJet_ChargedHadEFrac']
        
        
        labels = None 
        if not self.mode and self.sel:
            self.cumulative_df = self.cumulative_df.sample(frac=1, random_state=42).reset_index(drop=True) # shuffling
            labels = self.cumulative_df["classID"].values
        
        data = self.cumulative_df[features]
        normed_data = pd.DataFrame()
        for feature in data.columns:
            normed_data[feature] = (data[feature] - CONSTANTS[feature][0])/ CONSTANTS[feature][1]
        
        print(normed_data.describe())
        
        print("Processing Complete")
        self.data = normed_data
        return normed_data, labels

    
sig = [
    "minituple_v3.6_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-15_CTau1000_13p6TeV_2024_03_02_TRAIN.root",
        "minituple_v3.6_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-15_CTau1000_13p6TeV_2024_03_02_TEST.root",
    "minituple_v3.6_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-50_CTau3000_13p6TeV_2024_03_02_batch1.root",
        "minituple_v3.6_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-50_CTau3000_13p6TeV_2024_03_02_batch2.root",
    "minituple_v3.6_LLP_MC_ggH_HToSSTobbbb_MH-250_MS-120_CTau10000_13p6TeV_2024_03_02_batch1.root",
        "minituple_v3.6_LLP_MC_ggH_HToSSTobbbb_MH-250_MS-120_CTau10000_13p6TeV_2024_03_02_batch2.root",
    "minituple_v3.6_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-160_CTau10000_13p6TeV_2024_03_02_batch1.root",
        "minituple_v3.6_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-160_CTau10000_13p6TeV_2024_03_02_batch2.root",
    "minituple_v3.6_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-80_CTau500_13p6TeV_2024_03_02_TRAIN.root",
        "minituple_v3.6_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-80_CTau500_13p6TeV_2024_03_02_TEST.root"]
    
bkg = [
        "minituple_v3.6_LLPskim_Run2023Cv1_2024_03_02.root",
        "minituple_v3.6_LLPskim_Run2023Cv3_2024_03_02.root",
        "minituple_v3.6_LLPskim_Run2023Dv1_2024_03_02.root",
        "minituple_v3.6_LLPskim_Run2023Bv1_2024_03_02.root",
        "minituple_v3.6_LLPskim_Run2023Cv2_2024_03_02.root",
        "minituple_v3.6_LLPskim_Run2023Cv4_2024_03_02.root",
        "minituple_v3.6_LLPskim_Run2023Dv2_2024_03_02.root"
    ]    

processor = DataProcessor(num_classes=2)  
processor.load_data(sig, bkg)
processor.apply_selections(inclusive=False)
X, y = self.processor.process_data()
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)
    
# -----------------------------
run["sys/tags"].add(["Optune-Project", "BBH-PE"])

# Define the objective function for Optuna
def objective(trial):

    # ======================== Your code here ========================
    # HYPER PARMETER TUNING!
    # Using the trial.suggest_XXX() functions, propose some hyper-parameters 
    # to tune. Examples of different possibiliies using different suggest options 
    # (categorial, float, int) are given below. Experiment with what else you 
    # can try. 
    
    learning_rate = trial.suggest_float('learning_rate', 5e-5, 3e-3, log=True)
    num_neurons1 = trial.suggest_int('num_neurons1', 16, 256)
    num_neurons2 = trial.suggest_int('num_neurons2', 16, 256)
    num_neurons3 = trial.suggest_int('num_neurons3', 16, 256)
    num_layers = trial.suggest_int('num_layers', 0, 10)
    is_dropout = trial.suggest_categorical('is_dropout', [0, 1])
    dropout = trial.suggest_float('dropout', 0.01, 0.4, log=True)
    is_batchnorm = trial.suggest_categorical('is_batchnorm', [0, 1])
    #optimizer = trial.suggest_categorical('optimizer', [Adam, Nadam, Adamax])
    
    model = Sequential()
    model.add(Dense(num_neurons1, activation="sigmoid"))
    if is_batchnorm:
        model.add(BatchNormalization())
    for _ in range(num_layers):
        model.add(Dense(num_neurons2, activation="sigmoid"))
        if is_batchnorm:
            model.add(BatchNormalization())
        if is_dropout:
            model.add(Dropout(dropout))
    model.add(Dense(num_neurons3, activation="sigmoid"))
    model.add(Dense(3, activation="softmax"))
              
    # Compile 
    
    model.compile(optimizer=Adam(learning_rate=learning_rate), loss="sparse_categorical_crossentropy")
    
    history = model.fit(X_train, 
                        y_train, 
                        epochs          = 10, ## Fill in 
                        batch_size      = 512, ## Fill in 
                        verbose         = 1, 
                        validation_data = (X_test, y_test),
                        callbacks       = [NeptuneCallback(run = run)])
    val_loss = np.min(history.history['val_loss'])

    run["train/loss"].append(val_loss)
    
    return val_loss

# Create Optuna study and optimize
neptune_callback = npt_utils.NeptuneCallback(run)
study = optuna.create_study(direction = 'minimize')
study.optimize(
    objective, 
    n_trials = 10, ## Fill in  
    callbacks=[neptune_callback]
)
run.stop()

# Print out the best model that the network found
best_model = study.best_trial
print(f"Best Model: {best_model}")

# Submit this script to Assignment 3.5 and upload plots from your different 
# trainings. What is the best combination of hyper-parameters you can find?
# What hyper-parameters have the biggest influence in network performance?