{	
    std::unique_ptr<fvScalarMatrix> hEqnPtr;
    if(solidRegions[i].name() == "electrolyte")
    {
        hEqnPtr = std::make_unique<fvScalarMatrix>
        (
	    fvm::ddt(betav*rho, h)
	    - (
	        thermo.isotropic()
	        ? fvm::laplacian(betav*thermo.alpha(), h, "laplacian(alpha,h)")
	        : fvm::laplacian(betav*taniAlpha(), h, "laplacian(alpha,h)")
	    )
	    ==
	    fvOptions(rho, h)
	    + electrochemicalHeating
        );
    }
    else
    {
        hEqnPtr = std::make_unique<fvScalarMatrix>
        (
            fvm::ddt(betav*rho, h)
            - (
                thermo.isotropic()
                ? fvm::laplacian(betav*thermo.alpha(), h, "laplacian(alpha,h)")
                : fvm::laplacian(betav*taniAlpha(), h, "laplacian(alpha,h)")
            )
            ==
            fvOptions(rho, h)
        );
    }
    
    fvScalarMatrix& hEqn = *hEqnPtr;
    
    hEqn.relax();

    fvOptions.constrain(hEqn);

    if (coupled)
    {
        fvMatrixAssemblyPtr->addFvMatrix(hEqn);
    }
    else
    {
        Info<< "\nSolving for solid region "<< solidRegions[i].name() << endl;

        if (finalIter)
        {
            mesh.data::add("finalIteration", true);
        }

        hEqn.solve(mesh.solver(h.select(finalIter)));

        fvOptions.correct(h);

        thermo.correct();

        Info<< "Min/max T:" << min(thermo.T()).value() << ' '
            << max(thermo.T()).value() << endl;

        if (finalIter)
        {
            mesh.data::remove("finalIteration");
        }
    }
}
