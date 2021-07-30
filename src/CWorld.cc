/**
 * @file CWorld.cc
 * @author Ligia Diana Amorim
 * @date 06/2021
 * @copyright GPos 2021 LBNL
 */

#include "CWorld.hpp"

/**
 * Constructor.
 */
CWorld::CWorld ()
{}

/**
 * Destructor.
 */
CWorld::~CWorld ()
{}

/**
 * Function to set world properties read from input.txt.
 * 
 * @param[in] name Name given to the world space.
 * @param[in] dimensions 3D size of the world space.
 * @param[in] material Type of material of the world space 
 * (G4_Galactic has 1e-25g/cm3 and 21.8 eV).
 */
void CWorld::set_world (G4String name, G4ThreeVector dimensions, G4String material)
{
    c_name = name;
    c_dimensions = dimensions;
    c_material = material;
}
