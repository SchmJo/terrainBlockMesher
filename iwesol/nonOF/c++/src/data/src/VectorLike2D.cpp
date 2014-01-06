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
#include <cmath>
#include <iostream>
using namespace std;

#include "VectorLike2D.h"

namespace jlib{

inline void VectorLike2D::rotate(double cosval, double sinval){

	double x = getRefX();
	double y = getRefY();

	getRefX() =  cosval*x + sinval*y;
	getRefY() = -sinval*x + cosval*y;

}


inline void VectorLike2D::translate(double delta_x, double delta_y){

	getRefX() += delta_x;
	getRefY() += delta_y;

}


inline void VectorLike2D::rescale(double factor){

	getRefX() *= factor;
	getRefY() *= factor;

}

}
