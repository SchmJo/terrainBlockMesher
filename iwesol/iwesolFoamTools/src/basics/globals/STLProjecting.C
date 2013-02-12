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

namespace Foam{
namespace iwesol{

bool STLProjecting::getSTLHit(
		searchableSurface const * stl,
		const point & p_start,
		const point & p_end,
		point & surfacePoint
		){

	// hit a line through the surface:
	List< pointIndexHit > hitList;
	pointField start(1,p_start);
	pointField end(1,p_end);
	stl->findLine(start,end,hitList);

	// check hit:
	if(!hitList[0].hit()){
		return false;
	}

	surfacePoint = hitList[0].hitPoint();
	return true;
}

STLProjecting::STLProjecting():
	stl(0){
}

STLProjecting::STLProjecting(searchableSurface const * stl):
	stl(stl){
}

STLProjecting::~STLProjecting() {
}

bool STLProjecting::attachPoint(point & p, const point & p_projTo){

	// prepare:
	point p_stl(0,0,0);

	// project:
	if(stl != 0){
		if(!getSTLHit(stl,p,p_projTo,p_stl)){
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

bool STLProjecting::projectPoint(point & p, const Foam::vector & dir_proj, scalar maxDist){

	// prepare:
	point p1 = p;
	point p2 = p + dir_proj;

	// projection loop:
	while(mag(p - p2) <= maxDist && !attachPoint(p1,p2)){
		p1  = p2;
		p2 += dir_proj;
	}

	// success:
	if(mag(p - p2) <= maxDist){
		p = p1;
		return true;
	}

	return false;

}

} /* iwesol */
} /* Foam */
