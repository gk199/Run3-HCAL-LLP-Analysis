import argparse
import awkward as ak
import h5py
import numpy as np
import numpy.typing as npt
import os
import uproot

from dataclasses import dataclass
from pathlib import Path
from skimage.measure import block_reduce
from typing import List

@dataclass
class DataSource:
    et: ak.highlevel.Array
    ids: ak.highlevel.Array
    phi: ak.highlevel.Array
    eta: ak.highlevel.Array
    acceptanceFlag: ak.highlevel.Array
    size: int
    _calo_vars = ["rechit_E", "rechit_reliEta", "rechit_reliPhi", "rechit_depth", "gLLP_DecayVtx_R",
                  'jet_Phi', 'gLLP_Pt', 'gLLP_Eta', 'gLLP_DecayVtx_Z', 'rechit_iPhi', 'jet_Eta', 'jet_Pt', 'n_PV', 'n_PVCand']
    
    _acceptance_vars = ["jet_Eta", "jet_Pt"]

    def __init__(self, in_file, tree_name, tree_gen):
        with uproot.open(in_file) as in_file:
            tree = in_file[tree_name]
            arrays = tree.arrays(self._calo_vars)
            et = arrays["rechit_E"]
            print("Et", et)
            eta = arrays["rechit_reliEta"]
            phi = arrays["rechit_reliPhi"]
            depth = arrays["rechit_depth"]
            vertex = arrays["gLLP_DecayVtx_R"]
            
            z_vertex = arrays["gLLP_DecayVtx_Z"]
            jet_Phi = arrays["jet_Phi"]
            LLP_pt = arrays["gLLP_Pt"]
            LLP_eta = arrays["gLLP_Eta"]
            iphi = arrays["rechit_iPhi"]
            jet_eta = arrays['jet_Eta']
            jet_pt = arrays['jet_Pt']
            jet_npv = arrays['n_PV']
            jet_npvcand = arrays['n_PVCand']
            
            
            
            self.size = len(eta)

            ids = np.arange(len(et))
            self.ids = ak.flatten(ak.broadcast_arrays(ids, eta)[0])
            self.et = ak.flatten(et, None)
            self.eta = ak.flatten(eta, None)
            self.phi = ak.flatten(phi, None)
            self.depth = ak.flatten(depth - 1, None)
            self.vertex = ak.flatten(vertex, None)
            self.z_vertex = ak.flatten(z_vertex, None)
            self.jet_Phi = ak.flatten(jet_Phi, None)
            self.LLP_pt = ak.flatten(LLP_pt, None)
            self.LLP_eta = ak.flatten(LLP_eta, None)
            self.iphi = ak.flatten(iphi, None)
            self.jet_eta = ak.flatten(jet_eta, None)
            self.jet_pt = ak.flatten(jet_pt, None)
            self.jet_npv = ak.flatten(jet_npv, None)
            self.jet_npvcand = ak.flatten(jet_npvcand, None)

            try:
                tree_gen = in_file[tree_gen]
                arrays = tree_gen.arrays(self._acceptance_vars)
                jet_Pt = arrays["jet_Pt"]
                jet_Eta = arrays["jet_Eta"]
                mask = (jet_Pt > 40.0) & (abs(jet_Eta) < 1.4)
                self.acceptanceFlag = ak.any(mask, axis=-1)
            except uproot.exceptions.KeyInFileError:
                self.acceptanceFlag = ak.Array([])

    def __len__(self):
        return self.size

def absoluteFilePaths(directory: Path, extension: str = "root"):
    for dirpath, _, filenames in os.walk(directory):
        for f in filenames:
            if f.endswith(extension):
                yield os.path.abspath(os.path.join(dirpath, f))

def get_split(events: int, split: npt.NDArray = np.array([0.6, 0.2, 0.2])):
    split = np.array(split)
    cumsum = np.cumsum(events * split).astype(int)
    cumsum = np.insert(cumsum, 0, 0)
    return [(i, j) for i, j in zip(cumsum, cumsum[1:])]

