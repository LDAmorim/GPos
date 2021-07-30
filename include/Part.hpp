/**
 * @file Part.hpp
 * @author Ligia Diana Amorim
 * @date 06/2021
 * @copyright GPos 2021 LBNL
 */

#ifndef PART_hpp
#define PART_hpp

#include <vector>
#include <globals.hh>
#include <unordered_map>
#include <G4ThreeVector.hh>
#include <G4PhysicalConstants.hh>

#include "Beam.hpp"
#include "Query.hpp"

using namespace std;


/**
 * Class with each particle information at begining/end of run.
 */
class Part {
public:
    Part ();
    ~Part ();
    void set_part (G4String p_name, G4String p_process, G4double p_x, G4double p_y, G4double p_z,
    G4double p_px, G4double p_py, G4double p_pz, G4double p_tEn, G4double p_tg, G4int p_step);

    /** Particle species name. */  
    G4String name;
    /** Particle creation process. */  
    G4String process;
    /** Particle x position. */  
    G4double x;
    /** Particle y position. */  
    G4double y;
    /** Particle z position. */  
    G4double z;
    /** Particle px momentum. */  
    G4double px;
    /** Particle py momentum. */  
    G4double py;
    /** Particle pz momentum. */  
    G4double pz;
    /** Particle total Energy. */  
    G4double tEn;
    /** Particle global end time. */  
    G4double tg;
    /** Particle end Step. */  
    G4int steps;
};


/**
 * Class with trajectory and properties information for particles of each species (= beam).
 */
class PartBeams {
public:
    PartBeams ();
    ~PartBeams ();
    void set_partBeams (vector<Part> * p, G4String n, G4double m, G4double q);
    void set_partBeams (Beam * b, G4String n, G4String proc, G4double m, G4double q);
    void set_velocities ();
    void Select (Query in, G4int ifdrift);
    void Propagate (G4double * tf);
    void Analysis (G4int ifdrift);
    void Centroids (G4double arr[]);
    void set_globalC (G4int ifdrift, G4double arr[]);
    void StandardDeviation (Query in, G4int ifdrift, G4double arr[]);
    void set_globalSTD (G4int ifdrift, G4double arr[]);
    void Divergence (G4double arr[]);
    void set_globalDiv (G4int ifdrift, G4double arr[]);
    void Div_Emittance (G4int ifdrift, G4double arr[]);
    void set_globalDivEm (G4int ifdrift, G4double arr[], G4int * nT);
    G4double get_tdrift (Query in);
    void Drift (G4double * g_td, Query in);
    void Print (G4String l);
    G4int get_nsteps ();
    void set_nsteps (G4int * ns);
    void set_MQ (G4double m, G4double q);
    
    /** Beam total number of particles. */  
    G4int np;
    /** Beam species name. */  
    G4String name;
    /** Beam species mass. */  
    G4double mass;
    /** Beam species charge. */  
    G4double charge;
    
    /** Beam vector of creation processes. */  
    vector<G4String> process;
    /** Beam vector of x positions. */  
    vector<G4double> x;
    /** Beam vector of y positions. */  
    vector<G4double> y;
    /** Beam vector of z positions. */  
    vector<G4double> z;
    /** Beam vector of x veclotities. */  
    vector<G4double> vx;
    /** Beam vector of vy veclotities. */  
    vector<G4double> vy;
    /** Beam vector of vz veclotities. */  
    vector<G4double> vz;
    /** Beam vector of px momentum. */  
    vector<G4double> px;
    /** Beam vector of py momentum. */  
    vector<G4double> py;
    /** Beam vector of pz momentum. */  
    vector<G4double> pz;
    /** Beam vector of Lorentz factors. */  
    vector<G4double> gg;
    /** Beam vector of normalized velocities amplitude. */ 
    vector<G4double> bb;
    /** Beam vector of total Energies. */ 
    vector<G4double> tEn;
    /** Beam vector of global end times. */ 
    vector<G4double> tg;
    /** Beam vector of end Steps. */ 
    vector<G4int> steps;

    /** Beam centroid in x accross all MPI ranks. */  
    G4double b_mean_x[2];
    /** Beam centroid in y accross all MPI ranks. */  
    G4double b_mean_y[2];
    /** Beam centroid in z accross all MPI ranks. */  
    G4double b_mean_z[2];
    /** Beam RMS size in x accross all MPI ranks. */  
    G4double b_std_x[2];
    /** Beam RMS size in x accross all MPI ranks. */  
    G4double b_std_y[2];
    /** Beam RMS size in z accross all MPI ranks. */  
    G4double b_std_z[2];
    /** Beam average vx velocity accross all MPI ranks. */  
    G4double b_mean_vx[2];
    /** Beam average vy velocity accross all MPI ranks. */ 
    G4double b_mean_vy[2];
    /** Beam average vz velocity accross all MPI ranks. */ 
    G4double b_mean_vz[2];
    /** Beam RMS vx velocity accross all MPI ranks. */ 
    G4double b_std_vx[2];
    /** Beam RMS vy velocity accross all MPI ranks. */ 
    G4double b_std_vy[2];
    /** Beam RMS vz velocity accross all MPI ranks. */ 
    G4double b_std_vz[2];
    /** Beam emittance in x accross all MPI ranks. */ 
    G4double b_em_x[2];
    /** Beam emittance in y accross all MPI ranks. */ 
    G4double b_em_y[2];
    /** Beam average total Energy accross all MPI ranks. */ 
    G4double b_mean_en[2];
    /** Beam RMS total Energy accross all MPI ranks. */ 
    G4double b_std_en[2];
    /** Beam divergence in x accross all MPI ranks. */ 
    G4double b_div_x[2];
    /** Beam divergence in y accross all MPI ranks. */ 
    G4double b_div_y[2];
    /** Beam RMS divergence in x accross all MPI ranks. */ 
    G4double b_std_div_x[2];
    /** Beam RMS divergence in y accross all MPI ranks. */ 
    G4double b_std_div_y[2];
    /** Beam total number of steps accross all MPI ranks. */ 
    G4int nsteps;
};

#endif /* PART_hpp */
