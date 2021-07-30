Scientific context
==================

.. centered::
    "About 14 billion years ago, matter, energy, time and space came into being in what is known as the Big Bang. The story of these fundamental features of our universe is called physics"
    Opening of *Sapiens: A Brief History of Humankind*, by Yuval Harari [:ref:`Harari <Harari>`].

In the quest for understanding the fundamental laws of nature and answering questions such as - what is dark matter made of? why is there more matter than anti-matter -  physicsts continue to develope large-scale international experimental tools called particle colliders [:ref:`Gray <Gray>`].
The Large Hadron Collider, at The European Organization for Nuclear Research (CERN), is a prime example of such tools.
It is a 27 km long circular tunnel placed 100 meters underground where two beams counter-propagate until they collide at ultra-high energy, 13 Tera-electron-Volt (TeV :math:`= 10^12` eV), creating explosions holding measurements of rare particles - like the Higgs Boson [:ref:`ATLAS <ATLAS>`].
The key challenge to build future, more compact and cost-effective, colliders reaching beyond the current energy frontier lies in particle acceleration.
Standard particle accelerators, employing for example Radio-Frequency cavities, hold electro-magnetic fields with gradients corresponding to hundreds of Mega-eV (MeV :math:`= 10^6` eV) per meter-scale distance propagated by charged particles.
Plasma-based technology can sustain gradients with tens of Giga-eV (GeV :math:`= 10^9` eV) per meter holding the potential to enable the construction of future table-top-sized accelerators [:ref:`Joshi <Joshi>`].

Recent breakthroughs in using plasma accelerators to deliver multi-GeV electron beams with high quality and brightness make it a good candidate for future lepton (elettron-positron) colliders [:ref:`Gonsalves <Gonsalves>`].
Nevertheless, it is still challenging to use them to accelerate positrons to the same energies efficiently [:ref:`Sarri <Sarri>`].
The reason for that lies in the assymetry of the focusing and accelerating region of plasma accelerators operating in the high-gradient non-linear regime - which is too narrow to make it technically simple to inject positrons into it.
Multiple new methods, where the plasmas were excited by Laguerre Gaussian laser pulses, dognut shaped electron beams or positron beams, are being proposed and explored numerically to overcome that challenge.
Although there are positron sources available and codes that have been explored to model them [:ref:`Ushakov <Ushakov>`], our goal is to build a higher-energy (GeV) and more compact positron source to test and develop those methods experimentally at the BELLA center of the Lawrence Berkeley National Laboratory [:ref:`BELLA <BELLA>`].
We used GPos as well as the Particle-In-Cell Warpx code [:ref:`WarpX <WarpX>`] to find the optimal configuration for that source using the Petta-Watt (PW) laser and plasma capillary technology available at BELLA, numerically.

The C++ GPos code is a parallelized tool to apply the physics modeled by the Geant4 toolkit [:ref:`Geant4 <Allison>`] to the case in which a relativistic electron beam is sent into a single, thin, solid and high atomic-number foil.
As each of the modeled electrons propagate near the deflecting fields of the intense foil atoms, they are deccelerated and produce what is called *Bremsstrahlung* radiation.
That radiation, also near the fields of the foil atoms, can be converted into electron-positron pairs from where positrons can be extracted to form the positron beam of interest.
In our study, we understood that the key challenges for producing high quality positron beams directly from the foil is their broad energy spectrum and high divergence that increase with the number of interactions responsible for creating more positrons.
We found that in the optimal case, the foil is thin enough (near 1 mm) to provide sufficient positrons whilst maintaining the beam and positrons quality (transverse size and divergence).
Varying the foil type (i.e. material) has a similar effect, in numerical terms, as changing the thickness by increasing or reducing the number of electron-atom and radiation-atom interactions.
We also used GPos to explore how making the beams drift in vacuum (ballistically, i.e. in regimes where space-charge effects are negligible) with and witout a focusing lens (modeled by the thin lens approximation) could aid to improving the final positron beam quality and longitudinal separation of the three species.