def convert(
    input_path: Path, output_dir: Path, calo_tree: str, acceptance_tree: str, split: bool
):
    # Ensure the output directory exists
    output_dir.mkdir(parents=True, exist_ok=True)

    input_files = list(absoluteFilePaths(input_path)) if input_path.is_dir() else [input_path]
    for in_file in input_files:
        print(f"Processing {in_file}")
        source = DataSource(in_file, calo_tree, acceptance_tree)
        deposits = np.zeros((len(source), 72, 56, 4))
        ids = source.ids.to_numpy().astype(int)
        et = source.et.to_numpy()
        eta = source.eta.to_numpy().astype(int)
        phi = source.phi.to_numpy().astype(int)
        depth = source.depth.to_numpy().astype(int)
        flags = np.ones(len(eta))
        vertex = source.vertex.to_numpy()
        
        j_phi = source.jet_Phi.to_numpy()
        llp_pt = source.LLP_pt.to_numpy()
        llp_eta = source.LLP_eta.to_numpy()
        z_vertex = source.z_vertex.to_numpy()
        iphi = source.iphi.to_numpy()
        jet_eta = source.jet_eta.to_numpy()
        jet_pt = source.jet_pt.to_numpy()
        jet_npv = source.jet_npv.to_numpy()
        jet_npvcand = source.jet_npvcand.to_numpy()

        
        
        deposits[ids, phi, eta, depth] = et
        region_et = block_reduce(deposits, (1, 1, 1, 1), np.sum)
        print("REGION SHAPE", region_et.shape)
        region_et = region_et[:, :10, :10, :]

        # Construct the output file path
        output_file = output_dir / f"{Path(in_file).stem}_mod.h5"
        with h5py.File(output_file, "w") as h5f:
            h5f.create_dataset(
                "CaloRegions", data=region_et, maxshape=(None, 72, 56, 4), chunks=True
            )
            h5f.create_dataset(
                "AcceptanceFlag", data=flags, maxshape=(None,), chunks=True
            )
            h5f.create_dataset("RadialVertex", data=vertex, maxshape=(None,), chunks=True
            )
            h5f.create_dataset("Jet_Phi", data=j_phi, maxshape=(None,), chunks=True
            ) 
            h5f.create_dataset("ZVertex", data=z_vertex, maxshape=(None,), chunks=True
            )
            h5f.create_dataset("LLP_Eta", data=llp_eta, maxshape=(None,), chunks=True
            )   
            h5f.create_dataset("LLP_Pt", data=llp_pt, maxshape=(None,), chunks=True
            )
            h5f.create_dataset("iPhi", data=iphi, maxshape=(None,), chunks=True
            )
            h5f.create_dataset("jet_eta", data=jet_eta, maxshape=(None,), chunks=True
            )
            h5f.create_dataset("jet_pt", data=jet_pt, maxshape=(None,), chunks=True
            )
            h5f.create_dataset("jet_npv", data=jet_npv, maxshape=(None,), chunks=True
            )
            h5f.create_dataset("jet_npvcand", data=jet_npvcand, maxshape=(None,), chunks=True
            )


        if split:
            with h5py.File(output_file, "r") as h5f_in:
                cr = h5f_in["CaloRegions"]
                fs = h5f_in["AcceptanceFlag"]
                rv = h5f_in["RadialVertex"]
                jp = h5f_in["Jet_Phi"]
                zv = h5f_in["ZVertex"]
                le = h5f_in["LLP_Eta"]
                lp = h5f_in["LLP_Pt"]
                ip = h5f_in["iPhi"]
                jet_eta = h5f_in["jet_eta"]
                jet_pt = h5f_in["jet_pt"]
                jet_npv = h5f_in["jet_npv"]
                jet_npvcand = h5f_in["jet_npvcand"]
                
                for part, (s, e) in enumerate(get_split(cr.shape[0])):
                    split_output_file = output_dir / f"{Path(in_file).stem}_{part}.h5"
                    with h5py.File(split_output_file, "w") as h5f_out:
                        h5f_out.create_dataset("CaloRegions", data=cr[s:e])
                        h5f_out.create_dataset("AcceptanceFlag", data=fs[s:e])
                        h5f_out.create_dataset("RadialVertex", data=rv[s:e])
                        h5f_out.create_dataset("Jet_Phi", data=jp[s:e])
                        h5f_out.create_dataset("ZVertex", data=zv[s:e])
                        h5f_out.create_dataset("LLP_Eta", data=le[s:e])
                        h5f_out.create_dataset("LLP_Pt", data=lp[s:e])
                        h5f_out.create_dataset("iPhi", data=ip[s:e])
                        h5f_out.create_dataset("jet_eta", data=jet_eta[s:e])
                        h5f_out.create_dataset("jet_pt", data=jet_pt[s:e])
                        h5f_out.create_dataset("jet_npv", data=jet_npv[s:e])
                        h5f_out.create_dataset("jet_npvcand", data=jet_npvcand[s:e])

def parse_arguments():
    parser = argparse.ArgumentParser(
        description="""Converts Jet AK4 Images with HCAL Rechit data from
           ROOT format to HDF5"""
    )

    parser.add_argument(
        "filepath", help="Input ROOT directory or file", type=Path
    )
    parser.add_argument("output_dir", help="Output HDF5 directory", type=Path)
    parser.add_argument(
        "--calotree",
        help="Data tree",
        default="JetHCalRechits",
        type=str,
    )
    parser.add_argument(
        "--acceptance",
        help="Store acceptance flag",
        default="JetHCalRechits",
        type=str,
    )
    parser.add_argument(
        "--split", action="store_true", help="Split the dataset 60:20:20"
    )
    return parser.parse_args()

def main(args_in=None) -> None:
    args = parse_arguments()
    convert(args.filepath, args.output_dir, args.calotree, args.acceptance, args.split)

if __name__ == "__main__":
    main()
