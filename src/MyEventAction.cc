/**
 * @file MyEventAction.cc
 * @author Ligia Diana Amorim
 * @date 06/2021
 * @copyright GPos 2021 LBNL
 */

#include "MyEventAction.hpp"

/**
 * Constructor, where memory size for particle data storage is allocated 
 * to ensure code doesn't run into memory load issues.
 * 
 * @param[in] in Parameters read from input.txt file. 
 * @param[in] runAction Pointer overall run (= all steps) for each MPI rank. 
 */
MyEventAction::MyEventAction (Query in, MyRunAction * runAction)
: G4UserEventAction(),
  fMyRunAction(runAction)
{
    input = in;
    if (input.ifback) tfoil = numeric_limits<G4double>::max();
    else tfoil = 0.0;
    // Multiplying factor should be the same as size set in MyRunAction()
    // 20 is sufficient for up to 2 mm thick foils
    parts.reserve(40);
    G4ExceptionDescription msg;
    msg << "\nGPos only allocates 40 parts per MPI rank for each Event\n";
    G4Exception("MyUserSteppingAction::MyUserSteppingAction()",
                "GPos warning #7.0",JustWarning,msg);
}

/**
 * Destructor, where particle and species data are cleared.
 */
MyEventAction::~MyEventAction ()
{
    parts.clear();
    masscharge.clear();
}

/**
 * Particle data object is cleared for new event to be processed.
 */
void MyEventAction::BeginOfEventAction (const G4Event*)
{
    parts.clear();
}

/**
 * At the end of each event, the global time at end of world for all particles, their data as well 
 * as mass and charge information are sent to fMyRunAction.
 * 
 * @param anevent Event identifier pointer.
 */
void MyEventAction::EndOfEventAction (const G4Event* anevent)
{
    G4cout << "Event # " << anevent-> GetEventID() << "\n";
    fMyRunAction -> ComputeTFoil(tfoil);
    fMyRunAction -> AddParticles(parts);
    fMyRunAction -> AddMapMQ(masscharge);
}

/**
 * Function to update the time to reach the end of world after each event.
 * 
 * @param tg Global end time at end of event for all particles of each event.
 */
void MyEventAction::CompareTFoil (G4double tg){
    if (input.ifback) {
        tfoil = min(tfoil, tg);
    }
    else {
        tfoil = max(tfoil, tg);
    }
}

/**
 * Storing particle data after each event.
 * 
 * @param p Particle data of each event.
 */
void MyEventAction::AddParticle (Part p)
{
    parts.push_back(p);
    if (parts.size() == parts.capacity()) {
        G4ExceptionDescription msg;
        msg << "\nProgram ran out of memory -> Use more MPI ranks or change parts reserved size.\n"
        << "#parts : " << parts.size() << "\ncapacity : " << parts.capacity() << "\n";
        G4Exception("MyEventAction::AddParticle()",
        "GPos error #7.0",FatalException,msg);
    }
}

/**
 * Storing particle species information after each event.
 * 
 * @param name Particle species name.
 * @param mass Particle species mass.
 * @param charge Particle species charge.
 */
void MyEventAction::AddMapMQ (G4String name, G4double mass , G4double charge)
{
    if ( masscharge.find(name) == masscharge.end() ){
        masscharge[name]["mass"] = mass;
        masscharge[name]["charge"] = charge;;
    }
}
