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

#ifndef POLARPLOT_H_
#define POLARPLOT_H_

#include "Plot.h"

namespace jlib {

/**
 * @class jlib::PolarPlot
 * @brief A plot in polar coordinates.
 *
 * Connection with class Plot: x-axis is angle wrt. (1 0 0), y-axis is data.
 */
class PolarPlot:
	public Plot{

public:

	/// Constructor
	PolarPlot(
			const std::string & name = "",
			const std::string & writeToFolder = ".",
			const std::string & label_data = "frequency"
			);


private:

	/// disabled
	void addLine(const PlotLineInfo & line);

	// disabled
	void addLine(
				Plotable const * source,
				int column_x,
				int column_y,
				std::string name
				);

	/// disabled
	void addLine(
				Plotable const * source_x,
				Plotable const * source_y,
				int column_x,
				int column_y,
				std::string name
				);

};

} /* namespace jlib */
#endif /* POLARPLOT_H_ */
