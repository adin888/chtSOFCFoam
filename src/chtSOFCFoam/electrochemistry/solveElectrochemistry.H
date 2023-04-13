{
    Info<< "\nSolving for electrochemistry " << endl;
    
    label patchId = (fluidRegionName == "fuel" ? anodePatchId : cathodePatchId);
    
    /*const dimensionedScalar& R = constant::physicoChemical::R.value();
    const dimensionedScalar& Pstd = constant::standard::Pstd.value();
    const dimensionedScalar& F = eletrochemistryConstant::F.value();
    const dimensionedScalar& FF = eletrochemistryConstant::FF.value();
    const dimensionedScalar& Tr = 298.15;  //reference Temperature [K] for thermodynamics*/

	const scalar& R = constant::physicoChemical::R.value(); //[J/(mol*K)]
    const scalar& Pstd = constant::standard::Pstd.value();  //[Pa]
    const scalar& F = eletrochemistryConstant::F.value();   //[C/mol]
    const scalar& FF = eletrochemistryConstant::FF.value(); //[C/kmol]
      
    scalarField moleFraction(anodePatchSize, 0);
    scalarField NernstAnode(anodePatchSize, 0);
    
    PtrList<scalarField> xSpecie (chtSOFCSpecies.size());  //mole fractions of species
    
    /* calculate anode mole fractions except for inert specie*/
    #include "calculatexSpecie.H"
    
    /* calculate Nernst potential in Anode*/
    #include "NernstEqn.H"
        
    // Activation overpotential - added SBB 04-09-2013
    // scalarField etaAnode is computed from an inverse Butler-Volmer eqn. 	
    if (runTime.value() > PreIterAct)
    {
    	#include "activationOverpotential.H"
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
		Info << "min,mean,max(mFluxOld): "
		<< Foam::gMin(mFluxOld) <<" , "
		<< Foam::gAverage(mFluxOld) <<" , "
		<< Foam::gMax(mFluxOld) << endl;   
        /*
		- Total mass flow is that of O2
		- for O2, nElectrons = 4, molWeight = 31.9988
		*/
		jDensityOld = cathodeToAnode.faceInterpolate
		(
			-4. * FF * mFluxOld / 31.9988                  
		);

		if (runTime.value() <= 0.1) 
		{
			jDensityOld = jbar0.value();                      
		} 
		Info << "min,mean,max(jDensityOld): "
		<< Foam::gMin(jDensityOld) <<" , "
		<< Foam::gAverage(jDensityOld) <<" , "
		<< Foam::gMax(jDensityOld) << endl;    
    }
    


	/* Calculate the electrochemistry when the loop reaches the last region */    
	if(i == (fluidRegions.size() - 1))
	{
		/* calculate area specific resistance resist based on anodeT */
		#include "ASRfunction.H"
		
		/* Takes voltage from cell center of cell adjacent to boundary face and applies it at the face */
		forAll(*electrolyteAnodePatch, facei)
		{
			voltage.boundaryFieldRef()[electrolyteAnodePatchId][facei] =
			voltage[electrolyteAnodePatch->faceCells()[facei]];
		}
		
		/* 
		- Solve Current
		- Add etaA and etaC into the current function after the pre-iteration
		*/ 

		if (runTime.value() > PreIterAct)
		{
			jDensityNew = 
			(
				( NernstAnode
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
				( NernstAnode 
				- voltage.boundaryField()[electrolyteAnodePatchId])
				/ resist
			);
		}

		/* Relax Density */
		jDensityAnode = 
		(
			relaxj 
			* jDensityNew 
			+ (1-relaxj)
			* jDensityOld
		);
		
		/* ensure jDensity > 0 */ 
		jDensityAnode = Foam::max(jDensityAnode, SMALL);
		Info << "min,mean,max(jDensityAnode): "
			<< Foam::gMin(jDensityAnode) <<" , "
			<< Foam::gAverage(jDensityAnode) <<" , "
			<< Foam::gMax(jDensityAnode) << endl;        
		Info << "min,mean,max(jDensityNew): "
			<< Foam::gMin(jDensityNew) <<" , "
			<< Foam::gAverage(jDensityNew) <<" , "
			<< Foam::gMax(jDensityNew) << endl;    
		Info << "min,mean,max(jDensityOld): "
			<< Foam::gMin(jDensityOld) <<" , "
			<< Foam::gAverage(jDensityOld) <<" , "
			<< Foam::gMax(jDensityOld) << endl;   	     
		// Smear boundary current and Nernst potential over electrolyte volume
		#include "smearJDensity.H"
	
		// Calculate mean current density
		#include "solveCurrent.H"

		// mean current density
		Info << "min,mean,max(current jDensity): "
			<< gMin(jDensity.primitiveField()) << " , "
			<< gAverage(jDensity.primitiveField()) << " , "
			<< gMax(jDensity.primitiveField()) << endl; 

		//Calculate mFlux and set boudary conditions for mass fractions and velocity
		#include "mFlux.H"
	}   

    // Calculate electrochemical heating
    #include "electrochemicalHeating.H" 
    
}
