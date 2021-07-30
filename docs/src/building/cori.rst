.. _cori_nersc:

CORI @ NERSC
============

Although GPos runs are quick and can be performed in local machines, for high number of particles or thick foils (i.e. high number of interactions), it is recommended to run it using MPI on computer clusters - such as `CORI <https://docs.nersc.gov/systems/cori/>`_.

.. note::

   As GPos main struggle is not with clock time, but with memory overload, it is recommended to use the Haswell nodes (KNL installation is done in similar manner).

The necessary dependencies can be loaded from the modules that are already available and can be loaded as follows:

.. code-block:: bash

   $ module load craype-haswell # larger memory
   $ module swap PrgEnv-intel PrgEnv-gnu
   $ module load mpich
   $ module load cmake/3.18.2
   $ module load cray-hdf5-parallel
   $ module load adios2/2.5.0
   $ export GPos_packages=GPos_packages

openPMD-api
-----------

`openPMD-api <https://openpmd-api.readthedocs.io>`_ is an open-source API for the openPMD data standard.
If `openpmd-api` installation  is required, it can be done from source with `CMake <https://cmake.org/>`_ via the commands below:

.. code-block:: bash

   $ export opmd_dir=$GPos_packages/openPMD-api
   $ mkdir $opmd_dir
   $ git clone https://github.com/openPMD/openPMD-api.git $opmd_dir --depth 1 --branch 0.13.4
   $ mkdir $opmd_dir/build
   $ mkdir $opmd_dir/install
   $ cd $opmd_dir/build
   $ cmake .. -DopenPMD_USE_PYTHON=OFF -DCMAKE_INSTALL_PREFIX=$opmd_dir/install  -DopenPMD_USE_HDF5=ON  -DopenPMD_USE_PYTHON=OFF  -DBUILD_TESTING=OFF  -DBUILD_EXAMPLES=OFF -DBUILD_CLI_TOOLS=OFF -DCMAKE_INSTALL_RPATH_USE_LINK_PATH=ON -DCMAKE_INSTALL_RPATH='$ORIGIN'
   $ cmake --build . --target install --parallel 8

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
   $ cmake -DGeant4_DIR=$G4INSTALL/lib64/Geant4-10.7.2  -DCMAKE_INSTALL_PREFIX=$G4INSTALL -DBUILD_SHARED_LIBS=ON ..
   $ cmake --build . --target install --parallel 8

.. warning::

   Please confirm and correct if current CORI system installs in `$G4INSTALL/lib/` instead of `$G4INSTALL/lib64/`.


GPos
----

The commands below will install the GPos executable in the path `GPos_packages/GPos/install/bin`.

.. code-block:: bash

   $ export GPos_dir=$GPos_packages/GPos
   $ git clone https://github.com/GPos.git $GPos_dir --depth 1 --branch 1.0
   $ mkdir $GPos_dir/build
   $ cd $GPos_dir/build
   $ export G4DIR=$GPos_packages/geant4/install/lib64/Geant4-10.7.2
   $ export G4MPIDIR=$GPos_packages/geant4/install/lib64/G4mpi-10.7.2
   $ source $GPos_packages/geant4/install/bin/geant4.sh
   $ export LD_LIBRARY_PATH=$GPos_packages/openPMD-api/install/lib64:$LD_LIBRARY_PATH
   $ export PKG_CONFIG_PATH=$GPos_packages/openPMD-api/install/lib64/pkgconfig:$PKG_CONFIG_PATH
   $ export CMAKE_PREFIX_PATH=$GPos_packages/openPMD-api/install:$CMAKE_PREFIX_PATH
   $ export G4PINSTALL=$GPos_dir/install
   $ cmake -DGeant4_DIR=$G4DIR -DG4mpi_Dir=$G4MPIDIR -DCMAKE_INSTALL_PREFIX=$G4PINSTALL -DGEANT4_BUILD_MULTITHREADED=ON ..
   $ cmake --build . --target install --parallel 4


Example submitjob
-----------------

Bellow is the skelleton for a sample `submitjob.sh` that launches GPos on *debug* mode to 1 Haswell node.

.. code-block:: bash

      #!/bin/bash -l

      #SBATCH -N 1
      #SBATCH -t 00:04:00
      #SBATCH -q debug
      #SBATCH -C haswell
      #SBATCH -J <run_label>
      #SBATCH -A <project_ID>
      #SBATCH -e error.txt
      #SBATCH -o output.txt

      module load craype-haswell
      module swap PrgEnv-intel PrgEnv-gnu
      module load mpich
      module load cmake/3.18.2
      module load cray-hdf5-parallel
      module load adios2/2.5.0

      GPos_packages=<directory_selected/GPos_packages>
      export LD_LIBRARY_PATH=$GPos_packages/openPMD-api/install/lib64:$LD_LIBRARY_PATH
      export PKG_CONFIG_PATH=$GPos_packages/openPMD-api/openPMD-install/lib64/pkgconfig:$PKG_CONFIG_PATH
      export CMAKE_PREFIX_PATH=$GPos_packages/openPMD-api/openPMD-install:$CMAKE_PREFIX_PATH
      source $GPos_packages/geant4/geant4-install/bin/geant4.sh

      export OMP_NUM_THREADS=1
      export OMP_PLACES=threads
      export OMP_PROC_BIND=spread

      date
      srun --cpu_bind=cores -n 2 $GPos_packages/GPos/install/bin/GPos 1 > output.txt 2> error.txt
      date

      exit

This submitjob should be in the folder that containes the input file - described in :ref:`egind`.

.. note::

   For thicker targets or high number of particles (:math:`~ 10^6`) the simulation might run into lack of memory and crash with error messages such as :code:`segmentation fault`.
   In such cases it is recommended to increase the number of Haswell nodes (:code:`#SBATCH -N 1`) and MPI ranks / CPUs (:code:`srun --cpu_bind=cores -n 2`) accordingly.
   Although it is usually not necessary, the number of threads per MPI rank can be increased to make the simulation quicker (:code:`OMP_NUM_THREADS=1` and :code:`(...)/bin/GPos 1`).

For reference, a 2 MP simulation of 1 mm thick Tungsten target in CORI took less than 12 minutes to run on only 1 node using 20 ranks and 1 thread per rank.