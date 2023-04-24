    // calculate volumetric electrochemical heating

    Info << nl << "Calculating electrochemical heating" << endl;
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
    scalarField hAbsolute(anodeT->size(),0);
    scalarField hReactSrc(anodeT->size(),0);
    scalarField hProdSrc(anodeT->size(),0);

    //PtrList<scalarField> hSp(chtSOFCSpecies.size());
    forAll(chtSOFCSpecies, s)
    {
        if(chtSOFCSpecies[s].nE() != 0)
        {
            forAll(*anodeT, celli)
    	    {
                const scalar& currentT = (*anodeT)[celli];
                const scalar& currentP = (*anodeP)[celli];
                hAbsolute[celli] = 
                (
            	    specieThermoData[s].Ha(currentP, currentT)
                    * (chtSOFCSpecies[s].W() / 10e3 * 10)
                ); 
   	        }  
            /* hAbsolute already contains hForm */
            //hFormSum += chtSOFCSpecies[s].hForm()/chtSOFCSpecies[s].nE();

            if(chtSOFCSpecies[s].rSign() > 0)
            {
                // product
                hProdSrc += hAbsolute/chtSOFCSpecies[s].nE();
            }
            else if(chtSOFCSpecies[s].rSign() < 0)
            {
                // reactant
                hReactSrc += hAbsolute/chtSOFCSpecies[s].nE();
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
    
    hSource +=(hProdSrc - hReactSrc);
    //Info<< "hSource  min max sum: "
    //    << gMin(hSource) << " "
    //    << gMax(hSource) << " " << gSum(hSource) << nl;
    //Info<< nl;
/* -------------------------------------------------------------------------- */

    if(i == (fluidRegions.size() - 1))
    {
        // jEA applied in cell volumes (not patch)
        const scalarField& jEA = jDensity.primitiveField();
        scalarField volMolRate = jEA/F/hE.value();    // [mol/(m^3 s)]

        //
        //    scalarField Tsource =
        //        -(H2Oform + hH2O - hH2 -0.5*hO2)*jEA/(2*F)/hE.value()
        //        - jEA*V.value()/hE.value();
        //
        scalarField& electrochemicalHeatingIn = electrochemicalHeating.primitiveFieldRef();
        electrochemicalHeatingIn =
        (
            //-hFormSum*volMolRate - hSource*volMolRate - jEA*voltage/hE.value()
            - hSource*volMolRate - jEA*voltage/hE.value()
        );
        electrochemicalHeating.correctBoundaryConditions();

        Info << "min,mean,max(electrochemicalHeating): "
            << gMin(electrochemicalHeating.primitiveField()) << "  "
            << gAverage(electrochemicalHeating.primitiveField()) << "  "
            << gMax(electrochemicalHeating.primitiveField()) << endl;
    }

