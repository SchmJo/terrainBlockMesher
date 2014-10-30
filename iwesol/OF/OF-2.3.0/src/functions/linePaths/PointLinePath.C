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

#include "OManager.h"
#include "PointLinePath.H"
#include "Globals.H"

namespace Foam
{

namespace oldev
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

PointLinePath::PointLinePath
(
		const std::string & name,
		int interpolOrder
):
		ObjectLinePath< Foam::point, PointDistance >
		(
				name,
				interpolOrder
		),
		outputPoints_(0){
}

PointLinePath::PointLinePath
(
		const Foam::point & p0,
		const Foam::point & p1,
		const std::string & name,
		int interpolOrder
):
		ObjectLinePath< Foam::point, PointDistance >
		(
				p0,
				p1,
				name,
				interpolOrder
		),
		outputPoints_(0){
}

PointLinePath::PointLinePath
(
		const Foam::point & p,
		const std::string & name,
		int interpolOrder
):
		ObjectLinePath< Foam::point, PointDistance >(p,name,interpolOrder){
}

PointLinePath::PointLinePath
(
		const pointField & points,
		const std::string & name,
		int interpolOrder
):
		ObjectLinePath< Foam::point, PointDistance >
		(
				name,
				interpolOrder
		){
	forAll(points,pI){
		addPoint(points[pI]);
	}
}

PointLinePath::PointLinePath
(
		const dictionary & dict,
		const word & keyWord
):
		ObjectLinePath< Foam::point, PointDistance >
		(
				dict,
				keyWord
		){

	// write path, if desired:
	if(dict.found("writeVTK")){
		label op(readLabel(dict.subDict("writeVTK").lookup("points")));
		setOutputPoints(op);
		writeVTK
		(
				fileName(dict.subDict("writeVTK").lookup("folder"))
		);
	} else {
		Info << "PointLinePath: keyword 'writeVTK' not found in dictionary." << endl;
	}
}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

jlib::Outputable::OData PointLinePath::getOData(const std::string & type) const
{
	std::vector<jlib::IO::OFILE::Data> out;
	jlib::IO::OFILE::Data dat;

	// prepare:
	pointField outPoints;
	if(outputPoints_ > 0){
		outPoints.resize(outputPoints_);
		scalar deltas = 1./scalar(outputPoints_ - 1);
		forAll(outPoints,opI){
			scalar s       = opI * deltas;
			outPoints[opI] = (*this)[s];
		}
	} else {
		outPoints = getPoints();
	}

	////////////////////////////////////////////////////////////
	//
	// 		VTK ascii file output:
	//
	if (type.compare(jlib::IO::OFILE::TYPE::VTK_ASCII) == 0) {

		// set pre and post fields:
		dat.name = name();
		dat.type = type;
		dat.origin = "PointLinePath";
		dat.pre_data  = "DATASET POLYDATA\n\n";
		dat.pre_data += "POINTS " + jlib::String(outPoints.size()) + " float\n";

		dat.data = "";

		forAll(outPoints,i){
			dat.data += Foam::name(chop(outPoints[i][0])) + " "
					+ Foam::name(chop(outPoints[i][1])) + " "
					+ Foam::name(chop(outPoints[i][2])) + "\n";
		}

		label lines = outPoints.size() - 1;
		dat.post_data = "\nLINES " + Foam::name(lines) + " " + Foam::name(3 * lines) + " \n";
		for(label i = 0; i < lines; i++){
			dat.post_data += "2 " + Foam::name(i) + " " + Foam::name(i+1) + "\n";
		}
	}
	out.push_back(dat);
	return out;
}

void PointLinePath::writeVTK
(
		const std::string & fileFolder
) const{
	mkDir(fileFolder);
	std::string filename = fileFolder.empty() ? "" : fileFolder + "/";
	filename += name() + ".vtk";
	jlib::OManager om(filename,jlib::IO::OFILE::TYPE::VTK_ASCII);
	om.addOLink(this);
	om.collectAll();
	om.write();
}


// ************************************************************************* //

} // End namespace oldev

} // End namespace Foam
