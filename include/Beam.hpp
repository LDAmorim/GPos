/**
 * @file Beam.hpp
 * @author Ligia Diana Amorim
 * @date 06/2021
 * @copyright GPos 2021 LBNL
 */

#ifndef BEAM_hpp
#define BEAM_hpp

#include <vector>
#include <globals.hh>
#include <Randomize.hh>
#include <G4ThreeVector.hh>
#include <CLHEP/Units/SystemOfUnits.h>
#include <G4PhysicalConstants.hh>
#include <G4MPImanager.hh>

#include "Query.hpp"

using namespace std;

using CLHEP::mm;

/**
 * Class where initial primary beam that is sent into foil is defined.
 */
class Beam {
public:
    Beam ();
    ~Beam ();
    /** Total number of beam particles per MPI rank (split between each thread). */
    G4int beam_np;
    /** Primary particle species. */
    G4String beam_type;
    /** Vector of primary particles average kinetic energy. */
    vector<G4double> beam_energy;
    /** Vector of primary particles 3D centroid. */
    vector<G4ThreeVector> beam_position;
    /** Vector of primary particles 3D normalized momentum. */
    vector<G4ThreeVector> beam_direction;
    
    void set_beam (Query in);
    G4ThreeVector get_position (G4int eventID);
    G4double get_energy (G4int eventID);
    G4ThreeVector get_direction (G4int eventID);
};

#endif /* BEAM_hpp */
