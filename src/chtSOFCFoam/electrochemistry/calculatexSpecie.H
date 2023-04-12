    forAll(Y, i)
    {
    	moleFraction += Y[i].boundaryFieldRef()[patchId] / chtSOFCSpecies[i].W();
    }
    
    forAll(chtSOFCSpecies, s)
    {
    	xSpecie.set
    	(
    	    s,
    	    new scalarField(anodePatchSize,0)
    	);
    	
    	if(chtSOFCSpecies[s].rSign() != 0)
    	{
    	    if(fluidRegionName == "fuel")
			{ 
				xSpecie[s] =	    
				(
					Y[s].boundaryFieldRef()[patchId]
					/ chtSOFCSpecies[i].W()
					/ (moleFraction + SMALL)
					);
			}
			else
			{ 
				xSpecie[s] =
				cathodeToAnode.faceInterpolate
				(
					Y[s].boundaryFieldRef()[patchId]
					/ chtSOFCSpecies[i].W()
					/ (moleFraction + SMALL)
				);
			}
    	    
    	    xSpecie[s] = Foam::max(xSpecie[s], SMALL);
    	}
    }
