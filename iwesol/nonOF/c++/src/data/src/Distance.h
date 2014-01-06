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

#ifndef DISTANCE_H_
#define DISTANCE_H_

#include <cmath>
#include <vector>
#include "Coordinate2D.h"

namespace jlib {

template< class Data >
struct Distance{

	double operator()(const Data & a, const Data & b) const{
		return std::sqrt( (b - a) * (b - a)  );
	}

};

template<>
double Distance< int >::operator()(
		const int & a,
		const int & b
		) const;

template<>
double Distance< std::vector< double > >::operator()(
		const std::vector< double > & a,
		const std::vector< double > & b
		) const;

template<>
double Distance< std::vector< int > >::operator()(
		const std::vector< int > & a,
		const std::vector< int > & b
		) const;

template<>
double Distance< Coordinate2D >::operator()(
		const Coordinate2D & a,
		const Coordinate2D & b
		) const;

}

#endif /* DISTANCE_H_ */
