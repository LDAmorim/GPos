/**
 * @file MyActionInitialization.hpp
 * @author Ligia Diana Amorim
 * @date 06/2021
 * @copyright GPos 2021 LBNL
 */

#ifndef MYACTIONINITIALIZATION_hpp
#define MYACTIONINITIALIZATION_hpp

#include <G4VUserActionInitialization.hh>
#include <G4Threading.hh>

#include "MyDetectorConstruction.hpp"
#include "MyPrimaryGeneratorAction.hpp"
#include "MyActionInitialization.hpp"
#include "MyUserSteppingAction.hpp"
#include "MyRunAction.hpp"
#include "MyEventAction.hpp"
#include "Beam.hpp"
#include "Query.hpp"

/**
 * Class that initializes generation, run, event and stepping actions.
 */
class MyActionInitialization : public G4VUserActionInitialization
{
public:
    MyActionInitialization (Beam * b, Query in);
    virtual ~MyActionInitialization ();
        
    virtual void BuildForMaster () const;
    virtual void Build () const;
private:
    /** Class access to the parameters red from input.txt file */
    Query input;
    /** Primary particles (= events) initial data for each MPI rank. */
    Beam * initial_beam;
    /** Pointer to store time at end of world accross all threads and MPI ranks. */
    G4double * tfoilM = new G4double;
    /** Pointer to store all relevant particles data at end of world. */
    vector<Part> * particles = new vector<Part>;
    /** Pointer to store all species mass and charge information. */
    map<G4String,map<G4String,G4double>> * masscharge = new map<G4String,map<G4String,G4double>>;
};

#endif /* MYACTIONINITIALIZATION_hpp */
