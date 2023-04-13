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
    const scalar molWeight,
    const scalar nElectrons,
    const label  rSign,
    const scalar hForm,
    const scalar sForm,
    const scalar stoiCoeff
)
:
    name_(name),
    Y_(Y),
    molWeight_(molWeight),
    nElectrons_(nElectrons),
    rSign_(rSign),
    hForm_(hForm),
    sForm_(sForm),
    stoiCoeff_(stoiCoeff)
{}


inline Foam::chtSOFCSpecie::chtSOFCSpecie
(
    const scalar Y,
    const scalar molWeight,
    const scalar nElectrons,
    const label  rSign,
    const scalar hForm,
    const scalar sForm,
    const scalar stoiCoeff   
)
:
    Y_(Y),
    molWeight_(molWeight),
    nElectrons_(nElectrons),
    rSign_(rSign),
    hForm_(hForm),
    sForm_(sForm),
    stoiCoeff_(stoiCoeff)    
{}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

inline Foam::chtSOFCSpecie::chtSOFCSpecie(const word& name, const chtSOFCSpecie& st)
:
    name_(name),
    Y_(st.Y_),
    molWeight_(st.molWeight_),
    nElectrons_(st.nElectrons_),
    rSign_(st.rSign_),
    hForm_(st.hForm_),
    sForm_(st.sForm_),
    stoiCoeff_(st.stoiCoeff_)
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

inline Foam::scalar Foam::chtSOFCSpecie::nE() const
{
    return nElectrons_;
}

inline Foam::label  Foam::chtSOFCSpecie::rSign() const
{
    return rSign_;
}

inline Foam::scalar Foam::chtSOFCSpecie::hForm() const
{
    return hForm_;
}

inline Foam::scalar Foam::chtSOFCSpecie::sForm() const
{
    return sForm_;
}

inline Foam::scalar Foam::chtSOFCSpecie::stoiCoeff() const
{
    return stoiCoeff_;
}

// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

inline void Foam::chtSOFCSpecie::operator=(const chtSOFCSpecie& st)
{
    // Preserve original name
    Y_ = st.Y_;
    molWeight_ = st.molWeight_;
    nElectrons_ = st.nElectrons_;
    rSign_ = st.rSign_;
    hForm_ = st.hForm_;
    sForm_ = st.sForm_;
    stoiCoeff_ = st.stoiCoeff_;
}

inline Foam::chtSOFCSpecie* Foam::chtSOFCSpecie::clone() const
{
    return new chtSOFCSpecie(*this);
}

