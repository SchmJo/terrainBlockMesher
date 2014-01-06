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

#include "RRFunction.h"

namespace jlib {

RRFunction::RRFunction(
		int interpolOrder,
		const std::string & name
		):
		GenericRFunction< double >
		(
				interpolOrder,
				name
		),
		m_plotPoints(0){
}

RRFunction::RRFunction(
		const std::vector< double > xvals,
		const std::vector< double > yvals,
		int interpolOrder,
		const std::string & name
		):
		GenericRFunction< double >
		(
				xvals,
				yvals,
				interpolOrder,
				name
		),
		m_plotPoints(0){
}

std::vector< double > RRFunction::getPlotColumn(int i) const{

	assert(i >= 0 && i < getPlotColumns());
	if(m_plotPoints <= 0){
		if(i == 0) return pointList();
		if(i == 1) return dataList();
		std::vector< double > out(size());
		for(unsigned int j = 0; j < size(); j++){
			out[j] = derivative
					(
							point(j),
							i - 1
					);
		}
		return out;
	}
	std::vector< double > out(m_plotPoints);
	double delta = ( lastPoint() - firstPoint() ) / double(m_plotPoints - 1);
	for(int j = 0; j < m_plotPoints; j++){
		if(i == 0){
			out[j] = firstPoint() + j * delta;
		} else {
			out[j] = derivative
					(
							firstPoint() + j * delta,
							i - 1
					);
		}
	}

	return out;
}

}
