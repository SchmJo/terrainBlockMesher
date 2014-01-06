/**
 * @file CoordinateSystem.H
 * @brief This file defines/implements the class CoordinateSystem.
 *
 */

#ifndef COORDINATESYSTEM_H_
#define COORDINATESYSTEM_H_

#include "JString.h"

#include "fvMesh.H"
#include "SquareMatrix.H"

namespace Foam {

namespace oldev {

/**
 * @class CoordinateSystem.H
 * @brief This class represents the basic coordinate system.
 *
 */
class CoordinateSystem {

public:

	/// Returns the rotational matrix
	static Foam::SquareMatrix<scalar> getRotationalMatrix(scalar alphaRad, const Foam::vector & axis);

	/// rotates a vector around an axis
	static void rotate(Foam::vector & v, const scalar & angleRad, const Foam::vector & axis);

	/// Constructor
	CoordinateSystem(const dictionary & dict);

	/// Constructor
	CoordinateSystem();

	/// Constructor
	CoordinateSystem(
			const Foam::vector & origin,
			const Foam::List< Foam::vector > & nList
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
	void clear() { regCoo.clear(); }

	/// prints the coordinate axes
	void print() const;

	/// Register a coordinate
	void regCoordinate(point & coord);

	/// Register coordinates
	inline void regCoordinates(const List<point*> & coords);
//	inline void regCoordinates(const List<point> & coords);



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

	/// transforms coordinate into absolute position
	inline point coord2pointV(const point & coord) const { return coord[0] * e(0) + coord[1] * e(1) + coord[2] * e(2); }


	/// transforms point to coordinate TODO SO FAR ONLY ORTHOGONAL BASE VECTORS ALLOWED!
	inline point point2coordV(const point & point) const;


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
			(point - o) & e(0),
			(point - o) & e(1),
			(point - o) & e(2)
	);
} 

inline point CoordinateSystem::point2coordV(const point & point) const{
	return Foam::vector(
			point & e(0),
			point & e(1),
			point & e(2)
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


//inline void CoordinateSystem::regCoordinates(const List<point> & coords)
//{
//	if(coords.size() > 0){
//		forAll(coords,cI){
//			regCoordinate( (coords[cI]) ) ;
//		}
//	}
//}





inline word CoordinateSystem::key(const point & coo){
	return jlib::String(&coo);
}

inline void CoordinateSystem::regCoordinate(point & coord){
	if(!regCoo.found(key(coord))){
		regCoo.set(key(coord),&coord);
	}
}

}

}

#endif /* COORDINATESYSTEM_H_ */
