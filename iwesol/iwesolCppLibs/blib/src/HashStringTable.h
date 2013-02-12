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
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Class
    blib::HashStringTable

Description
    See below

SourceFiles

References
	[1] J. Schmidt, C. Peralta, B. Stoevesandt, "Automated Generation of
	    Structured Meshes for Wind Energy Applications", Proceedings of the
	    Open Source CFD International Conference, 2012, London, UK

\*---------------------------------------------------------------------------*/

#ifndef HASHSTRINGTABLE_H_
#define HASHSTRINGTABLE_H_

#include <string>

#include "HashTable.h"

namespace blib{

/**
 * @class blib::HashStringTable
 * @brief A basic HashTable with string labels.
 *
 */
template <class T>
class HashStringTable:
	public HashTable<T,std::string> {

public:

	/// Constructor.
	HashStringTable(int size = 0): HashTable<T,std::string>(size){}

	/// Destructor.
	virtual ~HashStringTable(){}

};

}

#endif /* HASHSTRINGTABLE_H_ */
