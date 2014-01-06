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

Description

SourceFiles

References

\*---------------------------------------------------------------------------*/
#include <vector>
#include <string>
using namespace std;

#include "CoordinateSystem2D.h"
#include "JString.h"
using namespace std;

namespace jlib{

CoordinateSystem2D::CoordinateSystem2D() :
	flag_minmax_valid(0), angleRadTot(0), xmin(0), xmax(0), ymin(0), ymax(0),
			flag_angleUpdate_done(0) {
}

CoordinateSystem2D::~CoordinateSystem2D() {
}

void CoordinateSystem2D::print() {

	cerr << "## CoordinateSystem2D: Registered objects:" << endl;
	cerr << "   Scalars : " << scalars.size() << endl;
	cerr << "   Vectors : " << vectors.size() << endl;

	if(flag_minmax_valid){
		cerr << "## CoordinateSystem2D: Coordinate boundaries:" << endl;
		cerr << "   xmin = " << xmin << ", xmax = " << xmax <<endl;
		cerr << "   ymin = " << ymin << ", ymax = " << ymax <<endl;
	}

	/*
	// Print all positions:
	vector<VectorLike2D*>::iterator it = vectors.begin();
	while(it < vectors.end()){
		cout<< it -vectors.begin() << ": "<<(*it)->getX()<<" / "<<(*it)->getY()<<endl;
		it++;
	}
	 */
}

void CoordinateSystem2D::printGraphVectors(int xrange, int yrange) {

	if(vectors.empty()) return;
	if(!flag_minmax_valid) calcMinMax();

	double deltax = (xmax-xmin)/(xrange-1);
	double deltay = (ymax-ymin)/(yrange-1);

	cout<<" XX";
	for(int h = 0; h<=xrange+3; h++) cout << "X";
	cout << "XX "<<endl;

	for(int l = 0; l<yrange; l++){

		String line = "";
		for(int h = 0; h<=xrange+3; h++) line += " ";

		vector<VectorLike2D*>::iterator it = vectors.begin();
		while(it < vectors.end()){

			double x = (*it)->getX();
			double y = (*it)->getY();

			if(y - ymin >= l*deltay && y - ymin < (l+1)*deltay){

				int xpos = (x-xmin)/deltax;
				int nr = it-vectors.begin();
				line.insert(xpos,String(nr));
				line = line.substr(0,xrange+4);

			}

			it++;
		}

		cout << "Y  " << line << "  Y" <<endl;

	}

	cout<<" XX";
	for(int h = 0; h<=xrange+3; h++) cout << "X";
	cout << "XX "<<endl;


}

void CoordinateSystem2D::calcMinMax(){

	if(vectors.empty()) return;

	vector<VectorLike2D*>::iterator it = vectors.begin();

	double x = (*it)->getX();
	double y = (*it)->getY();

	xmin = x; xmax = x;
	ymin = y; ymax = y;

	while(it < vectors.end()){

		x = (*it)->getX();
		y = (*it)->getY();

		if(x < xmin) xmin = x;
		if(x > xmax) xmax = x;
		if(y < ymin) ymin = y;
		if(y > ymax) ymax = y;

		it++;
	}
	flag_minmax_valid = 1;

}

void CoordinateSystem2D::rotate(double angleRad) {

	if (angleRad == 0 || angleRad == TWOPI)
		return;

	// Update total angle:
	angleRadTot += angleRad;
	while (angleRadTot > TWOPI)
		angleRadTot -= TWOPI;
	flag_angleUpdate_done = 1;

	rotate(cos(angleRad), sin(angleRad));

}

void CoordinateSystem2D::rotate(double cosval, double sinval) {

	if (cosval == 0)
		return;

	if (!scalars.empty()) {
		vector<ScalarLike2D*>::iterator it = scalars.begin();
		while (it < scalars.end()) {
			(*it)->rotate(cosval, -sinval);
			it++;
		}
	}

	if (!vectors.empty()) {
		vector<VectorLike2D*>::iterator it = vectors.begin();
		int counter = 1;
		while (it < vectors.end()) {
			(*it)->rotate(cosval, -sinval);
			it++;
			counter++;
		}
	}

	// Update total angle:
	if (!flag_angleUpdate_done) {
		angleRadTot += acos(cosval);
		while (angleRadTot > TWOPI)
			angleRadTot -= TWOPI;
	}

	flag_minmax_valid = 0;

}

void CoordinateSystem2D::translate(double delta_x, double delta_y) {

	if (!scalars.empty()) {
		vector<ScalarLike2D*>::iterator it = scalars.begin();
		while (it < scalars.end()) {
			(*it)->translate(-delta_x, -delta_y);
			it++;
		}
	}

	if (!vectors.empty()) {
		vector<VectorLike2D*>::iterator it = vectors.begin();
		while (it < vectors.end()) {
			(*it)->translate(-delta_x, -delta_y);
			it++;
		}
	}

	flag_minmax_valid = 0;

}

void CoordinateSystem2D::translate(const Point2D & shift){
	translate(shift.x, shift.y);
}

void CoordinateSystem2D::rescale(double factor) {

	if (!scalars.empty()) {
		vector<ScalarLike2D*>::iterator it = scalars.begin();
		while (it < scalars.end()) {
			(*it)->rescale(factor);
			it++;
		}
	}

	if (!vectors.empty()) {
		vector<VectorLike2D*>::iterator it = vectors.begin();
		while (it < vectors.end()) {
			(*it)->rescale(factor);
			it++;
		}
	}

	flag_minmax_valid = 0;

}

bool CoordinateSystem2D::registerObj(Geometrical2D* obj) {

	// check for scalar:
	ScalarLike2D* scal = dynamic_cast<ScalarLike2D*> (obj);
	if (scal) {
		scalars.push_back(scal);
		return true;
	}

	// check for vector:
	VectorLike2D* vec = dynamic_cast<VectorLike2D*> (obj);
	if (vec) {
		vectors.push_back(vec);
		flag_minmax_valid = 0;
		return true;
	}

	return false;
}

}
