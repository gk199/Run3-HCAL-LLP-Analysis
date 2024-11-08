import tensorflow as tf

print("Num GPUs Available: ", len(tf.config.list_physical_devices('GPU')))

import h5py
import numpy as np
import matplotlib.pyplot as plt
import uproot
#from models import Classifier2DGrad as Classifier2D
from models import Classifier2DCNN as Classifier2D
from models import MultiInput
from generator import RegionETGenerator
from sklearn.model_selection import train_test_split
from tensorflow import data
import matplotlib.pyplot as plt
from drawing import Draw
import pandas as pd
from sklearn.utils import shuffle
from sklearn.preprocessing import StandardScaler
from models import Classifier3DCNN as Classifier3D
from sklearn.metrics import roc_curve, auc
import pandas as pd
from PIL import Image
import matplotlib.colors as mcolors
from tensorflow.keras.callbacks import EarlyStopping


class DataProcessor:
    def __init__(self, sig_files=None, bkg_files=None, dim=2, testing=False, multi=False, features="etaptpv"):
        self.dim = dim
        self.features = features
        self.sig_files = sig_files
        self.bkg_files = bkg_files
        self.testing = testing
        self.multi = multi
        self.aug_variable = 0
    
    def load_data(self):
        
        # initialising the things we will eventually concatenate and store all data for signal
        sig_images = tuple()
        sig_numerical = tuple()
        
        if self.sig_files is not None:
            for sig_file in self.sig_files:
                print("Opening ", sig_file)
                with h5py.File(sig_file, "r") as f:
                    # all the images from one file
                    print("here first")
                    images = np.array(f['CaloRegions'])
                    print("here 2")
                    #images = images[:, :10, :10,:]       
                    print("here")
                    if self.multi:
                        variables = tuple()
                        if "eta" in self.features:
                            eta = np.array(f['jet_eta']).reshape(-1,1)
                            variables += (eta,)
                        if "pt" in self.features:
                            pt = np.array(f['jet_pt']).reshape(-1,1)
                            variables += (pt,)
                        if "pv" in self.features:
                            pv = np.array(f['jet_npv']).reshape(-1,1)
                            variables += (pv,)
                        # this stores all the different metrics we want to feed the multi-input for a single file
                        numerical = np.concatenate(variables, axis=1)
                    
                        # now concatenating these metrics along 0 axis -- combining data from different files
                        sig_numerical += (numerical,)      
                    
                # concatenating the image data acorss different files        
                sig_images += (images,)
            
            # np array of all the signal image data
            print("here2")
            X_sig = np.concatenate(sig_images)
            if self.multi:
                # np array of all signal numerical data
                X_num_sig = np.concatenate(sig_numerical)
            print("All Signal: ", X_sig.shape)
            
        # repeating for background   
        bkg_images = tuple()
        bkg_numerical = tuple()
        
        if self.bkg_files is not None:
            for bkg_file in self.bkg_files:
                print("Opening ", bkg_file)
                with h5py.File(bkg_file, "r") as f:
                    images = np.array(f['CaloRegions'])
                    #images = images[:, :10, :10,:]
                    variables = tuple()                    
                    if self.multi:
                        if "eta" in self.features:
                            eta = np.array(f['jet_eta']).reshape(-1,1)
                            variables += (eta,)
                        if "pt" in self.features:
                            pt = np.array(f['jet_pt']).reshape(-1,1)
                            variables += (pt,)
                        if "pv" in self.features:
                            pv = np.array(f['jet_npv']).reshape(-1,1)
                            variables += (pv,)
                        numerical = np.concatenate(variables, axis=1)
                   
                        bkg_numerical += (numerical,)      
                    
                    
                bkg_images += (images,)
            
            X_bkg = np.concatenate(bkg_images)
            print("All bkg: ", X_bkg.shape)
        
            if self.multi:
                X_num_bkg = np.concatenate(bkg_numerical)   
                print("All bkg num", X_num_bkg.shape)
        
        if self.sig_files is None:
            self.X = X_bkg
            self.y = np.zeros(X_bkg.shape[0])
        
        elif self.bkg_files is None:
            self.X = X_sig
            self.y = np.ones(X_sig.shape[0])
            
        else:
            self.X = np.concatenate((X_sig, X_bkg)) # all data
            self.y = np.concatenate((np.ones(X_sig.shape[0]), np.zeros(X_bkg.shape[0]) ))
            
        
        #self.X = self.X[:, :10, :10] # cutting all the data once it's all aggregated 
        
                    
        
        
        print("All data: ", self.X.shape)
        if self.multi:
            self.X_num = np.concatenate((X_num_sig, X_num_bkg))
            print("X_num all: ", self.X_num.shape)
      
        
    def check_orientation(self, img, img_flip, name):
        norm1 = mcolors.Normalize(vmin=0, vmax=img.max())
        norm2 = mcolors.Normalize(vmin=0, vmax=img_flip.max())
    
        if self.dim == 2:
            fig, ax = plt.subplots(2)
            ax[0].imshow(img[:,:,0], norm=norm1)
            ax[1].imshow(img_flip[:,:,0], norm=norm2)
            fig.savefig(f"{name}2D.png")
        else: 
            fig, ax = plt.subplots(2, 4)
            for i in range(4):
                ax[0, i].imshow(img[:,:,i], norm=norm1, label=f"depth {i+1}")
                ax[1, i].imshow(img_flip[:,:,i], norm=norm2, label=f"depth {i +1}")
            fig.savefig(f"{name}3D.png")
    
    def augment_data(self):
        
        h = -3
        v = -4
        
        if self.dim == 2:
            h += 1
            v += 1
        X_h = np.flip(self.X, axis=h)
        self.check_orientation(self.X[0], X_h[0], "hor")
        X_v = np.flip(self.X, axis=v)
        self.check_orientation(self.X[0], X_v[0], "ver")
        X_b = np.flip(self.X, axis=(h,v))  
        self.check_orientation(self.X[0], X_b[0], "bot")
        
        
        # ----- temporary changes -----
        
        self.X = np.concatenate((self.X, X_h, X_v, X_b))
        self.y = np.concatenate((self.y, self.y, self.y, self.y))
        
        # looking how it performs
        if self.aug_variable == 10:
            self.X = np.concatenate((self.X, X_h, X_v, X_b))
            self.y = np.concatenate((self.y, self.y, self.y, self.y))
        
        if self.aug_variable == 111:
            self.X = X_h 
            print("Horizontal selection")
            
        elif self.aug_variable == 12:
            self.X = X_v
            print("Vertical Selectio")
        
        elif self.aug_variable == 13:
            self.X = X_b
            print("Diagonal Selection")
            
        #self.aug_variable +=1
        
        if self.multi:
            self.X_num = np.concatenate((self.X_num, self.X_num, self.X_num, self.X_num))
                                
    def process_data(self):
        shape = self.X.shape + (1,)
        self.X = np.array([img / img.max() for img in self.X]).reshape(shape)
        if self.multi:
            scaler = StandardScaler()
            self.X_num = scaler.fit_transform(self.X_num)
            print("mean: ", scaler.mean_)
            print("std: ", scaler.scale_)
            if not self.testing:
                self.X, self.X_num, self.y = shuffle(self.X, self.X_num, self.y, random_state=42)
            return
        
        #if not self.testing: 
            #self.X, self.y = shuffle(self.X, self.y, random_state=42) 
        #print("Data shape: ", self.X.shape)
     
    def get_data(self, augment=True):
        self.load_data()
        self.process_data()
        if augment:
            self.augment_data()
        if not self.testing:  
            #print("shuffle")
            self.X, self.y = shuffle(self.X, self.y, random_state=42)
        print("Total Data: ", self.y.shape)
        
        if not self.testing and not self.multi:
            self.X_train, self.X_test, self.y_train, self.y_test = train_test_split(self.X, self.y, test_size=0.2, random_state=42)
            #if augment:
                #self.X, _, self.y, _ = train_test_split(self.X_train, self.y_train, test_size=0.75, random_state=42) # only keeping 1/4
                #self.augment_data()
                #print(f"augmented shape train: {self.X.shape}")
                #return self.X, self.y
            print("shape train: ", self.X_train.shape, "shape test", self.X_test.shape)
            return self.X_train, self.X_test, self.y_train, self.y_test
        
        elif not self.testing and self.multi:
            self.X_train, self.X_test, self.X_num_train, self.X_num_test, self.y_train, self.y_test = train_test_split(self.X, self.X_num, self.y, test_size=0.4, random_state=42)
            return self.X_train, self.X_test, self.X_num_train, self.X_num_test, self.y_train, self.y_test
        
        elif self.testing and self.multi:
            print("Multi - Eval")
            return self.X, self.X_num, self.y
                
        return self.X, self.y                       
        

