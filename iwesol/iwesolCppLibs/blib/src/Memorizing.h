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
    blib::Memorizing

Description
    See below

SourceFiles

References
	[1] J. Schmidt, C. Peralta, B. Stoevesandt, "Automated Generation of
	    Structured Meshes for Wind Energy Applications", Proceedings of the
	    Open Source CFD International Conference, 2012, London, UK

\*---------------------------------------------------------------------------*/

#ifndef MEMORIZING_H_
#define MEMORIZING_H_

#include <vector>
#include "OrderedSet.h"

namespace blib{

/**
 * @class blib::Memorizing
 * @brief An interface for classes that want to store data T under a unique label U.
 *
 * The labels U are ordered.
 *
 */
template <class T, class U>
class Memorizing:
	public OrderedSet<U>{

public:

	/// Constructor.
	explicit Memorizing(int size = 0);

	/// Constructor.
	explicit Memorizing(
			const std::vector<T> & data,
			const std::vector<U> & labels
			);

	/// Destructor.
	virtual ~Memorizing(){}

	/// OrderedSet: Returns element i before ordering
	const U & getUnorderedElement(int i) const { return __memLabels[i]; }

	/// Ordered: Clear memory.
	virtual void clearMemory() { __memData.clear(); __memLabels.clear(); OrderedSet<U>::clearMemory(); }

	/** Ordered: Specification of the dimension of the problem,
	 * ie., the number of elements to be ordered.
	 */
	inline unsigned int size() const { return fillSize; }

	/// The current storage size
	inline unsigned int storageSize() const { return __memLabels.size(); }

	/// resize storage size
	inline void resize(const int size) { __memLabels.resize(size); __memData.resize(size); }

	/// Erases an entry.
	void erase(const U & label);

	/// Memorize data under a label. Returns success.
	bool memorize(const T & data, const U & label);

	/// Recall data from memory.
	bool recall(T & data, const U & label) const;

	/// Return emptiness.
	inline bool empty() { return __memLabels.empty(); }

	/// Returns the ordered labels.
	inline std::vector<U> getLabels() const { return OrderedSet<U>::getElements(); }

	/// Returns a data entry.
	inline const T & getMemData(const int i) const { return __memData[i]; }

	/// Returns a label.
	inline const U & getMemLabel(const int i) const { return __memLabels[i]; }

	/// Returns the labels.
	inline const std::vector<U> & getMemLables() const { return __memLabels; }


protected:

	/// Returns a data entry.
	T & getMemData(const int i) { return __memData[i]; }


private:

	/// The data container.
	std::vector<T> __memData;

	/// The label container.
	std::vector<U> __memLabels;

	/// fill size
	unsigned int fillSize;

};

template <class T,class U>
Memorizing<T,U>::Memorizing(int size):
	__memData(size),
	__memLabels(size),
	fillSize(0){
	Ordered::orderInit();
}

template <class T,class U>
Memorizing<T,U>::Memorizing(
			const std::vector<T> & data,
			const std::vector<U> & labels
			):
			__memData(data),
			__memLabels(labels),
			fillSize(labels.size()){
	Ordered::orderInit();
}

template <class T,class U>
void Memorizing<T,U>::erase(const U & label){

	// find index:
	const int l = find(label);
	if(l < 0) return;

	// erase:
	__memData.erase(__memData.begin() + l);
	__memLabels.erase(__memLabels.begin() + l);
	Ordered::orderEraseEl(l);

}

template <class T,class U>
bool Memorizing<T,U>::memorize(const T & data, const U & label){

	const int i = OrderedSet<U>::find(label);
	if(i < 0){
		if(fillSize < __memLabels.size()){
			__memData[fillSize]   = data;
			__memLabels[fillSize] = label;
		} else {
			__memData.push_back(data);
			__memLabels.push_back(label);
		}
		fillSize++;
		return Ordered::orderRegNewEl();
	} else {
		__memData[Ordered::elementIndex(i)] = data;
	}
	return true;
}

template <class T,class U>
inline bool Memorizing<T,U>::recall(T & data, const U & label) const{

	// find index:
	const int l = OrderedSet<U>::find(label);
	if(l < 0) return false;

	data = __memData[Ordered::elementIndex(l)];
	return true;
}


}

#endif /* MEMORIZING_H_ */
