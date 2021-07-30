/**
 * @file Query.hpp
 * @author Ligia Diana Amorim
 * @date 06/2021
 * @copyright GPos 2021 LBNL
 */

#ifndef QUERY_hpp
#define QUERY_hpp

#include <globals.hh>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <iterator>
#include <string>
#include <vector>
#include <unordered_map>
#include <CLHEP/Units/SystemOfUnits.h>
#include <G4ThreeVector.hh>
#include <G4MPImanager.hh>

using namespace std;
using CLHEP::mm;

/**
 * Class containing input file beam-world-foil parameters.
 */
class Query
{
public:
    /** Tests if running on debug mode. */
    bool debug;
    /** # of primary particles to be sent into the foil (= # events). */
    G4int np;
    /** Total beam charge that the primary particles represent [C]. */
    G4double totq;
    /** Primary beam mean energy [MeV] */
    G4double en;
    /** Primary beam energy spread [MeV] */
    G4double de;
    /** RMS spatial and momentum displacement multiplying factor for particle selection during 
     * analysis [mm] */
    G4double frms;
    /** Primary beam focal plane longitudinal position [mm] */
    G4double z_focal;
    /** Primary beam RMS size in x [mm] */
    G4double s_bprimary_x;
    /** Primary beam RMS size in y [mm] */
    G4double s_bprimary_y;
    /** Primary beam RMS size in z (longitudinal direction) [mm] */
    G4double s_bprimary_z;
    /** Primary beam RMS divergence in x [mm] */
    G4double s_angle_x;
    /** Primary beam RMS divergence in y [mm] */
    G4double s_angle_y;
    /** Primary beam species name */
    G4String type;
    /** Primary beam longitudinal profile */
    G4String profile;
    /** If to symmetrize primary beam particles distributions */
    G4bool symmetrize;
    /** 3D array (x,y,z) of primary beam centroid [mm] */
    G4ThreeVector x_beam;
    /** 3D array of world dimensions - centered at (0,0,0) [mm] */
    G4ThreeVector w_world;
    /** If to include a foil */
    G4bool hasfoil;
    /** 3D array of foil dimensions [mm] */
    G4ThreeVector w_foil;
    /** 3D array of foil position [mm] */
    G4ThreeVector x_foil;
    /** Chemical element of foil material */
    G4String t_foil;
    /** Vacuum distance for beams ballistic propagation after the foil [mm] */
    G4double drift;
    /** If thin lens is to be applied during drift */
    G4bool iflens;
    /** Longitudinal distance bwteen the lens and the end of the foil [mm] */
    G4double lens;
    /** Longitudinal momentum of positive particle that lens is tuned to [mm] */
    G4double lens_pz;
    /** If to propagate the particles generated backwards to same time step [mm] */
    G4bool ifback;
    /** List of output species to store particle data */
    vector<G4String> s_list;
    /** Minimum energy required to store particle data [MeV] */
    vector<G4double> encutoff;
    /** Minimum longitudinal momentum required to store particle data [MeV] */
    vector<G4double> pzcutoff;
    
    Query ();
    virtual ~Query ();
    void read_input ();
    void print_input ();
    void set_variable (G4String line);
    void set_cuttoff ();
};

#endif /* QUERY_hpp */
