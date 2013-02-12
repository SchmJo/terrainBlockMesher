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
    blib::Ordered

Description
    See below

SourceFiles
    Ordered.cpp

References
	[1] J. Schmidt, C. Peralta, B. Stoevesandt, "Automated Generation of
	    Structured Meshes for Wind Energy Applications", Proceedings of the
	    Open Source CFD International Conference, 2012, London, UK

\*---------------------------------------------------------------------------*/


#ifndef ORDERED_H_
#define ORDERED_H_

#include <vector>
#include <string>

namespace blib{

/**
 * @class blib::Ordered
 * @brief An interface for ordered objects.
 *
 * The interface contains a vector of integers corresponding
 * to the ordered list of positions of the elements in a derived class.
 *
 * The order is determined by the abstract method 'i_before_j(i,j)'
 * and calculated by 'order()'.
 *
 * Note that also a method 'size()'
 * is required since it determines the size of the vector of indices.
 *
 */
class Ordered {

public:

	/// Constructor.
	Ordered(const std::string & name = "");

	virtual ~Ordered();

	/** Ordered: Specification of the dimension of the problem,
	 * ie., the number of elements to be ordered.
	 */
	virtual unsigned int size() const = 0;

	/** Ordered: Returns if element at position i is before element at position j
	 * in the original numbering. This method is used by 'order()'.
	 */
	virtual bool i_before_j(int i, int j) const = 0;

	/// Ordered: Clear memory.
	virtual void clearMemory();

	/// Returns a reference to the order vector
	inline std::vector<int> & getOrder() { return n_order;}

	/// Returns the name of the order
	inline const std::string & getName() const { return name;}

	/// Renames the order
	inline void setName(const std::string & newname) { name = newname;}

	/** Please call after adding an element to the
	 * end of the data vector, data[size()-1];
	 * performs the ordering. Returns success.
	 */
	bool orderRegNewEl();

	/// Erase the order entry after erasing element data[i]
	bool orderEraseEl(int i);

	/// Call after shifting the ordered elements by d entries
	void orderRotate(int d);

	/// Initialization.
	void orderInit();

	/** Calculates the order wrt the method 'i_before_j',
	 * based on a merge sort
	 */
	void order();

	/// Prints the current order
	void orderPrint();

	/// Derived from i_before_j
	inline bool i_after_j(int i, int j) const { return i_before_j(j,i); }

	/// Derived from i_before_j
	inline bool i_aftereq_j(int i, int j) const { return !i_before_j(i,j); }

	/// Derived from i_before_j
	inline bool i_beforeeq_j(int i, int j) const { return !i_after_j(i,j); }

	/// Derived from i_before_j
	inline bool i_equal_j(int i, int j) const { return i_aftereq_j(i,j) && !i_after_j(i,j); }

	/// Derived from i_before_j
	inline bool j_before_i(int i, int j) const { return i_beforeeq_j(i,j) && !i_equal_j(i,j); }

	/// Returns the first index
	inline int orderFirst() const { return n_order.front(); }

	/// Returns the last index
	inline int orderLast() const { return n_order.back(); }

	/// Returns an index
	inline int orderIndex(int i) const { return n_order[i]; }

	/// Returns element index at position n of the order
	inline int elementIndex(int n) const { return map_n2i[n]; }

	/// Returns first element index
	inline int firstElementIndex() const { return map_n2i.front(); }

	/// Returns last element index
	inline int lastElementIndex() const { return map_n2i.back(); }

	/** Returns the number of elements, should agree with 'size()'
	 * after ordering
	 */
	inline unsigned int orderSize() const { return n_order.size(); }


protected:

	/// The total number of indices expected
	int dim;

	/// Name of the order
	std::string name;

	/// List of indices, representing the order
	std::vector<int> n_order;

	/// The map from n_order to i_order
	std::vector<int> map_n2i;

	/// Finds where to enter the element i into the list
	int enterWhere(int i, int nmin, int nmax) const;


private:

	/** left is the index of the leftmost element of the subarray; right is one
	 * past the index of the rightmost element.
	 *
	 * Code based on
	 * http://www.cprogramming.com/tutorial/computersciencetheory/merge.html,
	 * adopted to vector<int> and i_before_j
	 */
	void __merge_helper(int left, int right, std::vector<int> & scratch);

	/** mergesort returns true on success.
	 * Elements are sorted in reverse order -- greatest to least
	 *
	 * Code based on
	 * http://www.cprogramming.com/tutorial/computersciencetheory/merge.html,
	 * adopted to vector<int>
	 */
	int __mergesort();

	/// Inverts n_order
	void __calcMap_n2i();
};

inline void Ordered::clearMemory(){
	dim = 0;
	n_order.clear();
	map_n2i.clear();
	orderInit();
}

}

#endif /* ORDERED_H_ */
