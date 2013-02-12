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

Description
    Collection of basic global functions.

SourceFiles
    Globals.C

References
	[1] J. Schmidt, C. Peralta, B. Stoevesandt, "Automated Generation of
	    Structured Meshes for Wind Energy Applications", Proceedings of the
	    Open Source CFD International Conference, 2012, London, UK

\*---------------------------------------------------------------------------*/

#ifndef GLOBALS_H_
#define GLOBALS_H_

#include "String.h"
#include "BLIB.h"

#include "CoordinateSystem.H"

#include "SquareMatrix.H"
#include "vector.H"
#include "OStringStream.H"
#include "labelList.H"
#include "scalarList.H"
#include "point.H"
#include "mathematicalConstants.H"

namespace Foam{
namespace iwesol{

/// checks if point is inside a box
bool isInsideBox(
		const point & p,
		const point & p_SWL,
		const CoordinateSystem & cooSys,
		const scalarList & dimensions
		);

/// The vector dot product A.B
inline scalar dot(
		const Foam::vector & A,
		const Foam::vector & B
		) { return A & B; }

/// The matrix-vector dot product M.U
inline Foam::vector dot(
		const SquareMatrix<scalar> & M,
		const Foam::vector & U
		);

/// chops a number
inline scalar chop(scalar a);

/// chops a vector
inline Foam::vector & chop(Foam::vector & v);

/// chops a vector
inline Foam::vector chop(const Foam::vector & v);

/// normalizes a vector
inline void normalize(Foam::vector & v) { v /= mag(v); }

/// Returns the key for hash tables
inline word key(label i, label j);

/// Returns the key for hash tables
inline word key(label i, label j, label k);

/// Returns the key for hash tables
inline word key(label i, label j, label k, label l);

/// Returns the key for hash tables
inline word key(const labelList & labels);

/// Returns a key for scalars
word key(scalar height,label imax = 8, scalar resolution = 0.001, const word & fill = "0");

/// returns a key for points
inline word key(const point & p);

/// returns a key for OpenFOAM objects
template< class Type >
inline Foam::string key(const Type & p);

/// returns a list of labels from a key
labelList unkey_l(const word & k,const std::string & seperator = ".");

/// The cosine between two vectors
inline scalar getCos(const Foam::vector & A, const Foam::vector & B);

/// Extract tasks from a dictionary, ie for keyword X_nr store X. Empty keyWord allows all X.
List<dictionary> getDictTasks(const dictionary & dict, const word & keyWord = "");

/// The angle between two vectors
inline scalar getAngleRad(
		const Foam::vector & A,
		const Foam::vector & B
		);

/// returns linear interpolation
template<class T>
List<T> getLinearList(
		const T & A,
		const T & B,
		label innerPoints,
		bool withStart = true,
		bool withEnd = true
		);


///////////////////////////////////////////////////////////
//
//    Inline implementations:
//
///////////////////////////////////////////////////////////

inline scalar chop(scalar a){
	return mag(a) < blib::CHOP ? 0. : a;
}

inline Foam::vector & chop(Foam::vector & v){
	forAll(v,vI){
		v[vI] = chop(v[vI]);
	}
	return v;
}

inline Foam::vector chop(const Foam::vector & v){
	Foam::vector out = v;
	return chop(out);
}

inline Foam::vector dot(
		const SquareMatrix<scalar> & M,
		const Foam::vector & U
		){

	Foam::vector out;

	for(label row = 0; row < 3; row++){
		out[row] = 0;
		for(label col = 0; col < 3; col++){
			out[row] += M[row][col] * U[col];
		}
	}

	return out;
}

inline scalar getCos(const Foam::vector & A, const Foam::vector & B){
	return A & B /(mag(A)*mag(B));
}

inline scalar getAngleRad(
		const Foam::vector & A,
		const Foam::vector & B
		){
	scalar hcos = getCos(A,B);
	if(mag(hcos + 1.) < blib::CHOP) return Foam::constant::mathematical::pi;
	if(mag(hcos - 1.) < blib::CHOP) return 0;
	return Foam::acos(hcos);
}

inline word key(label i, label j){
	return word(blib::String(i) + "." + blib::String(j));
}

inline word key(label i, label j, label k){
	return word(blib::String(i) + "." + blib::String(j) + "." + blib::String(k));
}

inline word key(label i, label j, label k, label l){
	return word(blib::String(i) + "." + blib::String(j) + "." + blib::String(k) + "." + blib::String(l));
}

inline word key(const point & p){
	OStringStream oss;
	oss << "X" << chop(p[0]) << "Y" << chop(p[1]) << "Z" << chop(p[2]);
	return oss.str();
}

template< class Type >
inline Foam::string key(const Type & p){
	OStringStream oss;
	oss << p;
	return oss.str();
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

} /* iwesol */
} /* Foam */

#endif /* GLOBALS_H_ */
