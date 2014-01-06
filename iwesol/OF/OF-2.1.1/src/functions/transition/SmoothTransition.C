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

#include "SmoothTransition.H"
#include "addToRunTimeSelectionTable.H"

namespace Foam
{

namespace oldev
{

defineTypeNameAndDebug(SmoothTransition, 0);
addToRunTimeSelectionTable(TransitionFunction,SmoothTransition,type);
addToRunTimeSelectionTable(TransitionFunction,SmoothTransition,dict);


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

SmoothTransition::SmoothTransition(){
	pointField pts(3);
	pts[0] = point(0.01,0,0);
	pts[1] = point(0.5,0.5,0);
	pts[2] = point(0.99,1,0);
	setControlPoints(pts,4);
}

SmoothTransition::SmoothTransition(
			const dictionary & dict
			){
	pointField pts(3);
	pts[0] = point(0.01,0,0);
	pts[1] = point(0.5,0.5,0);
	pts[2] = point(0.99,1,0);
	if(dict.found("lowControlPoint")){
		pts[0] = point(dict.lookup("lowControlPoint"));
	} else {
		Info << "SmoothTransition: keyword 'lowControlPoint' not found, choosing " << pts[0] << endl;
	}
	if(dict.found("centreControlPoint")){
		pts[1] = point(dict.lookup("centreControlPoint"));
	} else {
		Info << "SmoothTransition: keyword 'centreControlPoint' not found, choosing " << pts[1] << endl;
	}
	if(dict.found("highControlPoint")){
		pts[2] = point(dict.lookup("highControlPoint"));
	} else {
		Info << "SmoothTransition: keyword 'highControlPoint' not found, choosing " << pts[2] << endl;
	}
	setControlPoints(pts,4);
}


// ************************************************************************* //

} // End namespace oldev

} // End namespace Foam
