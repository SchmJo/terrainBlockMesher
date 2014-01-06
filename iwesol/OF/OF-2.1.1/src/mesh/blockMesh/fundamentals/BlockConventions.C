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

#include "BlockConventions.H"

namespace Foam
{

namespace oldev
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

const label BlockConventions::SWL = 0;
const label BlockConventions::SEL = 1;
const label BlockConventions::NEL = 2;
const label BlockConventions::NWL = 3;
const label BlockConventions::SWH = 4;
const label BlockConventions::SEH = 5;
const label BlockConventions::NEH = 6;
const label BlockConventions::NWH = 7;

const label BlockConventions::X = 0;
const label BlockConventions::Y = 1;
const label BlockConventions::Z = 2;

const label BlockConventions::NONE   = -1;
const label BlockConventions::WEST   = 0;
const label BlockConventions::EAST   = 1;
const label BlockConventions::SOUTH  = 2;
const label BlockConventions::NORTH  = 3;
const label BlockConventions::GROUND = 4;
const label BlockConventions::SKY    = 5;

const label BlockConventions::SWL_SEL = 0;
const label BlockConventions::NEL_NWL = 1;
const label BlockConventions::NEH_NWH = 2;
const label BlockConventions::SWH_SEH = 3;
const label BlockConventions::SWL_NWL = 4;
const label BlockConventions::SEL_NEL = 5;
const label BlockConventions::SEH_NEH = 6;
const label BlockConventions::SWH_NWH = 7;
const label BlockConventions::SWL_SWH = 8;
const label BlockConventions::SEL_SEH = 9;
const label BlockConventions::NEL_NEH = 10;
const label BlockConventions::NWL_NWH = 11;

const label BlockConventions::SEL_SWL = 12;
const label BlockConventions::NWL_NEL = 13;
const label BlockConventions::NWH_NEH = 14;
const label BlockConventions::SEH_SWH = 15;
const label BlockConventions::NWL_SWL = 16;
const label BlockConventions::NEL_SEL = 17;
const label BlockConventions::NEH_SEH = 18;
const label BlockConventions::NWH_SWH = 19;
const label BlockConventions::SWH_SWL = 20;
const label BlockConventions::SEH_SEL = 21;
const label BlockConventions::NEH_NEL = 22;
const label BlockConventions::NWH_NWL = 23;


// * * * * * * * * * * * * * Static Member Functions * * * * * * * * * * * * //

labelList BlockConventions::mapToHexModel(const labelList & vertices){

	labelList out(8);

	out[0] = vertices[0];
	out[1] = vertices[4];
	out[2] = vertices[5];
	out[3] = vertices[1];
	out[4] = vertices[3];
	out[5] = vertices[7];
	out[6] = vertices[6];
	out[7] = vertices[2];

	return out;
}

label BlockConventions::edgeStartFace(label edgeID){
	if(
			edgeID == SWL_SEL ||
			edgeID == NWL_NEL ||
			edgeID == SWH_SEH ||
			edgeID == NWH_NEH
			) return WEST;
	if(
			edgeID == SEL_SWL ||
			edgeID == NEL_NWL ||
			edgeID == SEH_SWH ||
			edgeID == NEH_NWH
			) return EAST;
	if(
			edgeID == SWL_NWL ||
			edgeID == SEL_NEL ||
			edgeID == SWH_NWH ||
			edgeID == SEH_NEH
			) return SOUTH;
	if(
			edgeID == NWL_SWL ||
			edgeID == NEL_SEL ||
			edgeID == NWH_SWH ||
			edgeID == NEH_SEH
			) return NORTH;
	if(
			edgeID == SWL_SWH ||
			edgeID == SEL_SEH ||
			edgeID == NWL_NWH ||
			edgeID == NEL_NEH
			) return GROUND;
	if(
			edgeID == SWH_SWL ||
			edgeID == SEH_SEL ||
			edgeID == NWH_NWL ||
			edgeID == NEH_NEL
			) return SKY;
	return NONE;
}

label BlockConventions::edgeEndFace(label edgeID){
	if(
			edgeID == SWL_SEL ||
			edgeID == NWL_NEL ||
			edgeID == SWH_SEH ||
			edgeID == NWH_NEH
			) return EAST;
	if(
			edgeID == SEL_SWL ||
			edgeID == NEL_NWL ||
			edgeID == SEH_SWH ||
			edgeID == NEH_NWH
			) return WEST;
	if(
			edgeID == SWL_NWL ||
			edgeID == SEL_NEL ||
			edgeID == SWH_NWH ||
			edgeID == SEH_NEH
			) return NORTH;
	if(
			edgeID == NWL_SWL ||
			edgeID == NEL_SEL ||
			edgeID == NWH_SWH ||
			edgeID == NEH_SEH
			) return SOUTH;
	if(
			edgeID == SWL_SWH ||
			edgeID == SEL_SEH ||
			edgeID == NWL_NWH ||
			edgeID == NEL_NEH
			) return SKY;
	if(
			edgeID == SWH_SWL ||
			edgeID == SEH_SEL ||
			edgeID == NWH_NWL ||
			edgeID == NEH_NEL
			) return GROUND;
	return NONE;
}

label BlockConventions::switchedOrientationLabel(label edgeID){
	labelList vl = getEdgeVerticesI(edgeID);
	return getEdgeLabel(vl[1],vl[0]);
}

labelList BlockConventions::getConstantDirectionsEdge(label edgeID){

	labelList out;
	for(label fI = 0; fI < 6; fI++){
		if(edgeBelongsToFace(edgeID,fI)){
			out.append(getConstantDirectionFace(fI));
		}
	}

	return out;
}

label BlockConventions::getDirectionEdge(label edgeID){

	labelList cdir = getConstantDirectionsEdge(edgeID);
	if(cdir.size() != 2) return -1;
	if(cdir[0] == X && cdir[1] == Y) return Z;
	if(cdir[1] == X && cdir[0] == Y) return Z;
	if(cdir[0] == X && cdir[1] == Z) return Y;
	if(cdir[1] == X && cdir[0] == Z) return Y;
	if(cdir[0] == Y && cdir[1] == Z) return X;
	if(cdir[1] == Y && cdir[0] == Z) return X;
	return -1;
}

bool BlockConventions::edgeBelongsToFace(label edgeID, label faceID){

	labelList fs = getFaceEdgesI(faceID);
	forAll(fs,sI){
		if(fs[sI] == edgeID) return true;
	}
	return false;
}

labelList BlockConventions::getFaceEdgesI(label faceI){

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

label BlockConventions::getEdgeLabel(label vertex1, label vertex2){

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

labelList BlockConventions::getEdgeVerticesI(label edgeID){

	labelList out(2);

	if(edgeID == SWL_NWL) { out[0] = SWL; out[1] = NWL; return out; }
	if(edgeID == SWL_SEL) { out[0] = SWL; out[1] = SEL; return out; }
	if(edgeID == NEL_NWL) { out[0] = NEL; out[1] = NWL; return out; }
	if(edgeID == NEL_SEL) { out[0] = NEL; out[1] = SEL; return out; }
	if(edgeID == NWL_SWL) { out[1] = SWL; out[0] = NWL; return out; }
	if(edgeID == SEL_SWL) { out[1] = SWL; out[0] = SEL; return out; }
	if(edgeID == NWL_NEL) { out[1] = NEL; out[0] = NWL; return out; }
	if(edgeID == SEL_NEL) { out[1] = NEL; out[0] = SEL; return out; }

	if(edgeID == SEH_NEH) { out[0] = SEH; out[1] = NEH; return out; }
	if(edgeID == SWH_NWH) { out[0] = SWH; out[1] = NWH; return out; }
	if(edgeID == NEH_SEH) { out[1] = SEH; out[0] = NEH; return out; }
	if(edgeID == NWH_SWH) { out[1] = SWH; out[0] = NWH; return out; }

	if(edgeID == SEL_SEH) { out[0] = SEL; out[1] = SEH; return out; }
	if(edgeID == NEL_NEH) { out[0] = NEL; out[1] = NEH; return out; }
	if(edgeID == SEH_SEL) { out[1] = SEL; out[0] = SEH; return out; }
	if(edgeID == NEH_NEL) { out[1] = NEL; out[0] = NEH; return out; }

	if(edgeID == SWH_SEH) { out[0] = SWH; out[1] = SEH; return out; }
	if(edgeID == NWH_NEH) { out[0] = NWH; out[1] = NEH; return out; }
	if(edgeID == SEH_SWH) { out[1] = SWH; out[0] = SEH; return out; }
	if(edgeID == NEH_NWH) { out[1] = NWH; out[0] = NEH; return out; }

	if(edgeID == SWL_SWH) { out[0] = SWL; out[1] = SWH; return out; }
	if(edgeID == NWL_NWH) { out[0] = NWL; out[1] = NWH; return out; }
	if(edgeID == SWH_SWL) { out[1] = SWL; out[0] = SWH; return out; }
	if(edgeID == NWH_NWL) { out[1] = NWL; out[0] = NWH; return out; }

	return out;
}

label BlockConventions::getEdgeDirectionSign(label edgeID){

	if(edgeID < 0 || edgeID >= 24) return 0;

	if(edgeID == SWL_SEL) return 1;
	if(edgeID == SWL_NWL) return 1;
	if(edgeID == SWL_SWH) return 1;

	if(edgeID == SWH_SEH) return 1;
	if(edgeID == SWH_NWH) return 1;

	if(edgeID == NWL_NEL) return 1;
	if(edgeID == NWL_NWH) return 1;

	if(edgeID == NWH_NEH) return 1;

	if(edgeID == SEL_NEL) return 1;
	if(edgeID == SEL_SEH) return 1;

	if(edgeID == NEL_NEH) return 1;

	if(edgeID == SEH_NEH) return 1;

	return -1;
}

label BlockConventions::getSignedEdgeDirection(label edgeID){
	return getEdgeDirectionSign(edgeID) * getDirectionEdge(edgeID);
}

labelList BlockConventions::vertices2vertexList(
			const label & p_SWL,const label & p_SWH,
			const label & p_NWL,const label & p_NWH,
			const label & p_SEL,const label & p_SEH,
			const label & p_NEL,const label & p_NEH
			){
	labelList out(8);
	out[SWL] = p_SWL;
	out[SEL] = p_SEL;
	out[NEL] = p_NEL;
	out[NWL] = p_NWL;
	out[SWH] = p_SWH;
	out[SEH] = p_SEH;
	out[NEH] = p_NEH;
	out[NWH] = p_NWH;
	return out;
}

label BlockConventions::getConstantDirectionFace(label faceID){
	if(faceID == WEST || faceID == EAST) return X;
	if(faceID == SOUTH || faceID == NORTH) return Y;
	if(faceID == GROUND || faceID == SKY) return Z;
	return -1;
}

label BlockConventions::getOppositeFace(label faceID){
	if(faceID == WEST) return EAST;
	if(faceID == EAST) return WEST;
	if(faceID == NORTH) return SOUTH;
	if(faceID == SOUTH) return NORTH;
	if(faceID == SKY) return GROUND;
	if(faceID == GROUND) return SKY;
	return NONE;
}

scalarList BlockConventions::gradingFactors2ExpandRatios(
			const scalarList & gradingFactors
			){
	scalarList out(12);

    // x-direction
    out[0]  = gradingFactors[0];
    out[1]  = gradingFactors[0];
    out[2]  = gradingFactors[0];
    out[3]  = gradingFactors[0];

    // y-direction
    out[4]  = gradingFactors[1];
    out[5]  = gradingFactors[1];
    out[6]  = gradingFactors[1];
    out[7]  = gradingFactors[1];

    // z-direction
    out[8]  = gradingFactors[2];
    out[9]  = gradingFactors[2];
    out[10] = gradingFactors[2];
    out[11] = gradingFactors[2];

	return out;
}


// ************************************************************************* //

} // End namespace oldev

} // End namespace Foam
