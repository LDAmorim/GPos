/**
 * @file MyRunAction.cc
 * @author Ligia Diana Amorim
 * @date 06/2021
 * @copyright GPos 2021 LBNL
 */

#include "MyRunAction.hpp"

/**
 * Constructor, where memory size for particle data storage is allocated to ensure code doesn't run 
 * into memory load issues and tfoil parallel min/max values are initialized.
 * 
 * @param[in] in Parameters read from input.txt file. 
 * @param[in] tf Pointer to final time of each event. 
 * @param[in] p Pointer to particle data of each event. 
 * @param[in] mq Pointer to particle species mass and charge information. 
 */
MyRunAction::MyRunAction (Query in, G4double * tf, vector<Part> * p, 
map<G4String,map<G4String,G4double>> * mq)
: G4UserRunAction(),
tfoil(0.0, G4MergeMode::kMaximum)
{
    input = in;
    tfoilM = tf;
    particles = p;
    // Multiplying factor should be the same as size set in MyEventAction().
    // 20 is sufficient for up to 2 mm thick foils.
    particles->reserve(input.np*40);
    G4cout << "\nGPos only allocates 40 parts per MPI rank for each Event in each Run\n";
    masscharge = mq;
    if (input.ifback){
        tfoil = G4Accumulable<G4double>( numeric_limits<G4double>::max(), G4MergeMode::kMinimum);
    }
    G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
    accumulableManager->RegisterAccumulable(tfoil);
}

/**
 * Destructor.
 */
MyRunAction::~MyRunAction ()
{
}

/**
 * Run start function, where the runManager saves the random number seed so that simulation results 
 * can be reproduced and the accumulable tfoil is initialized.
 */
void MyRunAction::BeginOfRunAction (const G4Run*)
{
    G4RunManager::GetRunManager()->SetRandomNumberStore(false);
    G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
    accumulableManager->Reset();
}

/**
 * At the end of each run, tfoil is merged accross parallel threads and MPI ranks.
 * 
 * @param run Run identifier pointer.
 */
void MyRunAction::EndOfRunAction (const G4Run* run)
{
    G4int nofEvents = run->GetNumberOfEvent();
    if (nofEvents == 0) return;

    G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
    accumulableManager->Merge();

    G4int rank = G4MPImanager::GetManager()->GetRank();
    G4int tid = G4Threading::G4GetThreadId();
    if (IsMaster()) {
        *tfoilM = tfoil.GetValue();
        if (input.debug) G4cout << "Accumulated tfoil = " << *tfoilM << " ns\n";
        
        Reduce();

        G4cout << "End of Global Run for MPI rank "
        << rank << " with total # Events : " << nofEvents 
        << ", task " << tid  << ", # parts : " << particles-> size() << "\n";
        Properties p(input);
        p.ComputeBeams(tfoilM, particles, masscharge);
    }
    else {
        G4cout << "End of Local Run for MPI rank "
        << rank << " and task " << tid 
        << " with # parts : " << particles-> size() << "\n";
    }
}

/**
 * At the end of each event, tfoil is updated to represent the min/max global time of all particles 
 * up to that event.
 * 
 * @param tg Global end time of each event.
 */
void MyRunAction::ComputeTFoil (G4double tg){
    if (input.ifback) {
        tfoil = min(tfoil.GetValue(), tg);
    }
    else {
        tfoil = max(tfoil.GetValue(), tg);
    }
}

/**
 * At the end of each event, particle data is added to particles vector.
 * 
 * @param p vector containing all event particles data.
 */
void MyRunAction::AddParticles (vector<Part> p)
{
    particles->insert(particles->begin(), p.begin(), p.end());
    if (particles->size() == particles->capacity()) {
        G4ExceptionDescription msg;
        msg << "\nProgram ran out of memory -> Use more MPI ranks\n"
        << "#particles : " << particles->size()
        << "\ncapacity : " << particles->capacity() << "\n";
        G4Exception("MyRunAction::AddParticles()",
        "GPos error #9.0",FatalException,msg);
    }
}

/**
 * At the end of each event, each new species mass and charge information is added.
 * 
 * @param mq map of species mass and charge.
 */
void MyRunAction::AddMapMQ (map<G4String,map<G4String,G4double>> mq)
{
    for (const auto &name : mq){
        if ( masscharge -> find(name.first) == masscharge -> end() ){
            (*masscharge)[name.first] = mq[name.first];
        }
    }
}

/**
 * Function to merge the global end time accross all MPI ranks (called after thread information is 
 * meged).
 */
void MyRunAction::Reduce ()
{
    G4double tf = tfoil.GetValue();
    MPI_Op op;
    if (input.ifback) op = MPI_MIN;
    else op = MPI_MAX;

    G4int reduce = MPI_Allreduce(&tf, tfoilM, 1, MPI_DOUBLE,
    op, MPI_COMM_WORLD);
    if (reduce){
        G4ExceptionDescription msg;
        msg << "\nMPI Reduce failed\n";
        G4Exception("MyRunAction::Reduce()",
                    "GPos error #9.1",FatalException,msg);
    }
}
