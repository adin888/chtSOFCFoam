//#include "physicoChemicalConstants.H"

{
    Info<< "\nSolving for electrochemistry " << endl;

    const scalar R = constant::physicoChemical::R.value();
    const scalar Pstd = constant::standard::Pstd.value();
    const scalar F = eletrochemistryConstant::F.value();
    const scalar FF = eletrochemistryConstant::FF.value();
    
    const scalar Tr = 298.15;  //reference Temperature [K] for thermodynamics
    scalarField jDensity(anodePatchSize, 1);    
    scalarField jDensityOld(anodePatchSize);    
    scalarField mFluxOld(anodePatchSize);    
    scalarField jDensityNew(anodePatchSize);
    
    //Calculate boudary conditions for mass fractions and velocity
    {
        label nSpec = chtSOFCSpecies.size();
        label patchId = (fluidRegionName == "fuel" ? anodePatchId : cathodePatchId);
        List<scalarField> mFlux(nSpec);
    	List<Switch> isFlux(nSpec,false);
    	scalarField mFluxSum(anodePatchSize, 0);
    	
    	forAll(chtSOFCSpecies, i)
	{
  	    
  	    if (chtSOFCSpecies[i].nE() != 0)
  	    {
  	    	mFlux[i] = 
  	    	(
  	    	    chtSOFCSpecies[i].rSign() * chtSOFCSpecies[i].W()
  	    	    * jDensity / (FF * chtSOFCSpecies[i].nE())
  	    	);
  	    	mFluxSum += mFlux[i];
  	    	isFlux[i] = "true";
  	    }
  	    Info << "i species[] nE rSign hForm sForm isFlux[] mFlux[] ="
  	    << i << " " << chtSOFCSpecies[i].name() << " " 
  	    << chtSOFCSpecies[i].nE() << " " << chtSOFCSpecies[i].rSign() << " " 
  	    << chtSOFCSpecies[i].hForm() << " " << chtSOFCSpecies[i].sForm() << " " 
  	    << isFlux[i] << " " << gSum(mFlux[i]) << nl;
	}
	Info << endl;
	
	forAll(Y, i)
	{
	    if (i != inertIndex)
	    {
	    	//const scalarField& gammaS = turbulence.muEff(patchId)();
	    	
	    	//Info << "calculate gammas, size = " << gammaS.size() << endl;
	    	
	    	volScalarField& Yi = Y[i];
	    	fixedGradientFvPatchScalarField& YiBC =
	    	    refCast<fixedGradientFvPatchScalarField>
	    	    (
	    	    	Yi.boundaryFieldRef()[patchId]
    	    	    );
    	    	    
    	        Info << "calculate YiBC, size = " << YiBC.size() << endl;
    	        
    	    	YiBC.gradient() = 
    	    	(
    	    	    (fluidRegionName == "fuel" ? mFlux[i] : anodeToCathode.faceInterpolate(mFlux[i])())    //faceInterpolate（）返回tmp，需要operator()来调用
    	    	    * (1.0 - YiBC)
    	    	);

    	    	Info << "calculate YiBC.gradient(), size = " << YiBC.gradient().size() << endl;
    	    	
    	    	forAll(Y, j)
    	    	{
    	    	    if((j != i) && isFlux[j])
    	    	    {
    	    	    	YiBC.gradient() = 
	    	    	(
	    	    	    (fluidRegionName == "fuel" ? mFlux[j] : anodeToCathode.faceInterpolate(mFlux[j])())
	    	    	    * YiBC
	    	    	);
	    	    	Info << "fixed YiBC.gradient()" << endl;
    	    	    }
    	    	}
    	    	YiBC.gradient() /= turbulence.muEff(patchId) + SMALL;
    	    	Info << "calculated Y boundary condition" << endl;
	    }
	}
    	
    	U.boundaryFieldRef()[patchId] ==
    	(
    	    (fluidRegionName == "fuel" ? mFluxSum : anodeToCathode.faceInterpolate(mFluxSum)())
    	    / rho.boundaryFieldRef()[patchId]
    	    * mesh.Sf().boundaryField()[patchId]
    	    / mesh.magSf().boundaryField()[patchId]
    	);
    	Info << "calculated U boundary condition" << endl;
	
    }
}
