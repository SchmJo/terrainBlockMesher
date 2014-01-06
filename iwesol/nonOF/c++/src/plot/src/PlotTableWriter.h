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

#ifndef PLOTTABLEWRITER_H_
#define PLOTTABLEWRITER_H_

#include "PlotWriter.h"

namespace jlib {

class Plot;

/**
 * @class jlib::PlotTableWriter
 * @brief A class writing a table file (.txt) of a plot.
 */
class PlotTableWriter:
	public PlotWriter{

public:

	/// Constructor
	PlotTableWriter(
			Plot const * plot,
			int colWidth_x = 13,
			int colWidth_y = 13,
			int precision_x = 2,
			int precision_y = 2,
			bool scientific_x = false,
			bool scientific_y = true
			);

	/// Outputable: Provides the data that will be written to the output file.
	Outputable::OData getOData(
			const std::string & type = IO::OFILE::TYPE::EMPTY) const;

	/// PlotWriter: create the plot
	void write() const;

	/// set parameters
	inline void setParameters(
			int colWidth_x = 13,
			int colWidth_y = 13,
			int precision_x = 2,
			int precision_y = 2,
			bool scientific_x = false,
			bool scientific_y = true
			);


private:

	/// the columns width, x data
	int m_colWidth_x;

	/// the columns width, y data
	int m_colWidth_y;

	/// the precision, x data
	int m_precision_x;

	/// the precision, y data
	int m_precision_y;

	/// flag for scientific numbers, x data
	bool m_scientific_x;

	/// flag for scientific numbers, y data
	bool m_scientific_y;

	/// PlotWriter: write the gnuplot file
	inline void writeGnuplot() const {}

	/// PlotWriter: write the shell script
	inline void writeScript() const {}

	/// PlotWriter: run the shell script
	inline void runScript() const {}
};

inline void PlotTableWriter::setParameters(
			int colWidth_x,
			int colWidth_y,
			int precision_x,
			int precision_y,
			bool scientific_x,
			bool scientific_y
			){
	m_colWidth_x   = colWidth_x;
	m_colWidth_y   = colWidth_y;
	m_precision_x  = precision_x;
	m_precision_y  = precision_y;
	m_scientific_x = scientific_x;
	m_scientific_y = scientific_y;
}

} /* namespace jlib */
#endif /* PLOTTABLEWRITER_H_ */
