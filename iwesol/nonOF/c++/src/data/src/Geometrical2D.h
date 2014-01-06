/**
 * @file Geometrical2D.h
 * @class Geometrical2D
 * @brief An interface for geometric objects.
 *
 * Geometric objects may transform under rotations, translations
 * and scalings.
 *
 */

#ifndef GEOMETRICAL2D_H_
#define GEOMETRICAL2D_H_

#include <cmath>

namespace jlib{

class Geometrical2D{

public:

	/// Constructor.
	Geometrical2D(){}

	virtual ~Geometrical2D(){}

	/// Rotation by an angle in radiant.
	virtual void rotate(double angleRad){
		rotate(std::cos(angleRad),std::sin(angleRad));
	}

	/// Rotation by an cos/sin of angle.
	virtual void rotate(double cosval, double sinval) = 0;

	/// Cartesian translation.
	virtual void translate(double delta_x,double delta_y) = 0;

	/// Rescaling by a factor.
	virtual void rescale(double factor) = 0;

};

}

#endif /* GEOMETRICAL2D_H_ */
