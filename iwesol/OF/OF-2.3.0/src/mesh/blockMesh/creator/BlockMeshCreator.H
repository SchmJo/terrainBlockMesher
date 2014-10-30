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

#ifndef BlockMeshCreator_H
#define BlockMeshCreator_H

#include "fvMesh.H"
#include "blockMesh.H"
#include "Block.H"
#include "Time.H"
#include "BlockEdgeList.H"
#include "IOdictionary.H"
#include "HashSet.H"
#include "BlockMeshPatch.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

namespace oldev
{

/*---------------------------------------------------------------------------*\
                         Class BlockMeshCreator Declaration
\*---------------------------------------------------------------------------*/

class BlockMeshCreator
{
    // Private data

		/// the run time
		const Time & runTime_;

		/// the global points
		pointField points_;

		/// the global edges
		BlockEdgeList edges_;

		/// the blocks
		PtrList< Block > blocks_;

		/// the cells along an edge
		labelList edgeSteps_;

		/// the patches
		PtrList< BlockMeshPatch > patches_;

		/// the patch indices
		HashTable< label > patchIndices_;

		/// point-block memory
		List< labelHashSet > pointBlockMem_;

		/// edge-block memory
		List< labelHashSet > edgeBlockMem_;

		/// block neighbors, in face order
		labelListList blockBlocks_;

		/// open edge list (less than 4 blocks)
		labelHashSet openEdgesMem_;

		/// the point counter
		label pointCounter_;

		/// the block counter
		label blockCounter_;


    // Private Member Functions

		/// add edge to memory
		void addEdgeToMem
		(
				label edgeI,
				label start,
				label end
		);

		/// add block to memory
		void addBlockToMem
		(
				label blockI,
				const labelList & points
		);

		/// calculate edge steps
		void calcEdgeSteps(label edgeI);

		/// find neighbor blocks
		void findNeighbours(label blockI);

		/// check consistency of block with neighbors
		bool checkBlock(label blockI);

        /// Disallow default bitwise copy construct
        BlockMeshCreator(const BlockMeshCreator&);

        /// Disallow default bitwise assignment
        void operator=(const BlockMeshCreator&);


protected:

    // Protected Member Functions

        /// resize points
        inline void resizePoints(label size);

        /// resize blocks
        inline void resizeBlocks(label size);

        /// resize blocks
        inline void resizeEdges(label size);

        /// reference to patches
        inline PtrList< BlockMeshPatch > & patchesRef() { return patches_; }

        /// returns point counter
        inline label pointCounter() const { return pointCounter_; }

        /// returns block counter
        inline label blockCounter() const { return blockCounter_; }

        /// returns edge counter
        inline label edgeCounter() const { return edges_.edgeCounter(); }


public:

    // Constructors

        /// Construct from run time
        BlockMeshCreator(const Time & runTime);


    // Member Functions

        // Access

    	/// returns points
    	inline const pointField & points() const { return points_; }

    	/// returns a point
    	inline const point & getPoint(label pointI) const { return points_[pointI]; }

    	/// returns a point
    	inline point & getPoint(label pointI) { return points_[pointI]; }

    	/// returns curved edges
    	inline const BlockEdgeList & edges() const { return edges_; }

    	/// returns blocks
    	inline const PtrList< Block > & blocks() const { return blocks_; }

    	/// return cells along edges
    	inline const labelList & edgeSteps() const { return edgeSteps_; }

    	/// return patches
    	inline const PtrList< BlockMeshPatch > & patches() const { return patches_; }

    	/// return patch indices
    	inline const HashTable< label > & patchIndices() const { return patchIndices_; }

    	/// returns the total number of blocks
    	inline label size() const { return blocks_.size(); }

    	/// returns a block vertex
    	inline label blockVertex(label blockI, label vertexID) const;

    	/// returns point-block memory
    	inline const List< labelHashSet > & pointBlocks() const;

