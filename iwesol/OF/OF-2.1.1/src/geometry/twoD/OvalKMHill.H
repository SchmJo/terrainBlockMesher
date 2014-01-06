/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011 OpenFOAM Foundation
     \\/     M anipulation  |
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
    Foam::oldev::OvalKMHill

Description

SourceFiles
    OvalKMHillI.H
    OvalKMHill.C
    OvalKMHillIO.C

\*---------------------------------------------------------------------------*/

#ifndef OvalKMHill_H
#define OvalKMHill_H

#include "KasmiMassonHill.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

namespace oldev
{

/*---------------------------------------------------------------------------*\
                         Class OvalKMHill Declaration
\*---------------------------------------------------------------------------*/

class OvalKMHill
:
    public KasmiMassonHill
{
    // Private data

    	/// the width
    	scalar b_;

    	/// the length direction
    	Foam::vector n_length_;

    	/// the width direction
    	Foam::vector n_width_;

    	/// The data at an angle.
    	mutable HashTable<KasmiMassonHill> angleHills_;


public:

	// The OF run time selection type name
	TypeName("ovalKasmiMassonHill");


    // Constructors

    	/// Construct from sictionary
    	OvalKMHill(const dictionary & dict);

    	/// Construct from sictionary
    	OvalKMHill(){}


    // Member Functions

        // Access

    	/// GroundObject: Returns height at point (here this must me in the same plane, no projection)
    	scalar height(const point & p) const;

    	/// Returns the width
    	inline scalar coRadius() const { return b_; }

    	/// Returns the length direction
    	inline const Foam::vector & lengthDirection() const { return n_length_; }

    	/// Returns the width direction
    	inline const Foam::vector & widthDirection() const { return n_width_; }

    	/// Returns the KasmiMassonHill at an angle. zero angle corresponds to length direction.
    	const KasmiMassonHill & getKMHill(scalar angleRad) const;

    	/// Returns the height at a given angle and distance from the center. calculates if not known yet.
    	inline scalar height(scalar angleRad, scalar dist) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace oldev

} // End namespace Foam


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "OvalKMHillI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
