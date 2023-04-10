    CombustionModel<rhoReactionThermo>& reaction = reactionFluid[i];

    rhoReactionThermo& thermo = reaction.thermo();
    thermo.validate(args.executable(), "h", "e");

    basicMultiComponentMixture& composition = thermo.composition();
    PtrList<volScalarField>& Y = composition.Y();
    
    
    
    
    /*
    - read and store the chtSOFCSpecieProperties  
    */
    word fluidRegionName = fluidRegions[i].name();
    fileName chtSOFCSpeciePropertiesFile("constant"/fluidRegionName/"thermophysicalProperties");
    dictionary chtSOFCSpeciePropertiesDict = chtSOFCSpecie::chtSOFCSpecieDict(chtSOFCSpeciePropertiesFile);
    PtrList<chtSOFCSpecie> chtSOFCSpecies = chtSOFCSpecie::readAndStoreSOFCSpecies(chtSOFCSpeciePropertiesDict);
    
    /*
    - get the thermophysical Data to calculate cp etc.
    - Type gasHThermoPhysics = sutherlandTransport<species::thermo<janafThermo<perfectGas<specie>>, sensibleEnthalpy>> 
    - if differnet settings are used, it needs to be changed according to source fiel 'thermoPhysicsTypes.H' 
    */
    PtrList<gasHThermoPhysics> specieThermoData(Y.size());
    forAll(specieThermoData, i)
    {
        specieThermoData.set
        (
            i,
            new gasHThermoPhysics
            (
                dynamic_cast<const multiComponentMixture<gasHThermoPhysics>&>
                    (thermo).speciesData()[i]
            )
        );
    }
    
    speciesTable chtSOFCSpeciesNames
    (
    	chtSOFCSpeciePropertiesDict.lookup("species")
    );
    
    autoPtr<activationOverpotentialModel> activationOverpotential
    (
        activationOverpotentialModel::New(chtSOFCSpeciesNames, chtSOFCSpeciePropertiesDict)
    );
    
    
    
    

    label inertIndex = -1;
    if (Y.size())
    {
        const word inertSpecie(thermo.get<word>("inertSpecie"));
        inertIndex = composition.species().find(inertSpecie);
        if (inertIndex < 0)
        {
            FatalIOErrorIn(args.executable().c_str(), thermo)
                << "Inert specie " << inertSpecie
                << " not found in available species "
                << flatOutput(composition.species())
                << exit(FatalIOError);
        }
    }

    volScalarField& rho = rhoFluid[i];
    volVectorField& U = UFluid[i];
    surfaceScalarField& phi = phiFluid[i];

    compressible::turbulenceModel& turbulence = turbulenceFluid[i];
    volScalarField& K = KFluid[i];
    volScalarField& dpdt = dpdtFluid[i];

    volScalarField& p = thermo.p();
    const volScalarField& psi = thermo.psi();

    volScalarField& p_rgh = p_rghFluid[i];

    const volScalarField& gh = ghFluid[i];
    const surfaceScalarField& ghf = ghfFluid[i];

    multivariateSurfaceInterpolationScheme<scalar>::fieldTable& fields =
        fieldsFluid[i];

    volScalarField& Qdot = QdotFluid[i];

    radiation::radiationModel& rad = radiation[i];

    IOMRFZoneList& MRF = MRFfluid[i];
    fv::options& fvOptions = fluidFvOptions[i];

    fvVectorMatrix& UEqn = UEqFluid[i];

    const dimensionedScalar initialMass
    (
        "initialMass",
        dimMass,
        initialMassFluid[i]
    );

    bool frozenFlow = frozenFlowFluid[i];

    const label pRefCell = pRefCellFluid[i];
    const scalar pRefValue = pRefValueFluid[i];

    const dimensionedScalar rhoMax = rhoMaxFluid[i];
    const dimensionedScalar rhoMin = rhoMinFluid[i];

    const pressureControl& pressureControl = pressureControls[i];
