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
    Foam::iwesol::BlockManager

Description
    See below.

SourceFiles
    BlockManager.C

References
	[1] J. Schmidt, C. Peralta, B. Stoevesandt, "Automated Generation of
	    Structured Meshes for Wind Energy Applications", Proceedings of the
	    Open Source CFD International Conference, 2012, London, UK

\*---------------------------------------------------------------------------*/

#ifndef BlockManager_H_
#define BlockManager_H_

#include "Outputable.h"

#include "scalar.H"
#include "dictionary.H"
#include "labelList.H"

#include "HasCoordinateSystem.H"
#include "SplineBlock.H"
#include "Patch.H"

namespace Foam{
namespace iwesol{

/**
 * @class BlockManager
 * @brief Base class for the organization of block meshes. Writes to blockMeshDict.
 *
 */
class BlockManager:
	public blib::Outputable,
	public HasCoordinateSystem{

public:

	/// returns linear grading factor of element i
	static scalar getLinearGradingFactor(label i, label gradingBlocks, scalar grading);

	/// returns the sum of the grading factors
	static scalar getLinearGradingSum(label gradingBlocks, scalar grading);

	/// returns the fraction of a distance due to grading
	static scalar getLinearGradingFraction(label i, label gradingBlocks, scalar grading);

	/// returns the fraction for block i of blocks_total, of which the blocks_grading last blocks are graded
	static scalar getGradingFraction(
			label i,
			label blocks_total,
			label blocks_grading,
			scalar gradingFactor,
			bool gradingAtHighI = true
			);

	/// Constructor
	BlockManager(const dictionary & dict, CoordinateSystem * cooSys);

	/// Constructor
	BlockManager(
			CoordinateSystem * cooSys,
			scalar resolution = 0.001,
			const labelList & blocksNrs = labelList(3,5),
			const labelList & cellNrs = labelList(3,5)
			);

	/// Destructor
	virtual ~BlockManager();

	/// BlockManager: Returns block nr i
	virtual BasicBlock & getBasicBlock(label i) = 0;

	/// BlockManager: Returns block nr i
	virtual const BasicBlock & getBasicBlock(label i) const = 0;

	/// BlockManager: Returns the total number of blocks
	virtual label size() const = 0;

	/// Outputable: Provides the data that will be written to the output file.
	virtual blib::Outputable::OData getOData(
			const std::string & type = blib::IO::OFILE::TYPE::EMPTY) const;

	/// BlockManager: check
	virtual bool check(scalar tolerance = 0.000001) const {return true;}

	/// Returns a spline Block
	inline SplineBlock & getSplineBlock(label i) { return dynamic_cast<SplineBlock &>(getBasicBlock(i)); }

	/// Returns a spline Block
	inline const SplineBlock & getSplineBlock(label i) const { return dynamic_cast<const SplineBlock &>(getBasicBlock(i)); }

	/// Returns base normal 1
	inline const Foam::vector & getBaseNormal1() const { return cooSys->e(0); }

	/// Returns base normal 2
	inline const Foam::vector & getBaseNormal2() const { return cooSys->e(1); }

	/// Returns length normal
	inline const Foam::vector & getLengthNormal() const { return cooSys->e(2); }

	/// Returns cell number in a direction
	inline label getCellNr(label i) const { return cellNrs[i]; }

	/// Returns block number in a direction
	inline label getBlockNr(label i) const { return blockNrs[i]; }

	/// Returns spline point number in a direction
	inline label getSplinePointNr(label i) const { return splinePointNrs[i]; }

	/// Return patch
	inline const iwesol::Patch & getPatch(const word & name) const { return patches[patchIndices[name]]; }

	/// Get full grading command
	std::string getGradingCommand() const;

	/// returns point list
	inline const pointField & getPoints() const { return points; }

	/// finds neighbors of all blocks
	void findAllNeighbors();

	/// finds neighbors of a blocks
	void findAllNeighbors(label i);

	/// sets all dummy splines
	void setAllDummySplines();

	/// walk around box labels, starting at SWL (or SWH), going east. returns (i,j,v).
	labelList walkBox(label n,const word & HL, bool counterClockWise = true) const;

	/// returns box walk maximum number
	inline label walkBoxMaximum() const;

	/// checks if topology has been calculated
	inline bool topologyReady() const { return flag_topologyCalculated; }

	/// set a spline that goes along several blocks. nBlokMax < 0 means all
	void setInterBlockSpline(
			const pointField & splinep,
			label iStartBlock,
			label faceToNextBlock,
			label iSpline,
			label nBlockMax = -1
		);

	/** set a spline that goes along several blocks, with start and end
	 * deltaA and deltaB. nBlokMax < 0 means all
	 */
	void setInterBlockSpline(
			const Foam::vector & deltaA,
			const Foam::vector & deltaB,
			label iStartBlock,
			label faceToNextBlock,
			label iSpline,
			label nBlockMax = -1,
			const scalarList & sList = scalarList()
		);

	/** set a spline that goes along several blocks, with start and end
	 * pA and pB, and deltaA and deltaB. nBlokMax < 0 means all
	 */
	void setInterBlockSpline(
			const point & pA,
			const point & pB,
			const Foam::vector & deltaA,
			const Foam::vector & deltaB,
			label iStartBlock,
			label faceToNextBlock,
			label iSpline,
			label nBlockMax = -1,
			const scalarList & sList = scalarList()
		);

	/** set a spline that goes along several blocks, with start and end
	 * pA and deltaA and deltaB. nBlokMax < 0 means all
	 */
	void setInterBlockSpline(
			const point & pA,
			const Foam::vector & deltaA,
			const Foam::vector & deltaB,
			label iStartBlock,
			label faceToNextBlock,
			label iSpline,
			label nBlockMax = -1,
			const scalarList & sList = scalarList()
		);

	/// splines parallel to original spline are interpolated
	void interpolateParallelSplines(
			label iStartBlock,
			label faceToNextBlock,
			label iSplineStart,
			label iSplineEnd,
			label nBlockMax = -1,
			bool trueDistwise_falseIwise = true,
			const scalarList & sList = scalarList(),
			bool withFirst = true,
			bool withLast = true
		);

	/// interpolates a spline from the vertex points
	void interpolateInterBlockSpline(
			label iStartBlock,
			label faceToNextBlock,
			label iSpline,
			label order = 4,
			label nBlockMax = -1
		);

	/// returns the list of s values of the vertices in a block line
	scalarList getBlockLineVertexSList(
			label iStartBlock,
			label iSpline,
			label face,
			label nBlockMax = -1,
			bool withFirst = true,
			bool withLast = true
			);

	/// returns the list of s values of all points and in a block line
	scalarList getBlockLineSList(
			label iStartBlock,
			label iSpline,
			label face,
			label nBlockMax = -1,
			bool withFirst = true,
			bool withLast = true
			) const ;

	/// sets a spline to the average of two neighbors, along a line
	void setAverageInterBlockSpline(
			label iStartBlock,
			label iNeighborBlock,
			label faceAlongLine,
			label iOppositeSpline_thisBlock,
			label iOppositeSpline_neighborBlock,
			label iOppositeSpline_thisBlock_preSpline = SplineBlock::NONE,
			label iOppositeSpline_neighborBlock_preSpline = SplineBlock::NONE,
			label nBlockMax = -1,
			bool withFirst = true,
			bool withLast = true
			);

	/// returns inter block line path
	PointLinePath getInterBlockPath(
			label iStartBlock,
			label iSpline,
			label face,
			label nBlockMax = -1,
			bool withFirst = true,
			bool withLast = true
			) const;

	/// returns inter block vertex path
	PointLinePath getInterBlockVertexPath(
			label iStartBlock,
			label iSpline,
			label face,
			label nBlockMax = -1,
			bool withFirst = true,
			bool withLast = true
			) const;


protected:

	/// The list of vertex points.
	pointField points;

	/// The list of splines
	HashTable<Spline> splines;

	/// The list of patches
	Foam::List<iwesol::Patch> patches;

	/// The patch indices, key = name
	HashTable<label> patchIndices;

	/// Resolution
	scalar resolution;

	/// The number of blocks
	labelList blockNrs;

	/// cell number: radial, azimuthal, length
	labelList cellNrs;

	/// The number of points defining splines
	labelList splinePointNrs;

	/// The grading command
	std::string gradingCommand;

	/// the grading factors
	scalarList gradingFactors;

	/// flag for topology calculation
	bool flag_topologyCalculated;

	/// add a patch
	void addPatch(label i, const word & name, const word & type = "patch");

	/// BlockManager: called by constructor
	virtual void init(const dictionary & dict);

	/// BlockManager: calculate topology
	virtual bool calcTopology() { findAllNeighbors(); return true; }

};

inline void BlockManager::addPatch(label i, const word & name, const word & type){
	patches[i] = iwesol::Patch(name,type);
	patchIndices.set(name,i);
}

inline label BlockManager::walkBoxMaximum() const{
	return 2 * (blockNrs[BasicBlock::X]  + blockNrs[BasicBlock::Y]  );
}

} /* iwesol */
} /* Foam */

#endif /* BlockManager_H_ */
