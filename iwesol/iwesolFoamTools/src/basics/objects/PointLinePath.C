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
    This file is part of IWESOL and it is based on OpenFOAM.

    IWESOL and OpenFOAM are free software: you can redistribute them and/or modify
    them under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    IWESOL and OpenFOAM are distributed in the hope that they will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
    or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "String.h"
#include "OManager.h"
#include "GenericRFunction.h"
#include "PointLinePath.H"

namespace Foam{
namespace iwesol{

PointLinePath::PointLinePath(
		const std::string & name
		):
		ObjectLinePath<point>(name){
}

PointLinePath::PointLinePath(
		const point & p0,
		const point & p1,
		const std::string & name
		):
		ObjectLinePath<point>(p0,p1,name){
}

PointLinePath::PointLinePath(
		const point & p,
		const std::string & name
		):
		ObjectLinePath<point>(p,name){
}

PointLinePath::PointLinePath(
		const pointField & points,
		const std::string & name
		):
		ObjectLinePath<point>(name){
	forAll(points,pI){
		addPoint(points[pI]);
	}
}

PointLinePath::PointLinePath(
		const GenericRFunction<point> & f
		):
		ObjectLinePath<point>(f){
}

PointLinePath::PointLinePath(
		const dictionary & dict,
		const word & keyWord
		):
		ObjectLinePath<point>(dict,keyWord){
}

PointLinePath::~PointLinePath() {
}






void PointLinePath::writeVTK(
		const std::string & fileFolder
		) const{

	std::string filename = fileFolder.empty() ? "" : fileFolder + "/";
	filename += getName() + ".vtk";
	blib::OManager om(filename,blib::IO::OFILE::TYPE::VTK_ASCII);
	om.addOLink(this);
	om.collectAll();
	om.write();
}


blib::Outputable::OData PointLinePath::getOData(const std::string & type) const
{
	std::vector<blib::IO::OFILE::Data> out;
	blib::IO::OFILE::Data dat;

	////////////////////////////////////////////////////////////
	//
	// 		VTK ascii file output:
	//
	if (type.compare(blib::IO::OFILE::TYPE::VTK_ASCII) == 0) {

		// set pre and post fields:
		dat.name = getName();
		dat.type = type;
		dat.origin = "PointLinePath";
		dat.pre_data  = "DATASET POLYDATA\n\n";
		dat.pre_data += "POINTS " + blib::String(size()) + " float\n";

		dat.data = "";

		for(label i = 0; i < label(size()); i++){
			dat.data += blib::String(getPoint(i)[0]) + " " + blib::String(getPoint(i)[1]) + " " + blib::String(getPoint(i)[2]) + "\n";
		}

		dat.post_data = "\nLINES " + blib::String(lines()) + " " + blib::String(3 * lines()) + " \n";
		for(label i = 0; i < label(lines()); i++){
			dat.post_data += "2 " + blib::String(i) + " " + blib::String(i+1) + "\n";
		}
	}
	out.push_back(dat);
	return out;
}

} /* iwesol */
} /* Foam */