    	/// returns point-block memory
    	inline const List< labelHashSet > & edgeBlocks() const;

    	/// returns block neighbours, in face order
    	inline const labelListList & blockBlocks() const { return blockBlocks_; }

    	/// returns open edges
    	inline const labelHashSet & openEdges() const { return openEdgesMem_; }

    	/// return chain of neighbor blocks
    	labelList neighbourBlockChain
    	(
    			label startBlockI,
    			label toNeighbourFaceID,
    			label numberOfBlocks = 0
    	) const;

    	/// return blockMesh
    	IOdictionary createBlockMeshDict
    	(
    			bool writeDict = false,
    			const word & regionName = Foam::fvMesh::defaultRegion
    	) const;

    	/// return blockMesh
    	autoPtr< blockMesh > createBlockMesh
    	(
    			bool writeDict = false,
    			const word & regionName = Foam::fvMesh::defaultRegion
    	) const;

    	/// return polyMesh
    	autoPtr< polyMesh > createPolyMesh
    	(
    			bool writeDict = false,
    			const word & regionName = Foam::fvMesh::defaultRegion
    	) const;


        // Edit

        /// Clear geometry (internal points, cells, boundaryPatches)
        inline void clearGeom();

    	/// add a point, return index
    	inline label addPoint(const point & p);

    	/// add points
    	inline void addPoints(const pointField & pts);

    	/// add a block, return index
    	label addBlock
    	(
    			const labelList & pointLabels,
    			const Vector< label > & blockCells,
    			const scalarList & gradingFactors = scalarList(3,scalar(1)),
    			const word& zoneName = ""
    	);

    	/// add a block, return index
    	label addBlock
    	(
    			const label & p_SWL,const label & p_SWH,
    			const label & p_NWL,const label & p_NWH,
    			const label & p_SEL,const label & p_SEH,
    			const label & p_NEL,const label & p_NEH,
    			const label cells_x,
    			const label cells_y,
    			const label cells_z,
    			const scalarList & gradingFactors = scalarList(3,scalar(1)),
    			const word& zoneName = ""
    	);

    	/// add a block by adding a new face, return index
    	label addBlock
    	(
    			label startBlockI,
    			label faceID,
    			const face & newFace,
    			label faceOrthogonalCells,
    			const scalarList & gradingFactors = scalarList(3,scalar(1)),
    			const word& zoneName = ""
    	);

    	/// sets an edge, any type. return index
    	label setEdge
    	(
    			label blockI,
    			const word & type,
    			label edgeID,
    			const pointField & internalPoints
    	);

    	/// sets an edge, return index
    	label setEdge
    	(
    			label blockI,
    			label edgeID,
    			const pointField & internalPoints
    	);

    	/// resets an already defined edge
    	inline void resetEdge
    	(
    			label edgeI,
    			const pointField & internalPoints,
    			const word & type = "pointLinePath"
    	);

    	/// sets a linear edge
    	bool setDummyEdge
    	(
    			label blockI,
    			label edgeID,
    			bool overwrite = false
    	);

    	/// sets dummy spline, if needed
    	inline void ensureEdge
    	(
    			label blockI,
    			label edgeID
    	) { setDummyEdge(blockI,edgeID,false); }

    	/// sets all dummy edges
    	void setAllDummyEdges(bool overwrite = false);

    	/// sets dummy splines, if needed
    	inline void ensureAllEdges
    	(
    			label blockI,
    			label edgeID
    	) { setAllDummyEdges(false); }

    	/// add a patch
    	inline void addPatch
    	(
    			const word & name,
    			const word & type
    	);

    	/// contribute to existing patch
    	inline void contributeToPatch
    	(
    			label patchI,
    			label blockI,
    			label faceID
    	);


    // Member Operators

    	/// returns a block
    	inline const Block & operator[](label blockI) const { return blocks_[blockI]; }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace oldev

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "BlockMeshCreatorI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