The data of all three species at the exit of the foil, the primary electron beam and secondary electron-positron cloud, is stored in the `openPMD` standard `HDF5` format [:ref:`openPMD <openPMD-standard>`] that is compatible with `WarpX` input - allowing direct simulations of the beams evolution and acceleration in plasmas.
In our research we determined that the best option for building a source at BELLA is to send the positrons generated in the foil into a laser-driven plasma acceleration stage, as opposed to being sent directly into a plasma to be excited by the primary electron beam [:ref:`Amorim <Amorim>`].

Although GPos was designed and tested with properties similar to those delivered from PW laser driven plasma capillary at BELLA, it's simple input file (where beam, foil and space properties can be changed) can be easily adapted to model various configurations useful to other laboratories and generation of other particles (for example muons).

References
----------

.. _Harari:

Y. N. Harari, *Sapiens: A Brief History of Humankind*, Harvill Secker, 2014, `ISBN 1846558239, 9781846558238 <https://books.google.pt/books/about/Sapiens.html?id=B4ARBAAAQBAJ&redir_esc=y>`_.

.. _Gray:

H. M. Gray, *Future colliders for the high-energy frontier*, Reviews in Phys., Vol. 6, 100053, 2021, `ISSN 2405-4283 <https://doi.org/10.1016/j.revip.2021.100053>`_.

.. _ATLAS:

K. Anthony, *ATLAS kicks off a new year at 13 TeV*, 2017. `atlas.cern/updates/press-statement/new-year-13-tev <https://atlas.cern/updates/press-statement/new-year-13-tev>`_

.. _Joshi:

C. Joshi, *Plasma-based accelerators: then and now*, Plasma Phys. Control. Fusion, Vol. 61, 10, 104001, 2019. `DOI:10.1088/1361-6587/ab396a <https://doi.org/10.1088/1361-6587/ab396a>`_

.. _Gonsalves:

A. J. Gonsalves et al., *Petawatt Laser Guiding and Electron Beam Acceleration to 8 GeV in a Laser-Heated Capillary Discharge Waveguide*, Phys. Rev. Lett. 122, 084801, 2019. `DOI:10.1103/PhysRevLett.122.084801 <https://doi.org/10.1103/PhysRevLett.122.084801>`_.

.. _BELLA:

The Berkeley Lab Laser Accelerator (BELLA) Center, `bella.lbl.gov <https://bella.lbl.gov>`_.

.. _Ushakov:

A. Ushakov et al., *Positron Source simulation using Geant4*, Proceedings of IPAC’10, Kyoto, Japan, `POSITRON SOURCE SIMULATIONS USING GEANT4 <https://accelconf.web.cern.ch/IPAC10/papers/thpec023.pdf>`_

.. _WarpX:

J.-L. Vay et al., *Modeling of a chain of three plasma accelerator stages with the WarpX electromagnetic PIC code on GPUs*, Physics of Plasmas 28, 023105, 2021. `DOI:10.1063/5.0028512 <https://doi.org/10.1063/5.0028512>`_. Repository: `github.com/ECP-WarpX/WarpX <https://github.com/ECP-WarpX/WarpX>`__

.. _Sarri:

G. Sarri, *Spectral and spatial characterisation of laser-driven positron beams*, Plasma Phys. Control. Fusion, 59, 2017. `DOI:10.1088/0741-3335/59/1/014015 <https://doi.org/10.1088/0741-3335/59/1/014015>`_.

.. _Allison:

J. Allison et al., *Recent developments in Geant4*, Nucl. Instrum. Methods: Phys. Res 835, 186-225, 2016. `DOI:10.1016/j.nima.2016.06.125 <https://doi.org/10.1016/j.nima.2016.06.125>`_

.. _openPMD-standard:

A. Huebl et al., *openPMD: A meta data standard for particle and mesh based data*, 2015. `DOI:10.5281/zenodo.591699 <https://doi.org/10.5281/zenodo.591699>`_

.. _Amorim:

L. D. Amorim et al., *Positron beam plasma-based source for BELLA* (to be submitted 2021)
