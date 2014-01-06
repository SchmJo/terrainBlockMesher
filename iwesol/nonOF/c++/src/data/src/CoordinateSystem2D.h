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
License
    This file is part of IWESOL.

    IWESOL is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    IWESOL is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with IWESOL.  If not, see <http://www.gnu.org/licenses/>.

Class

Description

SourceFiles

References

\*---------------------------------------------------------------------------*/

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
