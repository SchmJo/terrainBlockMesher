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

Description
    A collection of global functions and definitions.

SourceFiles
    BLIB.C

References
	[1] J. Schmidt, C. Peralta, B. Stoevesandt, "Automated Generation of
	    Structured Meshes for Wind Energy Applications", Proceedings of the
	    Open Source CFD International Conference, 2012, London, UK

\*---------------------------------------------------------------------------*/

#ifndef BLIB_H_
#define BLIB_H_

#include <vector>

#include "String.h"

namespace blib{

/// chop limit
const double CHOP = 0.00000001;

/// chops a double at CHOP
inline double chop(double a);

/// double modulo
inline double mod(double a, double b);

/// transforms a double into a String
String key(double d, int imax, const std::string & zeroFill = "X");

/// Kills duplicates in a std::vector.
template <class T>
inline void reduceDup_v(std::vector<T> & vi);


///////////////////////////////////////////////////////////
//
//    Inline implementations:
//
///////////////////////////////////////////////////////////

inline double chop(double a){
	return a * a < CHOP * CHOP ? 0. : a;
}

inline double mod(double a, double b){
	//label temp = a / b;
	double out = chop(a);
	while(chop(out - b) >= 0.) out = chop(out - b);
	while(chop(out) < 0.) out = chop(out + b);
	return out;
}

template <class T>
inline void reduceDup_v(std::vector<T> & vi){

	// simple check:
	if(vi.size() < 2) return;

	// the first is always in:
	std::vector<T> out = std::vector<T>(1,vi[0]);

	// check the others:
	typename std::vector<T>::iterator it;
	for(it = vi.begin() + 1; it < vi.end(); it++)
		if(find(out.begin(),out.end(),*it) == out.end())
			out.push_back(*it);

	// return:
	vi = out;
}

} /* blib */

#endif /* BLIB_H_ */
