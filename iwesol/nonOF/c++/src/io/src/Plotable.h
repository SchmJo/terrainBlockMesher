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

#ifndef PLOTABLE_H_
#define PLOTABLE_H_

#include <vector>
#include <string>

namespace jlib {

/**
 * @class Plotable
 * @brief Interface for classes that can be plotted using the class Plot.
 *
 */
class Plotable {

public:

	/// Constructor
	Plotable();

	/// Destructor
	virtual ~Plotable();

	/// Plotable: Returns the number of columns
	virtual int getPlotColumns() const = 0;

	/// Plotable: Returns columns no. i as vector of doubles
	virtual std::vector<double> getPlotColumn(int i) const = 0;

	/// Plotable: Returns label of columns no. i
	virtual std::string getPlotColumnLabel(int i) const = 0;

	/// checks if plot is empty
	inline bool emptyPlot() const { return getPlotColumns() == 0; }

};

} /* namespace jlib */
#endif /* PLOTABLE_H_ */
