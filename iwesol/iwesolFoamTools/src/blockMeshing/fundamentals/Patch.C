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

#include "Patch.H"

namespace Foam{
namespace iwesol{

Patch::Patch(const word & name, const word & type):
	name(name),
	type(type){
}

Patch::~Patch() {
}

void Patch::addPatch(
		BasicBlock const * block,
		label blockFaceLabel,
		const word & blockKey){

	PatchFace jpf;
	jpf.block          = block;
	jpf.blockFaceLabel = blockFaceLabel;
	jpf.blockKey       = blockKey;
	pfaces.append(jpf);
	faceIndices.set(blockKey,pfaces.size() - 1);

}

std::string Patch::dictEntry() const{

	std::string out = "";

	out += "    " + name;
	out += "\n    {";
	out += "\n        type " + type + ";";
	if(isCyclic()){
		out += "\n        neighbourPatch " + cyclicPartner + ";";
	}
	out += "\n        faces";
	out += "\n        (";

	for(label i = 0; i < size(); i++){
		const PatchFace & jpf = pfaces[i];
		out += "\n            ";
		out += jpf.block->dictEntry_face(jpf.blockFaceLabel);
	}

	out += "\n        );";
	out += "\n    }";

	return out;

}

} /* iwesol */
} /* Foam */
