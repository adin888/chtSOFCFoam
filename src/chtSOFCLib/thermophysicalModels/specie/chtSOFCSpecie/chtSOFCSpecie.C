/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2017 OpenFOAM Foundation
    Copyright (C) 2020 OpenCFD Ltd.
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "chtSOFCSpecie.H"
#include "constants.H"

/* * * * * * * * * * * * * * * public constants  * * * * * * * * * * * * * * */

namespace Foam
{
    defineTypeNameAndDebug(chtSOFCSpecie, 0); 
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::chtSOFCSpecie::chtSOFCSpecie(const dictionary& dict)
:
    name_(dict.dictName()),
    Y_(dict.subDict("specie").getOrDefault<scalar>("massFraction", 1)),
    molWeight_(dict.subDict("specie").get<scalar>("molWeight")),
    nElectrons_(dict.subDict("specie").get<scalar>("nElectrons")),
    rSign_(dict.subDict("specie").get<label>("rSign")),
    hForm_(dict.subDict("specie").get<scalar>("hForm")),
    sForm_(dict.subDict("specie").get<scalar>("sForm"))
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

Foam::PtrList<Foam::chtSOFCSpecie> 
Foam::chtSOFCSpecie::readAndStoreSOFCSpecies
(const fileName& chtSOFCSpeciePropertiesFile)
{
    IFstream fileStream(chtSOFCSpeciePropertiesFile);
    dictionary chtSOFCSpeciePropertiesDict(fileStream);

    wordList speciesNames = chtSOFCSpeciePropertiesDict.toc();
    PtrList<chtSOFCSpecie> chtSOFCSpecies(speciesNames.size());

    //for (label i = 0; i < speciesNames.size(); ++i)
    forAll(chtSOFCSpecies, i)
    {
        const word& speciesName = speciesNames[i];
        const dictionary& specieDict = chtSOFCSpeciePropertiesDict.subDict(speciesName);
        // Allocate memory for each element in the PtrList
        chtSOFCSpecies.set(i, new chtSOFCSpecie(specieDict));
    }

    return chtSOFCSpecies;
}

void Foam::chtSOFCSpecie::write(Ostream& os) const
{
    // Entries in dictionary format
    {
        os.beginBlock("chtSOFCSpecie");
        os.writeEntryIfDifferent<scalar>("massFraction", 1, Y_);
        os.writeEntry("molWeight", molWeight_);
        os.writeEntry("nElectrons", nElectrons_);
        os.writeEntry("rSign", rSign_);
        os.writeEntry("hForm", hForm_);
        os.writeEntry("sForm", sForm_);      
        os.endBlock();
    }
}


// * * * * * * * * * * * * * * * Ostream Operator  * * * * * * * * * * * * * //

Foam::Ostream& Foam::operator<<(Ostream& os, const chtSOFCSpecie& st)
{
    st.write(os);
    os.check(FUNCTION_NAME);
    return os;
}


// ************************************************************************* //
