.. _spack_pm:

Linux/MacOS (Spack)
===================

The `spack <https://spack.readthedocs.io/en/latest/>`_ package manager can be used to install GPos using environments as described below.


GCC and GFortran
----------------

On your terminal you can run the following commands to install the required dependencies and create the main packages folder:

.. code-block:: bash

   sudo apt-get update

   sudo apt-get install -y --no-install-recommends build-essential g++ gfortran

   export GPos_packages=GPos_packages
   mkdir $GPos_packages

.. warning::

   Currently tested version of gcc is Ubuntu 9.3.0.
   
   You may also use spack to install gcc (which includes gfortran) on MacOS.

CMake, MPICH and openPMD-api
----------------------------

`openPMD-api <https://openpmd-api.readthedocs.io>`_ is an open-source API for the openPMD data standard and can be installed along with `CMake <https://cmake.org/>`_:

.. code-block:: bash

   spack env create gpos
   spack env activate gpos
   spack add cmake@3.20.3  mpich@3.4.2  openpmd-api@0.13.4


Geant4 and G4MPI
----------------

`Geant4 <https://geant4.web.cern.ch/node/1>`_ and `G4MPI <https://apc.u-paris.fr/~franco/g4doxy4.10/html/md___volumes__work_geant_geant4_810_800_8p01_examples_extended_parallel__m_p_i__r_e_a_d_m_e.html>`_ libraries are required to process the particles interactions with foil atoms in GPos and can be installed using the environment created via the commands:

.. code-block:: bash

   export g4_dir=$GPos_packages/geant4
   mkdir $g4_dir
   export G4INSTALL=$g4_dir/install
   git clone https://github.com/Geant4/geant4.git $g4_dir --depth 1 --branch geant4-10.7-release
   mkdir $g4_dir/build
   cd $g4_dir/build
   cmake -DCMAKE_INSTALL_PREFIX=$G4INSTALL -DGEANT4_BUILD_EXAMPLES=ON -DGEANT4_USE_OPENGL_X11=OFF -DGEANT4_INSTALL_DATA=ON -DGEANT4_BUILD_MULTITHREADED=ON -DBUILD_SHARED_LIBS=ON -DGEANT4_USE_SYSTEM_EXPAT=OFF ..
   cmake --build . --target install --parallel 16
   cd $G4INSTALL/bin
   source geant4.sh
   cd $g4_dir/examples/extended/parallel/MPI/source/
   mkdir build
   cd build
   cmake -DGeant4_DIR=$G4INSTALL/lib/Geant4-10.7.2  -DCMAKE_INSTALL_PREFIX=$G4INSTALL -DBUILD_SHARED_LIBS=ON ..
   cmake --build . --target install --parallel 8


GPos
----

The commands below will install the GPos executable in the path `GPos_packages/GPos/install/bin`.

.. code-block:: bash

   export GPos_dir=$GPos_packages/GPos
   git clone https://github.com/GPos.git $GPos_dir --depth 1 --branch 1.0
   mkdir $GPos_dir/build
   cd $GPos_dir/build
   G4MPIDIR=$GPos_packages/geant4/geant4-install/lib/G4mpi-10.7.0
   G4DIR=$GPos_packages/geant4/geant4-install/lib/Geant4-10.7.0
   source $GPos_packages/geant4/geant4-install/bin/geant4.sh
   G4PINSTALL=$GPos_dir/install
   cmake -DGeant4_DIR=$G4DIR -DG4mpi_Dir=$G4MPIDIR -DCMAKE_INSTALL_PREFIX=$G4PINSTALL -DGEANT4_BUILD_MULTITHREADED=ON ..
   cmake --build . --target install --parallel 4

To run the code on multiple MPI ranks (in this case 2) and threads (in this case 1) in a folder containing a valid *input.txt* file (see :ref:`egind` for more detailed information) you can use:

.. code-block:: bash

   source $GPos_packages/geant4/install/bin/geant4.sh
   mpirun -n 2 $GPos_packages/GPos/install/bin/GPos 1

   spack env deactivate


Scripts
-------

Alternatively, after the Spack environment, Geant4 and G4MPI codes are built, two Bash scripts are included in the GPos repo, *gpos-build.sh* and *gpos-run.sh* (the latter inside the `egind` folder), to allow quicker build and execution of GPos.
They can be ran using the commands:

.. code-block:: bash

   ./gpos-build.sh -s $GPos_packages/spack -o $g4_dir -r n

   ./gpos-run.sh -s $GPos_packages/spack -o $g4_dir -g $GPos_dir

.. note::

   You can use :code:`./gpos-***.sh --help` for more detailed information on how to use the scripts.


Documentation
-------------

GPos user and developer documentation source code is stored in the *GitHub* repo under the *docs* folder.

Both can be altered and compiled localy using `sphinx` and `doxygen` with the bash commands:

.. code-block:: bash

   $ spack install py-sphinx py-sphinx-rtd-theme py-sphinxcontrib-programoutput py-breathe graphviz@2.46.0
   $ spack activate py-sphinx py-sphinx-rtd-theme py-sphinxcontrib-programoutput py-breathe
   $ spack load -r py-sphinx py-sphinx-rtd-theme py-sphinxcontrib-programoutput py-breathe graphviz
   $ make clean
   $ make html

   $ spack install graphviz@2.46.0
   $ spack install doxygen
   $ spack load -r graphviz doxygen
   $ doxygen Doxyfile

After installing both packages, you can also use the available scripts via :code:`./gpos-docs-build.sh -s $GPos_packages/spack` and :code:`./gpos-doxy-build.sh -s $GPos_packages/spack` to build the documentation.

Then point your preferred browser to `build/html/index.html` or `doxyhtml/html/index.html` to open the respective initial pages.

.. warning::
   
   Graphviz newer version 2.47.2 can lead to the error message :code:`"dot: ortho.c:155: convertSPtoRoute: Assertion `sz >= 2' failed.\n"`, when building the documentation.