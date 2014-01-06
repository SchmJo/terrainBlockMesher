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

#include "Line2D.h"
#include "JString.h"
#include "BLIB.h"
using namespace std;

namespace jlib {

using namespace BLIB;

Line2D::Line2D(
		const Coordinate2D & start,
		const Coordinate2D & end,
		const string & name
		):
		Linear1D< Coordinate2D >
		(
				start,
				end,
				name
		){
}

Line2D::Line2D(
		const double & x0,
		const double & y0,
		const double & x1,
		const double & y1,
		const string & name
		):
		Linear1D< Coordinate2D >(name){
	start = Coordinate2D(x0,y0);
	end   = Coordinate2D(x1,y1);
}

Line2D::Line2D(
		const Linear1D< Coordinate2D > & lin
		):
		Linear1D< Coordinate2D >(lin){
}

int Line2D::coordSysRegister(CoordinateSystem2D* cs) {

	int out = 0;

	if (cs->registerObj(&start))
		out++;
	if (cs->registerObj(&end))
		out++;

	return out;
}

bool Line2D::getHitPoint(const Line2D & lineB,Coordinate2D & hitPoint) const{

	// prepare:
	const Coordinate2D u1   = end - start;
	const Coordinate2D u2   = lineB.getEnd() - lineB.getStart();
	const Coordinate2D & x1 = start;
	const Coordinate2D & x2 = lineB.getStart();

	// check parallel case:
	double det = u1.x * u2.y - u2.x * u1.y;
	if(det == 0.0){

		// collinear:
		if(u2.x != 0.0 && x1.y == x2.y - (x2.x - x1.x) * u2.y / u2.x){
			hitPoint = (x1 + x2) / 2.;
			return true;
		}
		if(u1.x != 0.0 && x2.y == x1.y - (x1.x - x2.x) * u1.y / u1.x){
			hitPoint = (x1 + x2) / 2.;
			return true;
		}

		// parallel with a distance:
		return false;
	}

	// otherwise there is a solution:
	double s1 = ((x2.x - x1.x) * u2.y - (x2.y - x1.y) * u2.x) / det;
	double s2 = ((x2.x - x1.x) * u1.y - (x2.y - x1.y) * u1.x) / det;

	// check if within line range:
	if(s1 < 0. || s1 > 1. || s2 < 0. || s2 > 1.) return false;

	// success:
	hitPoint.x = x1.x + s1 * u1.x;
	hitPoint.y = x1.y + s1 * u1.y;
	return true;

}

Outputable::OData Line2D::getOData(const string & type) const{


	vector<IO::OFILE::Data> out;
	IO::OFILE::Data dat;


	////////////////////////////////////////////////////////////
	//
	// 		VTK ascii file output:
	//
	if (type.compare(IO::OFILE::TYPE::VTK_ASCII) == 0) {

		// set pre and post fields:
		dat.name = "Line2D";
		dat.type = type;
		dat.origin = "Line2D";
		dat.pre_data = "# vtk DataFile Version 2.0 \n" + dat.name + "\nASCII \nDATASET POLYDATA\n\n"
				+ "POINTS 2 float\n";

		dat.data = String(start.x) + " " + String(start.y) + " 0\n"
				+ String(end.x) + " " + String(end.y) + " 0\n\n";

		dat.post_data = "LINES 1 3 \n2 0 1 \n";

	}

	out.push_back(dat);

	return out;


}

}
