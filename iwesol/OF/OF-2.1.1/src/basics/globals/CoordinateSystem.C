/**
 * @file CoordinateSystem.C
 * @brief This file defines/implements the class CoordinateSystem.
 *
 */

#include "CoordinateSystem.H"
#include "Globals.H"

namespace Foam {

namespace oldev {

void CoordinateSystem::rotate(
		Foam::vector & v,
		const scalar & angle,
		const Foam::vector & axis){
	Foam::SquareMatrix<scalar> M  = getRotationalMatrix(angle,axis);
	v = dot(M,v);
}

CoordinateSystem::CoordinateSystem(const dictionary & dict) {

	o     = Foam::vector(dict.lookup("origin"));
	nList = Foam::List<Foam::vector>(dict.lookup("baseVectors"));
	for(label i = 0; i < label(nList.size()); i++)
		normalize(nList[i]);
}

CoordinateSystem::CoordinateSystem(){
	o        = Foam::vector(0,0,0);
	nList    = Foam::List<Foam::vector>(3);
	nList[0] = Foam::vector(1,0,0);
	nList[1] = Foam::vector(0,1,0);
	nList[2] = Foam::vector(0,0,1);
}

CoordinateSystem::CoordinateSystem(
		const Foam::vector & origin,
		const Foam::List<Foam::vector> & nList
		):
	o(origin),
	nList(nList){
	for(label i = 0; i < label(nList.size()); i++)
		normalize(this->nList[i]);
}

CoordinateSystem::CoordinateSystem(
		const Foam::vector & n0,
		const Foam::vector & n1,
		const Foam::vector & n2
		):
	o(0,0,0),
	nList(3){
	nList[0] = n0;
	nList[1] = n1;
	nList[2] = n2;
	for(label i = 0; i < label(nList.size()); i++)
		normalize(nList[i]);
}

CoordinateSystem::CoordinateSystem(
		const Foam::vector & origin,
		const Foam::vector & n0,
		const Foam::vector & n1,
		const Foam::vector & n2
		):
	o(origin),
	nList(3){
	nList[0] = n0;
	nList[1] = n1;
	nList[2] = n2;
	for(label i = 0; i < label(nList.size()); i++)
		normalize(nList[i]);
}

CoordinateSystem::~CoordinateSystem() {
}

// compare http://en.wikipedia.org/wiki/Orthogonal_matrix
Foam::SquareMatrix<scalar> CoordinateSystem::getRotationalMatrix(
		scalar alpha, const Foam::vector & axis){

	// prepare:
	Foam::vector n(axis);
	normalize(n);
	Foam::SquareMatrix<scalar> out(3);
	scalar s  = Foam::sin(alpha / 2.);
	scalar c  = Foam::cos(alpha / 2.);
	scalar s2 = s * s;
	scalar sc = s * c;
	scalar x  = n[0];
	scalar y  = n[1];
	scalar z  = n[2];
	scalar x2 = x * x;
	scalar y2 = y * y;
	scalar z2 = z * z;

	out[0][0] = 1. - 2. * s2 + 2. * x2 * s2;
	out[0][1] = 2. * x * y * s2 - 2. * z * sc;
	out[0][2] = 2. * x * z * s2 + 2. * y * sc;

	out[1][0] = 2. * x * y * s2 + 2. * z * sc;
	out[1][1] = 1. - 2. * s2 + 2. * y2 * s2;
	out[1][2] = 2. * y * z * s2 - 2. * x * sc;

	out[2][0] = 2. * x * z * s2 - 2. * y * sc;
	out[2][1] = 2. * y * z * s2 + 2. * x * sc;
	out[2][2] = 1. - 2. * s2 + 2. * z2 * s2;

	return out;
}

void CoordinateSystem::translate(const Foam::vector & deltav){

	// translate origin:
	o += deltav;

	// translate registered points:
	wordList t = regCoo.toc();
	forAll(t,pI){
		*regCoo[t[pI]] -= deltav;
	}

}

void CoordinateSystem::rotate(scalar alpha, const Foam::vector & axis){

	// get rotational matrix:
	Foam::SquareMatrix<scalar> M  = getRotationalMatrix(alpha,axis);

	// perform rotation of coordinate axes:
	forAll(nList,eI){
		nList[eI] = dot(M,nList[eI]);
	}

	// perform rotation of registered coordinates:
	if(regCoo.size() > 0){
		Foam::SquareMatrix<scalar> mM = M.T();
		wordList t = regCoo.toc();
		forAll(t,pI){
			*regCoo[t[pI]] = dot(mM,*regCoo[t[pI]]);
		}
	}


}

void CoordinateSystem::rotate(
		scalar alpha, const Foam::vector & axis, const point & p_center){

	// prepare:
	point oo = o;

	// translate origin to center:
	translate(p_center - o);

	// rotate:
	rotate(alpha,axis);

	// translate back:
	translate(oo - o);
}

void CoordinateSystem::print() const{
	forAll(nList,eI){
		Info << "coordinate axis " << eI << ": " << nList[eI] << endl;
	}
	if(regCoo.size() > 0){
		wordList t = regCoo.toc();
		forAll(t,pI){
			Info << "Registered coordinate " << pI << ": " << jlib::String(regCoo[t[pI]])
			<< ", coord = " << *(regCoo[t[pI]]) << ", point = " << coord2point(*(regCoo[t[pI]])) << endl;
		}
	}
}

}

}
