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

#ifndef PLOTABLEOBJECT_H_
#define PLOTABLEOBJECT_H_

#include "Plotable.h"

/**
 * @class jlib::PlotableObject
 * @brief Data container class
 */
namespace jlib {

class PlotableObject:
	public Plotable{

public:

	/// Constructor
	PlotableObject(int size = 0);

	/// Plotable: Returns the number of columns
	inline int getPlotColumns() const { return int(m_colData.size()); }

	/// Plotable: Returns columns no. i as vector of doubles
	inline std::vector<double> getPlotColumn(int i) const { return m_colData[i]; }

	/// Plotable: Returns label of columns no. i
	inline std::string getPlotColumnLabel(int i) const { return m_colNames[i]; }

	/// return the col data
	inline const std::vector< std::vector< double > > & colData() const { return m_colData; }

	/// return the col data
	inline std::vector< std::vector< double > > & colData() { return m_colData; }

	/// return the col names
	inline const std::vector< std::string > & colNames() const { return m_colNames; }

	/// return the col names
	inline std::vector< std::string > & colNames() { return m_colNames; }

	/// resize
	inline void resize(int size) { m_colData.resize(size); m_colNames.resize(size); }


private:

	/// the col data
	std::vector< std::vector< double > > m_colData;

	/// the col names
	std::vector< std::string > m_colNames;
};

} /* namespace jlib */
#endif /* PLOTABLEOBJECT_H_ */
