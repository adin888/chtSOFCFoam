//  Activation term prescribed by Butler-Volmer Equation:
    if(fluidRegionName == "fuel")
    {
        const scalarField& pPatch = p.boundaryFieldRef()[anodePatchId];
        
		tmp<scalarField> etaAnode(
		activationOverpotential->overPotential
			(
				anodeT,
				pPatch,
				xSpecie,
				jDensity
			)
		);
		Info<< "min,mean,max calBV(etaAn) = " << gMin(etaAnode()) << "  "
		<< gAverage(etaAnode()) << "  " << gMax(etaAnode()) << nl;
		scalarField& etaAIn = etaA.primitiveFieldRef();
		smearPatchToMesh smearetaA(*electrolyteMesh, electrolyteAnodePatchId);
		etaAIn = smearetaA.smear(etaAnode());
		etaA.correctBoundaryConditions();
		etaA.boundaryFieldRef()[electrolyteAnodePatchId] == etaAnode();
    }

// Compute local cathode exchange current density
// SBB: Must add the air-side pressure
// SBB: Also Cathode overpotential computed with anode temperature
    else
    {
        scalarField pPatch = cathodeToAnode.faceInterpolate(p.boundaryFieldRef()[cathodePatchId]);
        
		tmp<scalarField> etaCathode(
		activationOverpotential->overPotential
			(
				cathodeT,
				pPatch,
				xSpecie,
				jDensity
			)
		);
		Info<< "min,mean,max calBV(etaCa) = " << gMin(etaCathode()) << "  "
			<< gAverage(etaCathode()) << "  " << gMax(etaCathode()) << nl;
		scalarField& etaCIn = etaC.primitiveFieldRef();
		smearPatchToMesh smearetaC(*electrolyteMesh, electrolyteAnodePatchId);
		etaCIn = smearetaC.smear(etaCathode());
		etaC.correctBoundaryConditions();
		etaC.boundaryFieldRef()[electrolyteAnodePatchId] == etaCathode();
    }


// End activation calculation







