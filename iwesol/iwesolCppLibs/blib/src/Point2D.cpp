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

#include "Point2D.h"
//#include "ParseComm.h"
//#include "BLIB.h"

namespace blib{

Point2D::Point2D(const vector<double> & xy):
	//Parseable("Point2D"),
	x(0),
	y(0)
	//parseSkipX(0),
	//parseSkipY(0),
	//parseBlockNameX("posx"),
	//parseBlockNameY("posy")
	{

	if(xy.size() > 1){
		x = xy[0];
		y = xy[1];
	}

}

/*
void Point2D::initParseDict(ParseDict & dictionary){

	// start with an empty dictionary:
	newParseDict(dictionary);

	// set up the dictionary entries:
	ParseDictEntry entryX("X",parseBlockNameX,1);
	ParseDictEntry entryY("Y",parseBlockNameY,1);

	// create the parse commands, and register them for killing by destructor:
	ParseComm<double>* parseX = new ParseComm<double>("x",&x,parseSkipX);
	ParseComm<double>* parseY = new ParseComm<double>("y",&y,parseSkipY);
	regForKilling(parseX);
	regForKilling(parseY);

	// Place the commands in the entries:
	entryX.addCommand(parseX);
	entryY.addCommand(parseY);

	// add the entries to the dictionary:
	dictionary.addParseDictEntry(entryX);
	dictionary.addParseDictEntry(entryY);

}
*/


}
