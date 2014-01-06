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

#include "BlockEdgeList.H"
#include "BlockMeshCreator.H"
#include "token.H"
#include "HashSet.H"
#include "word.H"
#include "PointLinePathEdge.H"
#include "lineEdge.H"

namespace Foam
{

namespace oldev
{


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

BlockEdgeList::BlockEdgeList
(
		const BlockMeshCreator & bmc
):
		bmc_(bmc),
		edgeCounter_(0){
}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

label BlockEdgeList::setEdge(
		const word & type,
		label start,
		label end,
		const pointField & internalPoints
		){

	if(mem_.found(key(end,start))){
		pointField nip(internalPoints.size());
		forAll(nip,npI){
			nip[npI] = internalPoints[internalPoints.size() - 1 - npI];
		}
		return setEdge(type,end,start,nip);
	}

	word k  = key(start,end);
	if(!mem_.found(k)){
		mem_.set(k,edgeCounter_);
		if(edgeCounter_ == size()) resize(size() + 1);
		edgeCounter_++;
	}

	OStringStream oss;
	oss << internalPoints;
	IStringStream iss
	(
			type + " " + Foam::name(start) + " " + Foam::name(end) + " " + oss.str()
	);

	set
	(
			mem_[k],
			curvedEdge::New
			(
					bmc_.points(),
					iss
			)
	);

	edgeList_[mem_[k]] = edge(start,end);

	return mem_[k];
}

vector BlockEdgeList::edgeStartTangent(label i) const{

	// try to cast:
	PointLinePath const * path = dynamic_cast< PointLinePath const * >(&(*this)[i]);

	// case this is pointLinePath:
	if(path){
		return path->getTangent(0);
	}

	// else calculate:
	else {

		PointLinePath path(edgePoints(i));
		return path.getTangent(0);
	}

	return vector(0,0,0);
}

vector BlockEdgeList::edgeEndTangent(label i) const{

	// try to cast:
	PointLinePath const * path = dynamic_cast< PointLinePath const * >(&(*this)[i]);

	// case this is pointLinePath:
	if(path){
		return path->getTangent(1);
	}

	// else calculate:
	else {

		PointLinePath path(edgePoints(i));
		return path.getTangent(1);
	}

	return vector(0,0,0);
}

label BlockEdgeList::setEdge(
		label start,
		label end,
		const pointField & internalPoints
		){

	if(mem_.found(key(end,start))){
		pointField nip(internalPoints.size());
		forAll(nip,npI){
			nip[npI] = internalPoints[internalPoints.size() - 1 - npI];
		}
		return setEdge(end,start,nip);
	}

	word k = key(start,end);
	if(!mem_.found(k)){
		mem_.set(k,edgeCounter_);
		if(edgeCounter_ == size())	resize(size() + 1);
		edgeCounter_++;
	}

	set
	(
			mem_[k],
			new PointLinePathEdge
			(
					bmc_.points(),
					start,
					end,
					internalPoints
			)
	);

	edgeList_[mem_[k]] = edge(start,end);

	return mem_[k];
}

void BlockEdgeList::resize(label s){
	label os = size();
	PtrList< curvedEdge >::resize(s);
	for(label i = os; i < size(); i++){
		set
		(
				i,
				new lineEdge
				(
						bmc_.points(),0,0
				)
		);
	}
	edgeList_.resize(s);
}

pointField BlockEdgeList::edgePoints(
		label edgeI,
		bool forceToSteps
		) const{

	// grab edge:
	const curvedEdge & e = (*this)[edgeI];

	// edge steps:
	label edgeSteps = bmc_.edgeSteps()[edgeI];

	// calc edge points:
	pointField epts(edgeSteps - 1);
	{
		// case pointLinePath:
		if( !forceToSteps && e.type().match("pointLinePath") ){
			const PointLinePathEdge & plpe = static_cast< const PointLinePathEdge & >(e);
			pointField pp(plpe.getPoints());
			epts.resize(pp.size() - 2);
			forAll(epts,pI){
	 			epts[pI] = pp[pI + 1];
			}
		}

		// generic case:
		else {
			scalar deltas = 1. / scalar( edgeSteps );
			forAll(epts,pI){
				scalar s = scalar( pI + 1 ) * deltas;
				epts[pI] = e.position(s);
			}
		}
	}

	return epts;
}

label BlockEdgeList::blockEdgeIndex(label blockI, label edgeID) const{
	labelList v = Block::getEdgeVerticesI(edgeID);
	const Block & b = bmc_.blocks()[blockI];
	return index(b.vertexLabels()[v[0]],b.vertexLabels()[v[1]]);
}

// * * * * * * * * * * * * * * IO Functions  * * * * * * * * * * * * * * //

Ostream & operator<<(Ostream & os, const BlockEdgeList & b){

	// open:
	os << token::BEGIN_LIST;

	// loop over edges:
	forAll(b,eI){

		// grab edge:
		const curvedEdge & e = b[eI];

		// calc edge points:
		pointField epts = b.edgePoints(eI,false);

		// output:
		{
			os 		<< e.type() << ' '
					<< e.start() << ' ' << e.end() << ' '
					<< epts
					<< endl;
		}
	}

	// close:
	os << token::END_LIST;

	return os;
}


// ************************************************************************* //

} // End namespace oldev

} // End namespace Foam
