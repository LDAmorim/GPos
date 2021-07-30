#!/usr/bin/env bash
#
# @copyright GPos 2021 LBNL
#
set -eu -o pipefail

usage="usage:
    ./$(basename "$0") [-h] [-s path] [-o path] [-g path] -- script to run GPos

requires:
    to be executed in folder containing input.txt
    gcc, mpich, openpmd-api, Geant4 (in geant4-install folder) and GPos installation

where:
    -h shows this help text
    -s <NONE> if all modules are already loaded
       or
       <path to spack repo> if modules were installed with it
    -o <path to geant4 repo>
    -g <path to GPos repo>
    
note:
    Script removes all GPos generated files and folders"

if [ "$#" -eq 0 ]; then
    printf "missing arguments\n" >&2
    echo "$usage" >&2
    exit 1
fi

while getopts ':hs:o:g:' option; do
  case "$option" in
    h) echo "$usage"
       exit
       ;;
    s) spath=$OPTARG
       ;;
    o) opath=$OPTARG
       ;;
    g) gpath=$OPTARG
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

source $opath/geant4-install/bin/geant4.sh

if [ "$spath" != "NONE" ];
then
    source $spath/share/spack/setup-env.sh
    spack env activate gpos
    echo "Using spack installation"
else
    LD_LIBRARY_PATH=$opath/../openPMD-api/install/lib:$LD_LIBRARY_PATH
    echo "Using system's installation"
fi

rm -f *.h5
rm -f output.txt
rm -f error.txt
rm -f data*
rm -f thread_*
rm -f mpi_*

mpirun -n 2 $gpath/install/bin/GPos 2 > output.txt 2> error.txt
echo "***************************************"
awk '/./{line=$0} END{print line}' output.txt
echo "***************************************"

if [ "$spath" != "NONE" ];
then
    spack env deactivate
fi