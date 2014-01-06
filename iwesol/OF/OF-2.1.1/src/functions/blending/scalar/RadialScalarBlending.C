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

\*---------------------------------------------------------------------------*/

#include "RadialScalarBlending.H"
#include "addToRunTimeSelectionTable.H"

namespace Foam
{

namespace oldev
{

defineTypeNameAndDebug(RadialScalarBlending, 0);
addToRunTimeSelectionTable(ScalarBlendingFunction,RadialScalarBlending,dict);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

RadialScalarBlending::RadialScalarBlending
(
			const dictionary & dict
):
			ScalarBlendingFunction
			(
					dict
			),
			centre_(dict.lookup("centre")),
			rMin_(readScalar(dict.lookup("rMin"))),
			rMax_(readScalar(dict.lookup("rMax"))),
			rTransition_
			(
					TransitionFunction::New
					(
							dict.subDict("radialTransitionFunction")
					)
			){
}

// * * * * * * * * * * * * * * * * Public member functions  * * * * * * * * * * * * * * //

void RadialScalarBlending::sTransformation
(
		const point & point0,
		const point & point1,
		const scalar & data0,
		const scalar & data1,
		scalar & s
) const{

	// measure distance:
	point ps0  = (1. - s) * point0 + s * point1;
	scalar ds0 = mag(ps0 - centre_);

	// case small distance:
	if(ds0 <= rMin_) {
		s = 0;
		return;
	}

	// case large distance:
	if(ds0 >= rMax_) {
		s = 1;
		return;
	}

	// in between return transition:
	scalar ds = ds0 - rMin_;
	s = rTransition_()(ds / (rMax_ - rMin_) );
}


// ************************************************************************* //

} // End namespace oldev

} // End namespace Foam
