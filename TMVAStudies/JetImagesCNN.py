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


class DataProcessor:
    def __init__(self, sig_files, bkg_files, dim, test=False, multi=False, features="etaptpv"):
        self.dim = dim
        self.features = features
        self.sig_files = sig_files
        self.bkg_files = bkg_files
        self.test = test
        self.multi = multi
    
    def load_data(self):
        
        # initialising the things we will eventually concatenate and store all data for signal
        sig_images = tuple()
        sig_numerical = tuple()
        
        for sig_file in self.sig_files:
            print("Opening ", sig_file)
            with h5py.File(sig_file, "r") as f:
                # all the images from one file
                images = np.array(f['CaloRegions'])                      
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
        X_sig = np.concatenate(sig_images)
        if self.multi:
            # np array of all signal numerical data
            X_num_sig = np.concatenate(sig_numerical)
        print("All Signal: ", X_sig.shape)
            
        # repeating for background   
        bkg_images = tuple()
        bkg_numerical = tuple()
        
        for bkg_file in self.bkg_files:
            print("Opening ", bkg_file)
            with h5py.File(bkg_file, "r") as f:
                images = np.array(f['CaloRegions'])
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
            
        self.X = np.concatenate((X_sig, X_bkg)) # all data
        if self.dim == 2:
            self.X = self.X[:, :10, :10] # cutting all the data once it's all aggregated 
        
                    
        self.y = np.concatenate((np.ones(X_sig.shape[0]), np.zeros(X_bkg.shape[0]) ))
        
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
        
        self.X = np.concatenate((self.X, X_h, X_v, X_b))
        self.y = np.concatenate((self.y, self.y, self.y, self.y))
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
            self.X, self.X_num, self.y = shuffle(self.X, self.X_num, self.y)
            return
        
        self.X, self.y = shuffle(self.X, self.y) # if test eventually should be unshuffled to match with ROCs
        print("Data shape: ", self.X.shape)
     
    def get_data(self, augment=True):
        self.load_data()
        self.process_data()
        if augment:
            self.augment_data()
        print("Total Data: ", self.y.shape)
        
        if not self.test and not self.multi:
            self.X_train, self.X_test, self.y_train, self.y_test = train_test_split(self.X, self.y, test_size=0.4)
            print("shape train: ", self.X_train.shape, "shape test", self.X_test.shape)
            return self.X_train, self.X_test, self.y_train, self.y_test
        
        elif not self.test and self.multi:
            self.X_train, self.X_test, self.X_num_train, self.X_num_test, self.y_train, self.y_test = train_test_split(self.X, self.X_num, self.y, test_size=0.4)
            return self.X_train, self.X_test, self.X_num_train, self.X_num_test, self.y_train, self.y_test
        
        elif self.test and self.multi:
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
        if self.multi:
            X_train, X_num_train, y_train = data_train
            self.model.fit(x=[X_train, X_num_train], y=y_train, batch_size=batch, epochs=epochs, validation_split=val, verbose=1)
            return
        
        X_train, y_train = data_train                     
        self.model.fit(X_train, y_train, batch_size=batch, epochs=epochs, validation_split=val, verbose=1)
      
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
            tape.watch(conv_outputs)  # Get the activations from the batch
            class_index = np.argmax(predictions) # looking for opposite-like
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
        batch, width, height, depth, filters = self.last_conv_layer.input_shape
        
        resized_img = img.reshape(1, 10, 10, 4, 1)
        resized_image = np.repeat(resized_img, filters, axis=-1)
        
        conv_output = self.last_conv_layer(resized_image)  # Convert to numpy array
        
        weighted_output = conv_output * pooled_grads[tf.newaxis, tf.newaxis, tf.newaxis, tf.newaxis, :]
        return conv_output, weighted_output                    
                                
                                
    def gradCAM(self, data, num_samples=15):
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
            norm2 = mcolors.Normalize(vmin=0, vmax=np.max(heatmaps[i]))
            for k in range(4):
                ax[i, k].imshow(images[i][:,:,k], norm=norm1)
                ax[i, k].axis("off")           
                ax[i, k].set_title(f"L {labels[i]}, P{predictions[i]}")
                                
                ax[i, k+4].imshow(heatmaps[i][:,:,k], norm=norm2, cmap="magma")           
                ax[i, k+4].axis("off")           
                ax[i, k+4].set_title(f"Depth{k+1}")
                                
        fig.savefig("GradCAM3D.png")
           
    def plot_roc(self, ax):
        y_true, score = self.roc_data                
        fpr, tpr, _ = roc_curve(y_true, score)                  
        roc_auc = auc(fpr, tpr)
        ax.semilogx(fpr, tpr)
    
    #--------WIP--------
    def write_to_root(self, input_files, output_file, branch_name):
        sig_file, bkg_file = input_files
        sig_events = uproot.open(sig_file)['JetHCalRechits']# these files aren't per-event so doesn't make sense
        sig_branches = sig_events.arrays(library="np")
        bkg_events = uproot.open(bkg_file)['JetHcalRechits']
        bkg_branches = bkg_events.arrays(library="np")
        
        labels, scores = self.roc_data
        
        sig_scores = scores[labels == 1] 
        bkg_scores = scores[labels == 0]
        #with uproot.recreate(output_file) as f:
            #f["JetHCalRechits"] = comb_dic
            #f[f"{branch_name}"] 
        print("Wrote to ROOT File")
                                
                                
