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

#include "Chain.H"

namespace Foam{
namespace iwesol{

Chain::Chain():
		blib::PartlyPointedList<point>(){
}

Chain::Chain(
		const pointField & points
		):
		blib::PartlyPointedList<point>(),
		dirichletFlags(points.size(),false){
	forAll(points,pI){
		append(points[pI]);
	}
}

Chain::Chain(
		Chain * c
		):
		blib::PartlyPointedList<point>(),
		dirichletFlags(c->size(),false){
	for(label pI = 0; pI < label(c->size()); pI++){
		append(&c->getPoint(pI));
	}
}

Chain::Chain(
		const List<point*> & points
		):
		blib::PartlyPointedList<point>(),
		dirichletFlags(points.size(),false){
	forAll(points,pI){
		append(points[pI]);
	}
}

Chain::Chain(
		point * first,
		point * last,
		const pointField & inBetween
		):
		blib::PartlyPointedList<point>(first),
		dirichletFlags(inBetween.size() + 2,false){

	forAll(inBetween,pI){
		append(inBetween[pI]);
	}
	append(last);
}

Chain::Chain(
		point * first,
		point * last,
		label pointsInBetween
		):
		blib::PartlyPointedList<point>(first),
		dirichletFlags(pointsInBetween + 2,false){

	point delta = (*last - *first) / (pointsInBetween + 1);
	point p     = *first;
	for(label i = 0; i < pointsInBetween; i++){
		p += delta;
		append(p);
	}
	append(last);
}

Chain::~Chain() {
}

pointField Chain::getPoints() const{
	pointField out(size());
	forAll(out,pI){
		out[pI] = (*this)[pI];
	}
	return out;
}

pointField Chain::getInnerPoints() const{
	pointField out(sizeLocal());
	forAll(out,pI){
		out[pI] = getElementsLocal()[pI];
	}
	return out;
}

List<point*> Chain::getPointers(){
	List<point*> out(size());
	forAll(out,pI){
		out[pI] = elements[pI];
	}
	return out;
}

void Chain::print() const{
	for(label i = 0; i < label(size()); i++){
		Info << getPoint(i) << endl;
	}
}

} /* iwesol */
} /* Foam */
