/**
 * @file GPos.hpp
 * @author Ligia Diana Amorim
 * @date 06/2021
 * @copyright GPos 2021 LBNL
 */

#ifndef GPOS_hpp
#define GPOS_hpp

#include <iostream>
#include <ctime>
#include <globals.hh>
#include <mpi.h>

#ifdef G4MULTITHREADED
#include <G4MTRunManager.hh>
#else
#include <G4RunManager.hh>
#endif /* G4MULTITHREADED */

#include <QGSP_BERT.hh>
#include <FTFP_BERT.hh>
#include <G4UIExecutive.hh>
#include <G4UImanager.hh>
#include <G4MPImanager.hh>
#include <G4MPIsession.hh>

#include "Beam.hpp"
#include "CWorld.hpp"
#include "CFoil.hpp"
#include "MyActionInitialization.hpp"
#include "MyDetectorConstruction.hpp"
#include "MyEventAction.hpp"
#include "MyPrimaryGeneratorAction.hpp"
#include "MyRunAction.hpp"
#include "MyUserSteppingAction.hpp"
#include "OutputOPMD.hpp"
#include "OutputTXT.hpp"
#include "Part.hpp"
#include "Properties.hpp"
#include "Query.hpp"

/**
 * \defgroup GPos Main file
*/
using namespace std;

#endif /* GPOS_hpp */
