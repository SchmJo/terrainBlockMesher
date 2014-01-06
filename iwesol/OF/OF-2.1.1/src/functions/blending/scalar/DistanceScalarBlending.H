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
    Foam::oldev::DistanceScalarBlending

Description

SourceFiles
    DistanceScalarBlendingI.H
    DistanceScalarBlending.C
    DistanceScalarBlendingIO.C

\*---------------------------------------------------------------------------*/

#ifndef DistanceScalarBlending_H
#define DistanceScalarBlending_H

#include "ScalarBlendingFunction.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

namespace oldev
{


/*---------------------------------------------------------------------------*\
                         Class DistanceScalarBlending Declaration
\*---------------------------------------------------------------------------*/

class DistanceScalarBlending
:
    public ScalarBlendingFunction
{

    // Private member data

    	/// the minimal distance
    	scalar dMin_;

    	/// the maximal distance
    	scalar dMax_;

    	/// the transition function from dMin to dMax
    	autoPtr< TransitionFunction > dTransition_;


public:

    // The OF run time selection type name
    TypeName("distance");

    // Constructors

    	/// Constructor
    	DistanceScalarBlending
    	(
    			const dictionary & dict
    	);


    // Public member functions

    	// Access

    	/// BlendingFunction: possible s-value transformation
    	void sTransformation
    	(
    			const point & point0,
    			const point & point1,
    			const scalar & data0,
    			const scalar & data1,
    			scalar & s
    	) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace oldev

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
