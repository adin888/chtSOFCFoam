    label cathodePatchId;
    label cathodePatchSize;
    label anodePatchId;
    label anodePatchSize;   
    const polyPatch* cathodePatch = nullptr;
    const polyPatch* anodePatch = nullptr;

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

