/**
 * @file OutputTXT.hpp
 * @author Ligia Diana Amorim
 * @date 06/2021
 * @copyright GPos 2021 LBNL
 */

#ifndef OUTPUTTXT_hpp
#define OUTPUTTXT_hpp

#include <vector>
#include <iostream>
#include <cstring>
#include <memory>
#include <numeric>
#include <cstdlib>
#include <fstream>
#include <globals.hh>
#include <G4RunManager.hh>
#include <G4PhysicalConstants.hh>

#include "Properties.hpp"


using namespace std;

/**
 * \defgroup OutputTXT Function to save properties to TXT
*/
void save_txt (Query in, map<G4String,PartBeams> * b, G4int * nT, G4int ifdrift, G4String * label);

#endif /* OUTPUTTXT_hpp */
