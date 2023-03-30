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

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

inline Foam::chtSOFCSpecie::chtSOFCSpecie
(
    const word& name,
    const scalar Y,
    const scalar molWeight
)
:
    specie(name, Y, molWeight)
{}


inline Foam::chtSOFCSpecie::chtSOFCSpecie
(
    const scalar Y,
    const scalar molWeight
)
:
    specie(Y, molWeight)
{}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

inline Foam::chtSOFCSpecie::chtSOFCSpecie(const word& name, const chtSOFCSpecie& st)
:
    specie(name, st.Y(), st.W())
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline const Foam::word& Foam::chtSOFCSpecie::name() const
{
    return name_;
}


inline Foam::scalar Foam::chtSOFCSpecie::W() const
{
    return molWeight_;
}


inline Foam::scalar Foam::chtSOFCSpecie::Y() const
{
    return Y_;
}


inline Foam::scalar Foam::chtSOFCSpecie::R() const
{
    return RR/molWeight_;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

inline void Foam::chtSOFCSpecie::operator=(const chtSOFCSpecie& st)
{
    // Preserve original name
    Y_ = st.Y_;
    molWeight_ = st.molWeight_;
}


inline void Foam::chtSOFCSpecie::operator+=(const chtSOFCSpecie& st)
{
    const scalar sumY = Y_ + st.Y_;
    if (mag(sumY) > SMALL)
    {
        molWeight_ = sumY/(Y_/molWeight_ + st.Y_/st.molWeight_);
    }

    Y_ = sumY;
}


inline void Foam::chtSOFCSpecie::operator*=(const scalar s)
{
    Y_ *= s;
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

inline Foam::chtSOFCSpecie Foam::operator+(const chtSOFCSpecie& st1, const chtSOFCSpecie& st2)
{
    const scalar sumY = max(st1.Y_ + st2.Y_, SMALL);

    if (mag(sumY) > SMALL)
    {
        return chtSOFCSpecie
        (
            sumY,
            sumY/(st1.Y_/st1.molWeight_ + st2.Y_/st2.molWeight_)
        );
    }
    else
    {
        return st1;
    }
}


inline Foam::chtSOFCSpecie Foam::operator*(const scalar s, const chtSOFCSpecie& st)
{
    return chtSOFCSpecie
    (
        s*st.Y_,
        st.molWeight_
    );
}


inline Foam::chtSOFCSpecie Foam::operator==(const chtSOFCSpecie& st1, const chtSOFCSpecie& st2)
{
    scalar diffY = st2.Y_ - st1.Y_;
    if (mag(diffY) < SMALL)
    {
        diffY = SMALL;
    }

    const scalar diffRW = st2.Y_/st2.molWeight_ - st1.Y_/st1.molWeight_;

    #ifdef __clang__
    // Using intermediate volatile bool to prevent compiler optimising out the
    // if block (above) - CLANG 3.7.1
    volatile const bool valid = (mag(diffRW) > SMALL);
    const scalar molWeight = valid ? diffY/diffRW : GREAT;
    #else
    scalar molWeight = GREAT;
    if (mag(diffRW) > SMALL)
    {
         molWeight = diffY/diffRW;
    }
    #endif

    return chtSOFCSpecie(diffY, molWeight);
}


// ************************************************************************* //