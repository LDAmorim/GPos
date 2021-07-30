/**
 * @file GPos.cc
 * @author Ligia Diana Amorim
 * @date 06/2021
 * @copyright GPos 2021 LBNL
 */

#include "GPos.hpp"

/**
 * \ingroup GPos
 * Main program where multithreads and MPI ranks are initialzied and finalized.
 * 
 * @param[in] argc Number of MPI arguments
 * @param[in] argv Pointer to MPI arguments
 */
int main (int argc, char *argv[])
{
    G4MPImanager* g4MPI = new G4MPImanager(argc, argv);
    G4MPIsession* session = g4MPI-> GetMPIsession();
    G4int nranks = g4MPI-> GetTotalSize();
    G4int master = g4MPI-> IsMaster();

    if (master){
        G4cout << "Program GPos started with:\n\tCompiler version ";
        if (__cplusplus == 201703L) G4cout << "C++17";
        else if (__cplusplus == 201402L) G4cout << "C++14";
        else if (__cplusplus == 201103L) G4cout << "C++11";
        else if (__cplusplus == 199711L) G4cout << "C++98";
        else std::cout << "pre-standard C++";
        G4cout << "\n";
        time_t t;
        struct tm * tt;
        time (&t);
        tt = localtime(&t);
        G4cout << "\t" << asctime(tt);
        G4cout << "\t" << nranks << " MPI rank(s)\n\n";
    }
    
    Query in;
    in.read_input();
    if (master){
        in.print_input();
    }

    Beam * be = new Beam;
    be->set_beam(in);
    
#ifdef G4MULTITHREADED
    vector<string> args{argv + 1, argv + argc};
    int threads_num = args.size() > 0 ? stoi(args[0]) : 1;
    if (master) G4cout << "\t" << threads_num << " threads per MPI\n\n";
    G4MTRunManager *runManager = new G4MTRunManager;
    runManager -> SetNumberOfThreads(threads_num);
#else
    G4RunManager *runManager = new G4RunManager;
    if (master) G4cout << "\t1 thread per MPI\n\n";
    << G4endl;
#endif
    if (in.debug) runManager->SetVerboseLevel(1);
    else runManager->SetVerboseLevel(0);

    MyDetectorConstruction *detector = new MyDetectorConstruction(in);
    runManager->SetUserInitialization(detector);

    G4VModularPhysicsList *physicsList = new QGSP_BERT;
    //FTFP_BERY should be used instead if primary articles energy is <5GeV;
    //Cut off ranges after which the energy loss is continuous because
    //particles no longer create secondaries (gamma is set first)
    physicsList->SetCutValue(detector->foil_thickness/10.,"gamma");
    physicsList->SetCutValue(detector->foil_thickness/10.,"e+");
    physicsList->SetCutValue(detector->foil_thickness/10.,"e-");
    if (in.debug) physicsList->SetVerboseLevel(1);
    else physicsList->SetVerboseLevel(0);
    runManager->SetUserInitialization(physicsList);

    MyActionInitialization * myActionInitialization = new MyActionInitialization(be, in);
    runManager->SetUserInitialization(myActionInitialization);
    runManager->Initialize();

    runManager->BeamOn(be->beam_np);
    session-> SessionStart();

    delete g4MPI;
    delete runManager;

    time_t t;
    struct tm * tt;
    time (&t);
    tt = localtime(&t);
    G4cout << "\n\n"<< asctime(tt);

    G4cout<<"\nProgram GPos ran successfully"<<G4endl;

    return 0;
}
