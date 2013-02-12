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

#include "String.h"

#include "Spline.H"
#include "Globals.H"

namespace Foam{
namespace iwesol{

Spline::Spline(){
}

Spline::Spline(
		pointField * globalPoints,
		label vertexA,
		label vertexB,
		const pointField & splinePointsBetween):
	Chain(&((*globalPoints)[vertexA]),&((*globalPoints)[vertexB]),splinePointsBetween),
	globalPoints(globalPoints),
	vA(vertexA),
	vB(vertexB){
}

Spline::Spline(
		pointField * globalPoints,
		label vertexA,
		label vertexB
	):
	Chain(&((*globalPoints)[vertexA]),&((*globalPoints)[vertexB])),
	globalPoints(globalPoints),
	vA(vertexA),
	vB(vertexB){
}

Spline::~Spline() {
}

std::string Spline::dictEntry() const{

	std::string out("");

	if(empty()) return out;

	out += "spline ";
	out += blib::String(getVertexA()) + " "
			+ blib::String(getVertexB());

	out += "\n(";

	for(unsigned int pI = 0; pI < size(); pI++){
		point p = (*this)[pI];
		out += "\n("
				+ blib::String(p.x()) + " "
				+ blib::String(p.y()) + " "
				+ blib::String(p.z()) + ")";
	}

	out += "\n)\n";

	return out;
}

bool Spline::order(const Foam::vector & direction){

	// prepare:
	Foam::vector n(direction);
	normalize(n);
	HashTable<label,scalar> dists(size() - 2);

	// sort inner points:
	for(label i = 1; i < label(size() - 1); i++){
		scalar d = dot(getPoint(i),n);
		dists.set(d,i);
	}
	scalarList newOrder = dists.sortedToc();
	pointField newp(size() - 2);
	forAll(newp,pI){
		newp[pI] = getPoint(dists[newOrder[pI]]);
	}

	// sort first and last point:
	scalar d0 = dot(first(),n);
	scalar d1 = dot(last(),n);
	label v0  = d0 <= d1 ? vA : vB;
	label v1  = v0 == vA ? vB : vA;
	bool out  = v0 == vA ? false : true;

	*this = Spline(globalPoints,v0,v1,newp);

	return out;
}

PointLinePath Spline::getPointLinePath(
		bool withFirst,
		bool withLast
		) const{

	// prepare:
	PointLinePath out;

	// first:
	if(withFirst){
		out.addPoint(getFirstPoint());
	}

	// middle:
	for(label i = 1; i < label(size()) - 1; i++){
		out.addPoint(getPoint(i));
	}

	// last:
	if(withLast){
		out.addPoint(getLastPoint());
	}

	return out;
}

scalarList Spline::getPointSList(
		bool withFirst,
		bool withLast
		) const{
	PointLinePath path = getPointLinePath(withFirst,withLast);
	scalarList out(path.size());
	forAll(out,sI){
		out[sI] = path.getPointS(sI);
	}
	return out;
}

void Spline::mixWithLinear(bool atFirst,label mixSize){

	// check:
	if(mixSize < 0){
		mixWithLinear(atFirst,size());
		return;
	}

	// prepare:
	label imax = mixSize - 1;
	point p0   = atFirst ? getFirstPoint() : getLastPoint();
	point p1   = atFirst ? getPoint(imax)  : getPoint(size() - 1 - imax);
	for(label i = 1; i < imax; i++){
		scalar j    = atFirst ? i : size() - 1. - i;
		point p_lin = p0 + (p1 - p0) * i / imax;
		getPoint(j) = (i * getPoint(j) + (imax - i) * p_lin) / scalar(imax);
	}
}

} /* iwesol */
} /* Foam */
