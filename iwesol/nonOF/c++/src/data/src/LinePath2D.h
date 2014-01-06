/**
 * @file LinePath2D.h
 * @brief Definition/Implementation of class LinePath2D.
 *
 */

#ifndef LINEPATH2D_H_
#define LINEPATH2D_H_

#include "GenericLinePath.h"
#include "GenericRFunction.h"
#include "Plotable.h"
#include "Line2D.h"

namespace jlib {


/**
 * @class jlib::LinePath2D
 * @brief A path of discrete points, parametrized by the path length.
 *
 * Points are also accessible via the parameter s, 0 <= s <= 1.
 */
class LinePath2D:
	public GenericLinePath< Coordinate2D >,
	public Plotable,
	public Outputable{

public:

	/// Constructor
	LinePath2D(const std::string & name = "");

	/// Constructor
	LinePath2D(
			const double & x0,
			const double & y0,
			const double & x1,
			const double & y1,
			const std::string & name = ""
			);

	/// Constructor
	LinePath2D(
			const double & x,
			const double & y,
			const std::string & name = ""
			);

	/// Constructor
	LinePath2D(
			const Coordinate2D & p0,
			const Coordinate2D & p1,
			const std::string & name = ""
			);

	/// Constructor
	LinePath2D(
			const Coordinate2D & p,
			const std::string & name = ""
			);

	/// Constructor
	LinePath2D(
			const std::vector<Coordinate2D> & points,
			const std::string & name = ""
			);

	/// Constructor
	LinePath2D(
			const std::vector<Point2D> & points,
			const std::string & name = ""
			);

	/// Destructor
	virtual ~LinePath2D();

	/// Outputable: Provides the data that will be written to the output file.
	virtual OData getOData(
			const std::string & type = IO::OFILE::TYPE::EMPTY) const;

	/// Plotable: Returns the number of columns. Here: s, x, y.
	inline int getPlotColumns() const { return 3; }

	/// Plotable: Returns label of columns no. i
	inline std::string getPlotColumnLabel(int i) const { switch(i){ case 0: return "s"; case 1: return "x"; case 2: return "y"; } return "none";}

	/// Plotable: Returns columns no. i as vector of doubles
	std::vector<double> getPlotColumn(int i) const;

	/// Return dist between two points p(s0), p(s1), 0 <= si <= 1
	inline double dist(const double & s0, const double & s1) const { return GenericLinePath<Coordinate2D>::dist(s0,s1); }

	/// Shortest distance to a point
	double dist(const Coordinate2D & p) const;

	/** Registers points at the coordinate system.
	 * Returns number of successful registrations.
	 */
	int coordSysRegister(CoordinateSystem2D* cs);

	/// prints to console
	//virtual void print(const double & s0, const double & s1) const;

	/// Prints to console
	virtual void print() const;

	/// Calculates the hit of a line with this line. Returns success.
	bool getHitPoint(const Line2D & lineB,Coordinate2D & hitPoint) const;

	/// Returns the s value of nearest point
	double getNearestS(const Coordinate2D & p) const;

	/// Returns a sub line path
	//LinePath2D getSubLinePath(const double & s0, const double & s1) const;

	/// flips the order. ie., the s direction
	LinePath2D & flip();

};

}

#endif /* LINEPATH2D_H_ */
