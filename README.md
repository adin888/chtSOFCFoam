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

## Some results and interpretations

The test case is crossFlow, where the air delivery duct and the fuel delivery duct are positioned perpendicularly to each other in direction, each consisting of 25 ducts. Due to the partial differential equations in this solver being different from those in the original solver, the fvSchemes and fvSolution in "crossFlow" cannot simply replicate the ones from the original example. The current setup follows a transient solution approach. However, given the thinness of the electrolyte, in order to prevent the thermodynamic diffusion number from becoming too large and causing divergence in the solution, only extremely small time steps can be utilized, which significantly constrains the solution speed. Therefore, it is necessary to attempt to optimize the relevant settings.  

The total duration of the case is 1s. The following three figures depict the distribution of Nernst Potential on the electrolyte at 0.25s, 0.5s, and 1s, respectively.  
![](https://github.com/adin888/chtSOFCFoam/blob/main/run/crossFlow/result/Nernst_0.25.png)
![](https://github.com/adin888/chtSOFCFoam/blob/main/run/crossFlow/result/Nernst_0.5.png)
![](https://github.com/adin888/chtSOFCFoam/blob/main/run/crossFlow/result/Nernst_1.0.png)
The distribution is uneven and changes over time, with the overall value of the Nernst Potential exhibiting a decreasing trend as the simulation progresses.  

The following two figures represent the mass fraction of H2 in the fuel delivery duct at 0.25s and 1s respectively, with the left side indicating the inlet.
![](https://github.com/adin888/chtSOFCFoam/blob/main/run/crossFlow/result/H2_0.25.png)
![](https://github.com/adin888/chtSOFCFoam/blob/main/run/crossFlow/result/H2_1.0.png)
The distribution of H2 corresponds with that of the Nernst Potential, which validates the calculation process.  

The current results show some discrepancies and a different distribution pattern compared to the original example. There are potentially two reasons for this:
1. Firstly, the simulation run time of the current model is only 1s (compared to 120s in the original example), implying that the results may not have fully converged.
2. Secondly, the transient solution method might inherently result in a continuously fluctuating field, which would inherently exhibit discrepancies compared to a steady-state solution.

## Future Updates

1. Review and optimize the variable definition locations and code structure due to changes in computational logic.
2. Compare the solver with examples provided by openFuelCell to verify the correctness of the code.
3. Create a user manual.
4. Try to use other methodes to calculate electrochemical reactions.

## Reference

S.B. Beale, et al., Open-source computational model of a solid oxide fuel cell, Computer Physics Communications (2015), http://dx.doi.org/10.1016/j.cpc.1015.10.007

## License

This program is licensed under the GNU General Public License (GPL) 2.0.
