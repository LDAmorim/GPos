/**
 * @file MyRunAction.hpp
 * @author Ligia Diana Amorim
 * @date 06/2021
 * @copyright GPos 2021 LBNL
 */

#ifndef MYRUNACTION_hpp
#define MYRUNACTION_hpp

#include <filesystem>
#include <G4UserRunAction.hh>
#include <G4Run.hh>
#include <globals.hh>
#include <G4UnitsTable.hh>
#include <G4SystemOfUnits.hh>
#include <G4Threading.hh>
#include <G4MPImanager.hh>
#include <G4AccumulableManager.hh>
#include <G4Accumulable.hh>

#include "MyPrimaryGeneratorAction.hpp"
#include "Properties.hpp"
#include "Query.hpp"

class G4Run;

/**
 * Class with all primary particles trajectory information  (as well as their associated secondary 
 * particles).
 */
class MyRunAction : public G4UserRunAction
{
public:
    MyRunAction (Query in, G4double * tf, vector<Part> * p,
    map<G4String,map<G4String,G4double>> * mq);
    virtual ~MyRunAction ();
    virtual void BeginOfRunAction (const G4Run*);
    virtual void EndOfRunAction (const G4Run*);
    void ComputeTFoil (G4double tf);
    void AddParticles (vector<Part> p);
    void AddMapMQ (map<G4String,map<G4String,G4double>> mq);
    void Reduce ();

private:
    /** Access to the parameters red from input.txt file. */
    Query input;
    /** Global time at end of world for all particles of each thread and MPI rank.
     * 
     * If ifback was set to True/False in the input.txt file, this time corresponds to the min/max 
     * time that particles took to reach the end of the world.
     */
    G4Accumulable<G4double> tfoil;
    /** (Min/Max) Global time at end of world accross all threads and MPI ranks. */
    G4double * tfoilM;
    /** Particle data for all events. */
    vector<Part> * particles;
    /** Particle species mass and charge. */
    map<G4String,map<G4String,G4double>> * masscharge;
};

#endif /* MYRUNACTION_hpp */
