    /* Calculate boudary conditions for mass fractions and velocity */
    /* Air side*/
    {
        label nSpec = chtSOFCSpeciesAir.size();
        
        List<scalarField> mFlux(nSpec);
    	List<Switch> isFlux(nSpec,false);
    	
    	scalarField mFluxSum(anodePatchSize, 0);
    	
    	forAll(chtSOFCSpeciesAir, s)
		{
			/* 
			- calculate the mass flow of non-inert species 
			- computed by Faraday’s law 
			*/
			if (chtSOFCSpeciesAir[s].nE() != 0)
			{
				mFlux[s] = 
				(
					chtSOFCSpeciesAir[s].rSign() * chtSOFCSpeciesAir[s].W()
					* jDensityAnode / (FF * chtSOFCSpeciesAir[s].nE())
				);
				mFluxSum += mFlux[s];
				isFlux[s] = "true";

                /*
                - calculate mass fraction boundary conditions YiBC for non-inert species
                - use fixed Gradient boundary condtion
                */
                fixedGradientFvPatchScalarField& YiBC =
                refCast<fixedGradientFvPatchScalarField>
                (
                    (*YAir)[s].boundaryFieldRef()[cathodePatchId]
                );
                
                /* mass fluxes on cathode patch(air region) are interpolated from anode patch */
                YiBC.gradient() = 
                (
                    anodeToCathode.faceInterpolate(mFlux[s])
                    * (1.0 - YiBC)
                );
                
                forAll(Y, j)
                {
                    if((j != s) && isFlux[j])
                    {
                        YiBC.gradient() = 
                        (
                            anodeToCathode.faceInterpolate(mFlux[j])
                            * YiBC
                        );
                    }
                }   
                YiBC.gradient() /= cathodeMuEff + SMALL;
			}
			Info << "s species[] nE rSign hForm sForm isFlux[] mFlux[] ="
			<< s << " " << chtSOFCSpeciesAir[s].name() << " " 
			<< chtSOFCSpeciesAir[s].nE() << " " << chtSOFCSpeciesAir[s].rSign() << " " 
			<< chtSOFCSpeciesAir[s].hForm() << " " << chtSOFCSpeciesAir[s].sForm() << " " 
			<< isFlux[s] << " " << gSum(mFlux[s]) << endl;
		}
		
    	/* calculate velocity boundary condition */
    	*cathodeU ==
    	(
    	    anodeToCathode.faceInterpolate(mFluxSum)
    	    / (*cathodeRho)
    	    * airMesh->Sf().boundaryField()[cathodePatchId]
    	    / airMesh->magSf().boundaryField()[cathodePatchId]
    	);
	
    }

    /* Fuel side*/
    {
        label nSpec = chtSOFCSpeciesFuel.size();
        
        List<scalarField> mFlux(nSpec);
    	List<Switch> isFlux(nSpec,false);
    	
    	scalarField mFluxSum(anodePatchSize, 0);
    	
    	forAll(chtSOFCSpeciesFuel, s)
		{
			if (chtSOFCSpeciesFuel[s].nE() != 0)
			{
				mFlux[s] = 
				(
					chtSOFCSpeciesFuel[s].rSign() * chtSOFCSpeciesFuel[s].W()
					* jDensityAnode / (FF * chtSOFCSpeciesFuel[s].nE())
				);
				mFluxSum += mFlux[s];
				isFlux[s] = "true";

                fixedGradientFvPatchScalarField& YiBC =
                refCast<fixedGradientFvPatchScalarField>
                (
                    (*YFuel)[s].boundaryFieldRef()[anodePatchId]
                );
            
                YiBC.gradient() = 
                (
                    mFlux[s]
                    * (1.0 - YiBC)
                );
                
                forAll(Y, j)
                {
                    if((j != s) && isFlux[j])
                    {
                        YiBC.gradient() = 
                        (
                            mFlux[j]
                            * YiBC
                        );
                    }
                }	
				YiBC.gradient() /= anodeMuEff + SMALL;
			}


			Info << "s species[] nE rSign hForm sForm isFlux[] mFlux[] ="
			<< s << " " << chtSOFCSpeciesFuel[s].name() << " " 
			<< chtSOFCSpeciesFuel[s].nE() << " " << chtSOFCSpeciesFuel[s].rSign() << " " 
			<< chtSOFCSpeciesFuel[s].hForm() << " " << chtSOFCSpeciesFuel[s].sForm() << " " 
			<< isFlux[s] << " " << gSum(mFlux[s]) << endl;
		}

    	/* calculate velocity boundary condition */
    	*anodeU ==
    	(
    	    mFluxSum
    	    / (*anodeRho)
    	    * fuelMesh->Sf().boundaryField()[anodePatchId]
    	    / fuelMesh->magSf().boundaryField()[anodePatchId]
    	);
	
    }
    