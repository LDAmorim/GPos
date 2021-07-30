.. _build:

Building
========

The GPos source code can be downloaded or cloned from the GitHub repo `https://github.com/GPos <https://github.com/GPos>`_ following the standard `instructions <https://github.com/git-guides/git-clone>`_.

The following packages are required to install and run GPos:

.. hlist::
   :columns: 1
   
   * CMake
   * GCC 
   * MPICH (preferred to openmpi)
   * openPMD-api
   * Geant4 + G4MPI (instructions to build both below)

You may follow the instructions for building the necessary packages, GPos code and documentation on the platforms listed below:

.. toctree::
   :maxdepth: 1

   spack
   linux
   cori