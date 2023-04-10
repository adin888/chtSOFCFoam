# Solid Oxide Fuel Cell Solver for OpenFOAM

This repository contains a solid oxide fuel cell (SOFC) solver for OpenFOAM, which is based on the work of Steven B. Beale in his paper **_"Open-source computational model of a solid oxide fuel cell"_** (reference provided below). The core architecture of the solver, openFuelCell, is preserved, focusing on the electrochemical reaction solution, and is combined with the multi-region solver chtMultiRegionFoam. The OpenFOAM version used is v2212.

## Purpose

The original openFuelCell solver is based on an older version of OpenFOAM. Updating the solver to work with a newer version of OpenFOAM makes it easier to expand the code by leveraging newer features (e.g., the JanafThermo component for obtaining specific heat capacity based on the current temperature). Utilizing a segregated solution strategy for each region can also potentially improve computational efficiency.

## Differences from openFuelCell

The main differences between this solver and openFuelCell are as follows:

1. The segregated solution strategy of chtMultiRegionFoam is employed, treating the solution of the energy equation for each region separately and coupling them at the boundaries, instead of considering the energy equation as one region.
2. The compressible turbulence control equation based on pressure is used.
3. The PIMPLE loop is employed for solving the pressure-velocity coupling.
4. The order and logic of some calculations have been modified.
5. Some library code has been modified to be compatible with the newer version of OpenFOAM.

## Future Updates

1. Review and optimize the variable definition locations and code structure due to changes in computational logic.
2. Compare the solver with examples provided by openFuelCell to verify the correctness of the code.
3. Create a user manual.

## Reference

S.B. Beale, et al., Open-source computational model of a solid oxide fuel cell, Computer Physics Communications (2015), http://dx.doi.org/10.1016/j.cpc.1015.10.007

## License

This program is licensed under the GNU General Public License (GPL) 2.0.
