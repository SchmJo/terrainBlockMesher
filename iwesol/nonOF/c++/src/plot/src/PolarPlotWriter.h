/**
 * @file PolarPlotWriter.h
 * @brief This file defines/implements the class PolarPlotWriter.
 */

#ifndef POLARPLOTWRITER_H_
#define POLARPLOTWRITER_H_

#include "PlotWriter.h"

namespace jlib {

class Plot;

/**
 * @class jlib::PolarPlotWriter
 * @brief Writes a polar plot via gnuplot.
 *
 * x-data is interpreted as angles in degrees, in mathematical sense.
 * y-data is interpreted as polar data.
 */
class PolarPlotWriter:
	public PlotWriter{

public:

	/// the mode
	static const int RADIANS, DEGREES;

	/// the start position
	static const int EAST, NORTH;

	/// the rotation direction
	static const int CLOCKWISE, COUNTER_CLOCKWISE;

	/// Constructor
	PolarPlotWriter(
			Plot const * plot,
			const double & tickInterval,
			int angleMode = DEGREES,
			int startAngle = EAST,
			int rotDirection = COUNTER_CLOCKWISE
			);

	/// Outputable: Provides the data that will be written to the output file.
	Outputable::OData getOData(
			const std::string & type = IO::OFILE::TYPE::EMPTY) const;

	/// updates evaluation of data
	void update();


private:

	/// the tick interval
	double m_tickInterval;

	/// the angle mode
	int m_angleMode;

	/// the start angle
	int m_startAngle;

	/// the rotational direction
	int m_rotDirection;

	/// the maximal value
	double m_maxVal;
};

} /* namespace jlib */
#endif /* POLARPLOTWRITER_H_ */
