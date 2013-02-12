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
    Foam::iwesol::TerrainManagerModuleCylinder

Description
    See below.

SourceFiles
    TerrainManagerModuleCylinder.C

Reference

\*---------------------------------------------------------------------------*/

#ifndef TerrainManagerModuleCylinder_H_
#define TerrainManagerModuleCylinder_H_

#include "point.H"
#include "scalarList.H"
#include "labelList.H"

#include "ClassModule.H"

namespace Foam{
namespace iwesol{

class TerrainManager;

/**
 * @class Foam::iwesol::TerrainManagerModuleCylinder
 * @brief The terrainBlockMesher module for an outer cylinder around the rectangular landscape.
 */
class TerrainManagerModuleCylinder:
	public ClassModule< TerrainManager >{

public:

	/// Constructor
	TerrainManagerModuleCylinder(
			TerrainManager * terrainManager
			);

	/// ClassModule: load the module
	bool load(
			const dictionary & dict
			);

	/// provide storage
	void reserveStorageCylinder();

	/// calculate
	bool calc();


private:

	/// the cylinder centre
	point cylinderCentre;

	/// the cylinder radius
	scalar cylinderRadius;

	/// the cylinder radial blocks
	label cylinderRadialBlocks;

	/// the cylinder radial cells
	label cylinderRadialCells;

	/// the cylinder radial grading
	label cylinderRadialGrading;

	/// the total number of pure cylinder blocks
	label cylinderBlockNr;

	/// the total number of pure cylinder points
	label cylinderPointNr;

	/// the direction of the first cylinder sector
	Foam::vector cylinderFirstSectionStartDir;

	/// the number of cylinder sectors
	label cylinderSectionNr;

	/// the subsections
	labelList cylinderSubSections;

	/// the linear interpolation fraction
	scalar cylinderLinearFraction;

	/// the cylinder point addressing. key = "sectionI.subsectionI.radialI.LH"
	HashTable< label > cylinderPointAdr;

	/// the cylinder block addressing. key = "faceI.ij.radialI"
	HashTable< label > cylinderBlockAdr;

	/// inner point to cylinder connections. key = inner point key, value = outer point key
	HashTable< word > cylinderConnections_innerOuterKeys;

	/// inner point to cylinder connections. key = absolute pointI of inner point, value = (sectionI,subSectionI)
	HashTable< labelList, label > cylinderConnections_pI_secSubsec;

	/// init the cylinder
	void initCylinder(const dictionary & dict);

	/// find inner-outer corner connections
	void cylinderFindSectionConnections();

	/// iterate section connections
	void cylinderIterateSectionConnections(
			label nFirst,
			labelList & deltaNSection,
			scalarList & distances
			);

	/// calculate inner-outer distances
	void cylinderCalcSectionDistances(
			label nFirst,
			const labelList & deltaNSection,
			scalarList & distances
			);

	/// calculate single inner-outer distance
	scalar cylinderCalcSectionDistance(
			label sectionI,
			label n
			);

	/// calculate points
	bool calcCylinderPoints();

	/// calculate blocks
	bool calcCylinderBlocks();

	/// set Patches
	bool setCylinderPatches();

	/// set splines
	bool setCylinderSplines();

};

} /* iwesol */
} /* Foam */

#endif /* TerrainManagerModuleCylinder_H_ */
