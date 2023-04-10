/*---------------------------------------------------------------------------*\
\*---------------------------------------------------------------------------*/

#include "error.H"
#include "activationOverpotentialModel.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

autoPtr<activationOverpotentialModel> activationOverpotentialModel::New
(
    const speciesTable& speciesNames,
    const dictionary& dict
)
{

    const dictionary& activationDict = dict.subDict("activation");
    
    word diffTypeName
    (
    	activationDict.lookup("type")
    );

    Info<< "Selecting activation overpotential model..." << diffTypeName << endl;

    auto* ctorPrt = dictionaryConstructorTable(diffTypeName);

    if (!ctorPrt)
    {
        FatalErrorIn
        (
            "activationOverpotentialModel::New(const speciesTable& speciesNames, const dictionary& dict)"
        )   << "Unknown activationOverpotentialModel type " << diffTypeName
            << endl << endl
            << "Valid activationOverpotentialModel types are :" << endl
            << dictionaryConstructorTablePtr_->toc()
            << exit(FatalError);
    }

    return autoPtr<activationOverpotentialModel>(ctorPrt(speciesNames, activationDict));
}




// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
