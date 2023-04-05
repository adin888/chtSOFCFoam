//    Activation term prescribed by Butler-Volmer Equation:

    tmp<scalarField> etaAnode(fuelActivationOverpotential->overPotential
    (
        anodeT,
        pFuelPatch,
        xFuel,
        i
    ));
   Info<< "min,mean,max calBV(etaAn) = " << gMin(etaAnode()) << "  "
       << gAverage(etaAnode()) << "  " << gMax(etaAnode()) << nl;


// Compute local cathode exchange current density
// SBB: Must add the air-side pressure
// SBB: Also Cathode overpotential computed with anode temperature

    tmp<scalarField> etaCathode(airActivationOverpotential->overPotential
    (
        cathodeT,
        pAirPatch,
        xAir,
        i
    ));
   Info<< "min,mean,max calBV(etaCa) = " << gMin(etaCathode()) << "  "
       << gAverage(etaCathode()) << "  " << gMax(etaCathode()) << nl;

// End activation calculation

//  Save data for output

    scalarField& etaAIn = etaA.primitiveFieldRef();
    smearPatchToMesh smearetaA(electrolyteMesh, electrolyteAnodeName);
    etaAIn = smearetaA.smear(etaAnode());
    etaA.correctBoundaryConditions();
    etaA.boundaryFieldRef()[electrolyteAnodeID] == etaAnode();

    scalarField& etaCIn = etaC.primitiveFieldRef();
    smearPatchToMesh smearetaC(electrolyteMesh, electrolyteAnodeName);
    etaCIn = smearetaC.smear(etaCathode());
    etaC.correctBoundaryConditions();
    etaC.boundaryFieldRef()[electrolyteCathodeID] == etaCathode();



