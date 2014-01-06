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
    Foam::oldev::BlendingFunction

Description

SourceFiles
    BlendingFunctionI.H

\*---------------------------------------------------------------------------*/

#ifndef BlendingFunction_H
#define BlendingFunction_H

#include "TransitionFunction.H"
#include "point.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

namespace oldev
{

/*---------------------------------------------------------------------------*\
                         Class BlendingFunction Declaration
\*---------------------------------------------------------------------------*/

template<class Data, class Point = point >
class BlendingFunction
{
    // Private data

    	/// The transition function
    	autoPtr< TransitionFunction > transition_;


public:

    // Constructors

    	/// Constructor
    	BlendingFunction
    	(
    			const word & transitionType
    	);

    	/// Constructor
    	BlendingFunction
    	(
    			const dictionary & dict
    	);


    /// Destructor
    virtual ~BlendingFunction(){}


    // Member Functions

		/// BlendingFunction: possible s-value transformation
		virtual void sTransformation(
			const Point & point0,
			const Point & point1,
			const Data & data0,
			const Data & data1,
			scalar & s
		) const {}

		/// Interpolates at s, 0 <= s <= 1
		inline Data blend(
			const Point & point0,
			const Point & point1,
			const Data & data0,
			const Data & data1,
			scalar & s
		) const;

		/// returns the transition function
		inline const TransitionFunction & transition() const { return transition_(); }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace oldev

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "BlendingFunctionI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
