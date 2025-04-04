import h5py
import sys
import os
import zipfile
import tempfile
import h5py
import numpy as np

# ------------------------------------------------------------------------------
def main():
    # Get the .keras file path from command line arguments
    path = sys.argv[1] # Get the file path as an argument
    if not os.path.exists(path):
        print(f"File not found: {path}")
        return

    # Extract model.weights.h5 from archive
    with zipfile.ZipFile(path, "r") as zipf:
        with tempfile.TemporaryDirectory() as tmpdir:
            weights_path = zipf.extract("model.weights.h5", path=tmpdir)

            with h5py.File(weights_path, "r") as f:
                print(f.keys())
                print(f["vars"])
                print(f["vars"].keys())
                
                # for "recovered" model
                print(f["layers"])
                print(f["layers"].keys())
                print(np.array(f['layers']['dense_2']['vars']['0'])) # 0 is storing weights, 1 biases (?)
                print(len(np.array(f['layers']['dense_2']['vars']['0'])))
                print(np.array(f['layers']['dense_2']['vars']['0']).shape)

                # for initial model
                # print(f["_layer_checkpoint_dependencies"])
                # print(f["_layer_checkpoint_dependencies"].keys())
                # print(np.array(f['_layer_checkpoint_dependencies']['dense_4']['vars']['0'])) # 0 is storing weights, 1 biases (?)
                # print(len(np.array(f['_layer_checkpoint_dependencies']['dense_4']['vars']['0'])))
                # print(np.array(f['_layer_checkpoint_dependencies']['dense_4']['vars']['0']).shape)

            f.close()

# ------------------------------------------------------------------------------
if __name__ == '__main__':
    main()
