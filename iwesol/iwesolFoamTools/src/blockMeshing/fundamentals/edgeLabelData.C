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

#include "SplineBlock.H"

namespace Foam{
namespace iwesol{

const label SplineBlock::SWL_SEL = 0;
const label SplineBlock::SWL_NWL = 1;
const label SplineBlock::SEL_NEL = 2;
const label SplineBlock::NEL_NWL = 3;
const label SplineBlock::SEL_SWL = 4;
const label SplineBlock::NWL_SWL = 5;
const label SplineBlock::NEL_SEL = 6;
const label SplineBlock::NWL_NEL = 7;

const label SplineBlock::SEH_NEH = 8;
const label SplineBlock::NWH_SWH = 9;
const label SplineBlock::NEH_SEH = 10;
const label SplineBlock::SWH_NWH = 11;

const label SplineBlock::SEL_SEH = 12;
const label SplineBlock::NEL_NEH = 13;
const label SplineBlock::SEH_SEL = 14;
const label SplineBlock::NEH_NEL = 15;

const label SplineBlock::SWH_SEH = 16;
const label SplineBlock::SEH_SWH = 17;
const label SplineBlock::NWH_NEH = 18;
const label SplineBlock::NEH_NWH = 19;

const label SplineBlock::SWL_SWH = 20;
const label SplineBlock::SWH_SWL = 21;
const label SplineBlock::NWL_NWH = 22;
const label SplineBlock::NWH_NWL = 23;

label SplineBlock::splineStartFace(label splineLabel){
	if(
			splineLabel == SWL_SEL ||
			splineLabel == NWL_NEL ||
			splineLabel == SWH_SEH ||
			splineLabel == NWH_NEH
			) return WEST;
	if(
			splineLabel == SEL_SWL ||
			splineLabel == NEL_NWL ||
			splineLabel == SEH_SWH ||
			splineLabel == NEH_NWH
			) return EAST;
	if(
			splineLabel == SWL_NWL ||
			splineLabel == SEL_NEL ||
			splineLabel == SWH_NWH ||
			splineLabel == SEH_NEH
			) return SOUTH;
	if(
			splineLabel == NWL_SWL ||
			splineLabel == NEL_SEL ||
			splineLabel == NWH_SWH ||
			splineLabel == NEH_SEH
			) return NORTH;
	if(
			splineLabel == SWL_SWH ||
			splineLabel == SEL_SEH ||
			splineLabel == NWL_NWH ||
			splineLabel == NEL_NEH
			) return GROUND;
	if(
			splineLabel == SWH_SWL ||
			splineLabel == SEH_SEL ||
			splineLabel == NWH_NWL ||
			splineLabel == NEH_NEL
			) return SKY;
	return NONE;
}

label SplineBlock::splineEndFace(label splineLabel){
	if(
			splineLabel == SWL_SEL ||
			splineLabel == NWL_NEL ||
			splineLabel == SWH_SEH ||
			splineLabel == NWH_NEH
			) return EAST;
	if(
			splineLabel == SEL_SWL ||
			splineLabel == NEL_NWL ||
			splineLabel == SEH_SWH ||
			splineLabel == NEH_NWH
			) return WEST;
	if(
			splineLabel == SWL_NWL ||
			splineLabel == SEL_NEL ||
			splineLabel == SWH_NWH ||
			splineLabel == SEH_NEH
			) return NORTH;
	if(
			splineLabel == NWL_SWL ||
			splineLabel == NEL_SEL ||
			splineLabel == NWH_SWH ||
			splineLabel == NEH_SEH
			) return SOUTH;
	if(
			splineLabel == SWL_SWH ||
			splineLabel == SEL_SEH ||
			splineLabel == NWL_NWH ||
			splineLabel == NEL_NEH
			) return SKY;
	if(
			splineLabel == SWH_SWL ||
			splineLabel == SEH_SEL ||
			splineLabel == NWH_NWL ||
			splineLabel == NEH_NEL
			) return GROUND;
	return NONE;
}

label SplineBlock::switchedOrientationLabel(label splineLabel){
	labelList vl = getSplineVerticesI(splineLabel);
	return getSplineLabel(vl[1],vl[0]);
}

labelList SplineBlock::getConstantDirectionsEdge(label splineLabel){

	labelList out;
	for(label fI = 0; fI < 6; fI++){
		if(edgeBelongsToFace(splineLabel,fI)){
			out.append(getConstantDirectionFace(fI));
		}
	}

	return out;
}

label SplineBlock::getDirectionEdge(label splineLabel){

	labelList cdir = getConstantDirectionsEdge(splineLabel);
	if(cdir.size() != 2) return -1;
	if(cdir[0] == X && cdir[1] == Y) return Z;
	if(cdir[1] == X && cdir[0] == Y) return Z;
	if(cdir[0] == X && cdir[1] == Z) return Y;
	if(cdir[1] == X && cdir[0] == Z) return Y;
	if(cdir[0] == Y && cdir[1] == Z) return X;
	if(cdir[1] == Y && cdir[0] == Z) return X;
	return -1;
}

bool SplineBlock::edgeBelongsToFace(label splineLabel, label faceLabel){

	labelList fs = getFaceSplinesI(faceLabel);
	forAll(fs,sI){
		if(fs[sI] == splineLabel) return true;
	}
	return false;
}

labelList SplineBlock::getFaceSplinesI(label faceI){

	labelList out(8);

	if(faceI == WEST){
		out[0] = SWL_SWH;
		out[1] = NWL_NWH;
		out[2] = SWH_NWH;
		out[3] = SWL_NWL;

		out[4] = SWH_SWL;
		out[5] = NWH_NWL;
		out[6] = NWH_SWH;
		out[7] = NWL_SWL;
	}
	if(faceI == EAST){
		out[0] = SEL_SEH;
		out[1] = NEL_NEH;
		out[2] = SEH_NEH;
		out[3] = SEL_NEL;

		out[4] = SEH_SEL;
		out[5] = NEH_NEL;
		out[6] = NEH_SEH;
		out[7] = NEL_SEL;
	}
	if(faceI == SOUTH){
		out[0] = SEL_SEH;
		out[1] = SWL_SWH;
		out[2] = SEH_SWH;
		out[3] = SEL_SWL;

		out[4] = SEH_SEL;
		out[5] = SWH_SWL;
		out[6] = SWH_SEH;
		out[7] = SWL_SEL;
	}
	if(faceI == NORTH){
		out[0] = NEL_NEH;
		out[1] = NWL_NWH;
		out[2] = NEH_NWH;
		out[3] = NEL_NWL;

		out[4] = NEH_NEL;
		out[5] = NWH_NWL;
		out[6] = NWH_NEH;
		out[7] = NWL_NEL;
	}
	if(faceI == GROUND){
		out[0] = SWL_SEL;
		out[1] = NWL_NEL;
		out[2] = SEL_NEL;
		out[3] = SWL_NWL;

		out[4] = SEL_SWL;
		out[5] = NEL_NWL;
		out[6] = NEL_SEL;
		out[7] = NWL_SWL;
	}
	if(faceI == SKY){
		out[0] = SWH_SEH;
		out[1] = NWH_NEH;
		out[2] = SEH_NEH;
		out[3] = SWH_NWH;

		out[4] = SEH_SWH;
		out[5] = NEH_NWH;
		out[6] = NEH_SEH;
		out[7] = NWH_SWH;
	}

	return out;
}

label SplineBlock::getSplineLabel(label vertex1, label vertex2){

	label out = -1;

	if(vertex1 == SWL && vertex2 == NWL) return SWL_NWL;
	if(vertex1 == SWL && vertex2 == SEL) return SWL_SEL;
	if(vertex1 == NEL && vertex2 == NWL) return NEL_NWL;
	if(vertex1 == NEL && vertex2 == SEL) return NEL_SEL;
	if(vertex2 == SWL && vertex1 == NWL) return NWL_SWL;
	if(vertex2 == SWL && vertex1 == SEL) return SEL_SWL;
	if(vertex2 == NEL && vertex1 == NWL) return NWL_NEL;
	if(vertex2 == NEL && vertex1 == SEL) return SEL_NEL;

	if(vertex1 == SEH && vertex2 == NEH) return SEH_NEH;
	if(vertex1 == SWH && vertex2 == NWH) return SWH_NWH;
	if(vertex2 == SEH && vertex1 == NEH) return NEH_SEH;
	if(vertex2 == SWH && vertex1 == NWH) return NWH_SWH;

	if(vertex1 == SEL && vertex2 == SEH) return SEL_SEH;
	if(vertex1 == NEL && vertex2 == NEH) return NEL_NEH;
	if(vertex2 == SEL && vertex1 == SEH) return SEH_SEL;
	if(vertex2 == NEL && vertex1 == NEH) return NEH_NEL;

	if(vertex1 == SWH && vertex2 == SEH) return SWH_SEH;
	if(vertex1 == NWH && vertex2 == NEH) return NWH_NEH;
	if(vertex2 == SWH && vertex1 == SEH) return SEH_SWH;
	if(vertex2 == NWH && vertex1 == NEH) return NEH_NWH;

	if(vertex1 == SWL && vertex2 == SWH) return SWL_SWH;
	if(vertex1 == NWL && vertex2 == NWH) return NWL_NWH;
	if(vertex2 == SWL && vertex1 == SWH) return SWH_SWL;
	if(vertex2 == NWL && vertex1 == NWH) return NWH_NWL;

	return out;
}

labelList SplineBlock::getSplineVerticesI(label splineLabel){

	labelList out(2);

	if(splineLabel == SWL_NWL) { out[0] = SWL; out[1] = NWL; return out; }
	if(splineLabel == SWL_SEL) { out[0] = SWL; out[1] = SEL; return out; }
	if(splineLabel == NEL_NWL) { out[0] = NEL; out[1] = NWL; return out; }
	if(splineLabel == NEL_SEL) { out[0] = NEL; out[1] = SEL; return out; }
	if(splineLabel == NWL_SWL) { out[1] = SWL; out[0] = NWL; return out; }
	if(splineLabel == SEL_SWL) { out[1] = SWL; out[0] = SEL; return out; }
	if(splineLabel == NWL_NEL) { out[1] = NEL; out[0] = NWL; return out; }
	if(splineLabel == SEL_NEL) { out[1] = NEL; out[0] = SEL; return out; }

	if(splineLabel == SEH_NEH) { out[0] = SEH; out[1] = NEH; return out; }
	if(splineLabel == SWH_NWH) { out[0] = SWH; out[1] = NWH; return out; }
	if(splineLabel == NEH_SEH) { out[1] = SEH; out[0] = NEH; return out; }
	if(splineLabel == NWH_SWH) { out[1] = SWH; out[0] = NWH; return out; }

	if(splineLabel == SEL_SEH) { out[0] = SEL; out[1] = SEH; return out; }
	if(splineLabel == NEL_NEH) { out[0] = NEL; out[1] = NEH; return out; }
	if(splineLabel == SEH_SEL) { out[1] = SEL; out[0] = SEH; return out; }
	if(splineLabel == NEH_NEL) { out[1] = NEL; out[0] = NEH; return out; }

	if(splineLabel == SWH_SEH) { out[0] = SWH; out[1] = SEH; return out; }
	if(splineLabel == NWH_NEH) { out[0] = NWH; out[1] = NEH; return out; }
	if(splineLabel == SEH_SWH) { out[1] = SWH; out[0] = SEH; return out; }
	if(splineLabel == NEH_NWH) { out[1] = NWH; out[0] = NEH; return out; }

	if(splineLabel == SWL_SWH) { out[0] = SWL; out[1] = SWH; return out; }
	if(splineLabel == NWL_NWH) { out[0] = NWL; out[1] = NWH; return out; }
	if(splineLabel == SWH_SWL) { out[1] = SWL; out[0] = SWH; return out; }
	if(splineLabel == NWH_NWL) { out[1] = NWL; out[0] = NWH; return out; }

	return out;
}

label SplineBlock::getEdgeDirectionSign(label splineLabel){

	if(splineLabel < 0 || splineLabel >= 24) return 0;

	if(splineLabel == SWL_SEL) return 1;
	if(splineLabel == SWL_NWL) return 1;
	if(splineLabel == SWL_SWH) return 1;

	if(splineLabel == SWH_SEH) return 1;
	if(splineLabel == SWH_NWH) return 1;

	if(splineLabel == NWL_NEL) return 1;
	if(splineLabel == NWL_NWH) return 1;

	if(splineLabel == NWH_NEH) return 1;

	if(splineLabel == SEL_NEL) return 1;
	if(splineLabel == SEL_SEH) return 1;

	if(splineLabel == NEL_NEH) return 1;

	if(splineLabel == SEH_NEH) return 1;

	return -1;
}

label SplineBlock::getSignedEdgeDirection(label splineLabel){
	return getEdgeDirectionSign(splineLabel) * getDirectionEdge(splineLabel);
}

} /* iwesol */
} /* Foam */
