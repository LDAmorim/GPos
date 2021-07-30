#!/usr/bin/env bash
#
# @copyright GPos 2021 LBNL
#
set -eu -o pipefail

usage="usage:
    ./$(basename "$0") [-h] [-s path] -- script to run GPos

requires:
    to be executed inside GPos/docs folder
    sphinx and sphinx-rtd-theme installation

where:
    -h shows this help text
    -s <NONE> if already loaded
       or
       <path to spack repo> if sphinx was installed with it"

if [ "$#" -eq 0 ]; then
    printf "missing arguments\n" >&2
    echo "$usage" >&2
    exit 1
fi

while getopts ':hs:' option; do
  case "$option" in
    h) echo "$usage"
       exit
       ;;
    s) spath=$OPTARG
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

if [ "$spath" != "NONE" ];
then
    source $spath/share/spack/setup-env.sh
    spack load -r doxygen graphviz
    echo "Using spack installation"
else
    echo "Using system's installation"
fi

doxygen Doxyfile
#open doxyhtml/html/index.html

