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

#include "BasicBlock.H"

namespace Foam{
namespace iwesol{

const label BasicBlock::SWL = 0;
const label BasicBlock::SEL = 1;
const label BasicBlock::NEL = 2;
const label BasicBlock::NWL = 3;
const label BasicBlock::SWH = 4;
const label BasicBlock::SEH = 5;
const label BasicBlock::NEH = 6;
const label BasicBlock::NWH = 7;

const label BasicBlock::X = 0;
const label BasicBlock::Y = 1;
const label BasicBlock::Z = 2;

const label BasicBlock::NONE   = -1;
const label BasicBlock::WEST   = 0;
const label BasicBlock::EAST   = 1;
const label BasicBlock::NORTH  = 2;
const label BasicBlock::SOUTH  = 3;
const label BasicBlock::GROUND = 4;
const label BasicBlock::SKY    = 5;

label BasicBlock::getConstantDirectionFace(label faceLabel){
	if(faceLabel == WEST || faceLabel == EAST) return X;
	if(faceLabel == SOUTH || faceLabel == NORTH) return Y;
	if(faceLabel == GROUND || faceLabel == SKY) return Z;
	return -1;
}

label BasicBlock::getOppositeFace(label faceLabel){
	if(faceLabel == WEST) return EAST;
	if(faceLabel == EAST) return WEST;
	if(faceLabel == NORTH) return SOUTH;
	if(faceLabel == SOUTH) return NORTH;
	if(faceLabel == SKY) return GROUND;
	if(faceLabel == GROUND) return SKY;
	return NONE;
}


labelList BasicBlock::getFaceI(label i) const{

	labelList out(4,NONE);

	if(i == WEST){
		out[0] = verticesI[SWL];
		out[1] = verticesI[SWH];
		out[2] = verticesI[NWH];
		out[3] = verticesI[NWL];
	} else if(i == EAST){
		out[0] = verticesI[NEL];
		out[1] = verticesI[SEL];
		out[2] = verticesI[SEH];
		out[3] = verticesI[NEH];
	} else if(i == NORTH){
		out[0] = verticesI[NWL];
		out[1] = verticesI[NWH];
		out[2] = verticesI[NEH];
		out[3] = verticesI[NEL];
	} else if(i == SOUTH){
		out[0] = verticesI[SWL];
		out[1] = verticesI[SEL];
		out[2] = verticesI[SEH];
		out[3] = verticesI[SWH];
	} else if(i == GROUND){
		out[0] = verticesI[SWL];
		out[1] = verticesI[NWL];
		out[2] = verticesI[NEL];
		out[3] = verticesI[SEL];
	} else if(i == SKY){
		out[0] = verticesI[SWH];
		out[1] = verticesI[SEH];
		out[2] = verticesI[NEH];
		out[3] = verticesI[NWH];
	}

	return out;
}

} /* iwesol */
} /* Foam */
