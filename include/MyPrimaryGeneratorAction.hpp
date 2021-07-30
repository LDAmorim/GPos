/**
 * @file MyPrimaryGeneratorAction.hpp
 * @author Ligia Diana Amorim
 * @date 06/2021
 * @copyright GPos 2021 LBNL
 */

#ifndef MYPRIMARYGENERATORACTION_hpp
#define MYPRIMARYGENERATORACTION_hpp

#include <G4VUserPrimaryGeneratorAction.hh>
#include <CLHEP/Units/SystemOfUnits.h>
#include <G4ParticleTable.hh>
#include <G4ParticleGun.hh>
#include <G4Event.hh>
#include <G4LogicalVolume.hh>
#include <G4Box.hh>
#include <G4LogicalVolumeStore.hh>

#include "Beam.hpp"
#include "Part.hpp"
#include "Properties.hpp"
#include "OutputOPMD.hpp"

/**
 * Class where each primary electron (= event) initial properties are set for each MPI rank 
 * (all threads receive the same data).
 */
class MyPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
    /** Class access to the parameters red from input.txt file */
    Query input;
    /** Primary particles (= events) initial data for each MPI rank. */
    Beam * initial_beam;
    /** Primary # of particles for each MPI rank. */
    G4int * np0 = new G4int;
    /** Primary total # of particles for all MPI rank. */
    G4int * npT = new G4int;
    /** Primary partial # of particles summed up to each MPI rank. */
    G4int * npS = new G4int;

    MyPrimaryGeneratorAction (Beam * b, Query in);
    virtual ~MyPrimaryGeneratorAction ();
    virtual void GeneratePrimaries (G4Event *anEvent);
    /** Function to return Geant4 particle gun pointer. */
    const G4ParticleGun* GetParticleGun () const { return pgun; }
    
private:
    /** Pointer to the Geant4 gun from where primary electrons are sent*/
    G4ParticleGun *pgun;
    void print_primary (vector<G4double> cq, G4double mass, G4double charge, G4String n, G4int eID);
};

#endif /* MYPRIMARYGENERATORACTION_hpp */