class ModelHandler:
    def __init__(self, model, dim, evaluate=True, lr=1e-3, multi=False, model_name=None):                     
            self.model = model
            self.dim = dim 
            self.model_name = f"classifier{dim}d.keras" if model_name is None else model_name  
            self.multi = multi
            if not evaluate:                     
                self.model.compile(optimizer=tf.keras.optimizers.Adam(learning_rate=lr), loss="sparse_categorical_crossentropy")
            
                
    def train(self, data_train, batch=512, val=0.2, epochs=10):
        
        early_stopping = EarlyStopping(monitor='val_loss', patience=2, restore_best_weights=True)
        
        if self.multi:
            X_train, X_num_train, y_train = data_train
            self.model.fit(x=[X_train, X_num_train], y=y_train, batch_size=batch, epochs=epochs, validation_split=val, verbose=1, callbacks=[early_stopping])
            return
        
        X_train, y_train = data_train                     
        self.model.fit(X_train, y_train, batch_size=batch, epochs=epochs, validation_split=val, verbose=1, callbacks=[early_stopping])
      
    def test(self, data_test):
        if self.multi:
            X_test, X_num_test, y_test = data_test
            preds = self.model.predict(x=[X_test, X_num_test])
            
        else:
            X_test, y_test = data_test                      
            preds = self.model.predict(X_test) 
        predictions = np.argmax(preds, axis=1)          
        accuracy = np.sum(predictions == y_test) / len(y_test)
        print("Accuracy: ", accuracy)
                                
        self.data = (X_test, y_test, predictions)
        self.roc_data = (y_test, preds[:, 1])
        print("preds: ", preds[:10]) 
        print("predictions: ", predictions[:10])
        print("labels: ", y_test[:10])
        
    def save(self):
        self.model.save(f"models/{self.model_name}")
        
        
    def get_grads(self, img):
        axis = (0, 1, 2)
        if self.dim == 3:
             axis += (3,)
                                
        self.last_conv_layer = self.model.get_layer("lastconv")                      
        grad_model = tf.keras.Model([self.model.inputs], [self.last_conv_layer.output, self.model.output])   
        
        with tf.GradientTape() as tape:
        # Record the operations for automatic differentiation
            inputs = tf.convert_to_tensor(img[np.newaxis, ...])
            conv_outputs, predictions = grad_model(inputs)
            tape.watch(conv_outputs)
            class_index = np.argmax(predictions) 
            pred_class = predictions[:, class_index]

        grads = tape.gradient(pred_class, conv_outputs)
        pooled_grads = tf.reduce_mean(grads, axis=axis) 
          
        return pooled_grads
                                
    def get_conv_output2D(self, img):
        pooled_grads = self.get_grads(img)                     
        batch, width, height, filters = self.last_conv_layer.input_shape
        img_resized = img.reshape(1, 10, 10, 1)
        img_resized = np.repeat(img_resized, filters, axis=-1) #make appropriate for pass
       
        conv_output = self.last_conv_layer(img_resized)
        weighted_output = conv_output * pooled_grads[tf.newaxis, tf.newaxis, tf.newaxis, :]                
        return conv_output, weighted_output     
                                
    def get_conv_output3D(self, img):
        pooled_grads = self.get_grads(img) 
        print("pooled grads", pooled_grads[:10])
        batch, width, height, depth, filters = self.last_conv_layer.input_shape
        
        resized_img = img.reshape(1, 10, 10, 4, 1)
        resized_image = np.repeat(resized_img, filters, axis=-1)
        
        conv_output = self.last_conv_layer(resized_image)  # Convert to numpy array
        
        weighted_output = conv_output * pooled_grads[tf.newaxis, tf.newaxis, tf.newaxis, tf.newaxis, :]
        return conv_output, weighted_output                    
                                
                                
    def gradCAM(self, data, num_samples=15):
        np.random.seed(42)
        X, y, predictions = data                    
        selected_indices = np.random.choice(X.shape[0], min(num_samples, X.shape[0]), replace=False)
        selected_images = X[selected_indices]
        selected_labels = y[selected_indices]
        selected_preds = predictions[selected_indices]
                                
        get_output = self.get_conv_output2D if self.dim ==2 else self.get_conv_output3D                      
        gradcams = []
        heatmaps = []                     
        for i, img in enumerate(selected_images):
            conv_output, weighted_output = get_output(img)               
            gradcam = tf.reduce_mean(weighted_output, axis=-1)
            gradcam /= np.max(gradcam)                 
            gradcam = np.maximum(0, gradcam)
            gradcam = tf.squeeze(gradcam)                 
            gradcams.append(gradcam)                 
                    
            heatmap = tf.reduce_mean(conv_output, axis=-1)                 
            heatmap /= np.max(heatmap)
            heatmap = np.maximum(0, heatmap)
            heatmap = tf.squeeze(heatmap)                     
            heatmaps.append(heatmap)
                                
        return selected_images, selected_labels, selected_preds,heatmaps, gradcams 
                                
                                
    def plot_gradcam2D(self):
        images, labels, predictions, heatmaps, gradcams = self.gradCAM(self.data)
        n = len(heatmaps) 
        print("heatmap: ", heatmaps[0])
        fig, ax = plt.subplots(n, 3, figsize=(15, 5 * n))
        for i in range(n):
            norm = mcolors.Normalize(vmin=0, vmax=images[i].max())
            ax[i, 0].imshow(images[i], norm=norm)               
            ax[i, 0].axis("off")
            ax[i, 0].set_title(f"L: {labels[i]}, P: {predictions[i]}")
                
            ax[i, 1].imshow(gradcams[i], cmap="magma")               
            ax[i, 1].axis("off")
            ax[i, 1].set_title("GradCAM")                
                
            ax[i, 2].imshow(heatmaps[i], cmap="magma")               
            ax[i, 2].axis("off")
            ax[i, 2].set_title("Conv Output")
        
        plt.tight_layout()
        fig.savefig("GradCam2D.png")                   
                                
          
    def plot_gradcam3D(self):
        images, labels, predictions, heatmaps, gradcams = self.gradCAM(self.data)
        n = len(heatmaps)                   
        fig, ax = plt.subplots(n, 8, figsize=(15, 5 * n))                   
        for i in range(n):
            norm1 = mcolors.Normalize(vmin=0, vmax=images[i].max())
            norm2 = mcolors.Normalize(vmin=0, vmax=np.max(gradcams[i]))
            for k in range(4):
                ax[i, k].imshow(images[i][:,:,k], norm=norm1)
                ax[i, k].axis("off")           
                ax[i, k].set_title(f"L {labels[i]}, P{predictions[i]}")
                                
                ax[i, k+4].imshow(gradcams[i][:,:,k], norm=norm2, cmap="magma")           
                ax[i, k+4].axis("off")           
                ax[i, k+4].set_title(f"Depth{k+1}")
                                
        fig.savefig("GradCAM3D.png")
           
    def plot_roc(self, ax):
        y_true, score = self.roc_data                
        fpr, tpr, _ = roc_curve(y_true, score)                  
        roc_auc = auc(fpr, tpr)
        ax.semilogx(fpr, tpr)
        print("--------ROC Data-----")
        print("fpr shape ", fpr.shape)
        print("First non-zero TPR", tpr[fpr !=0][0])
        index = np.where( fpr <= 1e-4)
        print("TPR", tpr[index][-1], "at FPR", fpr[index][-1])
    
    #--------WIP--------
    def write_to_root(self, root_file):
        # should be evaluated one at a time
        events = uproot.open(root_file)['JetHCalRechits']# these files aren't per-event so doesn't make sense
        df = events.arrays(library="np")
        
        labels, scores = self.roc_data
        
        df['CNN3D_scores'] = scores
        
        output_file = "scores_added_350_80.root"
        with uproot.recreate(output_file) as f:
            f["JetHCalRechits"] =  {key : df[key] for key in df.keys()}
            
        print("Wrote to ROOT File")
                                
                                
