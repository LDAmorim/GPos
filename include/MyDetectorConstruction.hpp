/**
 * @file MyDetectorConstruction.hpp
 * @author Ligia Diana Amorim
 * @date 06/2021
 * @copyright GPos 2021 LBNL
 */

#ifndef MYDETECTORCONSTRUCTION_hpp
#define MYDETECTORCONSTRUCTION_hpp

#include <G4VUserDetectorConstruction.hh>
#include <CLHEP/Units/SystemOfUnits.h>
#include <G4RotationMatrix.hh>
#include <G4VisAttributes.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4NistManager.hh>
#include <G4Material.hh>
#include <G4Box.hh>

#include "CWorld.hpp"
#include "CFoil.hpp"
#include "Query.hpp"

/**
 * Class that initializes the beam-world and foil setup.
 */
class MyDetectorConstruction : public G4VUserDetectorConstruction
{
public:
    /** Thickness of the foil used */
    G4double foil_thickness;
    /** Class access to the parameters red from input.txt file */
    Query input;
    /** Geant4 physical world containing the foil, where particle-foil or particle-particle 
     * interactions (= steps) occur.*/
    G4VPhysicalVolume *physWorld;

    MyDetectorConstruction (Query in);
    virtual ~MyDetectorConstruction ();
    virtual G4VPhysicalVolume *Construct ();
    G4LogicalVolume *create_world (CWorld kwargs);
    void create_thin_foil (CFoil kwargs, G4LogicalVolume *mother);
};

#endif /* MYDETECTORCONSTRUCTION_hpp */
