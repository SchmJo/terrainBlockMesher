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
    Foam::iwesol::TerrainManager

Description
    See below.

SourceFiles
    TerrainManager.C
    TBMcheck.C

Reference

\*---------------------------------------------------------------------------*/

#ifndef TERRAINMANAGER_H_
#define TERRAINMANAGER_H_

#include "BlockManager.H"
#include "TerrainBlock.H"

#include "modules/cylinder/TerrainManagerModuleCylinder.H"
#include "modules/orographyModifications/TerrainManagerModuleOrographyModifications.H"
#include "modules/blockGrading/TerrainManagerModuleBlockGrading.H"

namespace Foam{
namespace iwesol{

/**
 * @class Foam::iwesol::TerrainManager
 * @brief This class organizes blocks such that they follow a landscape stl.
 *
 */
class TerrainManager:
	public BlockManager {

public:

	/// declare friendship with modules:
	friend class TerrainManagerModuleCylinder;
	friend class TerrainManagerModuleOrographyModifications;
	friend class TerrainManagerModuleBlockGrading;

	/// Constructor
	TerrainManager(
			const dictionary & dict,
			CoordinateSystem * cooSys
	);

	/// Constructor
	TerrainManager(
			const dictionary & dict,
			CoordinateSystem * cooSys,
			searchableSurface const * landscape
	);

	/// Destructor
	virtual ~TerrainManager();

	/// BlockManager: Returns block nr i
	inline BasicBlock & getBasicBlock(label i) { return blocks[i]; }

	/// BlockManager: Returns block nr i
	inline const BasicBlock & getBasicBlock(label i) const { return blocks[i]; }

	/// BlockManager: Returns the total number of blocks
	inline label size() const { return blocks.size(); }

	/// Starts the calculation
	bool calc();

	/// Checks the blocks for consistency: SplineBlock.check
	bool check(scalar tolerance = 0.000001) const;


private:

	/// The stl
	searchableSurface const * landscape;

	/// The list of blocks
	Foam::List<TerrainBlock> blocks;

	/// the point counter
	label pointCounter;

	/// the block counter
	label blockCounter;

	/// The zero point of the box
	point p_corner;

	/// The zero point of the stl
	point p_corner_stl;

	/// List of sizes wrt coordinate axes
	scalarList dimensions;

	/// List of sizes wrt coordinate axes, inside stl
	scalarList dimensions_stl;

	/// A point well above the stl
	point p_above;

	/// The maximal distance searched during projection
	scalar maxDistProj;

	/// Memory of point adressing
	HashTable<label> pointAdr;

	/// Memory of point adressing
	HashTable<label> pointAdr_ijv;

	/// Memory of block adressing
	HashTable<label> blockAdr_ij;


	/// the distance factor for up splines
	scalar splineNormalDistFactor;

	/// the upward spline mode. 0 = nothing to do; 1 = all; 2 = ignoreBoundary
	label mode_upwardSplines;

	/// the zero level for out-of-STL points
	scalar zeroLevel;

	/// the constant A for interpolating function in TerrainBlock
	scalar f_constant_A;

	/// the constant B for interpolating function in TerrainBlock
	scalar f_constant_B;


	/// the cylinder module
	TerrainManagerModuleCylinder cylinderModule;

	/// the modify orography module
	TerrainManagerModuleOrographyModifications modificationModule;

	/// the block grading module
	TerrainManagerModuleBlockGrading gradingModule;


	/// called by constructors
	void init(const dictionary & dict);

	/// Init the points, create blocks:
	void initAll();

	/// adds a point, returns its label
	label _addPoint(const point & p, label upDown);

	/// Returns the key for pointAdr
	word hkey(const point & p, label upDown) const;

	/// block contribution to patches
	void contributeToPatches(label i, label j, const TerrainBlock & block);

	/// calculates orthogonal up splines
	bool calcUpSplines();

	/// BlockManager: calculate topology
	bool calcTopology();
};

} /* iwesol */
} /* Foam */

#endif /* TERRAINMANAGER_H_ */
