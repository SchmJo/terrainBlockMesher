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
    blib::UnstructuredGrid1D

Description
    See below

SourceFiles
    UnstructuredGrid1D.C

References
	[1] J. Schmidt, C. Peralta, B. Stoevesandt, "Automated Generation of
	    Structured Meshes for Wind Energy Applications", Proceedings of the
	    Open Source CFD International Conference, 2012, London, UK
	[2] B. Fornberg, "Generation of Finite Difference Formulas on Arbitrarily
	    Spaced Grids", Math. Comp. 51 (1988), 699-706

\*---------------------------------------------------------------------------*/

#ifndef UNSTRUCTUREDGRID1D_H_
#define UNSTRUCTUREDGRID1D_H_

#include "Grid.h"
#include "OrderedSet.h"
#include "HashStringTable.h"
#include "String.h"

namespace blib {

/**
 * @class blib::UnstructuredGrid1D
 * @brief An unstructured grid in 1D, including differentiation coefficients.
 *
 */
class UnstructuredGrid1D:
	public Grid,
	public OrderedSet<double>{

public:

	/// Constructor.
	UnstructuredGrid1D(
			bool periodic = false,
			const std::string & gridName = "UnstructuredGrid1D"
					);

	/// Constructor.
	UnstructuredGrid1D(
			const std::vector<double> & points,
			bool periodic = false,
			const std::string & gridName = "UnstructuredGrid1D"
					);

	/// Destructor.
	virtual ~UnstructuredGrid1D();

	/// UnstructuredGrid1D: adds a point
	virtual void add(double x);

	/// UnstructuredGrid1D: shifts values
	virtual void shift(double deltax);

	/// UnstructuredGrid1D: erases element i
	virtual void erase(int i);

	/// OrderedSet: Returns element i before ordering
	inline const double & getUnorderedElement(int i) const { return points[modi(i)]; }

	/// Returns the coordinate at label i
	inline const double  & getPoint1D(int i) const { return getElement(modi(i)); }

	/// Returns the first point
	inline const double & getFirstPoint1D() const { return getFirstElement(); }

	/// Returns the last point
	inline const double & getLastPoint1D() const { return getLastElement(); }

	/// Grid: Checks if point lies inside the range of the grid
	inline bool insideGrid(const COORD_GRID & p) const { return p[0] >= getFirstPoint1D() && p[0] <= getLastPoint1D() ? true : false; }

	/// Grid: Returns the total cell number.
	inline int cells() const { return int(points.size()) - 1; }

	/// Grid: Returns the number of the index cell surrounding a point.
	int getIndexCellNr(const COORD_GRID & p) const;

	/// Grid: Maps a non-Cartesian coordinate to a Cartesian coordinate
	inline COORD_CART map2Cart(const COORD_GRID & p) const { return COORD_CART(1,p[0]); }

	/// Grid: Returns the grid point in the non-Cartesian coordinates related to a label.
	inline COORD_GRID getGridPoint(const MultiIndexed::IndexSet & x) const { return COORD_GRID(1,getPoint1D(x[0])); }

	/// Grid: Returns the index cell of a given cell number.
	MultiIndexed::IndexCell getIndexCell(const int cellnr) const;

	/// Grid: Returns the connectors of a grid point.
	ConnectorSet getConnectors(const MultiIndexed::IndexSet & x) const;

	/// Returns size
	inline unsigned int size() const { return points.size(); }

	/// Returns true if empty
	inline bool empty() const { return points.empty(); }

	/// Returns the name
	inline const std::string & getName() const { return Grid::getName(); }

	/** Returns the derivative coefficients delta(m,n,i) for m-th derivative at x0, a la
	 * http://amath.colorado.edu/faculty/fornberg/Docs/MathComp_88_FD_formulas.pdf
	 * o = -1 corresponds to highest possible order o = points - m.
	 * The m-th derivative is
	 *
	 * 			d^m f / dx^m = sum_{nu = 0}^n delta(m,n,i) * f(point_near_i).
	 *
	 * The key is key(x0, m, n, i, o).
	 */
	HashStringTable<double> getDerivCoeffs_int(int x0,int m, int o = -1) const;

	/// interpolating at x0, possibly different from grid points
	HashStringTable<double> getDerivCoeffs(double x0,int m, int o = -1) const;

	/** Key for coefficient table, see
	 * http://amath.colorado.edu/faculty/fornberg/Docs/MathComp_88_FD_formulas.pdf
	 */
	std::string key(double x0, int m, int n, int i,int o) const;

	/** Returns the domain of the derivative at x0
	 * o = -1 corresponds to highest possible order o = points - m.
	 */
	std::vector<int> getDerivDomain_int(int x0, int m, int o = -1) const;

	/// interpolating at x0, possibly different from grid points
	std::vector<int> getDerivDomain(double x0, int m, int o = -1) const;

	/// prints the points
	virtual void print() const;

	/// prints a deriv domain
	void printDerivDomain(double x0, int m, int o = -1) const;

	/// prints the deriv coefficients
	void printDerivCoeffs(double x0, int m, int o) const;

	/// set periodicity
	inline void setPeriodic(double lastStepSizeX){ flag_periodic = true; lastStepX = lastStepSizeX; }

	/// unset periodicity
	inline void unsetPeriodic(){ flag_periodic = false; lastStepX = 0; }

	/// returns periodicity
	inline bool isPeriodic() const { return flag_periodic; }

	/// returns last step size
	double getLastStepSizeX() const;

	/// returns i inside range
	int modi(int i) const;

	/// returns x inside range
	double modx(double x) const;

	/// returns x range
	double getXRange() const;


protected:

	/// The grid points
	std::vector<double> points;

	/// flag for periodicity
	bool flag_periodic;

	/// the last step size for periodic case
	double lastStepX;


	/** Calculates the derivative coefficients at x0, a la
	 * http://amath.colorado.edu/faculty/fornberg/Docs/MathComp_88_FD_formulas.pdf
	 * o = -1 corresponds to highest possible order o = points - m.
	 */
	HashStringTable<double> calcDerivCoeffs(double x0, int m, int o = -1) const;


private:

	/// Returns intervall of 2N + 1 points around x0
	std::vector<int> getNearbyPointIntervall_int(int x0, int m, int o) const;

	/// interpolating at x0, possibly different from grid points
	std::vector<int> getNearbyPointIntervall(double x0, int m, int o) const;

	/// defines the point interval
	void _getPointIntervall(std::vector<int> & intervall, int iL, int iR) const;

	/// finds x position of point, including periodicity
	double _findXPeriodic(int oin) const;

};

inline std::string UnstructuredGrid1D::key(double x0, int m, int n, int i, int o) const{
	return String(x0) + "::" + String(m) + ":" + String(n) + ":" + String(i)+ ":" + String(o);
}

inline void UnstructuredGrid1D::add(double x){
	points.push_back(x);
	orderRegNewEl();
}

inline double UnstructuredGrid1D::getLastStepSizeX() const {
	return flag_periodic ? lastStepX : getLastPoint1D() - getElement(modi(size() - 2));
}

inline std::vector<int> UnstructuredGrid1D::getDerivDomain_int(int x0I, int m, int o) const{
	return getDerivDomain(getPoint1D(x0I),m,o);
}

inline std::vector<int> UnstructuredGrid1D::getDerivDomain(double x0, int m, int o) const{
	return getNearbyPointIntervall(modx(x0),m, o);
}

inline HashStringTable<double> UnstructuredGrid1D::getDerivCoeffs_int(int x0I,int m, int o) const{
	return getDerivCoeffs(getPoint1D(x0I),m,o);
}

inline HashStringTable<double> UnstructuredGrid1D::getDerivCoeffs(double x0,int m, int o) const{
	return calcDerivCoeffs(modx(x0),m,o);
}

inline double UnstructuredGrid1D::getXRange() const {
	if(size() < 2) return 0.;
	return getLastPoint1D() - getFirstPoint1D() + (flag_periodic ? lastStepX : 0.);
}

inline int UnstructuredGrid1D::modi(int i) const{
	if(!flag_periodic || size() < 2 || ( i >= 0 && i < int(size()) ) ) return i;
	if(i < 0) return modi(i + size());
	return modi(i - size());
}

inline void UnstructuredGrid1D::erase(int i){
	points.erase(points.begin() + i);
	OrderedSet<double>::orderEraseEl(i);
}


}

#endif /* UNSTRUCTUREDGRID1D_H_ */
