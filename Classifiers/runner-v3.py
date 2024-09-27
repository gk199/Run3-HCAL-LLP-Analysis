import numpy as np
import tensorflow as tf
from tensorflow.keras.callbacks import ModelCheckpoint
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
from sklearn.feature_selection import mutual_info_regression
from tensorflow.keras.callbacks import EarlyStopping
from tensorflow.keras.initializers import GlorotUniform, GlorotNormal
from tensorflow.keras import layers, models

tf.random.set_seed(311)

# CONSTANTS = pd.read_csv("norm_constants_v3.csv") # large negative values removed from mean / std dev computation 
CONSTANTS = pd.read_csv("norm_constants.csv")
FEATURES = ['perJet_LeadingRechitD', 'perJet_E', 'perJet_Pt', 'perJet_Eta',
       'perJet_Phi', 'perJet_Mass', # 'perJet_Area',
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
        filepath = '/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.8/'
    
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
            0 <= row['perJet_Track1Pt'] < 900 and 
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
            0 <= row['perJet_Track1Pt'] < 900 and 
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
       'perJet_Phi', 'perJet_Mass', # 'perJet_Area',
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
        constants_df.T.to_csv("norm_constants_v3.csv")
       
class ModelHandler:
    def __init__(self, num_classes=3, num_layers=2, optimizer="adam", lr=0.00027848106048644665, model_name="dense_model_v3.keras"):
        
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
            #x = layers.BatchNormalization()(x)
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
                  
    def train(self, X_train, y_train, epochs=50, batch_size=512, val=0.1):
        self.build_resnet() # Residual neural network 
        # self.build() # similar to runner-v2
        name="best_model_v3.keras"
        early_stopping = EarlyStopping(monitor='val_loss', patience=10, restore_best_weights=True)
        checkpoint = ModelCheckpoint(name, monitor='val_loss', save_best_only=True, save_weights_only=True)
        # auc_epoch = [roc_callback(training_data=(x_train, y_train),validation_data=(x_test,y_test))]
        # self.model.fit(X_train, y_train, epochs=epochs, batch_size=512, validation_split=val, verbose=1, callbacks = [early_stopping]) # this goes to 50 epochs
        history = self.model.fit(X_train, y_train, epochs=epochs, batch_size=512, validation_split=val, verbose=1, callbacks = [early_stopping, checkpoint])
        self.model.load_weights(name)

        # plot loss and val_loss vs number of epochs 
        plt.figure()
        plt.plot(history.history['loss'])
        plt.plot(history.history['val_loss'])
        plt.title('Model Loss')
        plt.ylabel('Loss')
        plt.xlabel('Epoch')
        plt.legend(['train', 'val'], loc='upper left')
        plt.savefig("ModelLoss.png")
        
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
            print("wrote to file")
        
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
            self.save_roc_data(fpr, tpr)
            ax.semilogx(fpr, tpr, color=color, label=f"Class {self.names[signal_class]} vs Rest, AUC: {roc_auc:.3f}")
            
        ax.set_xlabel('False Positive Rate (FPR)')
        ax.set_ylabel('True Positive Rate (TPR)')
        ax.set_title('One-vs-All ROC Curves')
        ax.legend(loc="lower right")
        ax.grid(True)  
        fig.savefig("ROC1vA.png")
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
        mi_results_sorted.to_csv("mutual_info.csv")
        print("computed mutual information")
        
       
                
        
class Runner:
    def __init__(self, sig_files=None, bkg_files=None, mode="train", num_classes=3, inclusive=False, load=True):
        self.mode = mode
        self.num_classes = num_classes
        self.inclusive = inclusive
        self.load = load
        self.sig = sig_files
        self.bkg = bkg_files
        self.model_name = "dense_model_v3.keras"
     
    def run_training(self):
        # modifying to do per-event splitting
        print("Training")
        if self.load:
            self.processor = DataProcessor(num_classes=self.num_classes - 1)
            self.processor.load_data(self.sig, self.bkg)
        self.processor.apply_selections(inclusive=self.inclusive)
        #X_train, y_train = processor.process_data()
        
        #processor.load_data(self.sig_test, self.bkg_test)
        #X_test, y_test = processor.process_data()
        X, y = self.processor.process_data() # no longer processing train and test samples separately
        X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)
        
        
        handler = ModelHandler(num_classes=self.num_classes, model_name=self.model_name)
        handler.train(X_train, y_train)
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
            self.processor = DataProcessor(mode="eval", num_classes=self.num_classes - 1)
            self.processor.load_data(self.sig, self.bkg)
        self.processor.apply_selections(inclusive=self.inclusive)
        
        X_eval, y_eval = self.processor.process_data()
        
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
            
        predicting_data, labels = self.processor.process_data()
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
    
    def set_model_name(self, model_name="dense_model.keras"):
        self.model_name = model_name
    
    
        
        
