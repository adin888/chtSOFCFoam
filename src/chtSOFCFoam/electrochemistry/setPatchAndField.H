    label cathodePatchId, cathodePatchSize, anodePatchId, anodePatchSize;
    label electrolyteAnodePatchId, electrolyteCathodePatchId;
    const polyPatch *cathodePatch = nullptr, *anodePatch = nullptr, *electrolyteAnodePatch = nullptr;
    const fvMesh *electrolyteMesh = nullptr, *airMesh = nullptr, *fuelMesh = nullptr;
	const fvPatchScalarField *cathodeT = nullptr, *cathodeP = nullptr, *cathodeRho = nullptr,
	                         *anodeT = nullptr, *anodeP = nullptr, *anodeRho = nullptr;
	fvPatchVectorField *cathodeU = nullptr, *anodeU = nullptr;
	//autoPtr<volScalarField> cathodeMuEff, anodeMuEff;
	scalarField cathodeMuEff, anodeMuEff;
    PtrList<volScalarField> *YAir, *YFuel;
	PtrList<chtSOFCSpecie> chtSOFCSpeciesAir, chtSOFCSpeciesFuel;
	fileName chtSOFCSpeciePropertiesFile;
	dictionary chtSOFCSpeciePropertiesDict;

    forAll(fluidRegions, i)
    {
    	fvMesh& mesh = fluidRegions[i];

		CombustionModel<rhoReactionThermo>& reaction = reactionFluid[i];
		rhoReactionThermo& thermo = reaction.thermo();
		basicMultiComponentMixture& composition = thermo.composition();
		compressible::turbulenceModel& turbulence = turbulenceFluid[i];

    	if(fluidRegions[i].name() == "air")
    	{
			airMesh = &mesh;
    	    cathodePatchId = mesh.boundaryMesh().findPatchID("air_to_electrolyte");
    	    cathodePatchSize = mesh.boundaryMesh()[cathodePatchId].size();
    	    //Info << "\nhave found cathodePatchId =" << cathodePatchId << endl;
    	    cathodePatch = &mesh.boundaryMesh()[cathodePatchId];
			cathodeT = &thermo.T().boundaryField()[cathodePatchId];
			cathodeP = &thermo.p().boundaryField()[cathodePatchId];
			cathodeRho = &rhoFluid[i].boundaryField()[cathodePatchId];
			cathodeU = &UFluid[i].boundaryFieldRef()[cathodePatchId];
			//cathodeMuEff.reset(new volScalarField(turbulence.muEff()));
			cathodeMuEff = turbulence.muEff(cathodePatchId);
			YAir = &composition.Y();

			chtSOFCSpeciePropertiesFile = "constant/air/thermophysicalProperties";
			chtSOFCSpeciePropertiesDict = chtSOFCSpecie::chtSOFCSpecieDict(chtSOFCSpeciePropertiesFile);
			chtSOFCSpeciesAir = chtSOFCSpecie::readAndStoreSOFCSpecies(chtSOFCSpeciePropertiesDict);
    	}
    	if(fluidRegions[i].name() == "fuel")
    	{
			fuelMesh = &mesh;
    	    anodePatchId = mesh.boundaryMesh().findPatchID("fuel_to_electrolyte");
    	    anodePatchSize = mesh.boundaryMesh()[anodePatchId].size();
    	    //Info << "\nhave found anodePatchId =" << anodePatchId << endl;
    	    anodePatch = &mesh.boundaryMesh()[anodePatchId];
			anodeT = &thermo.T().boundaryField()[anodePatchId];
			anodeP = &thermo.p().boundaryField()[anodePatchId];
			anodeP = &thermo.p().boundaryField()[anodePatchId];
			anodeRho = &rhoFluid[i].boundaryField()[anodePatchId];
			anodeU = &UFluid[i].boundaryFieldRef()[anodePatchId];
			//anodeMuEff.reset(new volScalarField(turbulence.muEff()));
			anodeMuEff = turbulence.muEff(anodePatchId);
			YFuel = &composition.Y();

			chtSOFCSpeciePropertiesFile = "constant/fuel/thermophysicalProperties";
			chtSOFCSpeciePropertiesDict = chtSOFCSpecie::chtSOFCSpecieDict(chtSOFCSpeciePropertiesFile);
			chtSOFCSpeciesFuel = chtSOFCSpecie::readAndStoreSOFCSpecies(chtSOFCSpeciePropertiesDict);
    	}
    }
    
    forAll(solidRegions, i)
    {
    	fvMesh& mesh = solidRegions[i];
    	if(solidRegions[i].name() == "electrolyte")
    	{
	   		electrolyteMesh = &mesh;
    	    electrolyteAnodePatchId = mesh.boundaryMesh().findPatchID("electrolyte_to_fuel");
    	    electrolyteCathodePatchId = mesh.boundaryMesh().findPatchID("electrolyte_to_air");
    	    electrolyteAnodePatch = &mesh.boundaryMesh()[electrolyteAnodePatchId];
    	}
    }
    
    PatchToPatchInterpolation<polyPatch, polyPatch> anodeToCathode
    (
    	*anodePatch,
    	*cathodePatch
    );
    
    PatchToPatchInterpolation<polyPatch, polyPatch> cathodeToAnode
    (
    	*cathodePatch,
    	*anodePatch
    );


