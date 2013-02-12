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
    along with IWESOL. If not, see <http://www.gnu.org/licenses/>.

Class
    blib::PartlyPointedList

Description
    See below

SourceFiles

References
	[1] J. Schmidt, C. Peralta, B. Stoevesandt, "Automated Generation of
	    Structured Meshes for Wind Energy Applications", Proceedings of the
	    Open Source CFD International Conference, 2012, London, UK

\*---------------------------------------------------------------------------*/

#ifndef PartlyPointedList_H_
#define PartlyPointedList_H_

#include <vector>

namespace blib {

/**
 * @class blib::PartlyPointedList
 * @brief A list of elements, some of them pointed to some stored locally.
 */
template<class T>
class PartlyPointedList {

public:

	/// Constructor
	PartlyPointedList(){}

	/// Constructor
	PartlyPointedList(const T & first);

	/// Constructor
	PartlyPointedList(T * first);

	/// Copy constructor
	PartlyPointedList(const PartlyPointedList<T> & ppl);

	/// Destructor
	virtual ~PartlyPointedList(){}

	/// PartlyPointedList: add an element
	virtual void append(const T & element);

	/// PartlyPointedList: add an element
	virtual void append(T * element);

	/// returns the size
	unsigned int size() const { return elements.size(); }

	/// returns the number of locally stored elements
	unsigned int sizeLocal() const { return _elements.size(); }

	/// returns emptyness
	bool empty() const { return elements.empty(); }

	/// returns local emptyness
	bool emptyLocal() const { return _elements.empty(); }

	/// returns a copy of the points
	std::vector<T> getElements() const;

	/// returns the pointers to the elements
	const std::vector<T*> & getPointers() const { return elements; }

	/// The operator =
	PartlyPointedList<T> & operator=(const PartlyPointedList<T> &rhs);

	/// returns the first element
	T & first() { return *(elements[0]); }

	/// returns the first element
	const T & first() const { return *(elements[0]); }

	/// returns the last element
	T & last() { return *(elements.back()); }

	/// returns the last element
	const T & last() const { return *(elements.back()); }

	/// Returns an element
	T & operator[](int i) { return *(elements[i]); }

	/// Returns an element
	const T & operator[](int i) const { return *(elements[i]); }


protected:

	/// Pointers to elements
	std::vector<T*> elements;

	/// returns locally stored elements
	const std::vector<T> & getElementsLocal() const { return _elements; }

	/// returns from where is pointed to local element
	const std::vector<int> & getElementsLocalIndices() const { return pointedBy; }


private:

	/// The locally stored elements
	std::vector<T> _elements;

	/// pointer memory
	std::vector<int> pointedBy;

};

template<class T>
inline PartlyPointedList<T>::PartlyPointedList(const T & first):
	elements(1),
	_elements(1),
	pointedBy(1){
	_elements[0] = first;
	elements[0]  = & _elements[0];
	pointedBy[0] = 0;
}

template<class T>
inline PartlyPointedList<T>::PartlyPointedList(T * first):
	elements(1),
	_elements(),
	pointedBy(){
	elements[0] = first;
}

template<class T>
PartlyPointedList<T>::PartlyPointedList(const PartlyPointedList<T> & ppl){
	  _elements = ppl._elements;
	  elements  = ppl.elements;
	  pointedBy = ppl.pointedBy;
	  for(int i = 0; i < int(pointedBy.size());i++){
		  elements[pointedBy[i]] = &(_elements[i]);
	  }
}

template<class T>
PartlyPointedList<T> & PartlyPointedList<T>::operator=(const PartlyPointedList<T> &rhs){
	  if (this != &rhs) {
		  _elements = rhs._elements;
		  elements  = rhs.elements;
		  pointedBy = rhs.pointedBy;
		  for(int i = 0; i < int(pointedBy.size());i++){
			  elements[pointedBy[i]] = &(_elements[i]);
		  }
	  }
	  return *this;
}

template<class T>
inline void PartlyPointedList<T>::append(const T & element){
	_elements.push_back(element);
	elements.push_back(&_elements.back());
	pointedBy.push_back(elements.size() - 1);
}

template<class T>
inline void PartlyPointedList<T>::append(T * element){
	elements.push_back(element);
}

template<class T>
std::vector<T> PartlyPointedList<T>::getElements() const{
	std::vector<T> out(size());
	for(unsigned int i = 0; i < size(); i++){
		out[i] = *(elements[i]);
	}
	return out;
}

}

#endif /* PartlyPointedList_H_ */
