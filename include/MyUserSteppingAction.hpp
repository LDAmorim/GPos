/**
 * @file MyUserSteppingAction.hpp
 * @author Ligia Diana Amorim
 * @date 06/2021
 * @copyright GPos 2021 LBNL
 */

#ifndef MYUSERSTEPPINGACTION_hpp
#define MYUSERSTEPPINGACTION_hpp

#include <iostream>
#include <filesystem>
#include <globals.hh>
#include <G4Track.hh>
#include <G4VProcess.hh>
#include <G4UserSteppingAction.hh>
#include <G4PhysicalConstants.hh>
#include <G4WorkerThread.hh>
#include <G4MPImanager.hh>
#include <G4AccumulableManager.hh>

#include "MyEventAction.hpp"
#include "Part.hpp"

using namespace std;

class MyEventAction;
class Part_step;
class Part;

/**
 * Class with each particle interaction information
 */
class MyUserSteppingAction : public G4UserSteppingAction
{
public:
    MyUserSteppingAction (Query in, MyEventAction * eventAction);
    virtual ~MyUserSteppingAction ();
    
    virtual void UserSteppingAction (const G4Step*);

private:
    /** Access to the parameters red from input.txt file */
    Query input;
    /** Debug auxiliary output file with particle data in each thread */
    ofstream f;
    /** Pointer to main event of each step */
    MyEventAction * fEventAction;
    /** Mass charge species added test variable */
    bool * mqlist;
};

#endif /* MYUSERSTEPPINGACTION_hpp */
