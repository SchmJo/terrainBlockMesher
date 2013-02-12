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

#include "ConvexPolygon.H"
#include "Globals.H"

namespace Foam{
namespace iwesol{

ConvexPolygon::ConvexPolygon(
		const pointField & points,
		const point & insidePoint,
		scalar height,
		const word & addType,
		scalar resolution
		):
	GroundObject(addType,resolution),
	points(points),
	p_inside(insidePoint),
	height(height){

	init();
}

ConvexPolygon::ConvexPolygon(
		const dictionary & dict
		){

	points	    = pointField(dict.lookup("points"));
	p_inside    = point(dict.lookup("p_inside"));
	height      = readScalar(dict.lookup("height"));
	addType     = word(dict.lookup("addType"));
	resolution  = readScalar(dict.lookup("resolution"));

	init();
}

ConvexPolygon::~ConvexPolygon() {
}

void ConvexPolygon::init(){

	p_inside[2] = 0;

	forAll(points,pI){

		point & q = points[pI];
		q[2]      = 0;
		scalar d  = mag(q - p_inside);

		if(pI == 0 || d > maxDist) maxDist = d;

	}

}

bool ConvexPolygon::isInside(const point & p){
	Info << "ConvexPolygon" << endl;
	Info << points << endl;
	point q = p;
	q[2]    = 0;
	if(mag(q - p_inside) > maxDist + resolution) return false;

	forAll(points, pI){
		label i0 = pI == 0 ? points.size() - 1 : pI - 1;
		if(!onOneSide(p,i0,pI)) return false;
	}

	return true;
}

bool ConvexPolygon::onOneSide(const point & p,label iA, label iB) const{

	// prepare:
	Foam::vector q  = points[iB] - points[iA];
	Foam::vector c0 = q ^ p_inside;

	// check line to p:
	Foam::vector delta = p - p_inside;
	delta[2]           = 0;
	scalar steps       = mag(delta) / resolution + 1;
	delta             /= steps + 1;
	for(label i = 1; i < steps; i++){

		point p_check  = p_inside + i * delta;
		Foam::vector c = q ^ p_check;
		if(sign(c[2]) != sign(c0[2])) return false;

	}
	return true;
}

} /* iwesol */
} /* Foam */
