/**
 * @file VectorLike2D.h
 * @class VectorLike2D
 * @brief An interface for geometrical vectors in 2D.
 *
 * Vectors transform under rotations, translations and dilations.
 */

#ifndef VECTORLIKE2D_H_
#define VECTORLIKE2D_H_

#include <cmath>

#include "Geometrical2D.h"

namespace jlib{


class VectorLike2D: public Geometrical2D {

public:

	/// Rotation by an cos/sin of angle.
	virtual void rotate(double cosval, double sinval);

	/// Cartesian translation.
	virtual void translate(double delta_x,double delta_y);

	/// Rescaling by a factor.
	virtual void rescale(double factor);

	/// Returns Cartesian x-component
	virtual double getX() const = 0;

	/// Returns Cartesian y-component
	virtual double getY() const = 0;

protected:

	/// Returns reference to Cartesian x-component
	virtual double & getRefX() = 0;

	/// Returns reference to Cartesian y-component
	virtual double & getRefY() = 0;

};


}

#endif /* VECTORLIKE2D_H_ */
