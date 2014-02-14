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

Class


Description


SourceFiles


Author
    Jonas Schmidt (jonas.schmidt@iwes.fraunhofer.de)

Modified by


Test location


Tested by


Used by


\*---------------------------------------------------------------------------*/

namespace Foam {

namespace oldev {


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //

inline void BlockMeshCreator::resizePoints(label s) {
	points_.resize(s);
	pointBlockMem_.resize(s);
}

inline void BlockMeshCreator::resizeBlocks(label s) {
	blocks_.resize(s);
	blockBlocks_.resize(s,labelList(6,-1));
}

inline void BlockMeshCreator::resizeEdges(label s) {
	edges_.resize(s);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline label BlockMeshCreator::blockVertex(label blockI, label vertexID) const{
	return blocks_[blockI].vertexLabels()[vertexID];
}

inline const List< labelHashSet > &  BlockMeshCreator::pointBlocks() const{
	return pointBlockMem_;
}

inline const List< labelHashSet > &  BlockMeshCreator::edgeBlocks() const{
	return edgeBlockMem_;
}

inline void BlockMeshCreator::clearGeom(){
	forAll(blocks_,bI){
		blocks_[bI].clearGeom();
	}
}

inline label BlockMeshCreator::addPoint(const point & p) {
	if(points_.size() == pointCounter_){
		resizePoints(pointCounter_ + 1);
	}
	points_[pointCounter_] = p;
	pointCounter_++;
	return pointCounter_ - 1;
}

inline void BlockMeshCreator::addPoints(const pointField & pts) {
	if(points_.size() < pointCounter_ + pts.size()){
		resizePoints(pointCounter_ + pts.size());
	}
	forAll(pts,pI){
		points_[pointCounter_] = pts[pI];
		pointCounter_++;
	}
}

inline void BlockMeshCreator::addPatch(
		const word & name,
		const word & type
		){
	patches_.resize(patches_.size() + 1);
	patches_.set
	(
			patches_.size() - 1,
			new BlockMeshPatch
			(
					*this,
					name,
					type
			)
	);
	patchIndices_.set(name,patches_.size() - 1);
}

inline void BlockMeshCreator::contributeToPatch(
			label patchI,
			label blockI,
			label faceID
			){
	patches_[patchI].addFace(blockI,faceID);
}

inline void BlockMeshCreator::resetEdge
(
   			label edgeI,
   			const pointField & internalPoints,
   			const word & type
){
	const curvedEdge & e = edges()[edgeI];
	edges_.setEdge(type,e.start(),e.end(),internalPoints);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// ************************************************************************* //

} // End namespace oldev

} // End namespace Foam
