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

#ifndef LINE2D_H_
#define LINE2D_H_

#include "Coordinate2D.h"
#include "CoordinateSystem2D.h"
#include "Outputable.h"
#include "Linear1D.h"

namespace jlib {

/**
 * @class jlib::Line2D
 * @brief A line between two Coordinate2D points.
 */
class Line2D:
	public Linear1D< Coordinate2D >,
	public Outputable {

public:

	/// Constructor
	Line2D(const std::string & name = ""): Linear1D< Coordinate2D >(name){}

	/// Constructor
	Line2D(
			const Coordinate2D & start,
			const Coordinate2D & end,
			const std::string & name = ""
			);

	/// Constructor
	Line2D(
			const double & x0,
			const double & y0,
			const double & x1,
			const double & y1,
			const std::string & name = ""
			);

	/// Constructor
	Line2D(const Linear1D< Coordinate2D > & lin);

	/// Outputable: Provides the data that will be written to the output file.
	OData getOData(
			const std::string & type = IO::OFILE::TYPE::EMPTY) const;

	/// Calculates the hit of a line with this line. Returns success.
	bool getHitPoint(const Line2D & lineB,Coordinate2D & hitPoint) const;

	/** Registers points at the coordinate system.
	 * Returns number of successful registrations.
	 */
	int coordSysRegister(CoordinateSystem2D* cs);

	/// Return dist of a point p to the line
	double dist(const Point2D & p) const;

	/// Return dist between two points p(s0), p(s1), 0 <= si <= 1
	inline double dist(
			const double & s0,
			const double & s1
			) const { return Linear1D<Coordinate2D>::dist(s0,s1); }

	/// Returns the s value of nearest point
	double getNearestS(const Point2D & p) const;

	/// Returns the nearest point
	inline Point2D getNearestPoint(const Point2D & p) const { return (*this)[getNearestS(p)]; }

};

inline double Line2D::getNearestS(const Point2D & p) const{

	// prepare:
	const Point2D u   = end - start;

	// calc s:
	double s = ((p.x - start.x) * u.x + (p.y - start.y) * u.y) / (u.x * u.x + u.y * u.y);

	return s <= 0. ? 0. : (s >= 1. ? 1. : s);
}

inline double Line2D::dist(const Point2D & p) const{

	// prepare:
	const Point2D u   = end - start;

	// calc s:
	double s = getNearestS(p);

	// return distance:
	return sqrt(
			(p.x - start.x - s * u.x) * (p.x - start.x - s * u.x)
			+ (p.y - start.y - s * u.y) * (p.y - start.y - s * u.y)
			);
}

}

#endif /* LINE2D_H_ */
