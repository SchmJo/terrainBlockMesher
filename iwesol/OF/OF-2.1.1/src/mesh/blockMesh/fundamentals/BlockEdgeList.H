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

Class
    Foam::oldev::BlockEdgeList

Description

SourceFiles
    BlockEdgeListI.H
    BlockEdgeList.C
    BlockEdgeListIO.C

\*---------------------------------------------------------------------------*/

#ifndef BlockEdgeList_H
#define BlockEdgeList_H

#include "curvedEdge.H"
#include "HashTable.H"
#include "edgeList.H"
#include "Globals.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward declaration of classes
class Ostream;

namespace oldev
{

// Forward declaration of friend functions and operators
class BlockEdgeList;
class BlockMeshCreator;
Ostream& operator<<(Ostream&, const BlockEdgeList&);


/*---------------------------------------------------------------------------*\
                         Class BlockEdgeList Declaration
\*---------------------------------------------------------------------------*/

class BlockEdgeList
:
    public PtrList< curvedEdge >
{
    // Private data

    	/// the underlying block mesh creator
    	const BlockMeshCreator & bmc_;

    	/// memory of edges. key = "i.j". i < j
    	HashTable< label > mem_;

    	/// the edge list
    	edgeList edgeList_;

    	/// the edge counter
    	label edgeCounter_;


    // Private Member Functions

        /// Disallow default bitwise copy construct
        BlockEdgeList(const BlockEdgeList&);

        /// Disallow default bitwise assignment
        void operator=(const BlockEdgeList&);


public:


    // Constructors

        /// Construct from components
        BlockEdgeList(const BlockMeshCreator & bmc);



    // Member Functions

        // Access

    	/// returns edge list
    	inline const edgeList & getEdgeList() const { return edgeList_; }

    	/// returns edge index
    	inline label index(label pI, label pJ) const;

    	/// returns edge index
    	label blockEdgeIndex(label blockI, label edgeID) const;

    	/// returns an edge
    	inline const curvedEdge & getEdge(label pI, label pJ) const { return (*this)[index(pI,pJ)]; }

    	/// returns relevant edge points
    	pointField edgePoints(label edgeI, bool forceToSteps = false) const;

    	/// returns edge counter
    	inline label edgeCounter() const { return edgeCounter_; }

    	/// returns edge tangent
    	vector edgeStartTangent(label i) const;

    	/// returns edge tangent
    	vector edgeEndTangent(label i) const;


        // Check

    	/// checks if edge is found
    	inline bool found(label pI, label pJ) const { return mem_.found(key(pI,pJ)); }

    	/// checks if edge is found
    	inline bool foundInBlock(label blockI, label edgeID) const;


        // Edit

    	/// resize
    	void resize(label size);

    	/// set edge, return index
    	label setEdge
    	(
    			const word & type,
    			label start,
    			label end,
    			const pointField & internalPoints
    	);

    	/// set edge, return index
    	label setEdge
    	(
    			label start,
    			label end,
    			const pointField & internalPoints
    	);


    // IOstream Operators

        friend Ostream& operator<<(Ostream&, const BlockEdgeList&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace oldev

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "BlockEdgeListI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
