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

#include "specie.H"
#include "word.H"
#include "scalar.H"
#include "dictionary.H"

#include "thermodynamicConstants.H"
using namespace Foam::constant::thermodynamic;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward Declarations
class chtSOFCSpecie;

inline chtSOFCSpecie operator+(const chtSOFCSpecie&, const chtSOFCSpecie&);
inline chtSOFCSpecie operator*(const scalar, const chtSOFCSpecie&);
inline chtSOFCSpecie operator==(const chtSOFCSpecie&, const chtSOFCSpecie&);

Ostream& operator<<(Ostream&, const chtSOFCSpecie&);


/*---------------------------------------------------------------------------*\
                           Class chtSOFCSpecie Declaration
\*---------------------------------------------------------------------------*/

class chtSOFCSpecie : public specie
{
    // Private Data

        //- Name of chtSOFCSpecie
        word name_;

        //- Number of moles of this component in the mixture
        scalar Y_;

        //- Molecular weight of chtSOFCSpecie [kg/kmol]
        scalar molWeight_;


public:

    //- Declare type-name (with debug switch)
    ClassName("chtSOFCSpecie");


    // Generated Methods

        //- Copy construct
        chtSOFCSpecie(const chtSOFCSpecie&) = default;


    // Constructors

        //- Construct from components without name
        inline chtSOFCSpecie(const scalar Y, const scalar molWeight);

        //- Construct from components with name
        inline chtSOFCSpecie
        (
            const word& name,
            const scalar Y,
            const scalar molWeight
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


        // IO

            //- Write to Ostream
            void write(Ostream& os) const;


    // Member Operators

        //- Copy assignment, preserve original name
        inline void operator=(const chtSOFCSpecie&);

        inline void operator+=(const chtSOFCSpecie&);
        inline void operator*=(const scalar);


    // Friend Operators

        inline friend chtSOFCSpecie operator+(const chtSOFCSpecie&, const chtSOFCSpecie&);
        inline friend chtSOFCSpecie operator*(const scalar, const chtSOFCSpecie&);
        inline friend chtSOFCSpecie operator==(const chtSOFCSpecie&, const chtSOFCSpecie&);


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
