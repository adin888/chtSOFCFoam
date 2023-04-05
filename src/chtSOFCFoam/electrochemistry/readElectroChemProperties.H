    IOdictionary electroChemProperties
    (
        IOobject
        (
            "electroChemProperties",                            
            runTime.constant(),         
            runTime,                     
            IOobject::MUST_READ,          
            IOobject::NO_WRITE             
        )
    );

    word fuelCellType
    (
    	electroChemProperties.lookup("fuelCellType")
    );
    		
    Switch galvanostatic
    (
    	"galvanostatic",
    	electroChemProperties
    );
    
    scalar zRxn
    (
    	electroChemProperties.lookupOrDefault<scalar>("zRxn", 2.0)
    );
    
    scalar PreIterAct
    (
    	electroChemProperties.lookupOrDefault<scalar>("PreIterAct", 0.0)
    );

    scalar PreIterV
    (
   	electroChemProperties.lookupOrDefault<scalar>("PreIterV", 0.0)
    );

    scalar relaxj
    (
    	electroChemProperties.lookupOrDefault<scalar>("relax", 0.1)
    );

    dimensionedScalar V
    (
    	"voltage",
    	dimensionSet(1, 2, -3, 0, 0, -1, 0),
    	electroChemProperties
    );

    dimensionedScalar ibar0
    (
	"ibar0",
	dimensionSet(0, -2, 0, 0, 0, 1, 0),
	electroChemProperties
    );

    dimensionedScalar Tinit
    (
	"Tinit",
	dimensionSet(0, 0, 0, 1, 0, 0, 0),
	electroChemProperties
    );

    scalar numCells
    (
    	electroChemProperties.lookupOrDefault<scalar>("numCells", 1.0)
    );

    dimensionedScalar Rhat
    (
	"Rhat",
	dimensionSet(1, 4, -3, 0, 0, -2, 0),
	electroChemProperties
    );

    dimensionedScalar Rhat0
    (
	"Rhat0",
	dimensionSet(1, 4, -3, 0, 0, -2, 0),
	electroChemProperties
    );