def main(evaluate=False):
    
    filepaths = [ {"sig_files": ['Inputs/h5/v3j1/jets_ggH_HToSSTobbbb_MH-350_MS-160_CTau10000.h5'], "bkg_files": ['Inputs/h5/v3j1/jets_Run2023C-EXOLLPJetHCAL-PromptReco-v1.h5']  }, 
                 
                 {"sig_files": ['Inputs/Jet3Dh5/HADD_jetntuple_ggH_HToSSTo4B_MH350_MS160_CTau500_mod.h5',
                     'Inputs/Jet3Dh5/HADD_jetntuple_ggH_HToSSTo4B_MH250_MS120_CTau500_mod.h5', 
                     'Inputs/Jet3Dh5/HADD_jetntuple_ggH_HToSSTo4B_MH125_MS50_CTau500_mod.h5',
                     'Inputs/Jet3Dh5/HADD_jetntuple_ggH_HToSSTo4B_MH125_MS125_CTau500_mod.h5',
                     'Inputs/Jet3Dh5/HADD_jetntuple_ggH_HToSSTo4B_MH350_MS80_CTau500_mod.h5'
                               ], "bkg_files": [#'Inputs/Jet3Dh5/jetntuple_Run2023C-EXOLLPJetHCAL-PromptReco-v1_partial.h5'#,
                                                'Inputs/Jet3Dh5/HADD_Run2023D-EXOLLPJetHCAL-PromptReco-v1_mod.h5',
                     #'Inputs/Jet3Dh5/HADD_Run2023D-EXOLLPJetHCAL-PromptReco-v2.h5' 
                     #,'Inputs/Jet3Dh5/HADD_bkg_partial.h5' 
                                               #
                 ]}]
    
    #root_file = "Inputs/hadded/HADD_jetntuple_ggH_HToSSTo4B_MH350_MS80_CTau500.root"
    fig, roc_ax = plt.subplots()
    roc_ax.set_xlabel("False Positive Rate")
    roc_ax.set_ylabel("True Positive Rate")
    dim = 2
    cut = 30000
    if not evaluate:   
        classifier2 = Classifier2D().get_model() 
        classifier3 = Classifier3D().get_model()
        classifiers = [classifier2, classifier3] 
        print(classifier3.summary())
        params = {3: 0.0010967517265048784, 2: 0.0008168928172943063}
        
        for classifier, filepath in zip(classifiers, filepaths):
            if dim == 2:
                dim+=1 
                continue
            print("Entered Loop: Train Mode")
            processor = DataProcessor(filepath['sig_files'], filepath['bkg_files'], dim=dim, testing=False)
            print("Got Data")
            _, X_TEST, _, y_TEST = processor.get_data(augment=False)
            X_train, _, y_train, _ = processor.get_data(augment=True)
            handler = ModelHandler(model=classifier, dim=dim, evaluate=False, model_name=f"classifier3D-AllMass-initialized.keras")
            handler.train((X_train, y_train))
            print("Trained")
            handler.test((X_TEST, y_TEST))
            print("Tested")
            #gradcam = handler.plot_gradcam2D if dim==2 else handler.plot_gradcam3D                      
            #gradcam()
            handler.plot_roc(roc_ax)                     
            dim += 1
            handler.save()
            
        roc_ax.legend(["3DCNN"])
        roc_ax.grid(True)
        roc_ax.set_title("All Mass Combinations")
        fig.savefig("ROC_plt.png")
        
        #adding in the training of the multi-input
        #classifier = MultiInput(num_features=3).get_model()
        #processor = DataProcessor(filepaths[1]['sig_files'], filepaths[1]['bkg_files'], dim=3, test=False, multi=True)
        #X_train, X_test, X_num_train, X_num_test, y_train, y_test = processor.get_data(augment=True)
        #handler = ModelHandler(model=classifier, dim=3, evaluate=False, multi=True, model_name="multiInput-10.keras")
        #handler.train((X_train, X_num_train, y_train))
        #handler.test((X_test, X_num_test, y_test))
        #handler.plot_roc(roc_ax)
        #handler.save()
      
    else:
        classifier2 = tf.keras.models.load_model("models/classifier2d.keras")
        classifier3 = tf.keras.models.load_model("models/classifier3D-AllMass.keras")
        classifiers = [classifier2, classifier3]
        for classifier, filepath in zip(classifiers, filepaths):
            if dim == 2:
                dim+=1
                continue
            print("Entered Loop: Eval Mode")
            processor = DataProcessor(filepath['sig_files'], filepath['bkg_files'], dim=dim, testing=True)
            print("Got Data")
            data_test = processor.get_data(augment=True) #returns the full dataset unshuffled so that the scores can be inputted back
            handler = ModelHandler(model=classifier, dim=dim, evaluate=True)
            handler.test(data_test)
            print("Tested")
            #gradcam = handler.plot_gradcam2D if dim==2 else handler.plot_gradcam3D                      
            #gradcam()                     
            handler.plot_roc(roc_ax)
            #if dim == 3:
               # handler.write_to_root(root_file)
            #dim += 1
        
        #classifier = tf.keras.models.load_model("models/multiInput.keras")
        #processor = DataProcessor(filepaths[1]['sig_files'], filepaths[1]['bkg_files'], dim=3, test=True, multi=True)
        #data_test = processor.get_data()
        #handler = ModelHandler(model=classifier, dim=3, evaluate=True, multi=True)
        #handler.test(data_test)
        #handler.plot_roc(roc_ax)
        #ROC_data = pd.read_csv("../../Run3-HCAL-LLP-Analysis/TMVAStudies/roc_binary_data.csv")
        #roc_ax.semilogx(np.array(ROC_data['FPR']), np.array(ROC_data['TPR']))
        #roc_ax.set_title("ROC.png")
        roc_ax.legend(["3DCNN"])
        roc_ax.set_title("Evaluated on All Masses, Augmented Dataset")
        fig.savefig("ROC_eval_plt.png")
        
        
        
