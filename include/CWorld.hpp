/**
 * @file CWorld.hpp
 * @author Ligia Diana Amorim
 * @date 06/2021
 * @copyright GPos 2021 LBNL
 */

#ifndef CWORLD_hpp
#define CWORLD_hpp

#include <G4ThreeVector.hh>

/**
 * Class where the Geant4 world properties are stored.
 */
class CWorld
{
public:
    /** Name of the world space. */
    G4String c_name;
    /** 3D size of the world space. */
    G4ThreeVector c_dimensions;
    /** Material of the world space. */
    G4String c_material;
    CWorld ();
    virtual ~CWorld ();
    void set_world (G4String name, G4ThreeVector dimensions, G4String material);
};

#endif /* CWORLD_hpp */
