#!/usr/bin/env python3
"""! Python script to check data in GPos generated openPMD files

Script to read GPos openPMD and data.tx output files and confirm beam properties for a input.txt.

- To run it use the command: `python gpos_test.py <path to folder containing openPMD files> <array of species>`

(example: `python gpos_test.py . e-_primary_initial e-_primary_foil > output_py.txt 2> error_py.txt`)

@author Ligia Diana Amorim
@date 06/2021
@copyright GPos 2021 LBNL
"""

# Modules required
import openpmd_api as api
import matplotlib.pyplot as plt
import scipy.constants as cte
import math
import numpy as np
import os, glob, sys
import gpos_funcs as gf

# Folder and species are selected at run time
if len(sys.argv) < 3:
    print('ABORT:')
    print(' Missing command line input parameters:\n',
          ' <path to folder containing openPMD, data.txt and input.txt files>\n',
          ' <list of species names>\n')
    exit()
else:
    if sys.argv[1][-1] != '/':
        Folder = sys.argv[1] + '/'
    else:
        Folder = sys.argv[1]
    species = sys.argv[2:]

# Initializing lists and factors defined in gpos_funcs
gf.initialize()

# Parameters read from the input file
inputs = {}
qi = 0
with open(Folder+"input.txt", 'r') as fi:
    text = fi.read().splitlines()
    for li in range(len(text)):
        if "=" in text[li]:
            line = text[li].split("=")
            if line[0] == gf.query[qi]:
                if line[0] == "iflens":
                    inputs[gf.query[qi]] = line[1]
                elif line[0] == "w_world":
                    inputs[gf.query[qi]] = float(line[1].split(" ")[2])*gf.qfac[qi]
                else:
                    inputs[gf.query[qi]] = float(line[1])*gf.qfac[qi]
                qi+=1
                if qi == len(gf.query):
                    break
print('\nRunning gpos_test.py in ', Folder, 'directory with inputs:\n',inputs)
        
opmd_list={}
nparts={}
parameters = {}
for sn in species:
    nparts[sn]=0
    opmd_list[sn]={}
    opmd_list[sn]["fn"]=Folder+sn+"_opmd.h5"
    if glob.glob(opmd_list[sn]["fn"]):
        #print("Found ", opmd_list[sn]["fn"])
        r = gf.read_opmd(opmd_list[sn]["fn"])
        #r1 = sel_data(r,4.0,inputs["np"],inputs["totq"])
        r2 = gf.set_vel(r[3],r[4],r[5])
        r3 = gf.divergence_emittance(r[0],r[1],r[3],r[4],r[5],r2[0],r2[1])
        parameters[sn] = r+r2+r3
        nparts[sn]=len(r[0])
        del r, r2, r3

data = gf.read_data(Folder)
gf.comp_data(data,species,nparts,parameters)
print("\nThe information in data.txt matches results from openPMD files")

ifcomp = False
for sn in species:
    if "drift" in sn:
        ifcompp = True
r4, tdrift = gf.drift_lens(species, nparts,parameters, inputs, ifcomp)
for sn in species:
    if "foil" in sn:
        sn_new = sn[:-5]+"_drift_new"
        parameters[sn_new] = r4[sn]
        nparts[sn_new]=len(r4[sn][0])
        species.append(sn_new)

print("\nThe drift and lens calculations match results from openPMD files")
print("\nProgram gpos_test ran successfully")