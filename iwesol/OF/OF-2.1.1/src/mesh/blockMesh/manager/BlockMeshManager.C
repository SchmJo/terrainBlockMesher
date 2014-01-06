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

#include "BlockMeshManager.H"

namespace Foam
{

namespace oldev
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

BlockMeshManager::BlockMeshManager
(
		const Time & runTime
) :
		BlockMeshCreator(runTime){
}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

void BlockMeshManager::setInterBlockSpline(
			const pointField & splinep,
			label iStartBlock,
			label faceToNextBlock,
			label splineI,
			label nBlockMax
		){

	// get block line:
	labelList blockLine = neighbourBlockChain(iStartBlock,faceToNextBlock,nBlockMax);

	// prepare:
	label pImax           = splinep.size() + 1;
	label lmax            = blockLine.size() - 1;
	scalar temp           = (splinep.size() + 1.) / (blockLine.size() - 1.);
	label subSplineLength = temp;
	label pI              = 0;

	// check:
	if(temp != subSplineLength){
		Info << "\n   BlockMeshManager: Error: setInterBlockSpline found " << lmax + 1 << " aligned blocks, but "
				<< splinep.size() << " total spline points. SubSplineLength = " << temp << endl;
		throw;
	}

	for(label l = 0; l <= lmax; l++){

		// prepare:
		pointField supp(subSplineLength);

		// extract sub-spline:
		for(label spI = 0; spI < subSplineLength; spI++){

			// next point:
			pI++;

			// set spline points:
			supp[spI] = splinep[pI - 1];

		}

		// count vertex:
		pI++;

		// set new spline:
		if(pI < pImax) supp.last() = splinep[pI - 1];
		setEdge(blockLine[l],splineI,supp);
	}
}


void BlockMeshManager::setInterBlockSpline(
			const Foam::vector & deltaA,
			const Foam::vector & deltaB,
			label iStartBlock,
			label faceToNextBlock,
			label iSpline,
			label nBlockMax,
			const scalarList & sList
		){

	// get block line:
	labelList blockLine = neighbourBlockChain(iStartBlock,faceToNextBlock,nBlockMax);

	// prepare:
	const labelList vI = Block::getEdgeVerticesI(iSpline);
	const point & pA   = points()[blocks()[blockLine.first()].vertexLabels()[vI[0]]];
	const point & pB   = points()[blocks()[blockLine.last()].vertexLabels()[vI[1]]];

	// find s values:
	const scalarList sval = getBlockLineSList(
			iStartBlock,
			iSpline,
			faceToNextBlock,
			nBlockMax,
			true,
			true
			);

	// find new path:
	PointLinePath newPath;
	newPath.addPoint(pA);
	newPath.addPoint(pA + deltaA);
	newPath.addPoint(pB + deltaB);
	newPath.addPoint(pB);

	// set new splines:
	label counter = 0;
	for(label l = 0; l < blockLine.size(); ++l){

		// prepare:
		ensureEdge(blockLine[l],iSpline);
		label edgeI    = edges().blockEdgeIndex(blockLine[l],iSpline);
		pointField pts = edges().edgePoints(edgeI,true);

		// move spline points:
		forAll(pts,pI){

			// move point:
			pts[pI] = newPath[sval[counter]];

			// count:
			counter++;
		}

		// set spline:
		setEdge(blockLine[l],iSpline,pts);
	}
}

scalarList BlockMeshManager::getBlockLineSList(
		label iBlock,
		label iSpline,
		label face,
		label nBlockMax,
		bool withFirst,
		bool withLast
		) const {

	// set up path:
	const PointLinePath path = getInterBlockPath(iBlock,iSpline,face,nBlockMax,withFirst,withLast);

	// calc s:
	scalarList out(path.size());
	forAll(out,sI){
		out[sI] = path.getPointS(sI);
	}

	return out;
}

PointLinePath BlockMeshManager::getInterBlockPath(
		label iBlock,
		label iSpline,
		label face,
		label nBlockMax,
		bool withFirst,
		bool withLast
		) const{

	// get block line:
	labelList blockLine = neighbourBlockChain(iBlock,face,nBlockMax);

	// prepare:
	const labelList vI  = Block::getEdgeVerticesI(iSpline);
	const label jSpline = Block::switchedOrientationLabel(iSpline);

	// set up path:
	PointLinePath path("interblockPathID" + Foam::name(iSpline) + "F" + Foam::name(face) + "B" + Foam::name(iBlock) + "M" + Foam::name(nBlockMax),4);

	// first point:
	if(withFirst) {
		path.addPoint
		(
				points()[blocks()[blockLine.first()].vertexLabels()[vI[0]]]
		);
	}

	for(label i = 0; i < blockLine.size(); ++i){

		// grab block:
		label blockI        = blockLine[i];
		const Block & block = blocks()[blockI];

		// case spline exists:
		if(edges().foundInBlock(blockI,iSpline)){

			// get spline:
			label edgeI    = edges().blockEdgeIndex(blockI,iSpline);
			pointField pts = edges().edgePoints(edgeI,true);

			// add spline points:
			forAll(pts,pI){
				path.addPoint(pts[pI]);
			}

			// add last point:
			path.addPoint(points()[blockVertex(blockI,vI[1])]);
		}

		// case opposite spline exists:
		else if(edges().foundInBlock(blockI,jSpline)){

			// get spline:
			label edgeI    = edges().blockEdgeIndex(blockI,jSpline);
			pointField pts = edges().edgePoints(edgeI,true);

			// add spline points:
			forAll(pts,pI){
				path.addPoint(pts[pts.size() - pI - 1]);
			}

			// add last point:
			path.addPoint(points()[blockVertex(blockI,vI[1])]);
		}

		// else no spline exists:
		else {

			// find points:
			pointField pts(
					getLinearList(
							points()[block.vertexLabels()[vI[0]]],
							points()[block.vertexLabels()[vI[1]]],
							block.blockCells()[Block::getDirectionEdge(iSpline)] - 1,
							false,
							true
							)
					);

			// add to path:
			forAll(pts,pI){
				path.addPoint(pts[pI]);
			}
		}
	}

	if(!withLast){
		path.erase(path.size() - 1);
	}

	return path;
}


// ************************************************************************* //

} // End namespace oldev

} // End namespace Foam