def main(evaluate=True):
    
    filepaths = [ {"sig_files": ['Inputs/h5/v3j1/jets_ggH_HToSSTobbbb_MH-350_MS-160_CTau10000.h5'], "bkg_files": ['Inputs/h5/v3j1/jets_Run2023C-EXOLLPJetHCAL-PromptReco-v1.h5']  }, 
                 
                 {"sig_files": ['Inputs/Jet3Dh5/jetntuple_ggH_HToSSTobbbb_MH-350_MS-160_CTau10000_partial.h5'#,
                     #'Inputs/Jet3Dh5/HADD_HToSSTobbbb_MH250_MS120_CTau1000.h5', 
                     #'Inputs/Jet3Dh5/HADD_jetntuple_ggH_HToSSTobbbb_HToSSTo4B_MH125_MS50_CTau3000.h5',
                     #'Inputs/Jet3Dh5/HADD_jetntuple_ggH_HToSSTo4B_MH125_MS15_CTau10000.h5',
                     #'Inputs/Jet3Dh5/HADD_jetntuple_ggH_HToSSTo4B_MH350_MS80_CTau500.h5'
                               ], "bkg_files": [#'Inputs/Jet3Dh5/jetntuple_Run2023C-EXOLLPJetHCAL-PromptReco-v1_partial.h5'#,
                                                'Inputs/Jet3Dh5/HADD_Run2023D-EXOLLPJetHCAL-PromptReco-v1.h5',
                     'Inputs/Jet3Dh5/HADD_Run2023D-EXOLLPJetHCAL-PromptReco-v2.h5' 
                     #,'Inputs/Jet3Dh5/HADD_bkg_partial.h5' 
                                               ]}]
    
    
    fig, roc_ax = plt.subplots()
    roc_ax.set_xlabel("False Positive Rate")
    roc_ax.set_ylabel("True Positive Rate")
    dim = 2
    if not evaluate:                                                       
        classifier2 = Classifier2D().get_model() 
        classifier3 = Classifier3D().get_model()
        classifiers = [classifier2, classifier3]                 
        params = {3: 0.0010967517265048784, 2: 0.0008168928172943063}
        for classifier, filepath in zip(classifiers, filepaths):
            print("Entered Loop: Train Mode")
            processor = DataProcessor(filepath['sig_files'], filepath['bkg_files'], dim=dim, test=False)
            print("Got Data")
            X_train, X_test, y_train, y_test = processor.get_data() #returns the full dataset unshuffled so that the scores can be inputted back
            handler = ModelHandler(model=classifier, dim=dim, evaluate=False, model_name=f"classifier{dim}d.keras")
            handler.train((X_train, y_train))
            print("Trained")
            handler.test((X_test, y_test))
            print("Tested")                    
            handler.plot_roc(roc_ax)                     
            dim += 1
            handler.save()
            
            
        #adding in the training of the multi-input
        classifier = MultiInput(num_features=3).get_model()
        processor = DataProcessor(filepaths[1]['sig_files'], filepaths[1]['bkg_files'], dim=3, test=False, multi=True)
        X_train, X_test, X_num_train, X_num_test, y_train, y_test = processor.get_data(augment=True)
        handler = ModelHandler(model=classifier, dim=3, evaluate=False, multi=True, model_name="multiInput.keras")
        handler.train((X_train, X_num_train, y_train))
        handler.test((X_test, X_num_test, y_test))
        handler.plot_roc(roc_ax)
        handler.save()
        roc_ax.legend(["3DCNN", "3DCNN + Dense"])
        fig.savefig("ROC_plt.png")
      
    else:
        classifier2 = tf.keras.models.load_model("models/classifier2d.keras")
        classifier3 = tf.keras.models.load_model("models/classifier3d.keras")
        classifiers = [classifier2, classifier3]
        for classifier, filepath in zip(classifiers, filepaths):
            print("Entered Loop: Eval Mode")
            processor = DataProcessor(filepath['sig_files'], filepath['bkg_files'], dim=dim, test=True)
            print("Got Data")
            data_test = processor.get_data() #returns the full dataset unshuffled so that the scores can be inputted back
            handler = ModelHandler(model=classifier, dim=dim, evaluate=True)
            handler.test(data_test)
            print("Tested")
            #gradcam = handler.plot_gradcam2D if dim==2 else handler.plot_gradcam3D                      
            #gradcam()                     
            handler.plot_roc(roc_ax)                     
            dim += 1
        
        classifier = tf.keras.models.load_model("models/multiInput.keras")
        processor = DataProcessor(filepaths[1]['sig_files'], filepaths[1]['bkg_files'], dim=3, test=True, multi=True)
        data_test = processor.get_data()
        handler = ModelHandler(model=classifier, dim=3, evaluate=True, multi=True)
        handler.test(data_test)
        handler.plot_roc(roc_ax)
        ROC_data = pd.read_csv("../../Run3-HCAL-LLP-Analysis/TMVAStudies/roc_binary_data.csv")
        roc_ax.semilogx(np.array(ROC_data['FPR']), np.array(ROC_data['TPR']))
        roc_ax.set_title("MH350-MS160 Comparison")
        roc_ax.legend([ "2DCNN", "3DCNN", "3DCNN+Dense", "Dense"])
        fig.savefig("ROC_eval_plt_comps.png")
        
        
        
