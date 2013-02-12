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

Class
    Foam::iwesol::CoordinateSystem

Description
    See below.

SourceFiles
    CoordinateSystem.C

References
	[1] J. Schmidt, C. Peralta, B. Stoevesandt, "Automated Generation of
	    Structured Meshes for Wind Energy Applications", Proceedings of the
	    Open Source CFD International Conference, 2012, London, UK

\*---------------------------------------------------------------------------*/

#ifndef COORDINATESYSTEM_H_
#define COORDINATESYSTEM_H_

#include "String.h"

#include "SquareMatrix.H"
#include "vector.H"
#include "point.H"
#include "dictionary.H"

namespace Foam{
namespace iwesol{

/**
 * @class Foam::iwesol::CoordinateSystem.H
 * @brief This class represents the basic coordinate system.
 *
 */
class CoordinateSystem {

public:

	/// Returns the rotational matrix
	static SquareMatrix<scalar> getRotationalMatrix(scalar alphaRad, const Foam::vector & axis);

	/// rotates a vector around an axis
	static void rotate(Foam::vector & v, const scalar & angleRad, const Foam::vector & axis);

	/// Constructor
	CoordinateSystem(const dictionary & dict);

	/// Constructor
	CoordinateSystem();

	/// Constructor
	CoordinateSystem(
			const Foam::vector & origin,
			const Foam::List<Foam::vector> & nList
			);

	/// Constructor
	CoordinateSystem(
			const Foam::vector & origin,
			const Foam::vector & n0,
			const Foam::vector & n1,
			const Foam::vector & n2
			);

	/// Constructor
	CoordinateSystem(
			const Foam::vector & n0,
			const Foam::vector & n1,
			const Foam::vector & n2
			);

	/// Destructor
	virtual ~CoordinateSystem();

	/// Returns the origin
	inline const point & origin() const { return o; }

	/// Returns a base vector
	inline const Foam::vector & e(label i) const { return nList[i]; }

	/// Returns a base vector
	inline const Foam::List<Foam::vector> & axes() const { return nList; }

	/// clears registrations
	inline void clear() { regCoo.clear(); }

	/// prints the coordinate axes
	void print() const;

	/// Register a coordinate
	void regCoordinate(point & coord);

	/// Register coordinates
	inline void regCoordinates(const List<point*> & coords);

	/// translates all points, ie, shifts origin
	void translate(const Foam::vector & deltav);

	/// Perform a rotation in 3D by angle alpha around given axis
	void rotate(scalar alpha, const Foam::vector & axis);

	/// rotate around a non-origin center
	void rotate(scalar alpha, const Foam::vector & axis, const point & p_center);

	/// transforms coordinate into absolute position
	inline point coord2point(const point & coord) const { return o + coord[0] * e(0) + coord[1] * e(1) + coord[2] * e(2); }

	/// transforms point to coordinate TODO SO FAR ONLY ORTHOGONAL BASE VECTORS ALLOWED!
	inline point point2coord(const point & point) const;


private:

	/// The origin
	point o;

	/// The base vectors
	Foam::List<Foam::vector> nList;

	/// The registered points, affected by rotations and translations
	Foam::HashTable<point*> regCoo;

	/// key for the hash tables
	word key(const point & coo);

};

inline point CoordinateSystem::point2coord(const point & point) const{
	return Foam::vector(
			dot(point - o,e(0)),
			dot(point - o,e(1)),
			dot(point - o,e(2))
	);
} 

inline void CoordinateSystem::regCoordinates(const List<point*> & coords)
{
	if(coords.size() > 0){
		forAll(coords,cI){
			regCoordinate(*(coords[cI]));
		}
	}
}


inline word CoordinateSystem::key(const point & coo){
	return blib::String(&coo);
}

inline void CoordinateSystem::regCoordinate(point & coord){
	if(!regCoo.found(key(coord))){
		regCoo.set(key(coord),&coord);
	}
}

} /* iwesol */
} /* Foam */

#endif /* COORDINATESYSTEM_H_ */
