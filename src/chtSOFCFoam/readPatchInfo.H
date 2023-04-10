    label cathodePatchId;
    label cathodePatchSize;
    label anodePatchId;
    label anodePatchSize; 
    label electrolyteAnodePatchId;
    label electrolyteCathodePatchId;
    const polyPatch* cathodePatch = nullptr;
    const polyPatch* anodePatch = nullptr;
    const polyPatch* electrolyteAnodePatch = nullptr;
    const fvMesh* electrolyteMesh = nullptr;

    forAll(fluidRegions, i)
    {
    	fvMesh& mesh = fluidRegions[i];
    	if(fluidRegions[i].name() == "air")
    	{
    	    cathodePatchId = mesh.boundaryMesh().findPatchID("air_to_electrolyte");
    	    cathodePatchSize = mesh.boundaryMesh()[cathodePatchId].size();
    	    //Info << "\nhave found cathodePatchId =" << cathodePatchId << endl;
    	    cathodePatch = &mesh.boundaryMesh()[cathodePatchId];
    	}
    	if(fluidRegions[i].name() == "fuel")
    	{
    	    anodePatchId = mesh.boundaryMesh().findPatchID("fuel_to_electrolyte");
    	    anodePatchSize = mesh.boundaryMesh()[anodePatchId].size();
    	    //Info << "\nhave found anodePatchId =" << anodePatchId << endl;
    	    anodePatch = &mesh.boundaryMesh()[anodePatchId];
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

