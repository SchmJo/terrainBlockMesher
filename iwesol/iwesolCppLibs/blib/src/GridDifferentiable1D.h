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
    blib::GridDifferentiable1D

Description
    See below

SourceFiles

References
	[1] J. Schmidt, C. Peralta, B. Stoevesandt, "Automated Generation of
	    Structured Meshes for Wind Energy Applications", Proceedings of the
	    Open Source CFD International Conference, 2012, London, UK

\*---------------------------------------------------------------------------*/

#ifndef GRIDDIFFERENTIABLE1D_H_
#define GRIDDIFFERENTIABLE1D_H_

#include "UnstructuredGrid1D.h"

namespace blib {

/**
 * @class GridDifferentiable1D
 * @brief An interface for classes that are differentiable on a 1D unstructured grid.
 *
 */
template<class T>
class GridDifferentiable1D {

public:

	/// Constructor.
	GridDifferentiable1D(UnstructuredGrid1D * grid);

	/// Destructor.
	virtual ~GridDifferentiable1D();

	/// GridDifferentiable1D: Provides data at a grid point
	virtual const T & getData(int i) const = 0;

	/** GridDifferentiable1D: Returns the m-th derivative at x0
	 * o = -1 corresponds to maximal precision, depending on
	 * number of grid points.
	 */
	virtual T derivative(double x0I, int m, int o = -1) const;


protected:

	/// resets the grid link
	inline void setGrid(UnstructuredGrid1D * g) { grid = g; }


private:

	/// Pointer to the underlying grid
	UnstructuredGrid1D * grid;
};

template<class T>
GridDifferentiable1D<T>::GridDifferentiable1D(UnstructuredGrid1D * grid):
	grid(grid){
}

template<class T>
GridDifferentiable1D<T>::~GridDifferentiable1D(){
}

template<class T>
T GridDifferentiable1D<T>::derivative(double x0, int m, int o) const{

	// prepare:
	double x                       = grid->modx(x0);
	std::vector<int> oi                 = grid->getDerivDomain(x,m,o);
	HashStringTable<double> coeffs = grid->getDerivCoeffs(x,m,o);
	unsigned int n                 = oi.size() - 1;

	// first point:
	T out = coeffs[grid->key(x,m,n,0,o)] * getData(oi[0]);

	// remaining points of domain:
	for(unsigned int i = 1; i <= n; i++){
		out += coeffs[grid->key(x,m,n,i,o)] * getData(oi[i]);
	}

	return out;
}


}

#endif /* GRIDDIFFERENTIABLE1D_H_ */
