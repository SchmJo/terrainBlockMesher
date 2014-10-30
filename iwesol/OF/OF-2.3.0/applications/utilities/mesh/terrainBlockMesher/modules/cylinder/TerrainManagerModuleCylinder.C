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

#include "TerrainManagerModuleCylinder.H"
#include "TerrainManager.H"

namespace Foam
{

namespace oldev
{

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void TerrainManagerModuleCylinder::initCylinder
(
		const dictionary & dict
){

	// read dictionary:
	cylinderCentre_               = point(dict.lookup("centrePoint"));
	cylinderRadius_               = readScalar(dict.lookup("radius"));
	cylinderRadialBlocks_         = 1;//readLabel(dict.lookup("radialBlocks"));
	cylinderRadialGrading_        = readScalar(dict.lookup("radialGrading"));
	cylinderRadialCells_          = readLabel(dict.lookup("radialBlockCells"));
	cylinderFirstSectionStartDir_ = point(dict.lookup("firstSectionStartDirection"));
	cylinderSectionNr_            = readLabel(dict.lookup("numberOfSections"));

	// check consistency:
	point pCheck = moduleBase().domainBox_.pMin();
	scalar dist  = mag(pCheck - cylinderCentre_);
	if(dist > cylinderRadius_){
		Info << "\nTerrainManager: Error: cylinder radius smaller than distance from box corner SWL to cylinder centre." << endl;
		Info << "   p_SWL            = " << pCheck <<endl;
		Info << "   p_cylinderCentre = " << cylinderCentre_ << endl;
		Info << "   dist             = " << dist << endl;
		Info << "   radius           = " << cylinderRadius_ << endl;
		throw;
	}
	pCheck = moduleBase().domainBox_.pMin()
			+ moduleBase().domainBox_.lengths()[TerrainManager::BASE1] * moduleBase().coordinateSystem().e(TerrainManager::BASE1);
	dist   = mag(pCheck - cylinderCentre_);
	if(dist > cylinderRadius_){
		Info << "\nTerrainManager: Error: cylinder radius smaller than distance from box corner SEL to cylinder centre." << endl;
		Info << "   p_SEL            = " << pCheck <<endl;
		Info << "   p_cylinderCentre = " << cylinderCentre_ << endl;
		Info << "   dist             = " << dist << endl;
		Info << "   radius           = " << cylinderRadius_ << endl;
		throw;
	}
	pCheck = moduleBase().domainBox_.pMin()
			+ moduleBase().domainBox_.lengths()[TerrainManager::BASE2] * moduleBase().coordinateSystem().e(TerrainManager::BASE2);
	dist   = mag(pCheck - cylinderCentre_);
	if(dist > cylinderRadius_){
		Info << "\nTerrainManager: Error: cylinder radius smaller than distance from box corner NWL to cylinder centre." << endl;
		Info << "   p_NWL            = " << pCheck <<endl;
		Info << "   p_cylinderCentre = " << cylinderCentre_ << endl;
		Info << "   dist             = " << dist << endl;
		Info << "   radius           = " << cylinderRadius_ << endl;
		throw;
	}
	pCheck = moduleBase().domainBox_.pMin()
			+ moduleBase().domainBox_.lengths()[TerrainManager::BASE1] * moduleBase().coordinateSystem().e(TerrainManager::BASE1)
			+ moduleBase().domainBox_.lengths()[TerrainManager::BASE2] * moduleBase().coordinateSystem().e(TerrainManager::BASE2);
	dist   = mag(pCheck - cylinderCentre_);
	if(dist > cylinderRadius_){
		Info << "\nTerrainManager: Error: cylinder radius smaller than distance from box corner NEL to cylinder centre." << endl;
		Info << "   p_NEL            = " << pCheck <<endl;
		Info << "   p_cylinderCentre = " << cylinderCentre_ << endl;
		Info << "   dist             = " << dist << endl;
		Info << "   radius           = " << cylinderRadius_ << endl;
		throw;
	}

	// init blending:
	blending_ = ScalarBlendingFunction::New(dict.subDict("blendingFunction"));
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

TerrainManagerModuleCylinder::TerrainManagerModuleCylinder
(
		TerrainManager * terrainManager
):
		ClassModule< TerrainManager >(terrainManager)
{}

// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

TerrainManagerModuleCylinder::~TerrainManagerModuleCylinder()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

bool TerrainManagerModuleCylinder::load
(
			const dictionary & dict
){

	// init:
	initCylinder(dict);

	return ClassModule< TerrainManager >::load(dict);
}

void TerrainManagerModuleCylinder::reserveStorageCylinder(){

	cylinderBlockNr_ = cylinderRadialBlocks_ * moduleBase().walkBoxMaximum();
	cylinderPointNr_ = cylinderRadialBlocks_ * moduleBase().walkBoxMaximum() * 2;

	if(cylinderSectionNr_ > cylinderBlockNr_){
		Info << "\nTerrainManager: Error: The outer cylinder cannot have that many sections." << endl;
		Info << "   sections       : " << cylinderSectionNr_ <<endl;
		Info << "   at most allowed: " << cylinderBlockNr_ << endl;
		throw;
	}

	moduleBase().resizePoints(moduleBase().points().size() + cylinderPointNr_);
	moduleBase().resizeBlocks(moduleBase().blocks().size() + cylinderBlockNr_);
	moduleBase().resizeEdges(moduleBase().edges().size() + 4 * cylinderBlockNr_);

	cylinderPointAdr_.resize(cylinderPointNr_);
	cylinderBlockAdr_.resize(cylinderBlockNr_);

	Info << "      reserved storage for " << cylinderPointNr_ << " points, " << cylinderBlockNr_
			<< " blocks and " << 4 * cylinderBlockNr_ << " edges" << endl;
}


// ************************************************************************* //

} // End namespace oldev

} // End namespace Foam

