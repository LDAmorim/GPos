.. _linux:

Linux/Windows (Ubuntu)
======================

If you are on a Windows machine it is recommended to follow the instructions to install the `Ubuntu terminal <https://ubuntu.com/tutorials/ubuntu-on-windows#1-overview>`_.
From that terminal or from an Ubuntu machine terminal, you can install GPos by following the steps described below.

GCC, GFortran, CMake, MPICH and HDF5
------------------------------------

Run the following commands to install the required dependencies (HDF5 is necessary to create the output files) and create the main packages folder:

.. code-block:: bash

   $ sudo apt-get update
   $ sudo apt-get install -y --no-install-recommends build-essential g++ gfortran cmake mpich libhdf5-mpich-dev
   $ export GPos_packages=GPos_packages
   $ mkdir $GPos_packages


openPMD
-------

`openPMD-api <https://openpmd-api.readthedocs.io>`_ is an open-source API for the openPMD data standard and can be installed from source with `CMake <https://cmake.org/>`_:

.. code-block:: bash

   $ export opmd_dir=$GPos_packages/openPMD-api
   $ mkdir $opmd_dir
   $ git clone https://github.com/openPMD/openPMD-api.git $opmd_dir --depth 1 --branch 0.13.4
   $ mkdir $opmd_dir/build
   $ cd $opmd_dir/build
   $ cmake .. -DCMAKE_INSTALL_PREFIX=$opmd_dir/install -DopenPMD_USE_HDF5=ON -DopenPMD_USE_PYTHON=OFF -DBUILD_TESTING=OFF -DBUILD_EXAMPLES=OFF -DBUILD_CLI_TOOLS=OFF
   $ cmake --build . --target install --parallel 4

.. note::

   You may wish to change the nr of cores used to parallelize the builds with CMake (in :code:`--parallel 4`).

Geant4 and G4MPI
----------------

`Geant4 <https://geant4.web.cern.ch/node/1>`_ and `G4MPI <https://apc.u-paris.fr/~franco/g4doxy4.10/html/md___volumes__work_geant_geant4_810_800_8p01_examples_extended_parallel__m_p_i__r_e_a_d_m_e.html>`_ libraries are required to process the particles interactions with foil atoms in GPos and can be installed via the commands:

.. code-block:: bash

   $ export g4_dir=$GPos_packages/geant4
   $ mkdir $g4_dir
   $ export G4INSTALL=$g4_dir/install
   $ git clone https://github.com/Geant4/geant4.git $g4_dir --depth 1 --branch geant4-10.7-release
   $ mkdir $g4_dir/build
   $ cd $g4_dir/build
   $ cmake -DCMAKE_INSTALL_PREFIX=$G4INSTALL -DGEANT4_BUILD_EXAMPLES=ON -DGEANT4_USE_OPENGL_X11=OFF -DGEANT4_INSTALL_DATA=ON -DGEANT4_BUILD_MULTITHREADED=ON -DBUILD_SHARED_LIBS=ON -DGEANT4_USE_SYSTEM_EXPAT=OFF ..
   $ cmake --build . --target install --parallel 16
   $ cd $G4INSTALL/bin
   $ source geant4.sh
   $ cd $g4_dir/examples/extended/parallel/MPI/source/
   $ mkdir build
   $ cd build
   $ cmake -DGeant4_DIR=$G4INSTALL/lib/Geant4-10.7.2  -DCMAKE_INSTALL_PREFIX=$G4INSTALL -DBUILD_SHARED_LIBS=ON ..
   $ cmake --build . --target install --parallel 8


GPos
----

The commands below will install the GPos executable in the path `GPos_packages/GPos/install/bin`.

.. code-block:: bash

   $ export GPos_dir=$GPos_packages/GPos
   $ git clone https://github.com/GPos.git $GPos_dir --depth 1 --branch 1.0
   $ mkdir $GPos_dir/build
   $ cd $GPos_dir/build
   $ export G4DIR=$GPos_packages/geant4/install/lib/Geant4-10.7.2
   $ export G4MPIDIR=$GPos_packages/geant4/install/lib/G4mpi-10.7.2
   $ source $GPos_packages/geant4/install/bin/geant4.sh
   $ export LD_LIBRARY_PATH=$GPos_packages/openPMD-api/install/lib:$LD_LIBRARY_PATH
   $ export PKG_CONFIG_PATH=$GPos_packages/openPMD-api/install/lib/pkgconfig:$PKG_CONFIG_PATH
   $ export CMAKE_PREFIX_PATH=$GPos_packages/openPMD-api/install:$CMAKE_PREFIX_PATH
   $ export G4PINSTALL=$GPos_dir/install
   $ cmake -DGeant4_DIR=$G4DIR -DG4mpi_Dir=$G4MPIDIR -DCMAKE_INSTALL_PREFIX=$G4PINSTALL -DGEANT4_BUILD_MULTITHREADED=ON ..
   $ cmake --build . --target install --parallel 4

To run the code on multiple MPI ranks (in this case 2) and threads (in this case 1) in a folder containing a valid *input.txt* file (see :ref:`egind` for more detailed information) you can use:

.. code-block:: bash

   $ source $GPos_packages/geant4/install/bin/geant4.sh
   $ export LD_LIBRARY_PATH=$GPos_packages/openPMD-api/install/lib:$LD_LIBRARY_PATH
   $ mpirun -n 2 $GPos_packages/GPos/install/bin/GPos 1

.. note::

   This procedure is currently tested at each `Pull Request` and `Push` in the *linux* workflow of the GitHub repo.


Scripts
-------

Alternatively, after the Spack environment, Geant4 and G4MPI codes are built, two Bash scripts are included in the GPos repo, *gpos-build.sh* and *gpos-run.sh* (the latter inside the `egind` folder), to allow quicker build and execution of GPos.
They can be ran using the commands:

.. code-block:: bash

   $ ./gpos-build.sh -s NONE -o $g4_dir -r n
   $ ./gpos-run.sh -s NONE -o $g4_dir -g $GPos_dir

.. note::

   You can use :code:`./gpos-***.sh --help` for more detailed information on how to use the scripts.


Documentation
-------------

GPos user and developer documentation source code is stored in the *GitHub* repo under the *docs* folder.

Both can be altered and compiled localy using `sphinx` and `doxygen` with the bash commands:

.. code-block:: bash

   $ sudo apt-get install -y sphinx sphinx-rtd-theme-common python-sphinxcontrib-programoutput graphviz breathe
   $ make clean
   $ make html
   $ sudo apt-get install graphviz doxygen
   $ doxygen Doxyfile

After installing both packages, you can also use the available scripts via :code:`./gpos-docs-build.sh -s NONE` and :code:`./gpos-doxy-build.sh -s NONE` to build the documentation.

Then point your preferred browser to `build/html/index.html` or `doxyhtml/html/index.html` to open the respective initial pages.