def evaluate_mass_trainings():
    
    # changing
    sig_files = ['Inputs/Jet3Dh5/HADD_jetntuple_ggH_HToSSTo4B_MH350_MS160_CTau500_mod.h5',
                     'Inputs/Jet3Dh5/HADD_jetntuple_ggH_HToSSTo4B_MH250_MS120_CTau500_mod.h5', 
                     'Inputs/Jet3Dh5/HADD_jetntuple_ggH_HToSSTo4B_MH125_MS50_CTau500_mod.h5',
                     'Inputs/Jet3Dh5/HADD_jetntuple_ggH_HToSSTo4B_MH125_MS125_CTau500_mod.h5',
                     'Inputs/Jet3Dh5/HADD_jetntuple_ggH_HToSSTo4B_MH350_MS80_CTau500_mod.h5'
                               ]
    
    model_names = ['classifier3d-350-160.keras', 'classifier3d-250-120.keras', 'classifier3d-125-50.keras', 'classifier3d-125-15.keras','classifier3d-350-80.keras']
    
    # constant
    background = [#'Inputs/Jet3Dh5/jetntuple_Run2023C-EXOLLPJetHCAL-PromptReco-v1_partial.h5'
        'Inputs/Jet3Dh5/HADD_Run2023D-EXOLLPJetHCAL-PromptReco-v1_mod.h5',
        #'Inputs/Jet3Dh5/HADD_Run2023D-EXOLLPJetHCAL-PromptReco-v2.h5',
                 ]
    
    names = ['MH350-MS160', 'MH250-MS120', 'MH125-MS50', 'MH125-MS15', 'MH350-MS80']
    
    
    for index, model3d  in enumerate(model_names):

        fig1, ax1 = plt.subplots()
        #fig2, ax2 = plt.subplots()

        classifier3 = tf.keras.models.load_model(f"models/{model3d}")
        #multiInput = tf.keras.models.load_model(f"models/{model_multi}")
        
        for signal in sig_files:
            
            # 3DCNN predictions
            processor = DataProcessor([signal], background, dim=3, testing=True)
            data_test = processor.get_data(augment=False)
            handler = ModelHandler(model=classifier3, dim=3, evaluate=True)
            handler.test(data_test)
            handler.plot_roc(ax1)
                
            #predictions on multiInput
            #processor = DataProcessor([signal], background, dim=3, test=True, multi=True)
            #data_test = processor.get_data()
            #handler = ModelHandler(model=multiInput, dim=3, evaluate=True, multi=True)
            #handler.test(data_test)
            #handler.plot_roc(ax2)
            
            ax1.set_xlabel("False Positive Rate")
            ax1.set_ylabel("True Positive Rate")
            ax1.legend(names)
            ax1.set_title(f"Trained on {names[index]} with 3DCNN")
            #ax2.set_xlabel("False Positive Rate")
            #ax2.set_ylabel("True Positive Rate")
            #ax2.legend(names)
            #ax2.set_title(f"Trained on {names[index]} with multi-input")
            
        plt.tight_layout()       
        fig1.savefig(f"Trained{names[index]}-3DCNN.png")
        #fig2.savefig(f"Trained{names[index]}-MultiInput.png")
        plt.close(fig1)
        #plt.close(fig2)
        
        
