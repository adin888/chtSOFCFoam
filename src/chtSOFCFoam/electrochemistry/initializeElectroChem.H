
    scalar hFormSum = 0;    //accumulator for enthalpy of formation
                            //normalized by n electrons assoc'd with ionization
    // initialize Qrxn, deltaHrxn, deltaSrxn                        
    scalarField Qrxn(anodePatchSize, 1);
    scalarField deltaHrxn(anodePatchSize, 0);
    scalarField deltaSrxn(anodePatchSize, 0);
    scalarField hSource(anodePatchSize, 0);
