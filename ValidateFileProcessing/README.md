# Validate File Processing

Scripts to validate file processing

## NTuple Validation

Run on the Wisconsin server:
```
python FindValidateListNTuples.py -v <NTuple Version Number> # Valid for NTuple Versions 4+
```

Note that due to file write restrictions on Wisconsin, the best method to modify this file as needed is to make all modifications on lxplus and then copy over:
```
scp FindValidateListNTuples.py "gkopp@login.hep.wisc.edu:/hdfs/store/user/gkopp/"
```
Change the name of the text and error files as needed when the version number increments. 