def verify_augmented():
    
    print("Verifying Augmented")
    
    fig, roc_ax = plt.subplots()
    roc_ax.set_xlabel("False Positive Rate")
    roc_ax.set_ylabel("True Positive Rate")
    
    signal = ['Inputs/Jet3Dh5/HADD_jetntuple_ggH_HToSSTo4B_MH350_MS160_CTau500_mod.h5',
                     #'Inputs/Jet3Dh5/HADD_HToSSTobbbb_MH250_MS120_CTau500_mod.h5', 
                     #'Inputs/Jet3Dh5/HADD_jetntuple_ggH_HToSSTobbbb_HToSSTo4B_MH125_MS50_CTau500_mod.h5',
                 #'Inputs/Jet3Dh5/HADD_jetntuple_ggH_HToSSTo4B_MH125_MS125_CTau500_mod.h5',
                #'Inputs/Jet3Dh5/HADD_jetntuple_ggH_HToSSTo4B_MH350_MS80_CTau500_mod.h5'
             ]
    
    background = [
        #'Inputs/Jet3Dh5/jetntuple_Run2023C-EXOLLPJetHCAL-PromptReco-v1_partial.h5'
        'Inputs/Jet3Dh5/HADD_Run2023D-EXOLLPJetHCAL-PromptReco-v1_mod.h5',
        #'Inputs/Jet3Dh5/HADD_Run2023D-EXOLLPJetHCAL-PromptReco-v2.h5'
                 ]
    
    classifier1 = Classifier3D().get_model()
    processor = DataProcessor(signal, background, dim=3, testing=False)
    
    # non-augmented
    print("Standard")
    X_train, X_TEST, y_train, Y_TEST = processor.get_data(augment=False)
    handler = ModelHandler(model=classifier1, dim=3, evaluate=False)    
    print("data train: ", len(X_train))
    handler.train((X_train, y_train))
    handler.test((X_TEST, Y_TEST))
    handler.plot_roc(roc_ax)
    
    
    print("Quarter of Original data, Augmented ")
    classifier2 = Classifier3D().get_model()
    X_train, y_train = processor.get_data(augment=True)
    handler = ModelHandler(model=classifier2, dim=3, evaluate=False)
    handler.train((X_train, y_train))
    handler.test((X_TEST, Y_TEST))
    handler.plot_roc(roc_ax)
    
    
    #print("Vertical flip")
    #classifier3 = Classifier3D().get_model()
    #X_train, _, y_train, _ = processor.get_data(augment=True)
    #print("data train: ", len(X_train))
    #handler = ModelHandler(model=classifier3, dim=3, evaluate=False)
    #handler.train((X_train, y_train))
    #handler.test((X_TEST, Y_TEST))
    #handler.plot_roc(roc_ax)
    
    #print("Both flip")
    #classifier4 = Classifier3D().get_model()
    #X_train, _, y_train, _ = processor.get_data(augment=True)
    #print("data train: ", len(X_train))
    #handler = ModelHandler(model=classifier4, dim=3, evaluate=False)
    #handler.train((X_train, y_train))
    #handler.test((X_TEST, Y_TEST))
    #handler.plot_roc(roc_ax)
    
    
    roc_ax.legend(["Original", "1/4 Original, Augmented"])
    #roc_ax.set_title("")
    fig.savefig("AugComparison.png")
    plt.close(fig)
    
                             
        
if __name__ == "__main__":
    #evaluate_mass_trainings()
    main()
    #verify_augmented()