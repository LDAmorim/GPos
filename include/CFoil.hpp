/**
 * @file CFoil.hpp
 * @author Ligia Diana Amorim
 * @date 06/2021
 * @copyright GPos 2021 LBNL
 */

#ifndef CFOIL_hpp
#define CFOIL_hpp

#include <G4Colour.hh>

#include "CWorld.hpp"

/**
 * Class where the Geant4 foil properties are stored.
 */
class CFoil: public CWorld
{
public:
    /** Translation required to place foil according to centroid values of 
     * x_foil read from input.txt. */
    G4ThreeVector c_translation;
    /** Colour used for describing the foil using Geant4 visual attributes. */
    G4Colour c_colour;
    CFoil ();
    virtual ~CFoil ();
    void set_foil (G4String name, G4ThreeVector dimensions, G4String material,
    G4ThreeVector translation);
};

#endif /* CFOIL_hpp */
