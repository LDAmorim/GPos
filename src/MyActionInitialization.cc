/**
 * @file MyActionInitialization.cc
 * @author Ligia Diana Amorim
 * @date 06/2021
 * @copyright GPos 2021 LBNL
 */

#include "MyActionInitialization.hpp"
/**
 * Constructor.
 * 
 * @param[in] b Pointer to initial primary particles data.
 * @param[in] in Parameters read from input.txt file. 
 */
MyActionInitialization::MyActionInitialization (Beam * b, Query in)
: G4VUserActionInitialization()
{
    input = in;
    initial_beam = b;
}

/**
 * Destructor where all particle data is cleared.
 */
MyActionInitialization::~MyActionInitialization ()
{
    particles -> clear();
    masscharge -> clear();
}

/**
 * Master thread build - invoked when running program in parallel.
 */
void MyActionInitialization::BuildForMaster () const
{
    MyRunAction* runAction = new MyRunAction(input, tfoilM, particles, masscharge);
    SetUserAction(runAction);
}

/**
 * Worker threads build allows communication between:
 * 1) generation of the primary particles to be sent into the foil
 * 2) initialization of the run
 * 3) initialization of each event - from when primary particle is sent to the end of 
 * its and its secondary particles trajectories
 * 4) initialization of the stepping actions corresponding to each interaction modelled
 */
void MyActionInitialization::Build () const
{
    MyPrimaryGeneratorAction *primarygeneratoraction =
    new MyPrimaryGeneratorAction(initial_beam,input);
    SetUserAction(primarygeneratoraction);

    MyRunAction* runAction = new MyRunAction(input, tfoilM, particles, masscharge);
    SetUserAction(runAction);

    MyEventAction* eventAction = new MyEventAction(input, runAction);
    SetUserAction(eventAction);

    MyUserSteppingAction *mysteppingaction = new MyUserSteppingAction(input, eventAction);
    SetUserAction(mysteppingaction);
}
