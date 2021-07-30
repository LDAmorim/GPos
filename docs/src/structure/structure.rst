.. _strct:

Source code
===========

This section contains a basic overview of the GPos source code to help you understand it and adapt it to your scientific case.


Labels and units
----------------

GPos code and documentation language follows the Geant4 concepts of:

#. **World** - volume of the virtual simulation (G4_Galactic to represent vacuum)
#. **Primary** - particle that is sent to interact with the target
#. **Foil** - solid target used to induce primary-atom/nucleous interactions
#. **Run** - contains all the events modelled
#. **Event** - starts when a primary particle with given (mass, charge, momentum, position) properties is shot and terminates when resulting particles decay or leave the world
#. **Track** - snapshot of particle properties (data is accessed in MyUserSteppingAction)
#. **Step** - start/end information for each interaction between primary and/or secondary particles and/or foil atoms
#. **Cut-off** - hard-coded cut-off of foil_thickness/10 corresponding to minimum stopping length of particles after which they don't generate radiation (energy loss becomes continuous)
#. **QGSP_BERT** - Physics list that contains information to model (at rest/continuous/discrete) physical processes (including Bremsstrahlung).

By convention all classes/functions that begin with "G4" are inherited from the Geant4 library.
The classes and functions created for GPos can be found in the GPos repo *src* and *include* folders and are detailed in the `Doxygen <file:///D:/Diana/opt/GPos/docs/doxyhtml/html/index.html>`_ documentation.

.. note::

   The system of units of Geant4 is described in the 
   `table <http://geant4.web.cern.ch/sites/geant4.web.cern.ch/files/geant4/collaboration/working_groups/electromagnetic/gallery/units/SystemOfUnits.html>`_,
   the `link <http://geant4.web.cern.ch/ooaandd/design/class_spec/part_int/particles>`_ 
   and the `header file <https://cmd.inp.nsk.su/old/cmd2/manuals/cernlib/CLHEP/RefGuide/Units/SystemOfUnits_h.html>`_.

For more information on the Geant4 related libraries and classes please visit the `documentation <https://geant4.web.cern.ch/support/user_documentation>`_ and `tutorials <https://geant4.web.cern.ch/collaboration/workshops/users2002/tutorial>`_.


Structure
---------

The GPos srouce code architecture is detailed (including standard class/collaboration diagrams) in the `Doxygen <file:///D:/Diana/opt/GPos/docs/doxyhtml/html/index.html>`_ documentation.

For a simpler overview of the code structure, the diagram bellow describes the key logic steps (as well as associated functions and classes) employed:

.. graphviz::

   digraph Classes {
      splines=ortho
      node [shape=box]
      a [label="MPI parallelization starts (GPos)"]
      b [label="User-defined input is read and stored (Query)"]
      c [label="Primaries distribution is&#92;nset and spread accross&#92;nMPI ranks (Beam)"]
      e [label="Single/multi-threading is specified (G4MTRunManager)"]
      f [label="World and foil geometries are set&#92;n(CWorld, CFoil and MyDetectorConstruction)"]
      g [label="QGSP_BERT with Bremsstrahlung is selected&#92;n(G4VModularPhysicsList)"]
      h [label="Run data containers and methods are registered (MyActionInitialization)"]
      i [label="1 Kernel per thead and MPI rank starts (Initialize)"]
      j [label="Loop over all events of the run (BeamOn)"]
      a -> b -> c -> h
      b -> e -> f -> g -> h
      h -> i -> j
      subgraph clusterstep1 {
         node [shape=box,style=filled,fillcolor=white];
         k [label="Event primary is created (MyPrimaryGeneratorAction)"]
         l [label="Primary data and properties are&#92;nstored (save_outOPMD/save_txt)"]
         k -> l
         m [label="Memory for each event is allocated (MyRunAction)"]
         n [label="Event is processed until all particles are gone (MyEventAction)"]
         o [label="Step is computed and selected particles final data is stored (MyUserSteppingAction)"]
         p [label="Threads data and run end time accross MPI ranks are merged (MyRunAction)"]
         q [label="Beam data is computed and stored (Properties, save_outOPMD/save_txt)"]
         r [label="Particles within frms drift ballistically with(out) lens (Part)"]
         l -> m [style=invis]
         k -> m -> n -> o
         o -> p [style=invis]
         o -> n -> p -> q -> r -> q
         style=filled;
         s [label="MPI ranks data is merged (MyRunAction)"]
         r -> s
      }
      j -> k
      t [label="Program ends (GPos)"]
      s -> t
   }


Selection
---------

GPos stores particles final track information if (according to user-defined values specified in input.txt):

* particle species is in ``s_list``
* particle did not decay / reached cut-off (explained in list below)
* particle did not leave the world space transversely
* particle energy is above species ``encutoff``
* particle longitudinal momentum is above species ``pzcutoff``
