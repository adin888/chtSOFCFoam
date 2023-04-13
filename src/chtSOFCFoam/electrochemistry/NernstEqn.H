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
/* 
- integral of Cp(T)dT for enthalpy
- integral of (Cp(T)/T)dT for entropy
*/
    scalarField Grxn(anodePatchSize, 0); //[J/kg]

    // update with fuel reactants and products
    forAll(chtSOFCSpecies, s)
    {
    	const label& chemFlag = chtSOFCSpecies[s].rSign(); // product(1) or reactant(-1)
    	const scalar& stoiCoeffs = chtSOFCSpecies[s].stoiCoeff();
    	if(chemFlag == 0)
    	{
    	    continue;
    	}

    	forAll(*anodeT, celli)
    	{
            const scalar& currentT = Foam::max((*anodeT)[celli], SMALL);
            Grxn[celli] = 
            (
            	specieThermoData[s].Gstd(currentT)  //[J/kg]
            ); 
   	    }

        Qrxn *= Foam::pow(Foam::pow(xSpecie[s], stoiCoeffs),chemFlag);

        Grxn *= (chtSOFCSpecies[s].W() / 10e3 * 10);       // [J/kg] to [J/mol]; why *10?

        deltaGrxn += (Grxn * chemFlag * stoiCoeffs);  

        Info << chtSOFCSpecies[s].name() << "  " << stoiCoeffs << nl;
        Info<< "   mean deltaGrxn = " << gAverage(deltaGrxn) << endl;
    }


/* Performed only after calculations for chemical reactions in all fluid regions */
    if(i == (fluidRegions.size() - 1))
    {
        // Gibbs
        // -----
        //scalarField deltaGrxn = deltaHrxn - anodeT*deltaSrxn;

        // Nernst
        // ------
        NernstAnode =
            (
                - deltaGrxn
                - (*anodeT) * R * Foam::log(Qrxn)
            )
            / (zRxn * F);

        Info << " mean anodeT   = " << gAverage(*anodeT) 
        << "  min,mean,max NernstAnode = " << gMin(NernstAnode) << "  "
        << gAverage(NernstAnode) << "  " << gMax(NernstAnode) << endl;
    }


