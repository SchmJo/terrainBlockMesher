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

#include "ArcTanTransition.H"
#include "addToRunTimeSelectionTable.H"
#include "mathematicalConstants.H"

namespace Foam
{

namespace oldev
{

defineTypeNameAndDebug(ArcTanTransition, 0);
addToRunTimeSelectionTable(TransitionFunction,ArcTanTransition,type);
addToRunTimeSelectionTable(TransitionFunction,ArcTanTransition,dict);


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

ArcTanTransition::ArcTanTransition():
		s0_(0.5),
		argMult_(30),
		coreLow_(0.3),
		coreHigh_(0.7){
}

ArcTanTransition::ArcTanTransition(
			const dictionary & dict
			):
			s0_(readScalar(dict.lookup("s0"))),
			argMult_(readScalar(dict.lookup("argumentMultiplicator"))),
			coreLow_(readScalar(dict.lookup("sCoreLow"))),
			coreHigh_(readScalar(dict.lookup("sCoreHigh"))){
}

// * * * * * * * * * * * * * * * * Public member functions  * * * * * * * * * * * * * * //

scalar ArcTanTransition::operator()(const scalar & s) const{

	// trivials:
	if(s <= 0) return 0;
	if(s >= 1) return 1;

	// pure:
	scalar pure = 0.5 + Foam::atan( argMult_ * (s - s0_ ) ) / Foam::constant::mathematical::pi;
	if(s >= coreLow_ && s <= coreHigh_) return pure;

	// prepare:
	scalar lin  = 0;
	scalar cLin = 0;

	// low:
	if(s < coreLow_){
		scalar pLow = 0.5 + Foam::atan( argMult_ * (coreLow_ - s0_ ) ) / Foam::constant::mathematical::pi;
		cLin = (coreLow_ - s) / coreLow_;
		lin  = (1. - cLin) * pLow;
	}

	// high:
	else {
		scalar pHigh = 0.5 + Foam::atan( argMult_ * (coreHigh_ - s0_ ) ) / Foam::constant::mathematical::pi;
		cLin = (s - coreHigh_) / (1. - coreHigh_);
		lin  = pHigh + cLin * (1. - pHigh);
	}

	return cLin * lin + (1. - cLin) * pure;
}


// ************************************************************************* //

} // End namespace oldev

} // End namespace Foam
