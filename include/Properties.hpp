/**
 * @file Properties.hpp
 * @author Ligia Diana Amorim
 * @date 06/2021
 * @copyright GPos 2021 LBNL
 */

#ifndef PROPERTIES_hpp
#define PROPERTIES_hpp

#include <G4PhysicalConstants.hh>
#include <globals.hh>
#include <iostream>
#include <math.h>

#include "Part.hpp"
#include "OutputOPMD.hpp"
#include "OutputTXT.hpp"
#include "Query.hpp"

using namespace std;

/**
 * Class used to compute all the beams properties after the Foil and Drift.
 */
class Properties{
public:
    Properties (Query in);
    ~Properties ();
    void ComputeBeams (G4double * tf, vector<Part> * p,
    map<G4String,map<G4String,G4double>> * mq);
    void Analysis (map<G4String,PartBeams> * b, G4int * n0, G4int * nT, G4int ifdrift);
    void MeanSTD (map<G4String,PartBeams> * b, G4int * n0, G4int * nT, G4int ifdrift);
    void ShareMQ (map<G4String,PartBeams> * b, G4int * n0, G4int * nT);
private:
    /** Access to the parameters red from input.txt file. */
    Query input;
    /** Number of beam particles in each MPI rank. */
    G4int * np0 = new G4int;
    /** Total beam particles accross all MPI ranks. */
    G4int * npT = new G4int;
    /** Number of beam particles summed up to each MPI rank. */
    G4int * npS = new G4int;
    /** Total steps (= interactions) of all beam particles accross all MPI ranks. */
    G4int * nsT = new G4int;
    /** Time to end drift (= time fastest particle takes to reach drift distance) at each MPI rank. 
    */
    G4double * td0 = new G4double;
    /** Time of drift merged accross all MPI ranks. */
    G4double * tdT = new G4double;
    /** Auxiliary array to store properties at each MPI rank. */
    G4double mpi_in[7] = {0};
    /** Auxiliary array to store properties merged accross all MPI ranks. */
    G4double mpi_out[7] = {0};
};

#endif /* PROPERTIES_HPP */