/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "BlockMeshCreator.H"
#include "emptyPolyPatch.H"
#include "PointLinePath.H"

namespace Foam
{

namespace oldev
{

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void BlockMeshCreator::addBlockToMem(
			label blockI,
			const labelList & points
			){

	// add to point-block memory:
	forAll(points,pI){
		pointBlockMem_[points[pI]].set(blockI);
	}

	// re-check open edges_:
	labelList oedges = openEdgesMem_.toc();
	if(oedges.size() > 0){
		labelHashSet bpts(points);
		forAll(oedges,oeI){
			const curvedEdge & e = edges_[oedges[oeI]];
			if(bpts.found(e.start() && bpts.found(e.end()))){
				edgeBlockMem_[oedges[oeI]].set(blockI);
				if(edgeBlockMem_[oedges[oeI]].size() == 4){
					openEdgesMem_.erase(oedges[oeI]);
				}
				if(edgeSteps_[oedges[oeI]] < 0){
					calcEdgeSteps(oedges[oeI]);
				}
			}
		}
	}
}

void BlockMeshCreator::addEdgeToMem(
		label edgeI,
		label start,
		label end
		){

	edgeBlockMem_.resize(edges_.size());
	labelList startBlocks          = pointBlockMem_[start].toc();
	const labelHashSet & endBlocks = pointBlockMem_[end];
	forAll(startBlocks,bI){
		if(endBlocks.found(startBlocks[bI])){
			edgeBlockMem_[edgeI].set(startBlocks[bI]);
			blocks_[startBlocks[bI]].clearGeom();
		}
	}
	if(edgeBlockMem_[edgeI].size() < 4){
		openEdgesMem_.set(edgeI);
	}
}

void BlockMeshCreator::calcEdgeSteps(
		label edgeI
		){

	// prepare:
	edgeSteps_.resize(edges_.size(),-1);

	// check:
	if(edgeBlockMem_[edgeI].size() == 0) return;
	label blockI = edgeBlockMem_[edgeI].toc()[0];

	// determine model edge order:
	labelList cellModelEdges = blocks_[blockI].blockShape().meshEdges
	(
		edges_.getEdgeList(),
		labelList(1,edgeI)
	);
	label cellModelEdgeI = -1;
	forAll(cellModelEdges,meI){
		if(cellModelEdges[meI] >= 0){
			cellModelEdgeI = meI;
			break;
		}
	}

	// x:
	if( cellModelEdgeI < 4 ) edgeSteps_[edgeI] = blocks_[blockI].blockCells()[0];

	// y:
	else if( cellModelEdgeI < 8 ) edgeSteps_[edgeI] = blocks_[blockI].blockCells()[1];

	// z:
	else if( cellModelEdgeI < 12 ) edgeSteps_[edgeI] = blocks_[blockI].blockCells()[2];
}

void BlockMeshCreator::findNeighbours(label blockI){

	// grab block:
	const Block & block = blocks_[blockI];

	// loop over faces:
	forAll(block.faces(),fI){

		// check of neighbour known:
		if(blockBlocks_[blockI][fI] >= 0) continue;

		// grab face:
		const face & f = block.faces()[fI];

		// order points_:
		labelList opoints = f;
		Foam::sort(opoints);

		// find opposite face:
		label ofI = Block::getOppositeFace(fI);

		// grab blocks_ connected to first point of face:
		labelList cblocks = pointBlockMem_[f[0]].toc();

		// loob over block candidates:
		forAll(cblocks,cbI){

			// block label:
			label cblockI = cblocks[cbI];

			// consider only different blocks_:
			if(cblockI == blockI) continue;

			// consider only blocks_ without neighbor:
			if(blockBlocks_[cblockI][ofI] >= 0) continue;

			// grab block:
			const Block & cblock = blocks_[cblockI];

			// grab opposite face:
			const face & cf = cblock.faces()[ofI];

			// order points_:
			labelList copoints = cf;
			Foam::sort(copoints);

			// if same this is the neighbour:
			if( opoints == copoints ){
				blockBlocks_[blockI][fI]   = cblockI;
				blockBlocks_[cblockI][ofI] = blockI;
				break;
			}
		}
	}
}

bool BlockMeshCreator::checkBlock(label blockI){

	// grab block:
	const Block & block = blocks_[blockI];

	// loop over neighbors:
	forAll(blockBlocks_[blockI],fI){
		if(blockBlocks_[blockI][fI] >= 0){

			// get constant direction:
			label cdir = Block::getConstantDirectionFace(fI);

			// neighbor label:
			label nblockI = blockBlocks_[blockI][fI];

			// grab block:
			const Block & nblock = blocks_[nblockI];

			// loop over directions:
			for(label i = 0; i < 3; i++){

				// skip constant direction:
				if(i != cdir){

					// compare cells:
					if( block.blockCells()[i] != nblock.blockCells()[i] ){
						Info << "\nBlockMeshCreator: Added block is inconsistent with neighbours." << endl;
						Info << "blockI  = " << blockI << ": cells = " << block.blockCells() << endl;
						Info << "nblockI = " << nblockI << ": cells = " << nblock.blockCells() << endl;
						Info << "faceID to neighbour   : " << fI << endl;
						Info << "inconsistent direction: " << i << endl;
						Info << nl;

						return false;
					}
				}
			}
		}
	}

	return true;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

BlockMeshCreator::BlockMeshCreator(
		const Time & runTime
		):
		runTime_(runTime),
		edges_(*this),
		pointCounter_(0),
		blockCounter_(0)
{
}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

label BlockMeshCreator::addBlock(
		const labelList & points,
		const Vector< label > & blockCells,
		const scalarList & gradingFactors,
		const word& zoneName
		){

	// resize:
	if(blockCounter_ == blocks_.size()){
		resizeBlocks(blocks_.size() + 1);
	}

	// set new block:
	blocks_.set
	(
			blockCounter_,
			new Block
			(
					points_,
					points,
					edges_,
					blockCells,
					gradingFactors,
		            zoneName
			)
	);

	// add to memory:
	addBlockToMem(blockCounter_,points);

	// find neighbor blocks:
	findNeighbours(blockCounter_);

	// check consistency with neighbors:
	if(!checkBlock(blockCounter_)) throw;

	// count:
	blockCounter_++;

	return blockCounter_ - 1;
}

label BlockMeshCreator::addBlock(
			const label & p_SWL,const label & p_SWH,
			const label & p_NWL,const label & p_NWH,
			const label & p_SEL,const label & p_SEH,
			const label & p_NEL,const label & p_NEH,
			const label cells_x,
			const label cells_y,
			const label cells_z,
			const scalarList & gradingFactors,
			const word& zoneName
			){
	labelList pts(8);
	pts[Block::SWL] = p_SWL;
	pts[Block::SEL] = p_SEL;
	pts[Block::NEL] = p_NEL;
	pts[Block::NWL] = p_NWL;
	pts[Block::SWH] = p_SWH;
	pts[Block::SEH] = p_SEH;
	pts[Block::NEH] = p_NEH;
	pts[Block::NWH] = p_NWH;
	Vector< label > cls(cells_x,cells_y,cells_z);
	return addBlock
	(
			pts,
			cls,
			gradingFactors,
			zoneName
	);
}

label BlockMeshCreator::addBlock(
			label startBlockI,
			label faceID,
			const face & newFace,
			label faceOrthogonalCells,
			const scalarList & gradingFactors,
			const word& zoneName
			){

	// grab block:
	const Block & startBlock = blocks_[startBlockI];

	// resize:
	if(blockCounter_ == blocks_.size()){
		resizeBlocks(blocks_.size() + 1);
	}

	// find vertices:
	labelList pts(8);
	{
		// new face:
		const face & mNewFace = startBlock.blockShape().model().modelFaces()[faceID];
		forAll(mNewFace,nfI){
			pts[mNewFace[nfI]] = newFace[nfI];
		}

		// opposite face:
		label ofI = Block::getOppositeFace(faceID);

		// old face:
		const face & mOldFace = startBlock.blockShape().model().modelFaces()[ofI];
		face oldFace          = startBlock.faces()[faceID];
		oldFace.flip();
		forAll(mOldFace,ofI){
			pts[mOldFace[ofI]] = oldFace[ofI];
		}
	}

	// find block cells:
	Vector< label > blockCells = startBlock.blockCells();
	label cdir                 = Block::getConstantDirectionFace(faceID);
	blockCells[cdir]           = faceOrthogonalCells;

	// set new block:
	blocks_.set
	(
			blockCounter_,
			new Block
			(
					points_,
					pts,
					edges_,
					blockCells,
					gradingFactors,
		            zoneName
			)
	);

	// add to memory:
	addBlockToMem(blockCounter_,pts);

	// find neighbor blocks:
	findNeighbours(blockCounter_);

	// check consistency with neighbors:
	if(!checkBlock(blockCounter_)) throw;

	// count:
	blockCounter_++;

	return blockCounter_ - 1;
}

label BlockMeshCreator::setEdge(
		label blockI,
		const word & type,
		label i,
		const pointField & internalPoints
		){

	// get model vertices:
	labelList v(Block::getEdgeVerticesI(i));

	// check if flip needed:
	if(i > 11){
		pointField nip(internalPoints.size());
		forAll(nip,npI){
			nip[npI] = internalPoints[internalPoints.size() - 1 - npI];
		}
		return setEdge
		(
				blockI,
				type,
				Block::getEdgeLabel(v[1],v[0]),
				nip
		);
	}

	// add edge:
	label edgeI = edges_.setEdge
	(
			type,
			blocks_[blockI].vertexLabels()[v[0]],
			blocks_[blockI].vertexLabels()[v[1]],
			internalPoints
	);

	// add to memory:
	addEdgeToMem
	(
			edgeI,
			blocks_[blockI].vertexLabels()[v[0]],
			blocks_[blockI].vertexLabels()[v[1]]
	);

	// calculate steps:
	calcEdgeSteps(edgeI);

	return edgeI;
}

label BlockMeshCreator::setEdge(
		label blockI,
		label i,
		const pointField & internalPoints
		){

	// get model vertices:
	labelList v(Block::getEdgeVerticesI(i));

	// check if flip needed:
	if(i > 11){
		pointField nip(internalPoints.size());
		forAll(nip,npI){
			nip[npI] = internalPoints[internalPoints.size() - 1 - npI];
		}
		return setEdge
		(
				blockI,
				Block::getEdgeLabel(v[1],v[0]),
				nip
		);
	}

	// add edge:
	label edgeI = edges_.setEdge
	(
			blocks_[blockI].vertexLabels()[v[0]],
			blocks_[blockI].vertexLabels()[v[1]],
			internalPoints
	);

	// add to memory:
	addEdgeToMem
	(
			edgeI,
			blocks_[blockI].vertexLabels()[v[0]],
			blocks_[blockI].vertexLabels()[v[1]]
	);

	// calculate steps:
	calcEdgeSteps(edgeI);

	return edgeI;
}

bool BlockMeshCreator::setDummyEdge(
		label blockI,
		label edgeID,
		bool overwrite
		){

	const Block & block = blocks()[blockI];
	labelList v(Block::getEdgeVerticesI(edgeID));
	if(
			edges().found(block.vertexLabels()[v[0]],block.vertexLabels()[v[1]]) ||
			edges().found(block.vertexLabels()[v[1]],block.vertexLabels()[v[0]])
	) return false;
	const point & pA = points()[block.vertexLabels()[v[0]]];
	const point & pB = points()[block.vertexLabels()[v[1]]];
	pointField pts(block.blockCells()[Block::getDirectionEdge(edgeID)] - 1);
	scalar deltas = 1. / scalar( pts.size() + 1 );
	forAll(pts,pI){
		pts[pI] = pA + scalar( pI + 1 ) * deltas * ( pB - pA );
	}

	setEdge(blockI,edgeID,pts);

	return true;
}

void BlockMeshCreator::setAllDummyEdges(bool overwrite){
	forAll(blocks(),bI){
		for(label eI = 0; eI < 12; eI++){
			setDummyEdge(bI,eI,overwrite);
		}
	}
}

labelList BlockMeshCreator::neighbourBlockChain(
			label startBlockI,
			label toNeighbourFaceID,
			label numberOfBlocks
			) const{

	// prepare:
	labelList out;
	if(numberOfBlocks > 0) {
		out.resize(numberOfBlocks);
	} else {
		out.resize(blocks_.size());
	}
	label counter = 0;
	label blockI  = startBlockI;

	// set first block:
	out[0] = startBlockI;
	counter++;

	// loop:
	while( counter < out.size() && blockBlocks_[blockI][toNeighbourFaceID] >= 0){
		out[counter] = blockBlocks_[blockI][toNeighbourFaceID];
		blockI       = out[counter];
		counter++;
	}

	// resize:
	out.resize(counter);

	return out;
}

IOdictionary BlockMeshCreator::createBlockMeshDict(
			bool writeDict,
			const word & regionName
			) const{

    Info<< nl << "BlockMeshCreator: creating blockMeshDict" << endl;

    // init:
	IOdictionary out
	(
			IOobject
			(
					"blockMeshDict",
		            runTime_.constant(),
		            "polyMesh",
		            runTime_,
		            Foam::IOobject::NO_READ,
		            Foam::IOobject::AUTO_WRITE
			)
	);

	// fill dict:
	{
		out.add("vertices",points_);
		out.add("blocks",blocks_);
		out.add("edges",edges_);
		out.add("boundary",patches_);
	}

	// write:
	if(writeDict) {
		Info << "BlockMeshCreator: writing blockMeshDict" << endl;
		out.regIOobject::write();
	}

	return out;
}

autoPtr< blockMesh > BlockMeshCreator::createBlockMesh(
		bool writeDict,
		const word & regionName
		) const{

	// init dict:
	IOdictionary bdict = createBlockMeshDict(writeDict,regionName);

    Info<< nl << "BlockMeshCreator: creating blockMesh" << endl;

	return autoPtr< blockMesh >
	(
			new blockMesh
			(
					bdict,
					regionName
			)
	);
}

autoPtr< polyMesh > BlockMeshCreator::createPolyMesh(
		bool writeDict,
		const word & regionName
		) const{

	autoPtr< blockMesh > blocks_ = createBlockMesh(writeDict,regionName);

    Info<< nl << "BlockMeshCreator: creating polyMesh from blockMesh" << endl;

    word defaultFacesName = "defaultFaces";
    word defaultFacesType = emptyPolyPatch::typeName;

    return autoPtr< polyMesh >
    (
    		new polyMesh
    	    (
    	        IOobject
    	        (
    	            regionName,
    	            runTime_.constant(),
    	            runTime_
    	        ),
    	        xferCopy(blocks_().points()),           // could we re-use space?
    	        blocks_().cells(),
    	        blocks_().patches(),
    	        blocks_().patchNames(),
    	        blocks_().patchDicts(),
    	        defaultFacesName,
    	        defaultFacesType
    	    )
    );
}

// ************************************************************************* //

} // End namespace oldev

} // End namespace Foam
