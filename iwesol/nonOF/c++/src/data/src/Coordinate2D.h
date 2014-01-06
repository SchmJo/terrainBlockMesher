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

#ifndef COORDINATE2D_H_
#define COORDINATE2D_H_

#include "Point2D.h"
#include "VectorLike2D.h"
#include "CoordinateSystem2D.h"
//#include "Parseable.h"
//#include "ParseComm.h"

namespace jlib{

class Coordinate2D:
	public Point2D,
	public VectorLike2D{

public:

	//! Constructor with direct assignement of x and y
	Coordinate2D(double x = 0, double y = 0);

	//! Construct from a std::vector
	Coordinate2D(const std::vector<double> & xy);

	/// Constructor
	Coordinate2D(const Point2D & p);

	/// Destructor.
	virtual ~Coordinate2D();

	/// normalization
	void normalize() { *this /= length(); }

	/// Returns x
	double getX() const;

	/// Returns y
	double getY() const;

	//! Returns x^2+y^2
	double sqrDist(void) const;

	//! gives back the length of the mathematical std::vector x,y (sqrt of x^2+y^2)
	double length(void) const;

	//! gives back the distance from  this point to r
	double length(const Coordinate2D&) const;

	//! Returns the quadratic distance between r and this position
	double sqrDist(const Coordinate2D&) const;

	//! Returns the distance
	double dist(const Coordinate2D&) const;


protected:

	/// Returns reference to Cartesian x-component
	double & getRefX(){return x;}

	/// Returns reference to Cartesian y-component
	double & getRefY(){return y;}


};

//////////
inline Coordinate2D::Coordinate2D(const std::vector<double> & xy):
		Point2D(xy){
}

//////////
inline Coordinate2D::Coordinate2D(double x,
		double y)
:
	Point2D(x,y){
}

//////////
inline Coordinate2D::Coordinate2D(const Point2D & p):
		Point2D(p){
}

///////////
inline double Coordinate2D::dist(const Coordinate2D& r) const {
	return std::sqrt(sqrDist(r));
}


inline double Coordinate2D::sqrDist(const Coordinate2D& r) const {
	return ((r.x - x) * (r.x - x) + (r.y - y) * (r.y - y));
}


///////////
inline double Coordinate2D::sqrDist(void) const {
	return x * x + y * y;
}

///////////
inline double Coordinate2D::getY() const {
	return y;
}

////////////
inline double Coordinate2D::getX() const {
	return x;
}


/////////////////

}

#endif /* COORDINATE2D_H_ */
