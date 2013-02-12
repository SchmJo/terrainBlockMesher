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
    blib::OrderedSet

Description
    See below

SourceFiles

References
	[1] J. Schmidt, C. Peralta, B. Stoevesandt, "Automated Generation of
	    Structured Meshes for Wind Energy Applications", Proceedings of the
	    Open Source CFD International Conference, 2012, London, UK

\*---------------------------------------------------------------------------*/

#ifndef ORDEREDSET_H_
#define ORDEREDSET_H_

#include "Ordered.h"

namespace blib {

/**
 * @class blib::OrderedSet
 * @brief Interface for an ordered set
 *
 * The template class T must be orderable via <,>,<=,>=
 */
template <class T>
class OrderedSet:
	public Ordered{

public:

		/// Constructor
		OrderedSet(){}

		/// Destructor
		virtual ~OrderedSet(){}

		/// OrderedSet: Returns element i before ordering
		virtual const T & getUnorderedElement(int i) const = 0;

		/** Ordered: Returns if element at position i is before element at position j
		 * in the original numbering. This method is used by 'order()'.
		 */
		bool i_before_j(int i, int j) const { return getUnorderedElement(i) < getUnorderedElement(j); }

		/// Returns element
		const T & getElement(int i) const { return getUnorderedElement(elementIndex(i)); }

		/// Returns first element
		const T & getFirstElement() const { return getUnorderedElement(firstElementIndex()); }

		/// Returns last element
		const T & getLastElement() const { return getUnorderedElement(lastElementIndex()); }

		/** Returns index of closest element < lowx.
		 * ihigh = -1 corresponds to last index.
		 * Returns -1 if impossible.
		 */
		int getIndex_before(const T & lowx, int ilow = 0, int ihigh = -1) const;

		/** Returns index of closest element <= lowx.
		 * ihigh = -1 corresponds to last index.
		 * Returns -1 if impossible.
		 */
		int getIndex_beforeeq(const T & lowx, int ilow = 0, int ihigh = -1) const;

		/** Returns index of closest element > lowx.
		 * ihigh = -1 corresponds to last index.
		 * Returns -1 if impossible.
		 */
		int getIndex_after(const T & lowx, int ilow = 0, int ihigh = -1) const;

		/** Returns index of closest element >= lowx.
		 * ihigh = -1 corresponds to last index.
		 * Returns -1 if impossible.
		 */
		int getIndex_aftereq(const T & lowx, int ilow = 0, int ihigh = -1) const;

		/// prints the ordered list
		void printOrderedSet() const;

		/// finds an entry, returns -1 if error
		int find(const T & x) const;

		/// checks if an element is in the list
		bool found(const T & x) const { return find(x) >= 0; }

		/// Returns the ordered content
		std::vector<T> getElements() const;


protected:

		/// Equality derived from <=, >= relation (string-safe)
		virtual bool _equal(const T & A, const T & B) const { return A <= B && A >= B; }

};


template <class T>
int OrderedSet<T>::getIndex_before(const T & lowx, int ilow, int ihigh) const{

	// check ihigh:
	if(ihigh < 0) return getIndex_before(lowx,ilow,size() - 1);

	// trivial cases:
	if(ilow > ihigh || size() == 0) return -1;
	if(getElement(ilow) >= lowx) return -1;
	if(ilow == ihigh) return ilow;
	if(ihigh == ilow + 1) return getElement(ihigh) < lowx ? ihigh : ilow;

	// split:
	int imid = (ilow + ihigh) / 2;
	if(getElement(imid) < lowx) return getIndex_before(lowx,imid,ihigh);

	return getIndex_before(lowx,ilow,imid - 1);
}

template <class T>
int OrderedSet<T>::getIndex_beforeeq(const T & lowx, int ilow, int ihigh) const{
	if(size() == 0 ) return -1;
	int ibefore = getIndex_before(lowx,ilow,ihigh);
	if(ibefore == int(size()) - 1) return ibefore;
	return _equal(getElement(ibefore + 1),lowx) ? ibefore + 1 : ibefore;
}

template <class T>
int OrderedSet<T>::getIndex_aftereq(const T & lowx, int ilow, int ihigh) const{

	int ibeforeeq = getIndex_beforeeq(lowx,ilow,ihigh);
	if(ibeforeeq < 0) return -1;
	if(_equal(getElement(ibeforeeq),lowx)) return ibeforeeq;
	if(ibeforeeq == int(size()) - 1) return -1;
	return ibeforeeq + 1;
}

template <class T>
int OrderedSet<T>::getIndex_after(const T & lowx, int ilow, int ihigh) const{

	int iaftereq = getIndex_aftereq(lowx,ilow,ihigh);
	if(iaftereq < 0) return -1;
	if(getElement(iaftereq) > lowx) return iaftereq;
	if(iaftereq == int(size()) - 1) return -1;
	return iaftereq + 1;

}

template <class T>
void OrderedSet<T>::printOrderedSet() const{
	for(unsigned int i = 0; i < size(); i++){
		std::cout << i << " " << getElement(i) << std::endl;
	}
}

template <class T>
inline int OrderedSet<T>::find(const T & x) const{
	int ibeforeeq = getIndex_beforeeq(x);
	if(ibeforeeq < 0 || !_equal(getElement(ibeforeeq),x)) return -1;
	return ibeforeeq;
}

template <class T>
std::vector<T> OrderedSet<T>::getElements() const{
	std::vector<T> out(size());
	for(unsigned int i = 0; i < size(); i++){
		out[i] = getElement(i);
	}
	return out;
}

}


#endif /* ORDEREDSET_H_ */
