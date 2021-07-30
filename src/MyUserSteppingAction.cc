/**
 * @file MyUserSteppingAction.cc
 * @author Ligia Diana Amorim
 * @date 06/2021
 * @copyright GPos 2021 LBNL
 */

#include "MyUserSteppingAction.hpp"

/**
 * Constructor, where auxiliary files are open if running in debug mode.
 * 
 * @param[in] in Parameters read from input.txt file. 
 * @param[in] eventAction Pointer to main event of each step. 
 */
MyUserSteppingAction::MyUserSteppingAction (Query in, MyEventAction * eventAction)
: G4UserSteppingAction(),
  fEventAction(eventAction)
{
    input = in;
    mqlist = new bool[in.s_list.size()]{};
    if (input.debug){
        G4int tid = G4Threading::G4GetThreadId();
        G4int rank = G4MPImanager::GetManager()->GetRank();
        G4String fname ="thread_"+to_string(tid)+
        "_mpi_"+to_string(rank)+".txt";
        f.open(fname);
        G4cout << "\nCreated " << fname << "\n";
        
        f << "\tData from thread # " << tid << "\n\nName\tProcess\tStep\t";
        f << "x\ty\tz\tpx\tpy\tpz\ttotE\tGlobalt\n";
    }
}

/**
 * Destructor, where auxiliary files are closed if they were opened.
 */
MyUserSteppingAction::~MyUserSteppingAction ()
{
    if (f.is_open()){
        f.close();
    }
}

/**
 * At the each step, of each event in each thread and MPI rank, the information of all particles 
 * generated is provided and data of those selected for output in the input.txt file is stored.
 * 
 * @param step Step identifier pointer.
 */
void MyUserSteppingAction::UserSteppingAction (const G4Step *step)
{
    G4Track *track = step->GetTrack();
    G4bool ifstore = false;
    G4String name = track->GetParticleDefinition()->GetParticleName();
    G4int pID = track->GetParentID(); // 0 for primary particles
    if (pID == 0){
        name = name + "_primary";
    }
    for (size_t i = 0; i < input.s_list.size(); ++i){
        if (input.s_list[i] == name){
            ifstore = true;
            // Collecting end of particle trajectory data
            if(track->GetTrackStatus() == fStopAndKill){
                G4double z = track->GetPosition().z();
                // Neglect particles that reach end of world transversely
                if (z == input.w_world.z()/2.0){
                    G4double pz = track->GetMomentum().z();
                    // Checking that particle lies above longitudinal threshold
                    if (pz > input.pzcutoff[i]){
                        G4double totE = track->GetTotalEnergy();
                        G4double mass = track->GetParticleDefinition()->GetPDGMass();
                        // Checking that particle has not decayed / been depleted
                        if (totE > max(input.encutoff[i],mass)){
                            Part particle;
                            if (pID == 0){
                                particle.set_part(name, "primary",
                                track->GetPosition().x(), track->GetPosition().y(), z, 
                                track->GetMomentum().x(), track->GetMomentum().y(), pz, totE, 
                                track->GetGlobalTime(), track->GetCurrentStepNumber());
                            }
                            else{
                                particle.set_part(name,
                                track->GetCreatorProcess()->GetProcessName(),
                                track->GetPosition().x(), track->GetPosition().y(), z, 
                                track->GetMomentum().x(), track->GetMomentum().y(), pz, totE, 
                                track->GetGlobalTime(), track->GetCurrentStepNumber());
                            }
                            fEventAction -> AddParticle(particle);
                            if ( !mqlist[i] ){
                                G4double charge = track->GetParticleDefinition()->GetPDGCharge();
                                fEventAction -> AddMapMQ(name, mass, charge);
                                mqlist[i] = 1;
                            }
                            fEventAction -> CompareTFoil(particle.tg);
                            if (input.debug){
                                if (pID == 0){
                                    f << name << "\tprimary\t";
                                }
                                else{
                                    f << name << "\t" 
                                    << track->GetCreatorProcess()->GetProcessName() << "\t";
                                }
                                f << track->GetCurrentStepNumber() << "\t"
                                << track->GetPosition().x() << "\t"
                                << track->GetPosition().y() << "\t"
                                << z << "\t"
                                << track->GetMomentum().x() << "\t"
                                << track->GetMomentum().y() << "\t"
                                << pz << "\t"
                                << totE << "\t"
                                << track->GetGlobalTime() << "\n";
                            }
                        }
                    }
                }
            }
        }
    }
    if(!ifstore){
        if (input.debug){
            if (track->GetCurrentStepNumber() == 1)
            G4cout << name << " generated\n";
        }
    }
}
