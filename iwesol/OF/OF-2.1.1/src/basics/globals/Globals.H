/**
 * @file Globals.H
 * @brief This file defines/implements the class Globals.
 *
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_

#include "JString.h"

#include "fvMesh.H"
#include "SquareMatrix.H"
#include "interpolationPointMVC.H"
#include "searchableSurface.H"
#include "CoordinateSystem.H"
#include "pointFields.H"
#include "Pair.H"

namespace Foam{

namespace oldev{

/// checks if point is inside a box
bool isInsideBox(
		const point & p,
		const point & p_SWL,
		const CoordinateSystem & cooSys,
		const scalarList & dimensions
		);

/// resets the seed
void reseed();

// return a uniform number in [0,1].
inline scalar urand() { return rand() / double(RAND_MAX); }

/// returns random normalized vector
point randomDirection();

/// chop limit
const scalar CHOP = 0.00000001;

/// return min and max of a list
template< class HList >
inline HList minMax(const HList & list);

/// return min and max of a list
template< class HList >
inline labelList minMaxPos(const HList & list);

/// return permutations of list entries
labelListList permutations(const labelList & list);

/// return permutations of listlist[A] to listlist[B]
labelListList mappings(const labelListList & listlist);

/// return permutations of listlist[A] to listlist[B]
labelListList indexMappings(const labelListList & listlist);

/// chops a number
scalar chop(scalar a);

/// chops a vector
Foam::vector & chop(Foam::vector & v);

/// chops a vector
Foam::vector chop(const Foam::vector & v);

/// Modulo for scalars
scalar mod(scalar a, scalar b);

/// The vector cross product
Foam::vector cross(const Foam::vector & A, const Foam::vector & B);

/// normalizes a vector
void normalize(Foam::vector & v);

/// Vector norm
inline scalar norm(const Foam::vector & U) { return mag(U); }

/// Square of vector norm
scalar normsqr(const Foam::vector & U);

/// The dist between two vectors
scalar dist(const Foam::vector & A, const Foam::vector & B);

/// The square of the dist between two vectors
scalar distsqr(const Foam::vector & A, const Foam::vector & B);

/// The vector dot product A.B
inline scalar dot(const Foam::vector & A, const Foam::vector & B) { return A & B; }

/// The matrix-vector dot product M.U
Foam::vector dot(const SquareMatrix<scalar> & M, const Foam::vector & U);

/// The cosine between two vectors
inline scalar getCos(const Foam::vector & A, const Foam::vector & B);

/// converts list to pair
template< class Type >
inline Pair< Type > list2Type(const List< Type > & list);

/// converts list to pair
inline Pair< bool > list2Type(const List< bool > & list) { return list2Type< bool >(list); }

/// converts list to pair
inline Pair< label > list2Type(const List< label > & list) { return list2Type< label >(list); }

/// converts list to pair
inline Pair< scalar > list2Type(const List< scalar > & list) { return list2Type< scalar >(list); }

/// converts list to pair
inline Pair< word > list2Type(const List< word > & list) { return list2Type< word >(list); }

/// converts list to pair
inline Pair< Foam::vector > list2Type(const List< Foam::vector > & list) { return list2Type< Foam::vector >(list); }

/// converts pair to list
template< class Type >
inline List< Type > pair2list(const Pair< Type > & p);

/// converts pair to list
inline List< bool > pair2list(const Pair< bool > & p) { return pair2list< bool >(p); }

/// converts pair to list
inline List< label > pair2list(const Pair< label > & p) { return pair2list< label >(p); }

/// converts pair to list
inline List< scalar > pair2list(const Pair< scalar > & p) { return pair2list< scalar >(p); }

/// converts pair to list
inline List< word > pair2list(const Pair< word > & p) { return pair2list< word >(p); }

/// converts pair to list
inline List< Foam::vector > pair2list(const Pair< Foam::vector > & p) { return pair2list< Foam::vector >(p); }

/// The angle between two vectors
inline scalar getAngleRad(const Foam::vector & A, const Foam::vector & B){ return Foam::acos(getCos(A,B)); }

/// rotates a vector by omega, using cross product.
void rotateVector(Foam::vector & v, const Foam::vector & omega, scalar resolution = 0.0001);

/// get the surface point on an stl between two points (hit point). Returns success.
bool getSTLHit(searchableSurface const * stl,const point & p_start, const point & p_end, point & surfacePoint);

/// Calculate orthonormal system, starting from nlong parallel to U
void calcOrthonormalSystem(
		const Foam::vector & U,
		Foam::vector & nlong,
		Foam::vector & nside,
		Foam::vector & nup);

/// Calculate orthonormal system, starting from nlong parallel to U
CoordinateSystem  calcOrthonormalSystem(
		const Foam::vector & U,
		const point & origin = point(0,0,0)
		);

/// Interpolates a volField at point p
template <class T>
T interpolate_MVC(
		const fvMesh & mesh,
		const GeometricField<T, fvPatchField, volMesh> & k,
		const Foam::vector & p
		);

/// Interpolates a volField at point p
template <class T>
T interpolate_MVC(
		const fvMesh & mesh,
		const interpolationPointMVC< T > & iMVC,
		const Foam::vector & p
		);

/// returns a face with three corners
inline face createFace(label p0, label p1, label p2);

/// returns a face with four corners
inline face createFace(label p0, label p1, label p2, label p3);

/// Returns the key for hash tables
word key(label i, label j);

/// Returns the key for hash tables
word key(label i, label j, label k);

/// Returns the key for hash tables
word key(label i, label j, label k, label l);

/// Returns the key for hash tables
word key(const labelList & labels);

/// Returns a key for scalars
word key(scalar height,label imax = 8, scalar resolution = 0.001, const word & fill = "0");

/// returns a key for points
inline word key(const point & p);

/// returns a key for OpenFOAM objects
template< class Type >
inline Foam::string key(const Type & p);

/// returns a list of labels from a key
labelList unkey_l(const word & k,const std::string & seperator = ".");

/// returns a list of scalars from a key
scalarList unkey_s(const word & k,const std::string & seperator = ".");

/// Extract tasks from a dictionary, ie for keyword X_nr store X. Empty keyWord allows all X.
List<dictionary> getDictTasks(const dictionary & dict, const word & keyWord = "");

/// returns global edge number that corresponds to local edge number of a patch
label patchEdgeToGlobalEdge(const polyPatch & patch, label patchEdgeI);

/// set Time object to closest time. returns success.
bool setClosestTime(Time & runTime, const scalar & goalTime);

/// returns linear interpolation
template<class T>
List<T> getLinearList(
		const T & A,
		const T & B,
		label innerPoints,
		bool withStart = true,
		bool withEnd = true
		);

/// calculate a volume field from a point field
volScalarField interpolatePointToVolScalar(
		const fvMesh & fvMesh_,
		const pointScalarField & p,
		const IOobject & io
		);

/// calculate a volume field from a point field
volVectorField interpolatePointToVolVector(
		const fvMesh & fvMesh_,
		const pointVectorField & p,
		const IOobject & io
		);

/// checks if a field is a point field
template< class Field >
inline bool isPointField(const polyMesh & mesh, const Field & field);

/// grabs an object from registry. returns success.
template< class Obj >
bool grabFromRegistry(const objectRegistry & reg, const word & name, Obj * out);

/// calculates boundary of a mesh
inline void calcMeshMinMax(
		const polyMesh & mesh,
		Foam::vector & min,
		Foam::vector & max
		);

// checks if point is inside bounds
inline bool pointInsideBox(
		const point & p,
		const Foam::vector & min,
		const Foam::vector & max
		);

inline scalar chop(scalar a){
	return mag(a) < CHOP ? 0. : a;
}

inline Foam::vector & chop(Foam::vector & v){
	forAll(v,vI){
		v[vI] = chop(v[vI]);
	}
	return v;
}

inline void normalize(Foam::vector & v){
	v /= norm(v);
}

inline Foam::vector cross(const Foam::vector & A, const Foam::vector & B){
	return Foam::vector(A[1]*B[2]-A[2]*B[1],A[2]*B[0]-A[0]*B[2],A[0]*B[1]-A[1]*B[0]);
}


inline Foam::vector chop(const Foam::vector & v){
	Foam::vector out = v;
	return chop(out);
}

inline face createFace(label p0, label p1, label p2){
	labelList pList(3);
	pList[0] = p0;
	pList[1] = p1;
	pList[2] = p2;
	return face(pList);
}

inline face createFace(label p0, label p1, label p2, label p3){
	labelList pList(4);
	pList[0] = p0;
	pList[1] = p1;
	pList[2] = p2;
	pList[3] = p3;
	return face(pList);
}

inline scalar dist(const Foam::vector & A, const Foam::vector & B){
	return norm(A - B);
}

inline scalar distsqr(const Foam::vector & A, const Foam::vector & B){
	return normsqr(A - B);
}

inline scalar normsqr(const Foam::vector & U){
	return dot(U,U);
}

inline scalar getCos(const Foam::vector & A, const Foam::vector & B){
	return A & B /(mag(A)*mag(B));
}

inline word key(label i, label j){
	return word(jlib::String(i) + "." + jlib::String(j));
}

inline word key(label i, label j, label k){
	return word(jlib::String(i) + "." + jlib::String(j) + "." + jlib::String(k));
}

inline word key(label i, label j, label k, label l){
	return word(jlib::String(i) + "." + jlib::String(j) + "." + jlib::String(k) + "." + jlib::String(l));
}

inline word key(const point & p){
	Foam::OStringStream oss;
	oss << "X" << chop(p[0]) << "Y" << chop(p[1]) << "Z" << chop(p[2]);
	return oss.str();
}

inline bool pointInsideBox(
		const point & p,
		const Foam::vector & min,
		const Foam::vector & max
		){
    bool outside = false;
    for(label dir = 0; dir < 3; dir++){
    	if(    p[dir] < min[dir]
    	    || p[dir] > max[dir]){
    		outside = true;
    		break;
    	}
    }
    return !outside;
}

inline void calcMeshMinMax(
		const polyMesh & mesh,
		Foam::vector & min,
		Foam::vector & max
		){
	forAll(mesh.boundaryMesh(),patchI){
		const polyPatch & patch = mesh.boundaryMesh()[patchI];
		forAll(patch,fI){
			const face & f = patch[fI];
			forAll(f,pI){
				const point & p = mesh.points()[f[pI]];
	    		for(label dir = 0; dir < 3; dir++){
	    			if((patchI == 0 && fI == 0 && pI == 0) || p[dir] < min[dir]) min[dir] = p[dir];
	    			if((patchI == 0 && fI == 0 && pI == 0) || p[dir] > max[dir]) max[dir] = p[dir];
	    		}
			}
		}
	}
}

template< class Type >
inline Foam::string key(const Type & p){
	Foam::OStringStream oss;
	oss << p;
	return oss.str();
}

inline scalar mod(scalar a, scalar b){
	scalar out = chop(a);
	while(out >= b) out = chop(out - b);
	while(out < 0.) out = chop(out + b);
	return out;
}

template <class T>
List<T> getLinearList(
		const T & A,
		const T & B,
		label innerPoints,
		bool withStart,
		bool withEnd
		){

	// prepare:
	List<T> out(innerPoints + (withStart ? 1 : 0) + (withEnd ? 1 : 0));
	label counter = 0;

	// first point:
	if(withStart){
		out[0] = A;
		counter++;
	}

	// inner points:
	label jmax = innerPoints + 1;
	for(label i = 0; i < innerPoints; i++){
		label j = i + 1;
		out[counter] = ( (jmax - j) * A + j * B ) / jmax;
		counter++;
	}

	// last point:
	if(withEnd){
		out[counter] = B;
	}

	return out;
}

template <class T>
T interpolate_MVC(
		const fvMesh & mesh,
		const GeometricField<T, fvPatchField, volMesh> & k,
		const Foam::vector & p
		){
	interpolationPointMVC< T > iMVC(k);
	return interpolate_MVC(mesh,iMVC,p);
}

template <class T>
T interpolate_MVC(
		const fvMesh & mesh,
		const interpolationPointMVC< T > & iMVC,
		const Foam::vector & p
		){
	label probeCell = mesh.findCell(p);
	return iMVC.interpolate(p,probeCell);
}

template< class Field >
inline bool isPointField(const polyMesh & mesh, const Field & field){
	return field.size() == mesh.points().size();
}

template< class HList >
inline HList minMax(const HList & l){
	HList out(2);
	forAll(l,lI){
		if(lI == 0 || l[lI] < out[0]) out[0] = l[lI];
		if(lI == 0 || l[lI] > out[1]) out[1] = l[lI];
	}
	return out;
}

template< class HList >
inline labelList minMaxPos(const HList & l){
	labelList out(2);
	HList minMax(2);
	forAll(l,lI){
		if(lI == 0 || l[lI] < minMax[0]) { out[0] = lI; minMax[0] = l[lI]; }
		if(lI == 0 || l[lI] > minMax[1]) { out[1] = lI; minMax[1] = l[lI]; }
	}
	return out;
}

template< class Type >
inline Pair< Type > list2Type(const List< Type > & list){
	return Pair< Type >
	(
			list[0],
			list[1]
	);
}

template< class Type >
inline List< Type > pair2list(const Pair< Type > & p){
	List< Type > out(2);
	out[0] = p.first();
	out[1] = p.second();
	return out;
}

}
}


#endif /* GLOBALS_H_ */
