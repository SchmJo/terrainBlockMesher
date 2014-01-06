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

#include "STLProjecting.H"
#include "Globals.H"

namespace Foam
{

namespace oldev
{


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

STLProjecting::STLProjecting():
	stl_(0){
}

STLProjecting::STLProjecting
(
		searchableSurface const * stl
):
	stl_(stl){
}

// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

STLProjecting::~STLProjecting() {
}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //


bool STLProjecting::attachPoint(point & p, const point & p_projTo) const{

	// prepare:
	point p_stl(0,0,0);

	// project:
	if(stl_ != 0){
		if(!getSTLHit(stl_,p,p_projTo,p_stl)){
			return false;
		}
	} else {
		Foam::vector n = (p - p_projTo) / mag(p - p_projTo);
		p_stl = p - (p & n) * n;
	}


	// success:
	p = p_stl;
	return true;

}

bool STLProjecting::projectPoint(point & p, const Foam::vector & dir_proj, scalar maxDist) const{

	// prepare:
	point p1 = p;
	point p2 = p + dir_proj;

	// projection loop:
	while(dist(p,p2) <= maxDist && !attachPoint(p1,p2)){
		p1  = p2;
		p2 += dir_proj;
	}

	// success:
	if(dist(p,p2) <= maxDist){
		p = p1;
		return true;
	}

	return false;

}


// ************************************************************************* //

} // End namespace oldev

} // End namespace Foam
