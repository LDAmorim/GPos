#!/usr/bin/env python3
"""! @brief Python script to check data in GPos generated openPMD files

Script containing the GPos units and variables namespace as well as the functions to read input.txt, openPMD and data.txt files for GPos runs

The functions defined here should be imported in other python scripts for use

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

## Function to generate the variables and respective units used for GPos data analysis
#
def initialize():
    # Conversion factors: Geant4 units to SI (openPMD standard unit system)
    # By default all properties here are kept in SI, but results are printed in Geant4 units
    global meMeV
    meMeV = cte.physical_constants["electron mass energy equivalent in MeV"][0]
    global f_mass
    f_mass = meMeV/(cte.m_e)
    global f_momentum
    f_momentum = meMeV/(cte.m_e*cte.c)
    global f_energy
    f_energy = meMeV/(cte.m_e*cte.c**2)
    global f_velocity
    f_velocity = 1./cte.c #1e3/1e9 this would be in mm/ns
    global f_time
    f_time = 1e9
    global lbl
    lbl = ["x","y","z","px","py","pz","p","e","q","m","gg","bb","vx","vy","vz","dx","dy","ex","ey"]
    global units
    units = ["um","um","mm","MeV","MeV","MeV","-","MeV","C","MeV","-","-","c","c","c","mrad","mrad",
            "um","um"]
    for uu in units:
        uu="["+uu+"]"
    global fac
    fac = [1e6, 1e6, 1e3, f_momentum, f_momentum, f_momentum, 1, f_energy, 1, f_mass, 1, 1,
            f_velocity, f_velocity, f_velocity, 1e3, 1e3, 1e6, 1e6]
    global query
    query = ["np","totq","w_world","drift","iflens","lens","lens_pz"]
    global qfac
    qfac = [1,1,1e-3,1e-3,1,1e-3,1]
    global coll
    coll = ["#parts", "#steps", "<En>", "RMS En", "<x>", "<y>", "<z>", "RMS x", "RMS y",
            "RMS z", "RMS atan(px/pz)", "RMS atan(py/pz)", "Emittance x", "Emittance y", "<vz>"]

## Function to read the openPMD files particle species data
#
# @param[in] fn Path to selected openPMD file
# @return results Array of numpy arrays corresponding to each of the particles properties
# (x, y, z, px, py ,pz, p, e, q, m)
def read_opmd(fn):
    series = api.Series(fn,api.Access_Type.read_only)
    i = series.iterations[1]
    ps=[i.particles[s] for s in i.particles]
    
    x=ps[0]["position"]["x"].load_chunk()
    unit_x=ps[0]["position"]["x"].unit_SI
    series.flush()
    x=x*unit_x
        
    y=ps[0]["position"]["y"].load_chunk()
    series.flush()
    y=y*unit_x
    
    z=ps[0]["position"]["z"].load_chunk()
    series.flush()
    z=z*unit_x
    
    px=ps[0]["momentum"]["x"].load_chunk()
    unit_px=ps[0]["momentum"]["x"].unit_SI
    series.flush()
    px=px*unit_px

    py=ps[0]["momentum"]["y"].load_chunk()
    series.flush()
    py=py*unit_px

    pz=ps[0]["momentum"]["z"].load_chunk()
    series.flush()
    pz=pz*unit_px
    
    p = ps[0]["process_id"][api.Mesh_Record_Component.SCALAR][:]
    series.flush()

    e = ps[0]["kinetic_energy"][api.Mesh_Record_Component.SCALAR][:]
    unit_e=ps[0]["kinetic_energy"][api.Mesh_Record_Component.SCALAR].unit_SI
    series.flush()    
    e=e*unit_e
    
    q = ps[0]["charge"][api.Mesh_Record_Component.SCALAR][:]
    unit_q=ps[0]["charge"][api.Mesh_Record_Component.SCALAR].unit_SI
    series.flush()
    q=q*unit_q

    m = ps[0]["mass"][api.Mesh_Record_Component.SCALAR][:]
    unit_m=ps[0]["mass"][api.Mesh_Record_Component.SCALAR].unit_SI
    series.flush() 
    m=m*unit_m
    
    results = [x, y, z, px, py ,pz, p, e, q, m]
    
    return results

## Function to make a selection closer to the core of the beam (GPos already outputs within frms)
#
# @param[in] res Array of numpy arrays corresponding to each of the particles properties
# (x, y, z, px, py ,pz, e, p, q, m)
# @param[in] nrms Corresponds to GPos input.txt parameter f_rms set by user (default 4.0)
# and determines the RMS distances from the centroid of the particles within which data is analysed
# @param[in] nini Number of primary particles sent into foil (default 200)
# @param[in] pini Real charge that primary particles modelled represent statistically (default 1e-9)
# @return selected Array of numpy arrays with beam particle properties
# (x, y, z, px, py ,pz, p, e, q, m)
def sel_data(res, nrms, nini, pini):
    ntot = res[0].shape[0]
    print("Total #parts in file = ", ntot, " = ", ntot/nini*pini, "pC")

    x = res[0]
    avx = np.average(x)
    sx = np.std(x)
    indx = tuple([np.abs(x-avx)<sx*nrms])

    y = res[1][indx]
    avy = np.average(res[1])
    sy = np.std(res[1])
    indy = tuple([np.abs(y-avy)<sy*nrms])

    z = res[2][indx][indy]
    avz = np.average(z)
    sz = np.std(z)
    indz = tuple([np.abs(z-avz)<sz*nrms])

    px = res[3][indx][indy][indz]
    avpx = np.average(px)
    spx = np.std(px)
    indpx = tuple([np.abs(px-avpx)<spx*nrms])

    py = res[4][indx][indy][indz][indpx]
    avpy = np.average(py)
    spy = np.std(py)
    indpy = tuple([np.abs(py-avpy)<spy*nrms])

    pz = res[5][indx][indy][indz][indpx][indpy]
    avpz = np.average(pz)
    spz = np.std(pz)
    indpz = tuple([np.abs(pz-avpz)<spz*nrms])
    
    indexes = np.arange(ntot)[indx][indy][indz][indpx][indpy][indpz]
    x = x[indx][indy][indz][indpx][indpy][indpz]
    y = y[indy][indz][indpx][indpy][indpz]
    z = z[indz][indpx][indpy][indpz]
    px = px[indpx][indpy][indpz]
    py = py[indpy][indpz]
    pz = pz[indpz]

    nsel = x.shape[0]
    print('Selected #parts within ', nrms, ' RMS of centroid in position and momentum = ',
          nsel, " = ", nsel/nini*pini, 'pC = ', nsel/ntot*100, '% of file #\n')
    
    p=res[6][indexes]
    e=res[7][indexes]
    q=res[8][indexes]
    m=res[9][indexes]
    
    selected = [x, y, z, px, py ,pz, p, e, q, m]
    
    return selected

## Function to determine the velocity from particle momentum information
#
# @param[in] px Numpy array of particles momentum in x data
# @param[in] py Numpy array of particles momentum in y data
# @param[in] pz Numpy array of particles momentum in z data
# @return arr Array of numpy arrays of particle energy and velocity (gg, bb, vx, vy, vz)
def set_vel(px, py, pz):
    gg = np.sqrt(1.0+(px**2+py**2+pz**2)/cte.m_e**2/cte.c**2)
    bb = np.sqrt(1.0-1.0/gg**2)
    vx = px/gg/cte.m_e
    vy = py/gg/cte.m_e
    vz = pz/gg/cte.m_e
    arr = [gg, bb, vx, vy, vz]

    return arr

## Function to determine the beam divergence
#
# @param[in] x Numpy array of particles position in x data
# @param[in] y Numpy array of particles position in y data
# @param[in] px Numpy array of particles momentum in x data
# @param[in] py Numpy array of particles momentum in y data
# @param[in] pz Numpy array of particles momentum in z data
# @param[in] gg Numpy array of particles Lorentz factor data
# @param[in] bb Numpy array of particles normalized velocity data
# @return arr Array of numpy arrays of particle divergence and emittance (divx, divy, emx, emy)
def divergence_emittance(x,y,px,py,pz,gg,bb):
    pxopz = px/pz
    pyopz = py/pz
    divx = np.arctan(pxopz)
    divy = np.arctan(pyopz)
    pxopzbg = pxopz*gg*bb
    pyopzbg = pyopz*gg*bb
    emx = math.sqrt(np.std(x)**2*np.std(pxopzbg)**2 - 
            np.mean((x-np.mean(x))*(pxopzbg-np.mean(pxopzbg)))**2 )
    emy = math.sqrt(np.std(y)**2*np.std(pyopzbg)**2 
                    - np.mean((y-np.mean(y))*(pyopzbg-np.mean(pyopzbg)))**2)

    arr = [divx, divy, emx, emy]
    return arr

## Function to determine beam properties from openPMD data files
#
# @param[in] species List of species names including position information
# @param[in] nparts Number of particles for each element in species
# @param[in] parameters Data of all variables values for all species
# @return dd Dictionary that associates average and RMS values to each variable for each species
def properties(species,nparts, parameters):
    cc = {}
    for sn in species:
        cc[sn] = {}
        if (nparts[sn]>0):
#             print("\n",sn)
            for i in range(len(lbl)):
                if i not in [6, 17, 18]:
                    avg = np.mean(parameters[sn][i])*fac[i]
                    std = np.std(parameters[sn][i])*fac[i]
                    cc[sn][i] = {}
                    cc[sn][i]["avg"] = avg
                    cc[sn][i]["std"] = std
#                     print(f"<{lbl[i]:2}>    : {avg:23},  RMS {lbl[i]:2} :"
#                         f"    {std:23} [{units[i]:3}]")
#                elif i in [17, 18]:
#                    print(f"<{lbl[i]:2}>    : {parameters[sn][i]:23}   [{units[i]:3}]")
#            for j in [0,3,12,15]:
#                avg = np.mean([np.mean(parameters[sn][j]),np.mean(parameters[sn][j+1])])*fac[j]
#                std = np.mean([np.std(parameters[sn][j]),np.std(parameters[sn][j+1])])*fac[j]
#                print(f"<{lbl[j]:2},{lbl[j+1]:2}> : {avg:23},  RMS {lbl[j]:2},{lbl[j+1]:2} :"
#                    f" {std:23} [{units[j]:3}]")
#            avg = np.mean([parameters[sn][17], parameters[sn][18]])*fac[17]
#            print(f"<{lbl[17]:2},{lbl[18]:2}> : {avg:23}   [{units[18]:3}]")
        else:
            print("There are no ", sn)
#    print("")
    return cc

## Function to read beams properties from data.txt
#
# @param[in] fn Folder containing data.txt file
# @return dd Dictionary that associates the avg/RMS properties to each beam at each position
def read_data(fn):
    dd = {}
    di = -1
    posl = ["Initial", "Foil", "Drift"]
    pos = ["initial", "foil", "drift"]
    with open(fn+"data.txt", 'r') as fi:
        text = fi.read().splitlines()
        for li in range(len(text)):
            line = text[li].split("\t")
            if di < 2 and line[0] == posl[di+1]:
                di+=1
            elif di >= 0:
                sn = line[0]+"_"+pos[di]
                dd[sn] = []
                for i in range(len(line)-1):
                    if i == 0:
                        dd[sn].append(int(line[1]))
                    else:
                        dd[sn].append(float(line[i+1]))
    return dd

## Function to compare data.txt information with properties from openPMD files (break if not match)
#
# @param[in] dd Dictionary that associates the avg/RMS properties to each beam at each position
# @param[in] species List of species names including position information
# @param[in] nparts Number of particles for each element in species
# @param[in] parameters Data of all variables values for all species
def comp_data(dd, species, nparts,parameters):
    fac_comp = [1e3,1e3,1]
    centroids = properties(species,nparts,parameters)
    for sn in species:
        if (nparts[sn]>0):
            if dd[sn][0] != nparts[sn]:
                print("ERROR in",coll[0],dd[sn][0],"!=",nparts[sn])
                exit(1)
            if not math.isclose(dd[sn][2],centroids[sn][7]["avg"],rel_tol=0.1):
                print("ERROR in",coll[2],dd[sn][2],"!=",centroids[sn][7]["avg"])
                exit(1)
            if not math.isclose(dd[sn][3],centroids[sn][7]["std"],rel_tol=0.0001):
                print("ERROR in",coll[3],dd[sn][3],"!=",centroids[sn][7]["std"])
                exit(1)
            for i in [4,5,6]:
                if not math.isclose(dd[sn][i]*fac_comp[i-4],centroids[sn][i-4]["avg"],rel_tol=0.00001):
                    print("ERROR in",coll[i],dd[sn][i]*fac_comp[i-4],"!=",centroids[sn][i-4]["avg"])
                    exit(1)
            for i in [7,8,9]:
                if not math.isclose(dd[sn][i]*fac_comp[i-7],centroids[sn][i-7]["std"],rel_tol=0.00001):
                    print("ERROR in",coll[i],dd[sn][i]*fac_comp[i-7],"!=",centroids[sn][i-7]["std"])
                    exit(1)
            for i in [10,11]:
                if not math.isclose(dd[sn][i]*1e3,centroids[sn][i+5]["std"],rel_tol=0.00001):
                    print("ERROR in",coll[i],dd[sn][i]*1e3,"!=",centroids[sn][i+5]["std"])
                    exit(1)
            for i in [12,13]:
                if not math.isclose(dd[sn][i],parameters[sn][i+5]*1e3,rel_tol=0.00001):
                    print("ERROR in",coll[i],dd[sn][i],"!=",parameters[sn][i+5]*1e3)
                    exit(1)
            if not math.isclose(dd[sn][14],centroids[sn][14]["avg"],rel_tol=0.0001):
                print("ERROR in",coll[14],dd[sn][14],"!=",centroids[sn][14]["avg"])
                exit(1)

## Function to compute the drift with(out) lens and compare results with openPMD data from GPos
#
# @param[in] species List of species names including position information
# @param[in] nparts Number of particles for each element in species
# @param[in] parameters Data of all variables values for all species
# @param[in] inputs List of parameters set by user in input.txt
# @param[in] ifcomp Bool variable to determine if to compare drift function to openPMD results
# @return drift Dictionary with particle data after computed drift with(out) foil
# @return tdrift Minimum time taken by the fastest particle to reach end of drift wihtout lens
def drift_lens(species, nparts, parameters, inputs, ifcomp):
    tdrift = 1.0
    zmax = inputs["drift"]+inputs["w_world"]/2
    for sn in species:
        if "foil" in sn:
                tdrift = np.min([tdrift,np.min((zmax-parameters[sn][2])/parameters[sn][14])])
#     print("\nTime of drift for all particles = ", tdrift*f_time, "ns =", tdrift*cte.c*1e3,"mm")

    gref = np.sqrt(1.0 + (inputs["lens_pz"]/meMeV)**2)
    drift={}
    for sn in species:
        if "foil" in sn:
            drift[sn]={}
            if (inputs["iflens"]):
                d0 = inputs["lens"]+inputs["w_world"]/2-parameters[sn][2]
                tlens = d0/parameters[sn][14]
                if sn[:2] == 'e+':
                    k = -2*(gref-1.0/gref)*cte.m_e*cte.c**2/d0
                else:
                    k = 2*(gref-1.0/gref)*cte.m_e*cte.c**2/d0
                vzinv = 1.0/parameters[sn][14]
                pzinv = 1.0/parameters[sn][5]
                M11 = 1.0
                M12 = d0*pzinv
                M21 = k*vzinv
                M22 = d0*k*vzinv*pzinv+1.0
                x0 = parameters[sn][0]
                y0 = parameters[sn][1]
                px0 = parameters[sn][3]
                py0 = parameters[sn][4]
                xlens = M11*x0+M12*px0
                ylens = M11*y0+M12*py0
                zlens = np.full((nparts[sn]),inputs["lens"] + inputs["w_world"]/2)
                drift[sn]["xlens"] = xlens
                drift[sn]["ylens"] = ylens
                drift[sn]["zlens"] = zlens
                drift[sn][3] = M21*x0+M22*px0
                drift[sn][4] = M21*y0+M22*py0
                drift[sn][5] = parameters[sn][5]
                drift[sn][6] = parameters[sn][6] # Proccess
                drift[sn][8] = parameters[sn][8] # Charge
                drift[sn][9] = parameters[sn][9] # Mass
                drift[sn][10] =  np.sqrt(1.0+(drift[sn][3]**2+drift[sn][4]**2+drift[sn][5]**2
                                        )/cte.m_e**2/cte.c**2)
                drift[sn][11] =  np.sqrt(1.0-1.0/drift[sn][10]**2)
                drift[sn][7] =  drift[sn][10]*cte.m_e*cte.c**2
                drift[sn][12] =  drift[sn][3]/cte.m_e/drift[sn][10]
                drift[sn][13] =  drift[sn][4]/cte.m_e/drift[sn][10]
                drift[sn][14] =  drift[sn][5]/cte.m_e/drift[sn][10]
                drift[sn][0] = xlens + (tdrift-tlens)*drift[sn][12]
                drift[sn][1] = ylens + (tdrift-tlens)*drift[sn][13]
                drift[sn][2] = zlens + (tdrift-tlens)*drift[sn][14]
                if (ifcomp):
                    for i in range(15):
                        if i not in [6, 7]:
                            if False in np.isclose(drift[sn][i],parameters[sn[:-5]+"_drift"][i],
                                                    rtol=0.0001):
                                print("ERROR in",lbl[i],drift[sn][i],"!=",
                                        parameters[sn[:-5]+"_drift"][i])
                                exit(1)
            else:
                drift[sn][0] = parameters[sn][0] + tdrift*parameters[sn][12]
                drift[sn][1] = parameters[sn][1] + tdrift*parameters[sn][13]
                drift[sn][2] = parameters[sn][2] + tdrift*parameters[sn][14]
    return drift, tdrift

## Function to create a new directory where to store the plots
#
# @param[in] newdir Name chosen for the directory
def new_dir(newdir):
    if os.path.isdir(newdir) == False:
        print('Created directory: ', newdir)
        os.mkdir(newdir)

## Function to make the beams 2D scatterplot
#
# @param[in] species List of species names including position information
# @param[in] nparts Number of particles for each element in species
# @param[in] parameters Data of all variables values for all species
# @param[in] pos Position of the beams to plot ("initial", "foil" or "drift")
# @param[in] dirs Array of labels of the 2 directions to plot (0 to 2)
# @param[in] npd Directory where plots will be stored
def plot_beams_2d(species, nparts, parameters, pos, dirs, npd):
    fig=plt.figure(figsize=(8, 5))
    ax1 = fig.add_subplot(111)
    skip = 1 # In case the # particles is too large to render quickly
    exists = 0
    for sn in species:
        if pos in sn:
            if nparts[sn]>0:
                exists = 1
                label = sn[:-len(pos)-1]
                plt.scatter(parameters[sn][dirs[0]][::skip]*fac[dirs[0]],
                            parameters[sn][dirs[1]][::skip]*fac[dirs[1]],
                            label=label,
                            s=20.0,
                            rasterized=True)
    if exists == 0:
        return
    plt.xlabel(lbl[dirs[0]]+" ["+units[dirs[0]]+"]")
    #plt.ylabel(lbl[dirs[1]]+" "+units[dirs[1]])
    plt.ylabel(lbl[dirs[1]]+r" [$\mu$m]")
    leg= plt.legend(loc='lower left',bbox_to_anchor=(0.0,0.4),frameon=False, markerscale=2.)
    plt.tight_layout()
    p_name=npd+'/Beams_2D_'+pos+"_"+lbl[dirs[0]]+"_"+lbl[dirs[1]]
    plt.savefig(p_name+'.png', dpi=300, bbox_inches = 'tight',pad_inches = 0.1)
    print('Saved ',p_name+'.png')

## Function to plot the propagation of the particles in drift comparing it with(out) lens and GPos
#
# @param[in] inputs List of parameters set by user in input.txt
# @param[in] nparts Number of particles for each element in species
# @param[in] parameters Data of all variables values for all species
# @param[in] sn Name of the beam species (ony, without position information)
# @param[in] td Time of drift expected for all particles
# @param[in] npd Directory where plots will be stored
def plot_drift(inputs,nparts,parameters,sn, td, npd):
    fig=plt.figure(figsize=(8, 5))
    ax1 = fig.add_subplot(111)
    snfoil = sn+"_foil"
    sndrift = sn+"_drift_new"
    for i in range(nparts[snfoil]):
        xaxis = np.asarray([parameters[snfoil][2][i],parameters[sndrift]["zlens"][i],
        parameters[sndrift][2][i]])
        yaxis =  np.asarray([parameters[snfoil][0][i],parameters[sndrift]["xlens"][i],
        parameters[sndrift][0][i]])
        p = plt.plot(xaxis*fac[2],yaxis*fac[0])
        xaxis2 = np.asarray([parameters[snfoil][2][i],parameters[snfoil][2][i] +
                td*parameters[snfoil][14][i]])
        yaxis2 =  np.asarray([parameters[snfoil][0][i],parameters[snfoil][0][i] + 
                td*parameters[snfoil][12][i]])
        clr = p[0].get_color()
        plt.plot(xaxis2*fac[2],yaxis2*fac[0], linestyle='dotted',color=clr)
        plt.scatter(parameters[sn+"_drift"][2][i]*fac[2],parameters[sn+"_drift"][0][i]*fac[0],
                c=clr,s=100)
        
    plt.xlabel("z [mm]")
    plt.ylabel("x [$\mu$m]")
    plt.title(sn)
    plt.vlines((inputs["lens"]+inputs["w_world"]/2)*fac[2],-400,400,color='b')
    plt.tight_layout()
    p_name=npd+'/Beams_2D_'+sn+'_drift_comp'
    plt.savefig(p_name+'.png', dpi=300, bbox_inches = 'tight',pad_inches = 0.1)
    print('Saved ',p_name+'.png')