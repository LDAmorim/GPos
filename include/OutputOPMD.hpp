/**
 * @file OutputOPMD.hpp
 * @author Ligia Diana Amorim
 * @date 06/2021
 * @copyright GPos 2021 LBNL
 */

#ifndef OUTPUTOPMD_hpp
#define OUTPUTOPMD_hpp

#include <iostream>
#include <cstring>
#include <memory>
#include <numeric>
#include <cstdlib>
#include <vector>
#include <mpi.h>
#include <openPMD/openPMD.hpp>
#include <G4ParticleTable.hh>
#include <G4ParticleDefinition.hh>
#include <G4MPImanager.hh>
#include <G4Threading.hh>
#include <globals.hh>

#include "Properties.hpp"
#include "MyUserSteppingAction.hpp"
#include "Query.hpp"


using namespace std;
using namespace openPMD;


/**
 * \defgroup OutputOPMD Functions to generate openPMD data files
*/
G4int process_map (G4String process);

void save_outOPMD (Query in, map<G4String, PartBeams> * b, G4int * nT, G4int * nS,G4String * label);

#endif /* OUTPUTOPMD_hpp */
