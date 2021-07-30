/**
 * @file CFoil.cc
 * @author Ligia Diana Amorim
 * @date 06/2021
 * @copyright GPos 2021 LBNL
 */

#include "CFoil.hpp"

/**
 * Constructor.
 */
CFoil::CFoil ()
{}

/**
 * Destructor.
 */
CFoil::~CFoil ()
{}

/**
 * Function to set foil properties read from input.txt.
 * 
 * @param[in] name Name given to the foil.
 * @param[in] dimensions 3D size of the foil.
 * @param[in] material Type of material of the foil.
 * @param[in] translation Translation required to place foil at centroid x_foil.
 */
void CFoil::set_foil (G4String name, G4ThreeVector dimensions, G4String material, 
G4ThreeVector translation)
{
    c_name = name;
    c_dimensions = dimensions;
    c_material = material;
    c_translation = translation;
}
