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

#include "BlockMeshPatch.H"
#include "BlockMeshCreator.H"

namespace Foam
{

namespace oldev
{


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

BlockMeshPatch::BlockMeshPatch
(
		const BlockMeshCreator & bmc,
		const word & name,
		const word & type
):
		bmc_(bmc),
		name_(name),
		type_(type){
}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

void BlockMeshPatch::addFace(label blockI, label f){

	if(!blockFaceMem_.found(blockI)){
		blockFaceMem_.set(blockI,labelList(1,f));
	} else {
		blockFaceMem_[blockI].append(f);
	}
}

Ostream & operator<<(Ostream & os, const BlockMeshPatch & b){

	os 		<< b.name_ << nl
			<< "{" << nl
			<< tab << "type" << tab << b.type_ << ";"
			<< tab << "faces" << nl
			<< tab << "(";

	labelList blocks = b.blockFaceMem_.sortedToc();
	forAll(blocks,bI){
		label blockI          = blocks[bI];
		const labelList faces =  b.blockFaceMem_[blockI];
		forAll(faces,fI){
			label faceID        = faces[fI];
			const Block & block = b.bmc_.blocks()[blockI];
			const face & f      = block.faces()[faceID];
			os << nl << tab << tab << f;
		}
	}
	os		<< tab << ");" << nl
			<< "}"
			<< endl;

	return os;
}


// ************************************************************************* //

} // End namespace oldev

} // End namespace Foam
