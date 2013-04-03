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
    blib::HashTable

Description
    See below

SourceFiles

References
	[1] J. Schmidt, C. Peralta, B. Stoevesandt, "Automated Generation of
	    Structured Meshes for Wind Energy Applications", Proceedings of the
	    Open Source CFD International Conference, 2012, London, UK

\*---------------------------------------------------------------------------*/

#ifndef HASHTABLE_H_
#define HASHTABLE_H_


#include "Memorizing.h"

namespace blib{

/**
 * @class blib::HashTable
 * @brief A basic HashTable
 *
 */
template <class T, class U>
class HashTable:
	public Memorizing<T,U> {

public:

	/// Constructor.
	HashTable(int size = 0): Memorizing<T,U>(size) {}

	/// Destructor.
	virtual ~HashTable(){}

	/// set an element
	inline void set(const T & data, const U & label) { Memorizing<T,U>::memorize(data,label); }

	/// clears the table
	void clear() { Memorizing<T,U>::clearMemory(); }

	/// Returns a reference
	T & operator[](const U & label);

	/// Returns a reference
	const T & operator[](const U & label) const;


private:

	/// spits out error
	void __spitError(const U & label) const;
};

template<class T, class U>
inline T & HashTable<T,U>::operator[](const U & label){
		int i = Memorizing<T,U>::find(label);
		if(i < 0) __spitError(label);
		return Memorizing<T,U>::getMemData(Memorizing<T,U>::elementIndex(i));
}

template<class T, class U>
inline const T & HashTable<T,U>::operator[](const U & label) const {
		int i = Memorizing<T,U>::find(label);
		if(i < 0) __spitError(label);
		return Memorizing<T,U>::getMemData(Memorizing<T,U>::elementIndex(i));
}

template<class T, class U>
void HashTable<T,U>::__spitError(const U & label) const{
	std::cout << "\nHashTable: Error: label '" << label << "' not found." << std::endl;
	throw;
}



}

#endif /* HASHTABLE_H_ */
