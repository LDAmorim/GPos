.. _eg:

Input
=======

GPos uses input text files (like *input.txt*, which can be found in the `example` folder) with the user-defined initialization parameters listed below:

``debug`` (`G4bool`) optional (default `false`)
  Determines wether to output particle information at all stages of the simulation.

``np`` (`G4int`) optional (default 200)
  Sets the number of primary beam particles to be shot into the foil and corresponds to the total number of Geant4 events.

``totq`` (`G4double`) optional (default 1.e-9)
  If set, it defines the total primary beam absolute charge that the primary particles, as ensembles of multiple real particles, represent, in C.

``en`` (`G4double`) optional (default 10.0e3)
  Represents the primary beam mean energy, in MeV.
  
``de`` (`G4double`) optional (default 1,0)
  Represents the primary beam RMS energy spread, in MeV.
  
``frms`` (`G4double`) optional (default 4.0)
  Only particles falling within this number times the standard deviation in each spatial / velocity direction will be included in the computation of the final beams properties.
  
``z_focal`` (`G4double`) optional (default -4)
  Represents the primary beam focal plane longitudinal (z) position, in mm - where the beam has the initial properties set by this input file or default values.

``s_bprimary_x`` (`G4double`) optional (default 1.e-2)
  Primary beam Gaussian RMS size (direction x, orthogonal to the propagation of the primary beam of electrons), in mm.
  
``s_bprimary_y`` (`G4double`) optional (default 1.e-2)
  Primary beam Gaussian RMS size (direction y, orthogonal to the propagation of the primary beam of electrons), in mm.
  
``s_bprimary_z`` (`G4double`) optional (default 1.e-2)
  Primary beam Gaussian RMS size (longitudinal direction z), in mm.

``s_angle_x`` (`G4double`) optional (default 0)
  RMS divergence (i.e. :math:`angle_x[i]  = atan (px[i] / pz[i])`) in direction x, in radians.

``s_angle_x`` (`G4double`) optional (default 0)
  RMS divergence in direction y, in radians.
  
``type`` (`G4String`) optional (default `e-`)
  Determines the physical species of the primary beam particles - charge and mass information are defined accordingly.

``profile`` (`G4String`) optional (default `gaussian`)
  Specifies the longitudinal profile for the primary beam - currently implemented available options are: `gaussian` and `flat-top`.
  
``symmetrize`` (`G4bool`) optional (default `false`)
  Determines wether to symmetrize the primary beam initial position (true) or not (false).
  If active, each beam primary electron of ne is multiplied by 4 to be sent with the 4 symmetrical initial positions.

``x_beam`` (`G4ThreeVector`) optional (default (0.0*mm,0.0*mm,-4.0*mm))
  Defines the three-dimensional centroid position of the primary beam, in mm.

``w_world`` (`G4ThreeVector`) optional (default (10.*mm,10.*mm,5.*mm))
  Defnes the three-dimensional width of the `G4_Galactic` world space, in mm - the world center defines the origin of the reference frame for all results.

``hasfoil`` (`G4bool`) optional (default `true`)
  When false deactivates the foil and the primary particles propagate in vaccuum (`G4_Galactic` world).

``w_foil`` (`G4ThreeVector`) optional (default (10.*mm,10.*mm,0.5*mm))
  Defnes the high-z foil three-dimensional width, in mm.

``x_foil`` (`G4ThreeVector`) optional (default (0.0*mm,0.0*mm,2.25*mm))
  Defnes the offset of the foil center in respect to the center of the world space, in mm - note that the foil needs to be contained within the world.

``t_foil`` (`G4String`) optional (default G4_W)
  Defines the material of the foil according to the `Geant4 Material Database <https://geant4-userdoc.web.cern.ch/UsersGuides/ForApplicationDeveloper/html/Appendix/materialNames.html>`_.
  Typical values are G4_W (Tungsten) and G4_Pt (Platinum).

``drift`` (`G4double`) optional (default 2.0)
  If specified (i.e. above 0), it determines the drift distance from the end of the world for the particles to be ballistically propagated for, in mm.
  The fastest particle will propagate until it reaches that distance, all other particles will propagate until the time in which the fastest particle finishes that drift.

``iflens`` (`G4bool`) optional (default `false`)
  Determines if a linear thin lens is added within the drift trajectory - defined by `lens` and `lens_pz`.
  
``lens`` (`G4double`) optional (default drift/2)
  This parameter is only available when ``iflens`` and ``drift`` are true and non zero and determines the position after the foil where the lens is placed, in mm.

``lens_pz`` (`G4double`) optional (default 65)
  This parameter is only available when ``iflens`` and ``drift`` are true and non zero and determines the longitudinal momentum for which the lens is tuned to, in MeV.

``ifback`` (`G4bool`) optional (default `true`)
  Determines if at the end of the Geant4 run all particles are transported backwards into the minimum run time instant (or forwards into the maximum run time).

``s_list`` (`G4String`) optional (default `e-_primary_initial e-_primary e- e+`)
  List of species of interest, for which the properties will be stored at the end of the simulation.

``encutoff`` (`G4double`) optional (default 0.0 0.0 0.0)
  Determines the list of the minimum longitudinal energy (to be compared with Geant4 `track->GetTotalEnergy()`) above which particle data is analysed and stored in MeV.
  List of energy cutoffs should be ordered according to the species listed in ``s_list``.

``pzcutoff`` (`G4double`) optional (default 0.0 0.0 0.0)
  Determines the list of the minimum longitudinal momentum (to be compared with Geant4 `track->GetMomentum().z()`) above which particle data is analysed and stored in MeV.
  List of pz cutoffs should be ordered according to the species listed in ``s_list``.


Where the Geant4 specific variables types are:
    
* G4int - equivalent to `int` in CPP
* G4double - equivalent to `double` in CPP
* G4String - equivalent to `string` in CPP
* G4ThreeVector - similar to `double` array in CPP of dimension 3, but with CHLEP units
* mm - milimeters
