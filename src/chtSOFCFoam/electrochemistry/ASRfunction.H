    // Area Specific Resistance function
    //
    // Choose an existing one or add your own


    // an empiracal lumped resistance
    //    range of validity = 550 to 1200 degrees Celsius = 823 to 1473 K
    //    original polynomial in r delivers R in units of ohm*cm^2
    //    we multiply by 1.0e-4 to convert to ohm*m^2

    scalarField resist (anodePatchSize, 1);
    if (fuelCellType == "SOFC")	
    {
        scalar T0 = 273.15;         // 273.15 Kelvin = 0 Celsius
        scalarField r = 1000.0/(*anodeT-T0) - 1.1463;
        
        resist = 
        (
            1.0e-4*
        (0.2064 
        + 0.3495*r 
        + 0.3451*Foam::sqr(r) 
        + 4.1293*Foam::pow(r,3) 
        + 11.4228*Foam::pow(r,4))
        );
    }

    else if (fuelCellType == "HTPEFC")
    {
        resist = 1.0e-4 * (0.4025 - 0.0007*(*cathodeT));
    }

    else 
    {
        FatalError
        << "not available fuel cell type! (available==>SOFC / HTPEFC)" <<nl
        << exit (FatalError);
    }
    Info << "min,mean,max(resist): "
    << Foam::gMin(resist) <<" , "
    << Foam::gAverage(resist) <<" , "
    << Foam::gMax(resist) << endl;    
    