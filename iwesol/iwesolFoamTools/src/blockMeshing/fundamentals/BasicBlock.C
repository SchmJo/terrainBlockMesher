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

#include "BasicBlock.H"
#include "HashSet.H"

namespace Foam{
namespace iwesol{

BasicBlock::BasicBlock(){
}

BasicBlock::BasicBlock(
		pointField* globalPoints,
		const labelList & verticesI,
		const labelList & cells,
		const std::string & gradingCommand,
		const scalarList & gradingFactors
		):
	globalPoints(globalPoints),
	verticesI(verticesI),
	cells(cells),
	gradingCommand(gradingCommand),
	gradingFactors(gradingFactors),
	neighbors(6){
	__init();
}

BasicBlock::BasicBlock(
		pointField* globalPoints,
		const label & p_SWL,const label & p_SWH,
		const label & p_NWL,const label & p_NWH,
		const label & p_SEL,const label & p_SEH,
		const label & p_NEL,const label & p_NEH,
		const label cells_x,
		const label cells_y,
		const label cells_z,
		const std::string & gradingCommand,
		const scalarList & gradingFactors
		):
	globalPoints(globalPoints),
	verticesI(8),
	cells(3),
	gradingCommand(gradingCommand),
	gradingFactors(gradingFactors),
	neighbors(6){
	verticesI[SWL] = p_SWL;
	verticesI[SWH] = p_SWH;
	verticesI[NWL] = p_NWL;
	verticesI[NWH] = p_NWH;
	verticesI[SEL] = p_SEL;
	verticesI[SEH] = p_SEH;
	verticesI[NEL] = p_NEL;
	verticesI[NEH] = p_NEH;
	cells[0] = cells_x;
	cells[1] = cells_y;
	cells[2] = cells_z;
	__init();
}

BasicBlock::~BasicBlock() {
}

void BasicBlock::__init(){

	// find min and max coordinates
	for(label i = 0; i < 8; i++){
		point & p = getVertex(i);
		if(i == 0){
			lowCoo = p;
			highCoo = p;
			continue;
		}
		for(label u = 0; u < 3; u++){
			if(p[u] < lowCoo[u])  lowCoo[u]  = p[u];
			if(p[u] > highCoo[u]) highCoo[u] = p[u];
		}
	}

	// zero neighbors:
	forAll(neighbors,nI) neighbors[nI] = 0;

}

void BasicBlock::print() const{
	for(label i = 0; i < 8; i++){
		Info << i <<  " " << getVertexI(i) << " " << getVertex(i)[0] << " "<< getVertex(i)[1] << " "<< getVertex(i)[2] << endl;
	}
}

blib::Outputable::OData BasicBlock::getOData(
			const std::string & type) const{

	blib::Outputable::OData out(1);
	blib::IO::OFILE::Data   pixeldata;

	// set basic info:
	pixeldata.name   = "BasicBlock";
	pixeldata.type   = type;
	pixeldata.origin = "BasicBlock";

	// open foam output:
	if(type.compare(blib::IO::OFILE::TYPE::OPEN_FOAM) == 0){

		// set header:
		pixeldata.header = blib::IO::OFILE::PREAMBLE::OPEN_FOAM;

		// set pre- and post-data:
		pixeldata.pre_data  = "";
		pixeldata.post_data = "";

		pixeldata.data = "vertices\n(\n";
		forAll(*globalPoints, pI){
			point p = (*globalPoints)[pI];
			pixeldata.data += "    ("
					+ blib::String(p[X]) + " "
					+ blib::String(p[Y]) + " "
					+ blib::String(p[Z]) + ") // " + blib::String(pI) + "\n";
		}
		pixeldata.data += "\n);\n";

		pixeldata.data += "\nblocks\n(\n    ";
		pixeldata.data += dictEntry_hex();
		pixeldata.data += "\n);\n";

		pixeldata.data += "\nedges\n(\n    ";
		pixeldata.data += dictEntry_splines();
		pixeldata.data += "\n);\n";

		pixeldata.data += "\n";
		pixeldata.data += dictEntry_boundary();
		pixeldata.data += "\n";

		pixeldata.data += "\n";
		pixeldata.data += dictEntry_mergePatchPairs();
		pixeldata.data += "\n";

	}

	out[0] = pixeldata;
	return out;

}

std::string BasicBlock::dictEntry_hex() const{

	// simple check:
	if(!ok()) return word("");

	std::string out("hex (");
	forAll(verticesI,vI){
		out += blib::String(verticesI[vI]);
		if(vI < 7) out += " " ;
	}
	out +=") (";
	forAll(cells,cI){
		out += blib::String(cells[cI]);
		if(cI < 2) out += " " ;
	}
	out += ") " + getGradingCommand();

	return out;
}

std::string BasicBlock::dictEntry_boundary() const{

	std::string out("boundary\n(");

	out += "\n    NORTH";
	out += "\n    {";
	out += "\n        type patch;";
	out += "\n        faces";
	out += "\n        (\n            ";
	out += dictEntry_face(NORTH);
	out += "\n        );";
	out += "\n    }";

	out += "\n    SOUTH";
	out += "\n    {";
	out += "\n        type patch;";
	out += "\n        faces";
	out += "\n        (\n            ";
	out += dictEntry_face(SOUTH);
	out += "\n        );";
	out += "\n    }";

	out += "\n    WEST";
	out += "\n    {";
	out += "\n        type patch;";
	out += "\n        faces";
	out += "\n        (\n            ";
	out += dictEntry_face(WEST);
	out += "\n        );";
	out += "\n    }";

	out += "\n    EAST";
	out += "\n    {";
	out += "\n        type patch;";
	out += "\n        faces";
	out += "\n        (\n            ";
	out += dictEntry_face(EAST);
	out += "\n        );";
	out += "\n    }";

	out += "\n    GROUND";
	out += "\n    {";
	out += "\n        type patch;";
	out += "\n        faces";
	out += "\n        (\n            ";
	out += dictEntry_face(GROUND);
	out += "\n        );";
	out += "\n    }";

	out += "\n    SKY";
	out += "\n    {";
	out += "\n        type patch;";
	out += "\n        faces";
	out += "\n        (\n            ";
	out += dictEntry_face(SKY);
	out += "\n        );";
	out += "\n    }";


	out += "\n);";

	return out;
}

std::string BasicBlock::dictEntry_face(label i) const{

	std::string out = "( ";
	labelList ind   = getFaceI(i);
	for(int i = 0; i < 4; i++){
		out += blib::String(ind[i]) + " ";
	}
	out += ")";

	return out;
}

std::string BasicBlock::dictEntry_mergePatchPairs() const{

	return std::string("mergePatchPairs\n(\n);");
}

pointField BasicBlock::getVertexPoints() const{

	pointField out(8);

	for(label v = 0; v < 8; v++){
		out[v] = getVertex(v);
	}

	return out;
}

std::string BasicBlock::getGradingCommand() const {

	std::string out = gradingCommand + " (";
		for(label i = 0; i < 3; i++){
			out += word(blib::String(gradingFactors[i]));
			if(i < 2) out += " ";
		}
	out += ")";
	return out;
}

label BasicBlock::checkSetNeighbor(BasicBlock & block){

	if(&block == this) return NONE;

	label checkFace = 0;
	for(checkFace = 0; checkFace < 6; checkFace++){

		// grab face point indices:
		HashSet<label> fI(getFaceI(checkFace));

		for(label checkFace2 = 0; checkFace2 < 6; checkFace2++){

			// grab face point indices:
			HashSet<label> fI2(block.getFaceI(checkFace2));

			// compare:
			if(fI.sortedToc() == fI2.sortedToc()) {
				if(neighbors[checkFace] != 0 && neighbors[checkFace] != &block){
					Info << "\n   BasicBlock: Error: Found two neighbors for face " << checkFace << endl;
					throw;
				}
				neighbors[checkFace] = &block;
				block.neighbors[checkFace2] = this;
				return checkFace;
			}

		}
	}

	return NONE;
}

label BasicBlock::commonFace(const BasicBlock & block) const{

	if(&block == this) return NONE;

	for(label checkFace = 0; checkFace < 6; checkFace++){
		if(neighbors[checkFace] != 0){
			BasicBlock const * nei = neighbors[checkFace];
			if(nei == &block) return checkFace;
		}
	}

	return NONE;
}

} /* iwesol */
} /* Foam */




