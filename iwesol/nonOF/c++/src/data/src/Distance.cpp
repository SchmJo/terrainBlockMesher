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

#include "Distance.h"
#include <stdlib.h>

namespace jlib{

template<>
double Distance< int >::operator()(
		const int & a,
		const int & b
		) const{
	return std::abs(b - a);
}

template<>
double Distance< std::vector< double > >::operator()(
		const std::vector< double > & a,
		const std::vector< double > & b
		) const{
	double out = 0;
	for(unsigned int i = 0; i< a.size(); i++){
		out     += (b[i] - a[i]) * (b[i] - a[i]);
	}
	return std::sqrt(out);
}

template<>
double Distance< std::vector< int > >::operator()(
		const std::vector< int > & a,
		const std::vector< int > & b
		) const{
	double out = 0;
	for(unsigned int i = 0; i< a.size(); i++){
		double d = b[i] - a[i];
		out     += d * d;
	}
	return std::sqrt(out);
}

template<>
double Distance< Coordinate2D >::operator()(
		const Coordinate2D & a,
		const Coordinate2D & b
		) const{
	return std::sqrt( (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) );
}

}