def main():
    sig_files = [
        "minituple_v3.8_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-15_CTau1000_13p6TeV_2024_06_03_TRAIN.root",
        "minituple_v3.8_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-50_CTau3000_13p6TeV_2024_06_03_batch1.root",
        "minituple_v3.8_LLP_MC_ggH_HToSSTobbbb_MH-250_MS-120_CTau10000_13p6TeV_2024_06_03_batch1.root",
        "minituple_v3.8_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-160_CTau10000_13p6TeV_2024_06_03_batch1.root",
        "minituple_v3.8_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-80_CTau500_13p6TeV_2024_06_03_TRAIN.root",
        # "minituple_v3.8_LLP_MC_ggH_HToSSTobbbb_MH-HADD_TRAIN-batch1.root",
        "minituple_v3.8_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-15_CTau1000_13p6TeV_2024_06_03_TEST.root",
        "minituple_v3.8_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-50_CTau3000_13p6TeV_2024_06_03_batch2.root",
        "minituple_v3.8_LLP_MC_ggH_HToSSTobbbb_MH-250_MS-120_CTau10000_13p6TeV_2024_06_03_batch2.root",
        "minituple_v3.8_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-160_CTau10000_13p6TeV_2024_06_03_batch2.root",
        "minituple_v3.8_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-80_CTau500_13p6TeV_2024_06_03_TEST.root",
        # "minituple_v3.8_LLP_MC_ggH_HToSSTobbbb_MH-HADD_TEST-batch2.root"
    ]
    
    bkg_files = [
        "minituple_v3.8_LLPskim_Run2023Bv1_2024_06_03.root",
        "minituple_v3.8_LLPskim_Run2023Cv1_2024_06_03.root",
        "minituple_v3.8_LLPskim_Run2023Cv2_2024_06_03.root",
        "minituple_v3.8_LLPskim_Run2023Cv3_2024_06_03.root",
        "minituple_v3.8_LLPskim_Run2023Cv4_2024_06_03.root",
        "minituple_v3.8_LLPskim_Run2023Dv1_2024_06_03.root",
        "minituple_v3.8_LLPskim_Run2023Dv2_2024_06_03.root"
    ]
    
    mode = "train" # "train", "eval", "filewrite", "constants"
    
    # running the depth and inclusive tagger sequentially, uncomment second part if want to run the depth tagger alone
    print("Running Depth Tagger")
    runner = Runner(sig_files=sig_files[:], bkg_files=bkg_files[:], mode=mode, num_classes=2, inclusive=False)
    runner.run()
    
    print("Running Inclusive Tagger")
    # first resetting some parameters for the inclusive tagger
    #runner.set_inclusive(inclusive=True)
    #runner.set_load(load=False)
    runner = Runner(sig_files=sig_files[:], bkg_files=bkg_files[:], mode=mode, num_classes=2, inclusive=True)
    runner.set_model_name(model_name="inclusive_model_v3.keras")
    # runner.run()
    
    
    # running the inclusive tagger by itself, uncomment if needed
    #print("Running Inclusive Tagger")
    #runner = Runner(sig_files=sig_files[:], bkg_files=bkg_files[:], mode=mode, num_classes=2, inclusive=True)
    #runner.set_load(load=False)
    #runner.run()
      
    
    
    
if __name__ == "__main__":
    main()
    
