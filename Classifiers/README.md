# Use of DNN Classifier

Two DNN classifiers are trained, one as a depth jet tagger, the other as an inclusive jet tagger. A virtual environment is used to do the training and score evaluations:

```
source ../Run/Condor/VirtualEnvs/keras2pt13pt1/bin/activate

python3 runner-v4.py
python3 runner-v4-depth.py

python3 ScoresToEventBased-v4.py <option-filename.root>
```

Need to run the inclusive tagger before the depth tagger training can be run, since depth training is done in the CR! 

List files to train over and to write scores to in these scripts. The output DNNs are `dense_model_v4.keras` and `inclusive_model_v4.keras`.

## Plot DNN Scores
```
python3 PlotScoresDNN.py
```

## Custom ROC Curve Plots
```
root -b -q -l MakeDNNPerformancePlots_SigBkg.C
```
The input root files are listed in the script. 

## Environment Details

Note that the training and evaluation of the DNN model is done in a python virtual environment created from `requirements_minimal.txt`. The full instructions for creating this are in the [Condor README](https://github.com/gk199/Run3-HCAL-LLP-Analysis/tree/dev-gillian/Run/Condor#debugging-issues-with-dnn-score-addition). 

Creation of python virtual environment for training and evaluation of the DNNs:
```
python3.9 -m venv keras2pt13pt1
source keras2pt13pt1/bin/activate
pip install -r ../../../Classifiers/requirements_minimal.txt
```