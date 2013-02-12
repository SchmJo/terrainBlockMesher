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

using namespace std;

#include "Point3D.h"
//#include "ParseComm.h"

namespace blib{

Point3D::Point3D(const vector<double> & xyz):
	Point2D(xyz),
	z(0)
	//parseSkipZ(0),
	//parseBlockNameZ("posz")
	{

	if(xyz.size() > 2)
		z = xyz[2];

	//dictName = "Point3D";
}

Point3D::~Point3D() {
}

/*
void Point3D::initParseDict(ParseDict & dictionary){

	// start with an empty dictionary:
	newParseDict(dictionary);

	// set up the dictionary entries:
	ParseDictEntry entryX("X",parseBlockNameX,1);
	ParseDictEntry entryY("Y",parseBlockNameY,1);
	ParseDictEntry entryZ("Z",parseBlockNameZ,1);

	// create the parse commands, and register them for killing by destructor:
	ParseComm<double>* parseX = new ParseComm<double>("x",&x,parseSkipX);
	ParseComm<double>* parseY = new ParseComm<double>("y",&y,parseSkipY);
	ParseComm<double>* parseZ = new ParseComm<double>("z",&z,parseSkipZ);
	regForKilling(parseX);
	regForKilling(parseY);
	regForKilling(parseZ);

	// Place the commands in the entries:
	entryX.addCommand(parseX);
	entryY.addCommand(parseY);
	entryZ.addCommand(parseZ);

	// add the entries to the dictionary:
	dictionary.addParseDictEntry(entryX);
	dictionary.addParseDictEntry(entryY);
	dictionary.addParseDictEntry(entryZ);

}
*/

}
