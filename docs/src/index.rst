GPos
====


Geant4 based positron beam source (GPos) is an easy-to-use publicly available `c++` code, with support for hybrid MPI and openPMD as well as parallel I/O, tool created to model relativistic particle beam and solid targets interactions.

It is developed at LBNL to address the problem of building a compact, high-energy (GeV) and efficient positron source (via Bremsstrahlung and photon-decay in the target) using the LBNL `BELLA <https://bella.lbl.gov/>`_ center laser-plasma technology.

The code relies on the Object-Oriented `Geant4 <https://geant4.web.cern.ch/node/1>`_ HEP Monte Carlo based toolkit libraries, produces particle data in `openPMD <https://openpmd-api.readthedocs.io/en/0.13.3/#>`_ format and has been adapted to run in the supercompter at `NERSC <https://www.nersc.gov/>`_.

Users can easily change the beam properties (such as mean energy, energy spread, divergence, size), the vacuum space and foil configuration (varying foil type and dimensions) as well as the drift space, where the resulting particles can be ballistically propagated and focused/defocused by a thin lens (users can alter the drift distance and tune the lens energy), in the input file.

GPos can be compiled with `CMAKE` and scripts using the open-source `spack` package manager (located in the `src`, `docs` and `example` folders) .


Code
----

The open-source code can be found in the *GitHub* repo `GPos <https://github.com/LDAmorim/GPos>`_.

For questions, bug reports or to request new features and changes, please feel free to open a new `issues <https://github.com/LDAmorim/GPos/issues>`_.

Changes can be tested locally using the scripts *GPos_test.py* and *GPos_plot.py* (located in the `example` folder).

There is a small stand-alone electron-beam-tungsten-foil example input file, run, test and analysis script in the repo (also located in the `example` folder).


Documentation
-------------

Please see the sections below for more information:

.. toctree::
   :caption: Contents:
   :maxdepth: 1

   building/index
   example/index
   visualization/visualization
   intro/intro
   structure/structure
   validation/index
   doxygen/doxygen

Additional API documentation can be found in `Doxygen <file:///D:/Diana/opt/GPos/docs/doxyhtml/html/index.html>`_.


Acknowledgements
----------------

GPos was initially built and released by Ligia Diana Amorim (LDianaAmorim@lbl.gov) for the scientific project funded by the Laboratory-Directed Research and Development (LDRD), supported by the Office of Science, US DOE, Contract No. DE-AC02-05CH11231.

Associated research was done in collaboration with S. S. Bulanov, C. Benedetti and J.-L. Vay at the `ATAP <https://atap.lbl.gov/>`_ division of the Lawrence Berkeley National Laboratory, 94720 CA, US.


Citation
--------

If you are referencing GPos in a publication, please cite the following paper:

 * L. D. Amorim, S. S. Bulanov, C. Benedetti and J.-L. Vay,
   **GPos: a simple tool to model positron sources** (to be submitted 2021).


License
-------

Geant4 based positron beam source (GPos) Copyright (c) 2021, The
Regents of the University of California, through Lawrence Berkeley
National Laboratory (subject to receipt of any required approvals
from the U.S. Dept. of Energy). All rights reserved.

If you have questions about your rights to use or distribute this software,
please contact Berkeley Lab's Intellectual Property Office at
IPO@lbl.gov.

NOTICE.  This Software was developed under funding from the U.S. Department
of Energy and the U.S. Government consequently retains certain rights.  As
such, the U.S. Government has been granted for itself and others acting on
its behalf a paid-up, nonexclusive, irrevocable, worldwide license in the
Software to reproduce, distribute copies to the public, prepare derivative 
works, and perform publicly and display publicly, and to permit others to do so.
