
    fvMesh* meshElectrolyte = nullptr;

    #include "readElectroChemProperties.H"
    
    forAll(solidRegions, i)
    {
        if (solidRegions[i].name() == "electrolyte")
        {
   	     meshElectrolyte = &solidRegions[i];
   	     break;
   	 }
    }

    if (meshElectrolyte == nullptr)
    {
        FatalErrorIn("Main program")
        << "Region 'electrolyte' not found! Please make sure region 'electrolyte' is defined."
        << exit(FatalError);
    }
    
    //volumetric electrochemical Heating

    volScalarField electrochemicalHeating
    (
        IOobject
        (
            "electrochemicalHeating",
            runTime.timeName(),
            *meshElectrolyte,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        *meshElectrolyte,
        dimensionedScalar
        (
            "electrochemicalHeating",
            dimensionSet(1, -1, -3, 0, 0, 0, 0),
            0
        ),
        zeroGradientFvPatchScalarField::typeName
    );

    Info<< "creating current density field\n" << endl;

    volScalarField jDensity
    (
        IOobject
        (
            "j",
            runTime.timeName(),
            *meshElectrolyte,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        *meshElectrolyte,
        dimensionedScalar
        (
            "j",
            dimensionSet(0, -2, 0, 0, 0, 1, 0),
            ibar0.value()
        ),
        zeroGradientFvPatchScalarField::typeName
    );
// Added SBB 02-02-14
 
    volScalarField etaA
    (
        IOobject
        (
            "etaA",
            runTime.timeName(),
            *meshElectrolyte,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        *meshElectrolyte,
        dimensionedScalar
        (
            "etaA",
            dimensionSet(1, 2, -3, 0, 0, -1, 0),
            0
        ),
        zeroGradientFvPatchScalarField::typeName
    );

    volScalarField etaC
    (
        IOobject
        (
            "etaC",
            runTime.timeName(),
            *meshElectrolyte,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        *meshElectrolyte,
        dimensionedScalar
        (
            "etaC",
            dimensionSet(1, 2, -3, 0, 0, -1, 0),
            0
        ),
        zeroGradientFvPatchScalarField::typeName
    );

    Info<< "creating voltage field\n" << endl;
    volScalarField voltage
    (
        IOobject
        (
            "V",
            runTime.timeName(),
            *meshElectrolyte,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        *meshElectrolyte,
        V.value(),
        zeroGradientFvPatchScalarField::typeName
    );

    Info<< "creating Nernst potential field\n" << endl;
    volScalarField Nernst
    (
        IOobject
        (
            "Nernst",
            runTime.timeName(),
            *meshElectrolyte,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        *meshElectrolyte,
        1,
        zeroGradientFvPatchScalarField::typeName
    );
