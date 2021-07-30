#!/usr/bin/env bash
#
# GPos Copyright 2021
#
# Author: Ligia Diana Amorim

set -eu -o pipefail

sudo apt-get update

sudo apt-get install -y --no-install-recommends\
    build-essential \
    g++ \
    gfortran \
	mpich \
    libhdf5-mpich-dev
