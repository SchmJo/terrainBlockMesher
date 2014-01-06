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

#ifndef RRFUNCTION_H_
#define RRFUNCTION_H_

#include "GenericRFunction.h"
#include "Plotable.h"

/**
 * @class jlib::RRFunction
 * @brief A function f: R -> R
 */
namespace jlib {

class RRFunction:
	public GenericRFunction< double >,
	public Plotable{

public:

		/// Constructor
		RRFunction(
				int interpolOrder = 4,
				const std::string & name = "RRFunction"
				);

		/// Constructor
		RRFunction(
				const std::vector< double > xvals,
				const std::vector< double > yvals,
				int interpolOrder = 4,
				const std::string & name = "RRFunction"
				);

		/// Plotable: Returns the number of columns
		inline int getPlotColumns() const { return 2 + GenericRFunction< double >::highestDerivative(); }

		/// Plotable: Returns columns no. i as vector of doubles
		std::vector< double > getPlotColumn(int i) const;

		/// Plotable: Returns label of columns no. i
		inline std::string getPlotColumnLabel(int i) const;

		/// set number of points in plot. zero = as many as size (default)
		inline void setPlotPoints(int plotPoints) const { m_plotPoints = plotPoints; }


private:

		/// the number of points in plot
		mutable int m_plotPoints;
};

inline std::string RRFunction::getPlotColumnLabel(int i) const{
	assert(i >= 0 && i < getPlotColumns());
	if(i == 0) return "x";
	if(i == 1) return "f(x)";
	return "fd" + String(i - 1) + "(x)";
}

}

#endif /* RRFUNCTION_H_ */
