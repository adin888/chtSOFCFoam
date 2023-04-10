    // calculate volumetric electrochemical heating

    Info << nl << "Calculating electrochemical heating" << endl;

    // jEA applied in cell volumes (not patch)
    scalarField jEA = jDensity.primitiveFieldRef();


    //scalar Tr = 298.15;  //reference Temperature [K] for enthalpy calculations


    // Note (hkr 2012.04.17):
    // Following Cp(T) are 6th degree (7th order) polynomials in T[K]/1000, from
    // Todd & Young, Thermodynamic and transport properties of gases for use in
    // solid oxide fuel cell modelling, J. Power Sources 110 (2002) pp186-200.
    // Cp(T) = sum(a[k]*(T/1000)^k), k = 0,..,6
    // Enthalpies are definite integrals from reference to ambient temperature.


    /***********************************************************************
    scalarField Tsource =
        -(H2Oform + hH2O - hH2 -0.5*hO2)*jEA/(2*F)/hE.value()
        - jEA*V.value()/hE.value();
    ***********************************************************************/




    // air side accumulators
    scalarField hReactSrc(anodeT.size(),0);
    scalarField hProdSrc(anodeT.size(),0);

    //PtrList<scalarField> hSp(chtSOFCSpecies.size());
    forAll(chtSOFCSpecies, s)
    {
        if(chtSOFCSpecies[s].nE() != 0)
        {
            forAll(anodeT, celli)
    	    {
                scalar currentT = anodeT[celli];
                cpInt[celli] = 
                (
            	    specieThermoData[s].Cp(0, currentT)
        	    - specieThermoData[s].Cp(0, Tr)
                ); // Assuming p = 0 since Cp is not pressure-dependent for ideal gases
    	    
   	     }
            //hSp.set(s, cpInt);

            hFormSum += chtSOFCSpecies[s].hForm()/chtSOFCSpecies[s].nE();
            // allow reactions on airside, S.Keuler (Dec. 2013)
         

            if(chtSOFCSpecies[s].rSign() > 0)
            {
                // product
                hProdSrc += cpInt/chtSOFCSpecies[s].nE();
            }
            else if(chtSOFCSpecies[s].rSign() < 0)
            {
                // reactant
                hReactSrc += cpInt/chtSOFCSpecies[s].nE();
            }
            else
            {
                Info<< "ERROR: rSign = 0 but ne != 0" << nl;
            }
        }
    }
    //Info<< "sum(hReactSrcAir)= " << gSum(hReactSrcAir) << nl;
    //Info<< "sum(hProdSrcAir)= " << gSum(hProdSrcAir) << nl;
    //Info<< "sum(prod - react)=" << gSum(hProdSrcAir - hReactSrcAir) << nl;

/* -------------------------------------------------------------------------- */

    scalarField volMolRate = jEA/F/hE.value();    // [mol/(m^3 s)]

    hSource +=(hProdSrc - hReactSrc)*volMolRate;

    //Info<< "hSource  min max sum: "
    //    << gMin(hSource) << " "
    //    << gMax(hSource) << " " << gSum(hSource) << nl;
    //Info<< nl;

//
//    scalarField Tsource =
//        -(H2Oform + hH2O - hH2 -0.5*hO2)*jEA/(2*F)/hE.value()
//        - jEA*V.value()/hE.value();
//

    scalarField& electrochemicalHeatingIn = electrochemicalHeating.primitiveFieldRef();
    electrochemicalHeatingIn =
    (
        -hFormSum*volMolRate - hSource - jEA*voltage/hE.value()
    );
    electrochemicalHeating.correctBoundaryConditions();

    Info << "min,mean,max(electrochemicalHeating): "
         << gMin(electrochemicalHeating.primitiveFieldRef()) << "  "
         << gAverage(electrochemicalHeating.primitiveFieldRef()) << "  "
         << gMax(electrochemicalHeating.primitiveFieldRef()) << endl;

