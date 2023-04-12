    /* accumulator for enthalpy of formation */
    scalar hFormSum = 0; 

    /* initialize Qrxn, deltaHrxn, deltaSrxn for Nernst equation*/                       
    scalarField Qrxn(anodePatchSize, 1);
    scalarField deltaHrxn(anodePatchSize, 0);
    scalarField deltaSrxn(anodePatchSize, 0);
    /* initialize hSource, mFluxOld, jDensityOld, jDensityNew, jDensityAnode for solve electrochemistry */ 
    scalarField hSource(anodePatchSize, 0);
    scalarField mFluxOld(anodePatchSize);
    scalarField jDensityOld(anodePatchSize);   
    scalarField jDensityNew(anodePatchSize); 
    scalarField jDensityAnode(anodePatchSize);    
