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

Class
    Foam::chtSOFCSpecie

Description
    Base class of the thermophysical property types.

SourceFiles
    chtSOFCSpecieI.H
    chtSOFCSpecie.C

\*---------------------------------------------------------------------------*/

#ifndef chtSOFCSpecie_H
#define chtSOFCSpecie_H

#include "word.H"
#include "scalar.H"
#include "dictionary.H"
#include "IFstream.H"

#include "thermodynamicConstants.H"
using namespace Foam::constant::thermodynamic;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward Declarations
class chtSOFCSpecie;

Ostream& operator<<(Ostream&, const chtSOFCSpecie&);

/*---------------------------------------------------------------------------*\
                           Class chtSOFCSpecie Declaration
\*---------------------------------------------------------------------------*/

class chtSOFCSpecie
{
    // Private Data

        //- Name of chtSOFCSpecie
        word name_;

        //- Number of moles of this component in the mixture
        scalar Y_;

        //- Molecular weight of chtSOFCSpecie [kg/kmol]
        scalar molWeight_;

        //- Number of electrons this specie contributes to electrochemistry
        scalar nElectrons_;

        //- Is the specie consumed(-1), produced(+1), or inert(0)?
        label rSign_;

        //- Enthalpy of formation at 298.15 K
        scalar hForm_;

        //- Standard entropy at 298.15 K
        scalar sForm_;
        
        scalar stoiCoeff_;
        
public:

    //- Declare type-name (with debug switch)
    ClassName("chtSOFCSpecie");


    // Generated Methods

        //- Copy construct
        chtSOFCSpecie(const chtSOFCSpecie&) = default;


    // Constructors

        //- Construct from components without name
        inline chtSOFCSpecie
        (
            const scalar Y,
            const scalar molWeight,
            const scalar nElectrons,
            const label  rSign,
            const scalar hForm,
            const scalar sForm,
            const scalar stoiCoeff        
        );

        //- Construct from components with name
        inline chtSOFCSpecie
        (
            const word& name,
            const scalar Y,
            const scalar molWeight,
            const scalar nElectrons,
            const label  rSign,
            const scalar hForm,
            const scalar sForm,
            const scalar stoiCoeff
        );

        //- Construct as named copy
        inline chtSOFCSpecie(const word& name, const chtSOFCSpecie&);

        //- Construct from dictionary
        explicit chtSOFCSpecie(const dictionary& dict);


    // Member Functions

        // Access

            //- Name
            inline const word& name() const;

            //- Molecular weight [kg/kmol]
            inline scalar W() const;

            //- No of moles of this chtSOFCSpecies in mixture
            inline scalar Y() const;

            //- Gas constant [J/(kg K)]
            inline scalar R() const;
            
            //- Number of electrons this specie contributes to electrochemistry
            inline scalar nE() const;

            //- Is the specie consumed(<0), produced(>0), or inert(0)?
            inline label rSign() const;

            //- Enthalpy of formation at 298.15 K
            inline scalar hForm() const;

            //- Standard entropy at 298.15 K
            inline scalar sForm() const;
            
            inline scalar stoiCoeff() const;


        // IO
        
            //read and store objects
            static dictionary chtSOFCSpecieDict(const fileName& chtSOFCSpeciePropertiesFile);
            static PtrList<chtSOFCSpecie> readAndStoreSOFCSpecies(const dictionary& chtSOFCSpeciePropertiesDict);
            
            //- Write to Ostream
            void write(Ostream& os) const;
            
    // Member Operators

        //- Copy assignment, preserve original name
        inline void operator=(const chtSOFCSpecie&);


    // IOstream Operators

        friend Ostream& operator<<(Ostream&, const chtSOFCSpecie&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "chtSOFCSpecieI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