def evaluate_mass_trainings():
    
    # changing
    sig_files = ['Inputs/Jet3Dh5/jetntuple_ggH_HToSSTobbbb_MH-350_MS-160_CTau10000_partial.h5',
                     'Inputs/Jet3Dh5/HADD_HToSSTobbbb_MH250_MS120_CTau1000.h5', 
                     'Inputs/Jet3Dh5/HADD_jetntuple_ggH_HToSSTobbbb_HToSSTo4B_MH125_MS50_CTau3000.h5',
                 'Inputs/Jet3Dh5/HADD_jetntuple_ggH_HToSSTo4B_MH125_MS15_CTau10000.h5',
                 'Inputs/Jet3Dh5/HADD_jetntuple_ggH_HToSSTo4B_MH350_MS80_CTau500.h5']
    
    model_names = [('classifier3d-350-160.keras', 'multiInput350-160.keras'), ('classifier3d-250-120.keras', 'multiInput250-120.keras'), ('classifier3d-125-50.keras', 'multiInput125-50.keras'), ('classifier3d-125-15.keras', 'multiInput125-15.keras'),
                  ('classifier3d-350-80.keras', 'multiInput350-80.keras')]
    
    # constant
    background = [#'Inputs/Jet3Dh5/jetntuple_Run2023C-EXOLLPJetHCAL-PromptReco-v1_partial.h5'
        'Inputs/Jet3Dh5/HADD_Run2023D-EXOLLPJetHCAL-PromptReco-v1.h5',
        'Inputs/Jet3Dh5/HADD_Run2023D-EXOLLPJetHCAL-PromptReco-v2.h5',
                 ]
    
    names = ['MH350-MS160', 'MH250-MS120', 'MH125-MS50', 'MH125-MS15', 'MH350-MS80']
    
    
    for index, (model3d, model_multi) in enumerate(model_names):

        fig1, ax1 = plt.subplots()
        fig2, ax2 = plt.subplots()

        classifier3 = tf.keras.models.load_model(f"models/{model3d}")
        multiInput = tf.keras.models.load_model(f"models/{model_multi}")
        
        for signal in sig_files:
            
            # 3DCNN predictions
            processor = DataProcessor([signal], background, dim=3, test=True)
            data_test = processor.get_data()
            handler = ModelHandler(model=classifier3, dim=3, evaluate=True)
            handler.test(data_test)
            handler.plot_roc(ax1)
                
            #predictions on multiInput
            processor = DataProcessor([signal], background, dim=3, test=True, multi=True)
            data_test = processor.get_data()
            handler = ModelHandler(model=multiInput, dim=3, evaluate=True, multi=True)
            handler.test(data_test)
            handler.plot_roc(ax2)
            
            ax1.set_xlabel("False Positive Rate")
            ax1.set_ylabel("True Positive Rate")
            ax1.legend(names)
            ax1.set_title(f"Trained on {names[index]} with 3DCNN")
            ax2.set_xlabel("False Positive Rate")
            ax2.set_ylabel("True Positive Rate")
            ax2.legend(names)
            ax2.set_title(f"Trained on {names[index]} with multi-input")
            
        plt.tight_layout()       
        fig1.savefig(f"Trained{names[index]}-3DCNN.png")
        fig2.savefig(f"Trained{names[index]}-MultiInput.png")
        plt.close(fig1)
        plt.close(fig2)
                             
        
if __name__ == "__main__":
    #evaluate_mass_trainings()
    main()
