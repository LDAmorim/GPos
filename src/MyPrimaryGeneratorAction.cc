/**
 * @file MyPrimaryGeneratorAction.cc
 * @author Ligia Diana Amorim
 * @date 06/2021
 * @copyright GPos 2021 LBNL
 */

#include "MyPrimaryGeneratorAction.hpp"

/**
 * Constructor, where pgun is initialized and primary particles are distributed accross MPI ranks.
 * 
 * @param[in] b Pointer to where primary particle data will be stored.
 * @param[in] in Parameters read from input.txt file.  
 */
MyPrimaryGeneratorAction::MyPrimaryGeneratorAction (Beam * b, Query in)
: G4VUserPrimaryGeneratorAction(), pgun(0)
{
    G4int n_particle = 1;
    pgun  = new G4ParticleGun(n_particle);

    G4ParticleTable *particle_table = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition *part = particle_table->FindParticle(b->beam_type);
    pgun->SetParticleDefinition(part);

    input = in;
    initial_beam = b;

    G4int rank = G4MPImanager::GetManager()->GetRank();
    G4int tid = G4Threading::G4GetThreadId();
    G4cout << "Run started for MPI rank " << rank << " and task " << tid  << " with # parts : "
    << input.np << "\n";

    if (tid == 0){
        G4int nspecies = input.s_list.size();
        G4String pname = input.type+"_primary";
        map<G4String, PartBeams> primary;
        G4int pindex = -1;
        for ( G4int i = 0; i < nspecies; ++i) {
            if (input.s_list[i] == pname){
                pindex = i;
                primary[pname].set_partBeams(initial_beam, pname, "primary",
                part->GetPDGMass(), part->GetPDGCharge());
                if (input.debug) primary[pname].Print("at the begining of the run");
                np0[0] = primary[pname].np;
            }
            else np0[i] = 0;
        }
        if (pindex >= 0 ){
            MPI_Allreduce(np0, npT, nspecies, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
            MPI_Scan(np0, npS, nspecies, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
            G4String * label = new G4String;
            *label = "_initial";
            save_outOPMD(input, &primary, npT, npS, label);
            Properties p(input);
            p.Analysis(&primary, np0, npT, 0);
            *label = "Initial";
            if (rank == 0) save_txt(input, &primary, npT, 0, label);
        }
    }
}

/**
 * Destructor, where pgun is deleted.
 */
MyPrimaryGeneratorAction::~MyPrimaryGeneratorAction ()
{
    delete pgun;
}

/**
 * Function to set pgun particle parameters and retrieve them for debugging, if chosen.
 * 
 * @param[in] anEvent Event identifier pointer.
 */
void MyPrimaryGeneratorAction::GeneratePrimaries (G4Event *anEvent)
{
    G4int eventID = anEvent->GetEventID();
    pgun->SetParticleEnergy(initial_beam->get_energy(eventID-1));
    pgun->SetParticleMomentumDirection(initial_beam->get_direction(eventID));
    pgun->SetParticlePosition(initial_beam->get_position(eventID));
    pgun->GeneratePrimaryVertex(anEvent);

    if (input.debug){
        vector<G4double> curr_quant;
        curr_quant.push_back(pgun->GetParticlePosition().x());
        curr_quant.push_back(pgun->GetParticlePosition().y());
        curr_quant.push_back(pgun->GetParticlePosition().z());
        curr_quant.push_back(pgun->GetParticleMomentum());
        curr_quant.push_back(pgun->GetParticleMomentumDirection().x());
        curr_quant.push_back(pgun->GetParticleMomentumDirection().y());
        curr_quant.push_back(pgun->GetParticleMomentumDirection().z());
        curr_quant.push_back(pgun->GetParticleEnergy());
        G4double mass = pgun->GetParticleDefinition()->GetPDGMass();
        G4double charge = pgun->GetParticleDefinition()->GetPDGCharge();
        G4String name = pgun->GetParticleDefinition()->GetParticleName();
        print_primary(curr_quant,mass,charge,name,eventID);
    }
}

/**
 * Function to print pgun particle information for debugging, if chosen.
 * 
 * @param[in] cq  Primary particle initial kinetic energy.
 * @param[in] mass  Primary particle mass.
 * @param[in] charge  Primary particle charge.
 * @param[in] n  Primary particle ID (= event #).
 * @param[in] eID  Primary particle ID (= event #).
 */
void MyPrimaryGeneratorAction::print_primary (vector<G4double> cq, G4double mass, G4double charge, 
G4String n, G4int eID){
    G4cout.precision(15);
    G4cout << "------------------------------------------------------"
    <<"------\n\n";
    G4cout << "Event #" << eID << " " << n << " information:\n"
    << "  Mass = "<<mass<<" MeV/c^2\n"
    << "  Charge = "<< charge <<" e_SI\n"
    << "  Kinetic energy = "<<cq[7]<<"\n"
    << "  Position = ("<<cq[0]<<" , "<<cq[1]<<" , "<<cq[2]<<")\n"
    << "  Momentum = ("<<cq[4]<<" , "<<cq[5]<<" , "<<cq[6]<<") * "<<cq[3]<<"\n";
    G4cout << "\n------------------------------------------------------"
    <<"------\n\n";
}
