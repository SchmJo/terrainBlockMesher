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

#ifndef PLOTHISTOGRAMWRITER_H_
#define PLOTHISTOGRAMWRITER_H_

#include "PlotWriter.h"

namespace jlib {

/**
 * @class jlib::PlotHistogramWriter
 * @brief Histogram style pdf writer, via gnuplot.
 */
class PlotHistogramWriter:
	public PlotWriter{

public:

	/// Constructor
	PlotHistogramWriter(
			Plot const * plot,
			const double & binWidth
			);

	/// Destructor
	virtual ~PlotHistogramWriter(){}

	/// Outputable: Provides the data that will be written to the output file.
	Outputable::OData getOData(
			const std::string & type = IO::OFILE::TYPE::EMPTY) const;

	/// PlotWriter: optional last step in write process
	void writePostCare() const;


private:

	/// the bin width
	double m_binWidth;

};

} /* namespace jlib */
#endif /* PLOTHISTOGRAMWRITER_H_ */
