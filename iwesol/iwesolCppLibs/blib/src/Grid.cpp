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
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright  held by original author
     \\/     M anipulation  |
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

\*---------------------------------------------------------------------------*/

#include <algorithm>
using namespace std;

#include "Grid.h"
#include "String.h"

namespace blib{

Grid::Grid(const int nx, const string & name) :
	MultiIndexed(nx), flag_output_cart(true), gridName(name) {
	__init();
}

Grid::Grid(const int nx, const int ny, const string & name) :
	MultiIndexed(nx, ny), flag_output_cart(true), gridName(name) {
	__init();
}

Grid::Grid(const int nx, const int ny, const int nz, const string & name) :
	MultiIndexed(nx, ny, nz), flag_output_cart(true), gridName(name) {
	__init();
}

Grid::Grid(const vector<int> & hn, const string & name) :
	MultiIndexed(hn), flag_output_cart(true), gridName(name) {
	__init();
}

Grid::~Grid() {
}

void Grid::__init() {
	gSize = indexSize();

	if (trueDimensions() != dimensions())
		cout << gridName << ": dim = " << dimensions() << ", trueDim = "
				<< trueDimensions() << ", index range = " << String(getN())
				<< ", index size = " << indexSize() << endl;

}

void Grid::identifyPoints() {

	// clear lists:
	filterMap_i2g.clear();
	filterMap_g2i.clear();

	// prepare:
	MultiIndexed::IndexSetSet identical;
	vector<COORD_CART> checked;
	vector<int> checked_i;
	vector<COORD_CART> doubles;
	int duplicates = 0;
	int actLabel = 0;

	// loop:
	for (unsigned int i = 0; i < indexSize(); i++) {

		// current index set:
		MultiIndexed::IndexSet is = i2x(i);

		// get Cartesian point:
		COORD_CART cp = getPoint(is);

		// check if this has been found before:
		vector<COORD_CART>::iterator bef = find(checked.begin(), checked.end(),
				cp);

		// yes, this is a duplicate:
		if (bef != checked.end()) {

			// the old label:
			int i_old = 0;

			// search the original:
			vector<COORD_CART>::iterator found = find(doubles.begin(),
					doubles.end(), cp);

			// yes, found it:
			if (found != doubles.end()) {

				// get position:
				int pos = found - doubles.begin();
				i_old = (identical[pos])[0];

				// write:
				identical[pos].push_back(i);

				// tell:
				cout << gridName << ": Found duplicate. Identify ("
						<< String(is) << ") with (" << String(i2x(i_old))
						<< ")." << endl;

				// this reduces the grid size, so count:
				duplicates++;

			}

			// no, it's new:
			else {

				// add to lists:
				doubles.push_back(cp);
				i_old = bef - checked.begin();
				MultiIndexed::IndexSet newpair(2);
				newpair[0] = i_old;
				newpair[1] = i;
				identical.push_back(newpair);

				// tell:
				cout << gridName << ": Found duplicate. Identify ("
						<< String(is) << ") with (" << String(i2x(
						(identical.back())[0])) << ")" << endl;

				// this reduces the grid size, so count:
				duplicates++;

			}

			// write original label to filter:
			filterMap_i2g.push_back(i_old);

		}

		// no, this is not a duplicate:
		else {

			// give label:
			filterMap_i2g.push_back(actLabel);
			filterMap_g2i.push_back(i);
			actLabel++;

			// mark as checked:
			checked.push_back(cp);
			checked_i.push_back(i);

		}

	}

	// reduce grid size:
	gSize -= duplicates;

	cout<<"i2gMap: " << endl;
	cout<<String(filterMap_i2g)<<endl;
	cout<<"g2iMap: " << endl;
	cout<<String(filterMap_g2i)<<endl;

	cout << gridName << ": Index size = " << indexSize() << ", grid size = "
			<< gridSize() << endl;

}

void Grid::print_indices() const {

	// check indexing:
	if (!check()) {
		print_labels();
		return;
	}

	for (unsigned int i = 0; i < indexSize(); i++) {

		// get index set:
		MultiIndexed::IndexSet is = i2x(i);

		cout << "i = " << i << " --> ";
		for (unsigned int j = 0; j < is.size(); j++) {
			cout << "x(" << j << ") = " << is[j] << "  ";
		}
		cout << "---> i = " << x2i(is);
		cout << endl;
	}

}

void Grid::print_gridPoints() const {

	// check indexing:
	if (!check()) {
		print_labels();
		return;
	}

	for (unsigned int i = 0; i < gridSize(); i++) {

		// get index set:
		MultiIndexed::IndexSet is = i2x(i);

		// get grid point:
		COORD_GRID gp = getGridPoint(is);

		cout << "i = " << i << " --> ";
		for (unsigned int j = 0; j < gp.size(); j++) {
			cout << "x(" << j << ") = " << gp[j] << "  ";
		}

		cout << endl;
	}

}

void Grid::print_points() const {

	// check indexing:
	if (!check()) {
		print_labels();
		return;
	}

	for (unsigned int i = 0; i < gridSize(); i++) {

		// get index set:
		MultiIndexed::IndexSet is = i2x(i);

		// get Cartesian point:
		COORD_CART cp = getPoint(is);

		cout << "i = " << i << " --> ";
		for (unsigned int j = 0; j < cp.size(); j++) {
			cout << "x(" << j << ") = " << cp[j] << "  ";
		}

		cout << endl;
	}

}

bool Grid::connected(const MultiIndexed::IndexSet & x,
		const MultiIndexed::IndexSet & y) const {

	// simple checks:
	if (x.empty() || y.empty())
		return false;
	if (x == y)
		return true;

	// get connectors of x:
	ConnectorSet csx = getConnectors(x);
	if (csx.empty())
		return false;

	// search for y among the connectors:
	ConnectorSet::iterator fy = find(csx.begin(), csx.end(), y);

	return fy != csx.end();
}

double Grid::dist_i(const MultiIndexed::IndexSet & x,
		const MultiIndexed::IndexSet & y) const {

	// trivial case:
	if (x.empty() || x.size() != y.size())
		return 0;

	// get space positions:
	COORD_GRID X = getGridPoint(x);
	COORD_GRID Y = getGridPoint(y);

	// apply metric:
	return dist(X, Y);

}

vector<IO::OFILE::Data> Grid::getOData(const string & type) const {

	vector<IO::OFILE::Data> out;

	////////////////////////////////////////////////////////////
	//
	// 		Text file output:
	//
	if (type.compare(IO::OFILE::TYPE::TXT) == 0) {

		// add a piece of data: points
		IO::OFILE::Data dat;

		// set pre and post fields:
		dat.type = type;
		dat.origin = gridName;

		// Output Cartesian points:
		if (flag_output_cart) {

			dat.name = "grid_points_cartesian";
			dat.pre_data = "// " + gridName + ": " + String(gSize)
					+ " points in Cartesian space (" + String(getN()) + ")\n";

			for (unsigned int g = 0; g < gridSize(); ++g) {

				// pick a coordinate:
				const MultiIndexed::IndexSet coord = g2x(g);

				// get Cartesian coordinate:
				COORD_GRID x = getPoint(coord);

				// write:
				dat.data += String(x) + "   ";

			}

		}

		// grid point output:
		else {

			dat.name = "grid_points";
			dat.pre_data = "// " + gridName + ": " + String(indexSize())
					+ " points in grid space (" + String(getN()) + ")\n";

			for (unsigned int i = 0; i < indexSize(); i++) {

				// pick a coordinate:
				MultiIndexed::IndexSet coord = i2x(i);

				// get Cartesian coordinate:
				COORD_GRID x = getGridPoint(coord);

				// write:
				dat.data += String(x) + "   ";

			}

		}

		// write points:
		dat.data += "\n";
		out.push_back(dat);

	}

	////////////////////////////////////////////////////////////
	//
	// 		VTK ascii file output:
	//
	else if (type.compare(IO::OFILE::TYPE::VTK_ASCII) == 0) {

		// add a piece of data: points
		IO::OFILE::Data dat;

		// set pre and post fields:
		dat.type = type;
		dat.origin = gridName;

		// Output Cartesian points:
		if (flag_output_cart) {

			// meta data for grid points:
			vector<int> dims = getN();
			dims.resize(3, 1);
			dat.name = "grid_points_cartesian";
			dat.pre_data = String("DATASET UNSTRUCTURED_GRID\n")
				//	+ "DIMENSIONS " + IO::vi2s(dims) + "\n"
					+ "POINTS " + String(gridSize()) + " \n";

			for (unsigned int g = 0; g < gridSize(); g++) {

				// pick a coordinate:
				MultiIndexed::IndexSet coord = g2x(g);

				// get Cartesian coordinate:
				COORD_GRID x = getPoint(coord);

				// vtk expects 3 dimensions:
				x.resize(3, 0);

				// write:
				dat.data += String(x) + "   ";

			}

		}

		// grid point output:
		else {

			// meta data for grid points:
			vector<int> dims = getN();
			dims.resize(3, 1);
			dat.name = "grid_points";
			dat.pre_data = String("DATASET STRUCTURED_GRID\n")
					+ "DIMENSIONS " + String(dims) + "\n"
					+ "POINTS " + String(indexSize()) + " \n";

			for (unsigned int i = 0; i < indexSize(); i++) {

				// pick a coordinate:
				MultiIndexed::IndexSet coord = i2x(i);

				// get Cartesian coordinate:
				COORD_GRID x = getPoint(coord);

				// vtk expects 3 dimensions:
				x.resize(3, 0);

				// write:
				dat.data += String(x) + "   ";

			}

		}

		// write points:
		dat.data += "\n";
		out.push_back(dat);

		// add new piece of data: cells
		dat = IO::OFILE::Data();

		// set pre and post fields:
		dat.type = type;
		dat.origin = gridName;
		dat.pre_data = "CELLS " + String(cells()) + " ";
		dat.data = "";

		// Output in Cartesian space:
		if (flag_output_cart) {

			dat.name = "grid_cells_cartesian";

			// count data output:
			int dcounter = 0;

			// cell output:
			for(int i = 0; i < cells(); i++) {

				// get cell:
				MultiIndexed::IndexCell cell = getIndexCell(i);

				// grid filter:
				for(unsigned j = 0; j < cell.size(); j++)
					cell[j] = filter_i2g(cell[j]);
				reduceDup_v(cell);

				// write cell:
				dat.data += String(cell.size());
				dcounter++;
				for(unsigned j = 0; j < cell.size(); j++){
					cout<<String(cell[j])<<" -> "<<String(x2g(cell[j]))<<endl;
					dat.data += " " + String(x2g(cell[j]));
					dcounter++;
				}
				cout<<endl;
				dat.data += "\n";

			}

			dat.pre_data += String(dcounter) + "\n";

		}

		dat.data += "\n";
		out.push_back(dat);

	}

	return out;

}

}

