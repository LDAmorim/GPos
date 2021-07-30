#!/usr/bin/env bash
#
# @copyright GPos 2021 LBNL
#
set -eu -o pipefail

usage="usage:
    ./$(basename "$0") [-h] [-s path] [-o path] [r Y/N] -- script to install GPos

requires:
    to be executed in GPos repo
    gcc, cmake, mpich, openpmd-api and Geant4 (in geant4-install folder) installation

where:
    -h shows this help text
    -s <NONE> if all modules are already loaded
       or
       <path to spack repo> if modules were installed with it
    -o <path to geant4 repo>
    -r to re-do cmake build or just code compilation
    
note:
    Script removes all GPos generated files and folders"

if [ "$#" -eq 0 ]; then
    printf "missing arguments\n" >&2
    echo "$usage" >&2
    exit 1
fi

while getopts ':hs:o:r:' option; do
  case "$option" in
    h) echo "$usage"
       exit
       ;;
    s) spath=$OPTARG
       ;;
    o) opath=$OPTARG
       ;;
    r) reset=$OPTARG
       ;;
    :) printf "missing argument for -%s\n" "$OPTARG" >&2
       echo "$usage" >&2
       exit 1
       ;;
   \?) printf "illegal option: -%s\n" "$OPTARG" >&2
       echo "$usage" >&2
       exit 1
       ;;
  esac
done
shift $((OPTIND - 1))


G4MPIDIR=$opath/geant4-install/lib/G4mpi-10.7.0
G4DIR=$opath/geant4-install/lib/Geant4-10.7.0
G4PINSTALL=$PWD/install

if [ "$spath" != "NONE" ];
then
    echo "Using spack installation"
    source $spath/share/spack/setup-env.sh
    spack env activate gpos
else
    LD_LIBRARY_PATH=$opath/../openPMD-api/install/lib:$LD_LIBRARY_PATH
    PKG_CONFIG_PATH=$opath/../openPMD-api/install/lib/pkgconfig:$PKG_CONFIG_PATH
    CMAKE_PREFIX_PATH=$opath/../openPMD-api/install:$CMAKE_PREFIX_PATH
    echo "Using system's installation"
fi

source $opath/geant4-install/bin/geant4.sh

mkdir -p $G4PINSTALL
mkdir -p build
cd build

if [ "$reset" == "Y" ]
then
    rm -rf *
    cmake -DGeant4_DIR=$G4DIR -DG4mpi_Dir=$G4MPIDIR -DCMAKE_INSTALL_PREFIX=$G4PINSTALL -DGEANT4_BUILD_MULTITHREADED=ON ..
fi

make
make install
cd ..

if [ "$spath" != "NONE" ];
then
    spack env deactivate
fi