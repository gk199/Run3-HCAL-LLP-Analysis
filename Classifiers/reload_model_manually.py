# Usage: `python reload_model_manually.py <model.keras>`

import zipfile
import tempfile
import h5py
import numpy as np
from keras.models import Sequential
from keras.layers import InputLayer, Dense, BatchNormalization
import sys

model_file = sys.argv[1]

# Rebuild model ===
model = Sequential([
    InputLayer(input_shape=(32,), dtype='float32', name='dense_input'),

    Dense(185, activation='sigmoid', dtype='float32', name='dense'),
    BatchNormalization(dtype='float32', name='batch_normalization'),

    Dense(159, activation='sigmoid', dtype='float32', name='dense_2'),
    Dense(159, activation='sigmoid', dtype='float32', name='dense_4'),
    BatchNormalization(dtype='float32', name='batch_normalization_2'),

    Dense(181, activation='sigmoid', dtype='float32', name='dense_6'),
    Dense(2, activation='softmax', dtype='float32', name='dense_8'),
])

model(np.zeros((1, 32), dtype='float32'))  # build model

# Extract model.weights.h5 from archive
with zipfile.ZipFile(model_file, "r") as zipf:
    with tempfile.TemporaryDirectory() as tmpdir:
        weights_path = zipf.extract("model.weights.h5", path=tmpdir)

        with h5py.File(weights_path, "r") as f:
            for layer in model.layers:
                lname = layer.name
                layer_path = f"_layer_checkpoint_dependencies/{lname}/vars"
                if layer_path in f:
                    weights = []
                    group = f[layer_path]
                    for i in range(len(group)):
                        weights.append(group[str(i)][()])

# Save
model.save( model_file.replace(".keras","_recovered.keras") )
