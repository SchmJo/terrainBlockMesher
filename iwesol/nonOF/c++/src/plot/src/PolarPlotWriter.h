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
