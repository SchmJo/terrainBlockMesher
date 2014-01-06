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

#ifndef PLOTLINEINFO_H_
#define PLOTLINEINFO_H_

#include "Plotable.h"

namespace jlib{

/**
 * @class PlotLineInfo
 * @brief A info struct for lines in class Plot.
 *
 */
/// data of a plot line
class PlotLineInfo{

public:

	/// The origin of first axis data
	Plotable const * source_x;

	/// The origin of second axis data
	Plotable const * source_y;

	/// The column of first axis data
	int column_x;

	/// The column of second axis data
	int column_y;

	/// the name of the plot line
	std::string name;
};

}

#endif /* PLOTLINEINFO_H_ */
