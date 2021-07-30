.. _vis:

Visualization
=============


GPos output data can be analysed using the Python (version 3 or higher) example scripts present in the `example` repo folder:

``gpos_funcs.py``
  Script that includes the functions to read, analyse and plot GPos data files of selected species and positions.

``gpos_test.py`` (python gpos_test.py <path to folder containing openPMD files> <array of species>)
  Script that uses the necessary functions of `gpos_funcs.py` to compare selected GPos output particle data and species properties.

``gpos_plot.py`` (python gpos_test.py <path to folder containing openPMD files> <array of species>)
  Script that generates plots of the overlap of the 2D distributions of selected species at selected positions  and stores them under a new "Plots" folder.


The packages required to run those scripts are:

* numpy
* openPMD-api (installed with python)
* matplotlib
* scipy


To install them with `pip <https://pip.pypa.io/en/stable/>`_ (for Python 3.9) you can use the command:

.. code-block:: bash

  $ pip install -r <package_name>
  $ python ...


To install using the `spack <https://spack.readthedocs.io/en/latest/>`_ package manager the use of an environment is highly recommended:

.. code-block:: bash

  $ git clone https://github.com/spack/spack.git
  $ source spack/share/spack/setup-env.sh
  $ spack env create vis
  $ spack env activate vis
  $ spack add python@3.8.10 py-scipy@1.6.3  py-matplotlib@3.4.2  py-numpy@1.20.3
  $ spack add openpmd-api@0.13.4 +python
  $ spack concretize
  $ spack install
  $ spack activate py-scipy@1.6.3  py-matplotlib@3.4.2  py-numpy@1.20.3
  $ spack load openpmd-api
  $ python ...
  $ spack env deactivate 

.. note::

  Syntax of all Python scripts is tested at each `Pull Request` and `Push` in the *linux* workflow of the GitHub repo and *gpos_test.py* is executed to confirm accuracy of results.
