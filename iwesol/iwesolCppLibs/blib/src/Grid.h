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
    blib::Grid

Description
    See below

SourceFiles
    Grid.C

References
	[1] J. Schmidt, C. Peralta, B. Stoevesandt, "Automated Generation of
	    Structured Meshes for Wind Energy Applications", Proceedings of the
	    Open Source CFD International Conference, 2012, London, UK

\*---------------------------------------------------------------------------*/


#ifndef GRID_H_
#define GRID_H_

//#include <algorithm>

#include "MultiIndexed.h"
#include "EuclideanMetric.h"
#include "Outputable.h"
//#include "BLIB.h"

namespace blib{


/**
 * @class blib::Grid
 * @brief Abstract Class for a grid 1, 2, 3 or N dimensions.
 *
 * A Grid is a set of labeled points, forming cells and connectors.
 * Points that are mapped to the same point by map2Cart in derived
 * grids should be identified by calling identifyPoints in the
 * constructors; this reduces the size of the grid.
 *
 */
class Grid:
	public MultiIndexed,
	public Outputable {

public:

	/// Define Cartesian coordinates.
	typedef std::vector<double> COORD_CART;

	/// Define transformed coordinates.
	typedef std::vector<double> COORD_GRID;

	/// Define a connector to a grid point.
	typedef MultiIndexed::IndexSet Connector;

	/// Define a set of connectors.
	typedef std::vector<Connector> ConnectorSet;

	/// Constructor of a 1d grid.
	Grid(const int n1 = 0, const std::string & gridName = "Grid");

	/// Constructor of a 2d grid.
	Grid(const int n1, const int n2,
			const std::string & gridName = "Grid");

	/// Constructor of a 3d grid.
	Grid(const int n1, const int n2, const int n3,
			const std::string & gridName = "Grid");

	/// Constructor of a grid in arbitrary dimension
	Grid(const MultiIndexed::IndexSet & n,
			const std::string & gridName = "Grid");

	/// Destructor.
	virtual ~Grid();

	/// Grid: Checks if point lies inside the range of the grid
	virtual bool insideGrid(const COORD_GRID & p) const = 0;

	/// Grid: Returns the total cell number.
	virtual int cells() const = 0;

	/// Grid: Returns the number of the index cell surrounding a point.
	virtual int getIndexCellNr(const COORD_GRID & p) const = 0;

	/// Grid: Maps a non-Cartesian coordinate to a Cartesian coordinate
	virtual COORD_CART map2Cart(const COORD_GRID & p) const = 0;

	/// Grid: Returns the grid point in the non-Cartesian coordinates related to a label.
	virtual COORD_GRID getGridPoint(const MultiIndexed::IndexSet & x) const = 0;

	/// Grid: Returns the index cell of a given cell number.
	virtual MultiIndexed::IndexCell getIndexCell(const int cellnr) const = 0;

	/// Grid: Returns the connectors of a grid point.
	virtual ConnectorSet getConnectors(const MultiIndexed::IndexSet & x) const = 0;

	/// Print the indices.
	void print_indices() const;

	/// Print the grid points.
	void print_gridPoints() const;

	/// Print the Cartesian points.
	void print_points() const;

	/// Sets the flag for Cartesian output.
	inline void setOutputCartesian(const bool cartOutOn) { flag_output_cart = cartOutOn; }

	/// Returns flag for Cartesian output.
	inline bool getOutputCartesian() const { return flag_output_cart; }

	/// Check whether a point lies in a cell
	inline bool pointInCell(const COORD_GRID & p,
			const MultiIndexed::IndexCell & c) const {
		return getIndexCell(p) == c;
	}

	/// Check whether a point lies in a cell
	inline bool pointInCell(const COORD_GRID & p,
			const Metric::DoubleCell & c) const {
		return getCell(p) == c;
	}

	/// Checks whether two grid points are connected.
	inline bool connected(const MultiIndexed::IndexSet & x,
			const MultiIndexed::IndexSet & y) const;

	/// The number of independent grid points.
	inline unsigned int gridSize() const { return gSize; }

	/// Returns the grid index, given the coordinate labels, 1d case
	int x2g(const int x) const;

	/// Returns the grid index, given the coordinate labels, 2d case
	int x2g(const int x, const int y) const;

	/// Returns the grid index, given the coordinate labels, 3d case
	int x2g(const int x, const int y, const int z) const;

	/// Returns the grid index, given the coordinate labels, Nd case
	int x2g(const IndexSet & x) const;

	/// Returns a std::vector of positions corresponding to grid index i, 1d case
	int g2x_1d(const int i) const;

	/// Returns a std::vector of positions corresponding to grid index i, Nd case
	IndexSet g2x(const int i) const;

	/** Returns the distance between two points, defined by their indices.
	 * This used the induced metric, ie., it returns the true external distance.
	 */
	double dist_i(const MultiIndexed::IndexSet & x,
			const MultiIndexed::IndexSet & y) const;

	/** Returns the distance between two points in non-Cartesian coordinates.
	 * This used the induced metric, ie., it returns the true external distance.
	 */
	double dist(const COORD_GRID & x, const COORD_GRID & y) const;

	/// Returns the grid cell surrounding a point.
	inline Metric::DoubleCell getCell(const COORD_GRID & p) const{
		return getCell(getIndexCell(p)); }

	/// Returns the grid cell, given the index cell.
	Metric::DoubleCell getCell(const MultiIndexed::IndexCell & ic) const;

	/// Returns the grid cell, given the index cell.
	inline Metric::DoubleCell getCell(const int cellnr) const {
		return getCell(getIndexCell(cellnr));  }

	/// Returns the index cell surrounding a point.
	inline MultiIndexed::IndexCell getIndexCell(const COORD_GRID & p) const {
		return getIndexCell(getIndexCellNr(p)); }

	/// Returns the point in Cartesian coordinates related to a label.
	COORD_CART getPoint(const MultiIndexed::IndexSet & x) const;

	/// Returns the name of the grid.
	inline const std::string & getName() const { return gridName; }

	/// Provides the data that will be written to the output file.
	std::vector<IO::OFILE::Data> getOData(
			const std::string & type = IO::OFILE::TYPE::EMPTY) const;

protected:

	/// Switch for output in Cartesian coordinates via getOData.
	bool flag_output_cart;

	/// Name of the grid
	std::string gridName;

	/// The Cartesian grid is equipped with an Euclidean metric.
	EuclideanMetric eucMet;

	/// Filter from pure index to grid index
	MultiIndexed::IndexSet filterMap_i2g;

	/// Mapping from grid index to pure index.
	MultiIndexed::IndexSet filterMap_g2i;

	/** identify identical points in Cartesian space,
	 * and reduce size accordingly.
	 */
	void identifyPoints();

	/** Checks if point is identified with another point; if so,
	 * the corresponding index set is returned. if not, the index set
	 * to the point is returned.
	 */
	int filter_i2g(const int i) const;

	/** The inverse operation of filter_i2g.
	 */
	int filter_g2i(const int i) const;

	/** Checks if point is identified with another point; if so,
	 * the corresponding index set is returned. if not, the index set
	 * to the point is returned.
	 */
	MultiIndexed::IndexSet filter_i2g(const MultiIndexed::IndexSet & x) const;


private:

	/// The grid size, ie., the number of independent grid points.
	unsigned int gSize;

	/// Called by constructors.
	void __init();

};

inline int Grid::x2g(const int x) const {
	return filter_i2g(x2i(x));
}

inline int Grid::x2g(const int x, const int y) const {
	return filter_i2g(x2i(x,y));
}

inline int Grid::x2g(const int x, const int y, const int z) const{
	return filter_i2g(x2i(x,y,z));
}

inline int Grid::x2g(const IndexSet & x) const {
	return filter_i2g(x2i(x));
}

inline int Grid::g2x_1d(const int i) const {
	return i2x_1d(filter_g2i(i));
}

inline MultiIndexed::IndexSet Grid::g2x(const int i) const {
	return i2x(filter_g2i(i));
}

inline Metric::DoubleCell Grid::getCell(const MultiIndexed::IndexCell & ic) const{
	Metric::DoubleCell gc;
	for(unsigned int i = 0; i < ic.size(); i++)
		gc.push_back(Point3D(getGridPoint(ic[i])));
	return gc;
}

inline double Grid::dist(const Grid::COORD_GRID & x, const Grid::COORD_GRID & y) const{
	return eucMet.dist(map2Cart(x),map2Cart(y));
}

inline Grid::COORD_CART Grid::getPoint(const MultiIndexed::IndexSet & x) const{
	return map2Cart(getGridPoint(x));
}

inline int Grid::filter_i2g(const int i) const{
	return int(filterMap_i2g.size()) < i + 1 ? i : filterMap_i2g[i];
}

inline int Grid::filter_g2i(const int i) const{
	return int(filterMap_g2i.size()) < i + 1 ? i : filterMap_g2i[i];
}

inline MultiIndexed::IndexSet Grid::filter_i2g(
		const MultiIndexed::IndexSet & x) const{
	return filterMap_i2g.size() < indexSize() ? x :
	MultiIndexed::i2x(filter_i2g(MultiIndexed::x2i(x)));
}

}


#endif /* GRID_H_ */
