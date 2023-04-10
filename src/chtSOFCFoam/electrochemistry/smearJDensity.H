// Current density

    scalarField& jDensityIn = jDensity.primitiveFieldRef();
    smearPatchToMesh smearJDensity(*electrolyteMesh, electrolyteAnodePatchId);
    jDensityIn = smearJDensity.smear(jDensityAnode);
    jDensity.correctBoundaryConditions();

// Nernst Potential

    scalarField& NernstIn = Nernst.primitiveFieldRef();
    smearPatchToMesh smearNernst(*electrolyteMesh, electrolyteAnodePatchId);
    NernstIn = smearNernst.smear(NernstAnode);
    Nernst.correctBoundaryConditions();


