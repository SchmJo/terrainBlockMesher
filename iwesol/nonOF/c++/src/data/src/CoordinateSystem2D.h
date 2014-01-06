/**
 * @file CoordinateSystem2D.h
 * @class CoordinateSystem2D
 * @brief This class represents the 2D coordinate system.
 *
 * The main purpose of this class is to perform rotations,
 * translations and scaling operations for all linked objects
 * simultaneously.
 *
 */

#ifndef COORDINATESYSTEM2D_H_
#define COORDINATESYSTEM2D_H_

#include "Geometrical2D.h"
#include "ScalarLike2D.h"
#include "VectorLike2D.h"
#include "BLIB.h"
#include "Point2D.h"

namespace jlib{
using namespace BLIB;

class CoordinateSystem2D {

public:

	/// List of Scalars
	std::vector<ScalarLike2D*> scalars;

	/// List of Vectors
	std::vector<VectorLike2D*> vectors;

	/// Constructor.
	CoordinateSystem2D();

	virtual ~CoordinateSystem2D();

	/// Prints the number of registered objects
	void print();

	/// Prints ascii graphics of positions to the console
	void printGraphVectors(int xrange = 30, int yrange = 15);

	/// Calculates min and max coordinates
	void calcMinMax();

	/** Perform a rotation of all linked elements,
	 * angle in rad
	 */
	void rotate(double angleRad);

	/** Perform a rotation of all linked elements,
	 * angle in deg
	 */
	void rotateDeg(double angleDeg){rotate(deg2rad(angleDeg));}

	/// Perform a rotation of all linked elements
	void rotate(double cosval, double sinval);

	/// Perform a translation of all linked elements
	void translate(double delta_x,double delta_y);

	/// Perform a translation of all linked elements
	void translate(const Point2D & shift);

	/// Perform a rescaling of all linked elements
	void rescale(double factor);

	/// Add a link to a geometrical object. Returns success.
	bool registerObj(Geometrical2D* obj);

	/// Returns number of linked scalars
	unsigned int getNrScalars() const { return scalars.size();}

	/// Returns number of linked vectors
	unsigned int getNrVectors() const { return vectors.size();}

	/// Returns current total angle of rotation in rad
	double getAngleRad() const { return angleRadTot; }

	/// Returns current total angle of rotation in deg
	double getAngleDeg() const { return rad2deg(angleRadTot); }


protected:

	/// A flag telling if the min and max values are valid
	int flag_minmax_valid;

	/// The sum of all previous rotation angles [rad]
	double angleRadTot;

	/// Minimal x coordinate
	double xmin;

	/// Maximal x coordinate
	double xmax;

	/// Minimal y coordinate
	double ymin;

	/// Maximal y coordinate
	double ymax;


private:

	/** An internal flag telling if angleRadTot has been updated
	 * during the current rotation
	 */
	int flag_angleUpdate_done;


};

}

#endif /* COORDINATESYSTEM2D_H_ */
