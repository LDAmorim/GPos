/**
 * @file MyDetectorConstruction.cc
 * @author Ligia Diana Amorim
 * @date 06/2021
 * @copyright GPos 2021 LBNL
 */

#include "MyDetectorConstruction.hpp"

using CLHEP::mm;

/**
 * Constructor.
 * 
 * @param[in] in Parameters read from input.txt file. 
 */
MyDetectorConstruction::MyDetectorConstruction (Query in)
: G4VUserDetectorConstruction()
{
    input = in;
}

/**
 * Destructor.
 */
MyDetectorConstruction::~MyDetectorConstruction ()
{}

/**
 * Function where world and foil properties are set and initialized. 
 * 
 * @return physWorld Geant4 physical world containing the foil
 */
G4VPhysicalVolume *MyDetectorConstruction::Construct ()
{
    CWorld w_galactic;
    G4String w_name = "World";
    G4ThreeVector w_dimensions = input.w_world;
    G4String w_material = "G4_Galactic";
    w_galactic.set_world(w_name,w_dimensions,w_material);

    CFoil f_foil;
    if (input.hasfoil){
        G4String f_name="foil";
        G4ThreeVector f_dimensions=input.w_foil;
        G4String f_material=input.t_foil;
        G4ThreeVector translation=input.x_foil;
        f_foil.set_foil(f_name,f_dimensions,f_material,translation);
    }

    G4LogicalVolume* mother = create_world(w_galactic);
    if (input.hasfoil){
        create_thin_foil(f_foil, mother);
    }
    
    return physWorld;
}

/**
 * World initialization.
 * 
 * @param[in] kwargs Parameter with world information 
 * @return logicWorld Geant4 physical world without foil
 */
G4LogicalVolume *MyDetectorConstruction::create_world (CWorld kwargs)
{
    G4Material *material = G4NistManager::Instance()->FindOrBuildMaterial(kwargs.c_material);
    
    auto dimensions = kwargs.c_dimensions;
    G4String name = kwargs.c_name;
    
    // World is centered at (x,y,z) = (0,0,0)
    G4Box *solidWorld = new G4Box(name,dimensions[0]/2.,dimensions[1]/2.,dimensions[2]/2.);

    G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld,material,name);
    
    physWorld = new G4PVPlacement(G4Transform3D(),logicWorld,name,0,false,0,true);

    auto visual = new G4VisAttributes();
    visual->SetVisibility(false);
    logicWorld->SetVisAttributes(visual);

    return logicWorld;
}

/**
 * Foil initialization.
 * 
 * @param[in] kwargs Parameter with foil information 
 * @param[in] mother Geant4 physical world without foil
 */
void MyDetectorConstruction::create_thin_foil (CFoil kwargs, G4LogicalVolume* mother)
{
    G4String name = kwargs.c_name;
    G4Material *material = G4NistManager::Instance()->FindOrBuildMaterial(kwargs.c_material);
    auto dimensions = kwargs.c_dimensions;

    //Setting foil position relative to world center
    G4ThreeVector position = kwargs.c_translation;
    G4RotationMatrix rotm  = G4RotationMatrix();
    //rotm.rotateY(0*deg);
    G4Transform3D transform = G4Transform3D(rotm,position);

    G4Box *solidFoil = new G4Box(name,dimensions[0]/2,dimensions[1]/2,dimensions[2]/2);
    
    G4LogicalVolume *logicFoil = new G4LogicalVolume(solidFoil,material,name);
    
    new G4PVPlacement(transform,logicFoil,name,mother,false,0,true);
}
