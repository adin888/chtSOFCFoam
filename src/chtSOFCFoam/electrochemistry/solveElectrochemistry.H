{
    Info<< "\nSolving for electrochemistry " << endl;
    
    label patchId = (fluidRegionName == "fuel" ? anodePatchId : cathodePatchId);
    label airSpecieSize;
    label fuelSpecieSize; 
    
    const scalar R = constant::physicoChemical::R.value();
    const scalar Pstd = constant::standard::Pstd.value();
    const scalar F = eletrochemistryConstant::F.value();
    const scalar FF = eletrochemistryConstant::FF.value();
    const scalar Tr = 298.15;  //reference Temperature [K] for thermodynamics
    
    scalarField jDensity(anodePatchSize, 1);    
    scalarField jDensityOld(anodePatchSize);    
    scalarField mFluxOld(anodePatchSize);    
    scalarField jDensityNew(anodePatchSize);
    scalarField moleFraction(anodePatchSize, 0);
    scalarField Nernst(anodePatchSize, 0);
    
    const scalarField& anodeT=thermo.T().boundaryFieldRef()[anodePatchId];
    const scalarField& cathodeT=thermo.T().boundaryFieldRef()[cathodePatchId];
    
    PtrList<scalarField> xSpecie (chtSOFCSpecies.size());  //mole fractions of species
    
    /* calculate anode mole fractions except for inert specie*/
    #include "calculatexSpecie.H"
    
    /* calculate Nernst potential */
    #include "NernstEqn.H"
    
    /* calculate area specific resistance resist based on anodeT */
    #include "ASRfunction.H"
    
    /* Takes voltage from cell center of cell adjacent to boundary face and applies it at the face */
    forAll(*electrolyteAnodePatch, facei)
    {
    	voltage.boundaryFieldRef()[electrolyteAnodePatchId][facei] =
    	voltage[electrolyteAnodePatch->faceCells()[facei]];
    }
    /* calculate mass flux from "old" velocity boundary condition and "old" current density on cathode*/
    if(fluidRegionName == "air")
    {
    	mFluxOld =
    	(
    	    -rho.boundaryField()[cathodePatchId]
    	    * ((U.boundaryField()[cathodePatchId]
            & mesh.Sf().boundaryField()[cathodePatchId])
            / mesh.magSf().boundaryField()[cathodePatchId])
        );
        
	jDensityOld = cathodeToAnode.faceInterpolate
	(
	    -4. * FF * mFluxOld / 31.9988                  //Magic numbers - Total mass flow is that of O2
	);
    }
    
    if (runTime.value() <= 1) 
    {
    	jDensityOld = ibar0.value();                      //First time step
    }        
    
    
    /* 
    - Solve Current
    - Add etaA and etaC into the current function after the pre-iteration
    */ 
 

        if (runTime.value() > PreIterAct)
	{
	    jDensityNew = 
	    (
	         ( Nernst
	         - voltage.boundaryField()[electrolyteAnodePatchId] 
	         - etaA.boundaryField()[electrolyteAnodePatchId] 
	         - etaC.boundaryField()[electrolyteAnodePatchId])
	         / resist
	    );
	}
	else
	{
	     jDensityNew = 
	     (
	         ( Nernst 
	         - voltage.boundaryField()[electrolyteAnodePatchId])
	         / resist
	     );
	}
	
    jDensity = 
    (
    	relaxj 
    	* jDensityNew 
    	+ (1-relaxj)
    	* jDensityOld
    );
	
    // ensure jDensity > 0
    jDensity = Foam::max(jDensity, SMALL);
            Info << "min,mean,max(jDensity: "
                  << Foam::gMin(jDensity) <<" , "
                  << Foam::gAverage(jDensity) <<" , "
                 << Foam::gMax(jDensity) << endl;        
            Info << "min,mean,max(jDensityNew): "
                  << Foam::gMin(jDensityNew) <<" , "
                  << Foam::gAverage(jDensityNew) <<" , "
                 << Foam::gMax(jDensityNew) << endl;        
            Info << "min,mean,max(jDensityOld): "
                  << Foam::gMin(jDensityOld) <<" , "
                  << Foam::gAverage(jDensityOld) <<" , "
                 << Foam::gMax(jDensityOld) << endl;        
/*
    // Activation overpotential - added SBB 04-09-2013
    // scalarField etaAnode is computed from an inverse Butler-Volmer eqn.
  	
    if (runTime.value() > PreIterAct)
    {
    	#include "activationOverpotential.H"
    }

    // Smear boundary current and Nernst potential over electrolyte volume
    #include "idensity.H"
    
    // Calculate mean current density
    #include "solveCurrent.H"

    // mean current density
    Info << "min,mean,max(current): "
         << gMin(idensity.primitiveFieldRef()) << " , "
         << gAverage(idensity.primitiveFieldRef()) << " , "
         << gMax(idensity.primitiveFieldRef()) << endl;



    // Calculate electrochemical heating
    #include "electrochemicalHeating.H"
   */ 
    
    
    
    
    /* Calculate boudary conditions for mass fractions and velocity */
    {
        label nSpec = chtSOFCSpecies.size();
        
        List<scalarField> mFlux(nSpec);
    	List<Switch> isFlux(nSpec,false);
    	
    	scalarField mFluxSum(anodePatchSize, 0);
    	
    	forAll(chtSOFCSpecies, s)
	{
  	    /* 
  	    - calculate the mass flow of non-inert species 
  	    - computed by Faraday’s law 
  	    */
  	    if (chtSOFCSpecies[s].nE() != 0)
  	    {
  	    	mFlux[s] = 
  	    	(
  	    	    chtSOFCSpecies[s].rSign() * chtSOFCSpecies[s].W()
  	    	    * jDensity / (FF * chtSOFCSpecies[s].nE())
  	    	);
  	    	mFluxSum += mFlux[s];
  	    	isFlux[s] = "true";
  	    }
  	    Info << "s species[] nE rSign hForm sForm isFlux[] mFlux[] ="
  	    << s << " " << chtSOFCSpecies[s].name() << " " 
  	    << chtSOFCSpecies[s].nE() << " " << chtSOFCSpecies[s].rSign() << " " 
  	    << chtSOFCSpecies[s].hForm() << " " << chtSOFCSpecies[s].sForm() << " " 
  	    << isFlux[s] << " " << gSum(mFlux[s]) << nl;
	}
	Info << endl;
	
	/*
	- calculate mass fraction boundary conditions YiBC for non-inert species
	- use fixed Gradient boundary condtion
	*/
	forAll(Y, i)
	{
	    if (i != inertIndex)
	    {
	    
	    	fixedGradientFvPatchScalarField& YiBC =
	    	    refCast<fixedGradientFvPatchScalarField>
	    	    (
	    	    	Y[i].boundaryFieldRef()[patchId]
    	    	    );
    	        
    	        /* mass fluxes on cathode patch(air region) are interpolated from anode patch */
    	    	YiBC.gradient() = 
    	    	(
    	    	    (fluidRegionName == "fuel" ? mFlux[i] : anodeToCathode.faceInterpolate(mFlux[i])())  
    	    	    * (1.0 - YiBC)
    	    	);
    	    	
    	    	forAll(Y, j)
    	    	{
    	    	    if((j != i) && isFlux[j])
    	    	    {
    	    	    	YiBC.gradient() = 
	    	    	(
	    	    	    (fluidRegionName == "fuel" ? mFlux[j] : anodeToCathode.faceInterpolate(mFlux[j])())
	    	    	    * YiBC
	    	    	);
    	    	    }
    	    	}
    	    	
    	    	YiBC.gradient() /= turbulence.muEff(patchId) + SMALL;
	    }
	}
    	/* calculate velocity boundary condition */
    	U.boundaryFieldRef()[patchId] ==
    	(
    	    (fluidRegionName == "fuel" ? mFluxSum : anodeToCathode.faceInterpolate(mFluxSum)())
    	    / rho.boundaryFieldRef()[patchId]
    	    * mesh.Sf().boundaryField()[patchId]
    	    / mesh.magSf().boundaryField()[patchId]
    	);
	
    }
}
