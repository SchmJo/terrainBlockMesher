/**
 * @file ScalarLike2D.h
 * @class ScalarLike2D
 * @brief An interface for scalars in 2D.
 *
 * The basic property of scalars is that they are unimpressed by
 * rotations, translations and dilations. In fact, they are just constant.
 *
 */

#ifndef SCALARLIKE2D_H_
#define SCALARLIKE2D_H_

#include "Geometrical2D.h"

namespace jlib{

class ScalarLike2D: public virtual Geometrical2D {

public:

	/// Rotation by an angle in radiant.
	virtual void rotate(double angleRad){}

	/// Rotation by an cos/sin of angle.
	virtual void rotate(double cosval, double sinval){};

	/// Cartesian translation.
	virtual void translate(double delta_x,double delta_y){}

	/// Rescaling by a factor.
	virtual void rescale(double factor){}

};

}

#endif /* SCALARLIKE2D_H_ */
