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
    blib::MultiIndexed

Description
    See below

SourceFiles
    MultiIndexed.cpp

References
	[1] J. Schmidt, C. Peralta, B. Stoevesandt, "Automated Generation of
	    Structured Meshes for Wind Energy Applications", Proceedings of the
	    Open Source CFD International Conference, 2012, London, UK

\*---------------------------------------------------------------------------*/


#ifndef MULTIINDEXED_H_
#define MULTIINDEXED_H_

#include <vector>
#include <iostream>

namespace blib{

/**
 * @class blib::MultiIndexed
 * @brief An interface for organizing multi indexing.
 *
 * Basically a grid with equal spacing with canonical labeling
 * is defined. External indices (x,y,z) are translated into one
 * internal index (i) and vice-versa.
 *
 */
class MultiIndexed {

public:

	/// Definition of an index set:
	typedef std::vector<int> IndexSet;

	// Definition of a cell:
	typedef std::vector<IndexSet> IndexCell;

	// Definition of set of index sets:
	typedef std::vector<IndexSet> IndexSetSet;

	/// Constructor, 3 variable case
	MultiIndexed(const int n1 = 0, const int n2 = 0, const int n3 = 0);

	/// Constructor, general case
	MultiIndexed(const IndexSet & n);

	virtual ~MultiIndexed();

	/// Print the labeling of grid points
	void print_labels() const;

	/// Consistenxy check of the indexing
	bool check() const;

	/// Returns the index, given the coordinate labels, 1d case
	int x2i(const int x) const;

	/// Returns the index, given the coordinate labels, 2d case
	int x2i(const int x, const int y) const;

	/// Returns the index, given the coordinate labels, 3d case
	int x2i(const int x, const int y, const int z) const;

	/// Returns the index, given the coordinate labels, Nd case
	int x2i(const IndexSet & x) const;

	/// Returns a std::vector of positions corresponding to label i, 1d case
	int i2x_1d(const int i) const;

	/// Returns the total number of indices.
	unsigned int indexSize() const;

	/// Returns the dimension
	int dimensions() const;

	/// Returns the true dimension, ignoring zeros
	int trueDimensions() const;

	/// Returns the number of grid points in one dimension
	int getN(const int dimn) const;

	/// Returns a std::vector of positions corresponding to label i, Nd case
	IndexSet i2x(const int i) const;

	/// Returns the number of grid points per dimension
	const IndexSet & getN() const;

private:

	/// The dimensions
	int dim;

	/// The true dimensions; filtered zero entries.
	int trueDim;

	/// The size
	unsigned int size;

	/// The dimensions of the indices
	IndexSet n;

	/// Called by constructors
	void init(const IndexSet & n);

};

inline unsigned int MultiIndexed::indexSize() const {
	return size;
}

inline int MultiIndexed::dimensions() const {
	return dim;
}

inline int MultiIndexed::trueDimensions() const {
	return trueDim;
}

inline int MultiIndexed::getN(const int dimn) const {
	if(dimn >= dim || dimn < 0) return 0;
	return n[dimn];
}

inline const MultiIndexed::IndexSet & MultiIndexed::getN() const {
	return n;
}

inline int MultiIndexed::x2i(const int x) const {

	return (!dim || x < 0 || x > n[0]) ? -1 : x;

}

inline int MultiIndexed::x2i(const int x, const int y) const {

	int i = x2i(x);
	if(dim < 2) return x2i(x);
	return (i < 0 || y < 0 || y > n[1]) ? -1 :
		i + y * (n[0] > 0 ? n[0] : 1);

}

inline int MultiIndexed::x2i(const int x, const int y, const int z) const {

	int i = x2i(x, y);
	if(dim < 3) return x2i(x,y);
	return (i < 0 || z < 0 || z > n[2]) ? -1 :
		i + z * (n[0] > 0 ? n[0] : 1) * (n[1] > 0 ? n[1] : 1);

}

inline int MultiIndexed::x2i(const IndexSet & x) const {

	int i = x2i(x[0]);
	int f = n[0] > 0 ? n[0] : 1;

	for (int dd = 1; dd < dim; dd++) {

		if (i < 0 || x[dd] < 0 || x[dd] > n[dd])
			return -1;

		i += x[dd] * f;
		if(n[dd] > 0) f *= n[dd];

	}

	return i;
}

inline int MultiIndexed::i2x_1d(const int i) const {

	return (!dim || i < 0 || i >= int(size)) ? -1 : i;

}

inline MultiIndexed::IndexSet MultiIndexed::i2x(const int i) const {

	IndexSet x;

	if (dim && i >= 0 && i < int(size)) {
		int f = n[0] > 0 ? n[0] : 1;
		int a = i%f;
		x.push_back(a);

		for(int dd = 1; dd < dim; dd++){

			int ndd = n[dd] > 0 ? n[dd] : 1;

			a = (i%(f*ndd) - a) / f;
			f *= ndd;
			x.push_back(a);

		}

	}

	return x;
}

}

#endif /* MULTIINDEXED_H_ */
