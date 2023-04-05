// Nernst Eqn
// E = E0 - RT/(zF) ln(Q)
// where
//    E = cell potential
//    E0 = standard cell potential at temperature of interest, T
//       = -deltaG_rxn/(zf)
//         where deltaG_rxn = Gibbs free energy of reaction at temperature T
//    R = universal gas constant
//    F = Faraday constant
//    Q = reaction quotient
//    z = moles of electrons transferred
//
// for reaction Sum(ai Ri) = Sum(bj Pj)
//
//    Q = Qrxn = product([Pj]^bj)/product([Ri]^ai)
//               where [X] denotes moleFraction of X
//
//    deltaG_rxn = deltaH_rxn - T*deltaS_rxn
//
//        deltaH_rxn = sum(bj*deltaH(Pj)) - sum(ai*deltaH(Ri)
//
//            deltaH(X) = deltaH_f^0(X) + intgral_T0^T(Cp_X(T)dT)
//
//        deltaS_rxn = sum(bj*S(Pj)) - sum(ai*S(Ri))
//
//            S(X) = S^0(X) + intgral_T0^T(Cp_X(T)/T dT)
//
// eg for rxn
//                H2 + (1/2)O2 = H2O
// at T = 1000K,
// using tabulated gas phase thermochemistry data from NIST chemistry web book
// http://webbook.nist.gov/chemistry/ (accessed 2012 july 10)
// we find
//                deltaH_rxn = -247.86 kJ/mol  (ie, per mole of H2)
//                deltaS_rxn = -0.0553 kJ/(mol K)
//                deltaG_rxn = deltaH_rxn - T*deltaS_rxn = -192.56 kJ/mol
//
//		  scalar Tr = 298.15;    // reference temperature

    scalarField cpInt(anodePatchSize, 0);

    // update with fuel reactants and products
    forAll(chtSOFCSpecies, s)
    {
    	const label& chemFlag = chtSOFCSpecies[s].rSign(); // product or reactant
    	
    	if(chemFlag == 0)
    	{
    	    continue;
    	}
    	
    	forAll(anodeT, celli)
    	{
            scalar currentT = anodeT[celli];
            cpInt[celli] = 
            (
            	specieThermoData[s].Cp(0, currentT)
            	- specieThermoData[s].Cp(0, Tr)
            ); // Assuming p = 0 since Cp is not pressure-dependent for ideal gases
    	    
   	 }
        
        
	// product
	scalar stoiCoeffi = chtSOFCSpecies[s].stoiCoeff();

	Qrxn *= Foam::pow(Foam::pow(xSpecie[s], stoiCoeffi),chemFlag);

	scalarField Hi = chemFlag * stoiCoeffi *
	(
	   chtSOFCSpecies[s].hForm()
	   + cpInt
	);
	deltaHrxn += Hi;

	scalarField Si = chemFlag * stoiCoeffi *
	(
	   chtSOFCSpecies[s].sForm()
	   + cpInt
	);
	deltaSrxn += Si;

       
        Info << chtSOFCSpecies[s].name() << "  " << stoiCoeffi << nl;
        Info<< "   mean H(T) = " << gAverage(Hi) << nl;
        Info<< "   mean S(T) = " << gAverage(Si) << endl;
        
    }


/* Performed only after calculations for chemical reactions in all fluid regions*/
    if(i == (fluidRegions.size() - 1))
    {
        // Gibbs
        // -----
        scalarField deltaGrxn = deltaHrxn - anodeT*deltaSrxn;

        Info<< " mean deltaHrxn = " << gAverage(deltaHrxn) << nl
        << " mean anodeT    = " << gAverage(anodeT)    << nl
        << " mean deltaSrxn = " << gAverage(deltaSrxn) << nl
        << " mean TdeltaS   = " << gAverage(anodeT*deltaSrxn) << nl
        << " mean deltaG    = " << gAverage(deltaGrxn) << nl
        << endl;

        // Nernst
        // ------
        Nernst =
            (-deltaGrxn - R * anodeT * Foam::log(Qrxn))
            / (zRxn * F);

        Info<< "min,mean,max Nernst = " << gMin(Nernst) << "  "
        << gAverage(Nernst) << "  " << gMax(Nernst) << endl;
    }


