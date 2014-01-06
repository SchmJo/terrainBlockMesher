/**
 * @file Line2D.h
 * @brief Definition/Implementation of class Line2D.
 *
 */

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
