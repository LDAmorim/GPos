/**
 * @file MyEventAction.hpp
 * @author Ligia Diana Amorim
 * @date 06/2021
 * @copyright GPos 2021 LBNL
 */

#ifndef MYEVENTACTION_hpp
#define MYEVENTACTION_hpp

#include <globals.hh>
#include <G4UserEventAction.hh>
#include <G4Event.hh>

#include "MyRunAction.hpp"
#include "Query.hpp"
#include "Part.hpp"

class MyRunAction;

/**
 * Class with each primary particle trajectory information (as well as its associated secondary 
 * particles).
 */
class MyEventAction : public G4UserEventAction
{
public:
    MyEventAction (Query in, MyRunAction * runAction);
    virtual ~MyEventAction ();
    virtual void BeginOfEventAction (const G4Event* event);
    virtual void EndOfEventAction (const G4Event* event);
    void CompareTFoil (G4double tg);
    void AddParticle (Part p);
    void AddMapMQ (G4String name, G4double mass,G4double charge);

private:
    /** Access to the parameters red from input.txt file */
    Query input;
    /** Global time at end of world for all particles of each thread and MPI rank.
     * 
     * If ifback was set to True/False in the input.txt file, this time corresponds to the min/max
     *  time that particles took to reach the end of the world.
     */
    G4double tfoil;
    /** Particle data for each event. */
    vector<Part> parts;
    /** Particle species mass and charge. */
    map<G4String,map<G4String,G4double>>  masscharge;
    /** Pointer to overall run (= all steps) for each MPI rank. */
    MyRunAction * fMyRunAction;
};

#endif /* MYEVENTACTION_hpp */
