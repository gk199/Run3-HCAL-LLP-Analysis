# Use of DNN Classifier

Two DNN classifiers are trained, one as a depth jet tagger, the other as an inclusive jet tagger. A virtual environment is used to do the training and score evaluations:

```
source ../Run/Condor/VirtualEnvs/keras2pt13pt1/bin/activate

python3 runner-v4.py
python3 ScoresToEventBased-v4.py <option-filename.root>
```

List files to train over and to write scores to in these scripts. The output DNNs are `dense_model_v4.keras` and `inclusive_model_v4.keras`.

Note that the training and evaluation of the DNN model is done in a python virtual environment created from `requirements_minimal.txt`. The full instructions for creating this are in the [Condor README](https://github.com/gk199/Run3-HCAL-LLP-Analysis/tree/dev-gillian/Run/Condor#debugging-issues-with-dnn-score-addition). 