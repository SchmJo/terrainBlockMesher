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

#ifndef POLARPLOTPYTHONWRITER_H_
#define POLARPLOTPYTHONWRITER_H_

#include "PlotWriter.h"

namespace jlib {

/**
 * @class jlib::PolarPlotPythonWriter
 * @brief Writes a polar plot via python.
 *
 * x-data is interpreted as angles in degrees, in mathematical sense.
 * y-data is interpreted as polar data.
 */
class PolarPlotPythonWriter:
	public PlotWriter{

public:

	/// Constructor
	PolarPlotPythonWriter(
			Plot const * plot,
			int sectors,
			bool flag_12axes
			);

	/// Outputable: Provides the data that will be written to the output file.
	Outputable::OData getOData(
			const std::string & type = IO::OFILE::TYPE::EMPTY) const;

	/// PlotWriter: create the plot
	void write() const;

	/// load data files
	void load();


private:

	/// the number of sectors
	int m_sectors;

	/// the axes style: 12 or 8
	bool m_flag_12axes;

	/// Write the python script
	void writePython() const;
};

} /* namespace jlib */
#endif /* POLARPLOTPYTHONWRITER_H_ */
