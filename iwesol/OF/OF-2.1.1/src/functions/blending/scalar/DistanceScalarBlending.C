/*---------------------------------------------------------------------------*\
                               |
  _____        _______ ____    | IWESOL: IWES Open Library
 |_ _\ \      / / ____/ ___|   |
  | | \ \ /\ / /|  _| \___ \   | Copyright: Fraunhofer Institute for Wind
  | |  \ V  V / | |___ ___) |  | Energy and Energy System Technology IWES
 |___|  \_/\_/  |_____|____/   |
                               | http://www.iwes.fraunhofer.de
                               |
-------------------------------------------------------------------------------
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright  held by original author
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of IWESOL and it is based on OpenFOAM.

    IWESOL and OpenFOAM are free software: you can redistribute them and/or modify
    them under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    IWESOL and OpenFOAM are distributed in the hope that they will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
    or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "DistanceScalarBlending.H"
#include "addToRunTimeSelectionTable.H"
#include "LinearTransition.H"

namespace Foam
{

namespace oldev
{

defineTypeNameAndDebug(DistanceScalarBlending, 0);
addToRunTimeSelectionTable(ScalarBlendingFunction,DistanceScalarBlending,dict);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

DistanceScalarBlending::DistanceScalarBlending(
			const dictionary & dict
			):
			ScalarBlendingFunction
			(
					dict
			),
			dMin_(readScalar(dict.lookup("dMin"))),
			dMax_(readScalar(dict.lookup("dMax"))){

	if(dict.found("distanceTransitionFunction")){
		dTransition_ = TransitionFunction::New
						(
								dict.subDict("distanceTransitionFunction")
						);
	} else {
		Info << "DistanceScalarBlending: keyword 'distanceTransitionFunction' not found in dictionary, choosing linear." << endl;
		dTransition_ = autoPtr< TransitionFunction >
		(
				new LinearTransition()
		);
	}
}

// * * * * * * * * * * * * * * * * Public member functions  * * * * * * * * * * * * * * //

void DistanceScalarBlending::sTransformation(
		const point & point0,
		const point & point1,
		const scalar & data0,
		const scalar & data1,
		scalar & s
		) const{

	// measure distance:
	point ps0  = (1. - s) * point0 + s * point1;
	scalar ds0 = mag(ps0 - point0);

	// case small distance:
	if(ds0 <= dMin_) {
		s = 0;
		return;
	}

	// case large distance:
	if(ds0 >= dMax_) {
		s = 1;
		return;
	}

	// in between return transition:
	scalar ds = ds0 - dMin_;
	s = dTransition_()(ds / (dMax_ - dMin_) );
}


// ************************************************************************* //

} // End namespace oldev

} // End namespace Foam
