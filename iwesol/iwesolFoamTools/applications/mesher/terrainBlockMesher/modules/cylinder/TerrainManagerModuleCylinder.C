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

#include "mathematicalConstants.H"
#include "HashSet.H"

namespace Foam{
namespace iwesol{

TerrainManagerModuleCylinder::TerrainManagerModuleCylinder(
		TerrainManager * terrainManager
		):
		ClassModule< TerrainManager >(terrainManager){
}

bool TerrainManagerModuleCylinder::load(
			const dictionary & dict
			){

	// init:
	initCylinder(dict);

	return ClassModule< TerrainManager >::load(dict);
}


void TerrainManagerModuleCylinder::initCylinder(const dictionary & dict){

	// read dictionary:
	cylinderCentre               = point(dict.lookup("centrePoint"));
	cylinderRadius               = readScalar(dict.lookup("radius"));
	cylinderRadialBlocks         = 1;//readLabel(dict.lookup("radialBlocks"));
	cylinderRadialGrading        = readScalar(dict.lookup("radialGrading"));
	cylinderRadialCells          = readLabel(dict.lookup("radialBlockCells"));
	cylinderFirstSectionStartDir = point(dict.lookup("firstSectionStartDirection"));
	cylinderSectionNr            = readLabel(dict.lookup("numberOfSections"));
	cylinderLinearFraction       = readScalar(dict.lookup("interpolationLinearFraction"));

	// check consistency:
	point pCheck = moduleBase().p_corner;
	scalar dist  = mag(pCheck - cylinderCentre);
	if(dist > cylinderRadius){
		Info << "\nTerrainManager: Error: cylinder radius smaller than distance from box corner SWL to cylinder centre." << endl;
		Info << "   p_SWL            = " << pCheck <<endl;
		Info << "   p_cylinderCentre = " << cylinderCentre << endl;
		Info << "   dist             = " << dist << endl;
		Info << "   radius           = " << cylinderRadius << endl;
		throw;
	}
	pCheck = moduleBase().p_corner
			+ moduleBase().dimensions[TerrainBlock::BASE1] * moduleBase().cooSys->e(TerrainBlock::BASE1);
	dist   = mag(pCheck - cylinderCentre);
	if(dist > cylinderRadius){
		Info << "\nTerrainManager: Error: cylinder radius smaller than distance from box corner SEL to cylinder centre." << endl;
		Info << "   p_SEL            = " << pCheck <<endl;
		Info << "   p_cylinderCentre = " << cylinderCentre << endl;
		Info << "   dist             = " << dist << endl;
		Info << "   radius           = " << cylinderRadius << endl;
		throw;
	}
	pCheck = moduleBase().p_corner
			+ moduleBase().dimensions[TerrainBlock::BASE2] * moduleBase().cooSys->e(TerrainBlock::BASE2);
	dist   = mag(pCheck - cylinderCentre);
	if(dist > cylinderRadius){
		Info << "\nTerrainManager: Error: cylinder radius smaller than distance from box corner NWL to cylinder centre." << endl;
		Info << "   p_NWL            = " << pCheck <<endl;
		Info << "   p_cylinderCentre = " << cylinderCentre << endl;
		Info << "   dist             = " << dist << endl;
		Info << "   radius           = " << cylinderRadius << endl;
		throw;
	}
	pCheck = moduleBase().p_corner
			+ moduleBase().dimensions[TerrainBlock::BASE1] * moduleBase().cooSys->e(TerrainBlock::BASE1)
			+ moduleBase().dimensions[TerrainBlock::BASE2] * moduleBase().cooSys->e(TerrainBlock::BASE2);
	dist   = mag(pCheck - cylinderCentre);
	if(dist > cylinderRadius){
		Info << "\nTerrainManager: Error: cylinder radius smaller than distance from box corner NEL to cylinder centre." << endl;
		Info << "   p_NEL            = " << pCheck <<endl;
		Info << "   p_cylinderCentre = " << cylinderCentre << endl;
		Info << "   dist             = " << dist << endl;
		Info << "   radius           = " << cylinderRadius << endl;
		throw;
	}


}

void TerrainManagerModuleCylinder::reserveStorageCylinder(){

	cylinderBlockNr = cylinderRadialBlocks * moduleBase().walkBoxMaximum();
	cylinderPointNr = cylinderRadialBlocks * moduleBase().walkBoxMaximum() * 2;

	if(cylinderSectionNr > cylinderBlockNr){
		Info << "\nTerrainManager: Error: The outer cylinder cannot have that many sections." << endl;
		Info << "   sections       : " << cylinderSectionNr <<endl;
		Info << "   at most allowed: " << cylinderBlockNr << endl;
		throw;
	}

	moduleBase().points.resize(moduleBase().points.size() + cylinderPointNr);
	moduleBase().blocks = Foam::List<TerrainBlock>(moduleBase().blocks.size() + cylinderBlockNr);

	cylinderPointAdr.resize(cylinderPointNr);
	cylinderBlockAdr.resize(cylinderBlockNr);

	Info << "   reserved storage for " << cylinderPointNr << " points and " << cylinderBlockNr
			<< " blocks for the outer cylinder." << endl;

}

bool TerrainManagerModuleCylinder::calc(){

	// check request:
	if(!ready()) return false;

	Info << "   calculating outer Cylinder" << endl;

	// calc moduleBase().points:
	if(!calcCylinderPoints()){
		Info << "\nTerrainManager: Error during calculation of outer cylinder moduleBase().points." << endl;
		return false;
	}

	// calc moduleBase().blocks:
	if(!calcCylinderBlocks()){
		Info << "\nTerrainManager: Error during calculation of outer cylinder moduleBase().blocks." << endl;
		return false;
	}

	// reset moduleBase().patches:
	if(!setCylinderPatches()){
		Info << "\nTerrainManager: Error during calculation of outer cylinder moduleBase().patches." << endl;
		return false;
	}

	// set moduleBase().splines:
	if(!setCylinderSplines()){
		Info << "\nTerrainManager: Error during calculation of outer cylinder moduleBase().splines." << endl;
		return false;
	}


	return true;
}

bool TerrainManagerModuleCylinder::calcCylinderPoints(){

	// prepare:
	Foam::vector nFirstSectionStart    = cylinderFirstSectionStartDir / mag(cylinderFirstSectionStartDir);
	scalar sectionDeltaAngle           = 2. * Foam::constant::mathematical::pi / cylinderSectionNr;
	label oldPointCounter              = moduleBase().pointCounter;
	cylinderConnections_innerOuterKeys = HashTable< word >(cylinderBlockNr); // key = inner point key, value = outer point key
	cylinderConnections_pI_secSubsec   = HashTable< labelList, label >(cylinderBlockNr); // key = absolute pointI of inner point, value = (sectionI,subSectionI)

	// set outer section moduleBase().points:
	Foam::vector n(nFirstSectionStart);
	for(label sectionI = 0; sectionI < cylinderSectionNr; sectionI++){

		// calc point:
		moduleBase().points[moduleBase().pointCounter]     = cylinderCentre + cylinderRadius * n;
		moduleBase().points[moduleBase().pointCounter + 1] = cylinderCentre + cylinderRadius * n
				+ moduleBase().dimensions[TerrainBlock::UP] * moduleBase().get_e(TerrainBlock::UP);

		// memorize and count:
		cylinderPointAdr.set(key(sectionI,0,cylinderRadialBlocks) + ".L",moduleBase().pointCounter);
		cylinderPointAdr.set(key(sectionI,0,cylinderRadialBlocks) + ".H",moduleBase().pointCounter + 1);
		moduleBase().pointCounter += 2;

		// rotate:
		moduleBase().cooSys->rotate(n,sectionDeltaAngle,-moduleBase().get_e(TerrainBlock::UP));
	}

	// connect section border moduleBase().points by shortest distance:
	cylinderFindSectionConnections();

	// count sub sections:
	cylinderSubSections = labelList(cylinderSectionNr,label(0));
	label sectionI      = -1;
	label reserve       = 0;
	for(label n = 0; n < moduleBase().walkBoxMaximum(); n++){

		// get key L:
		labelList ijvL = moduleBase().walkBox(n,"L",false);
		word key_inner_L = key(ijvL[0],ijvL[1],ijvL[2]);
		label pI_inner_L = moduleBase().pointAdr_ijv[key_inner_L];

		// check if cylinderConnections_pI_secSubsec:
		if(cylinderConnections_pI_secSubsec.found(pI_inner_L)){
			sectionI = cylinderConnections_pI_secSubsec[pI_inner_L][0];
		}

		// else add to section:
		else if(sectionI >= 0){
			cylinderSubSections[sectionI]++;
		}

		// else remember for the last:
		else {
			reserve++;
		}
	}
	cylinderSubSections[sectionI] += reserve;
	Info << "      cylinder subsections: " << flush;
	label secSum = 0;
	forAll(cylinderSubSections,sI){
		Info << cylinderSubSections[sI] << " " << flush;
		secSum += cylinderSubSections[sI];
	}
	Info << "." << endl;
	Info << "      sum = " << secSum << ", innerPointSum = " << moduleBase().walkBoxMaximum()<<", sections = " <<cylinderSectionNr<< endl;

	// create outer sub section moduleBase().points:
	n = nFirstSectionStart;
	for(sectionI = 0; sectionI < cylinderSectionNr; sectionI++){

		// check:
		if(cylinderSubSections[sectionI] != 0){

			// prepare sub section rotation:
			scalar subSectionDeltaAngle  = scalar(sectionDeltaAngle) / scalar(cylinderSubSections[sectionI] + 1);
			Foam::vector nSub            = n;

			// loop over sub sections:
			for(label subSectionI = 0; subSectionI < cylinderSubSections[sectionI]; subSectionI++){

				// rotate:
				moduleBase().cooSys->rotate(nSub,subSectionDeltaAngle,-moduleBase().get_e(TerrainBlock::UP));

				// calc point:
				moduleBase().points[moduleBase().pointCounter]     = cylinderCentre + cylinderRadius * nSub;
				moduleBase().points[moduleBase().pointCounter + 1] = cylinderCentre + cylinderRadius * nSub
					+ moduleBase().dimensions[TerrainBlock::UP] * moduleBase().get_e(TerrainBlock::UP);

				// memorize and count:
				cylinderPointAdr.set(key(sectionI,subSectionI + 1,cylinderRadialBlocks) + ".L",moduleBase().pointCounter);
				cylinderPointAdr.set(key(sectionI,subSectionI + 1,cylinderRadialBlocks) + ".H",moduleBase().pointCounter + 1);
				moduleBase().pointCounter += 2;
			}
		}

		// rotate n:
		moduleBase().cooSys->rotate(n,sectionDeltaAngle,-moduleBase().get_e(TerrainBlock::UP));
	}

	// connect sub section moduleBase().points:
	sectionI                = -1;
	reserve                 = 0;
	label subSectionCounter = 0;
	for(label n = 0; n < moduleBase().walkBoxMaximum(); n++){

		// get key L:
		labelList ijvL = moduleBase().walkBox(n,"L",false);
		word key_inner_L = key(ijvL[0],ijvL[1],ijvL[2]);
		label pI_inner_L = moduleBase().pointAdr_ijv[key_inner_L];

		// check if cylinderConnections_pI_secSubsec:
		if(cylinderConnections_pI_secSubsec.found(pI_inner_L)){
			sectionI          = cylinderConnections_pI_secSubsec[pI_inner_L][0];
			subSectionCounter = 0;
		}

		// else connect:
		else if(sectionI >= 0){

			// get key H:
			labelList ijvH = moduleBase().walkBox(n,"H",false);
			word key_inner_H = key(ijvH[0],ijvH[1],ijvH[2]);

			// get outer point keys:
			word key_outer_L = key(sectionI,subSectionCounter + 1,cylinderRadialBlocks) + ".L";
			word key_outer_H = key(sectionI,subSectionCounter + 1,cylinderRadialBlocks) + ".H";

			// connect:
			cylinderConnections_innerOuterKeys.set(key_inner_L,key_outer_L);
			cylinderConnections_innerOuterKeys.set(key_inner_H,key_outer_H);
			labelList secsubsec(2,0);
			secsubsec[0] = sectionI;
			secsubsec[1] = subSectionCounter + 1;
			cylinderConnections_pI_secSubsec.set(pI_inner_L,secsubsec);

			// count:
			subSectionCounter++;
		}

		// else reserve:
		else {
			reserve++;
		}
	}
	// add reserve:
	for(label n = 0; n < reserve; n++){

		// get key L:
		labelList ijvL = moduleBase().walkBox(n,"L",false);
		word key_inner_L = key(ijvL[0],ijvL[1],ijvL[2]);
		label pI_inner_L = moduleBase().pointAdr_ijv[key_inner_L];

		// get key H:
		labelList ijvH = moduleBase().walkBox(n,"H",false);
		word key_inner_H = key(ijvH[0],ijvH[1],ijvH[2]);

		// get outer point keys:
		word key_outer_L = key(sectionI,subSectionCounter + n + 1,cylinderRadialBlocks) + ".L";
		word key_outer_H = key(sectionI,subSectionCounter + n + 1,cylinderRadialBlocks) + ".H";

		// connect:
		cylinderConnections_innerOuterKeys.set(key_inner_L,key_outer_L);
		cylinderConnections_innerOuterKeys.set(key_inner_H,key_outer_H);
		labelList secsubsec(2,0);
		secsubsec[0] = sectionI;
		secsubsec[1] = subSectionCounter + n + 1;
		cylinderConnections_pI_secSubsec.set(pI_inner_L,secsubsec);
	}


	// add inner cylinder moduleBase().points:
	for(label n = 0; n < moduleBase().walkBoxMaximum(); n++){

		// get inner point L:
		labelList ijvL          = moduleBase().walkBox(n,"L",false);
		word key_inner_L        = key(ijvL[0],ijvL[1],ijvL[2]);
		const point & p_inner_L = moduleBase().points[moduleBase().pointAdr_ijv[key_inner_L]];
		sectionI                = cylinderConnections_pI_secSubsec[moduleBase().pointAdr_ijv[key_inner_L]][0];
		label subSectionI       = cylinderConnections_pI_secSubsec[moduleBase().pointAdr_ijv[key_inner_L]][1];

		// get outer point L:
		word key_outer_L        = cylinderConnections_innerOuterKeys[key_inner_L];
		const point & p_outer_L = moduleBase().points[cylinderPointAdr[key_outer_L]];

		// get inner point H:
		labelList ijvH          = moduleBase().walkBox(n,"H",false);
		word key_inner_H        = key(ijvH[0],ijvH[1],ijvH[2]);
		const point & p_inner_H = moduleBase().points[moduleBase().pointAdr_ijv[key_inner_H]];

		// get outer point H:
		word key_outer_H        = cylinderConnections_innerOuterKeys[key_inner_H];
		const point & p_outer_H = moduleBase().points[cylinderPointAdr[key_outer_H]];

		// linear interpolate L:
		List< point > innerRadialPointsL = getLinearList< point >(
				p_inner_L,
				p_outer_L,
				cylinderRadialBlocks - 1,
				false,
				false
				);

		// linear interpolate H:
		List< point > innerRadialPointsH = getLinearList< point >(
				p_inner_H,
				p_outer_H,
				cylinderRadialBlocks - 1,
				false,
				false
				);

		// add moduleBase().points:
		forAll(innerRadialPointsL,irpI){

			// L:
			moduleBase().points[moduleBase().pointCounter] = innerRadialPointsL[irpI];
			cylinderPointAdr.set(key(sectionI,subSectionI,irpI + 1) + ".L",moduleBase().pointCounter);
			moduleBase().pointCounter++;

			// H:
			moduleBase().points[moduleBase().pointCounter] = innerRadialPointsH[irpI];
			cylinderPointAdr.set(key(sectionI,subSectionI,irpI + 1) + ".H",moduleBase().pointCounter);
			moduleBase().pointCounter++;
		}

	}

	Info << "      added " << moduleBase().pointCounter - oldPointCounter << " points. Total points = " << moduleBase().pointCounter << endl;

	return true;
}

bool TerrainManagerModuleCylinder::calcCylinderBlocks(){

	// prepare:
	label oldBlockCounter = moduleBase().blockCounter;
	labelList v(8);
	labelList cylCells(3);
	scalarList gradingList(moduleBase().gradingFactors);


	// west:
	cylCells[TerrainBlock::BASE2]    = moduleBase().cellNrs[TerrainBlock::BASE2];
	cylCells[TerrainBlock::BASE1]    = cylinderRadialCells;
	cylCells[TerrainBlock::UP]       = moduleBase().cellNrs[TerrainBlock::UP];
	gradingList[TerrainBlock::BASE1] = 1./cylinderRadialGrading;
	gradingList[TerrainBlock::BASE2] = 1;
	for(label j = 0; j < moduleBase().blockNrs[TerrainBlock::BASE2]; j++){

		// get point labels:
		v[BasicBlock::SEL] = moduleBase().pointAdr_ijv[key(0,j,BasicBlock::SWL)];
		v[BasicBlock::NEL] = moduleBase().pointAdr_ijv[key(0,j,BasicBlock::NWL)];
		v[BasicBlock::SEH] = moduleBase().pointAdr_ijv[key(0,j,BasicBlock::SWH)];
		v[BasicBlock::NEH] = moduleBase().pointAdr_ijv[key(0,j,BasicBlock::NWH)];

		// get section info:
		labelList secionInfo_SEL = cylinderConnections_pI_secSubsec[v[BasicBlock::SEL]];
		labelList secionInfo_NEL = cylinderConnections_pI_secSubsec[v[BasicBlock::NEL]];

		// loop over radius:
		for(label rI = 0; rI < cylinderRadialBlocks; rI++){

			// get opposite point labels:
			v[BasicBlock::SWL] = cylinderPointAdr[key(secionInfo_SEL[0],secionInfo_SEL[1],rI + 1) + ".L"];
			v[BasicBlock::NWL] = cylinderPointAdr[key(secionInfo_NEL[0],secionInfo_NEL[1],rI + 1) + ".L"];
			v[BasicBlock::SWH] = cylinderPointAdr[key(secionInfo_SEL[0],secionInfo_SEL[1],rI + 1) + ".H"];
			v[BasicBlock::NWH] = cylinderPointAdr[key(secionInfo_NEL[0],secionInfo_NEL[1],rI + 1) + ".H"];

			// create block:
			moduleBase().blocks[moduleBase().blockCounter] =
				TerrainBlock(
					SplineBlock
					(
						&moduleBase().points,
						v,
						cylCells,
						&moduleBase().splines,
						"simpleGrading",
						gradingList
					)
				);
			cylinderBlockAdr.set(key(BasicBlock::WEST,j,rI),moduleBase().blockCounter);
			moduleBase().blockCounter++;

			// rotate:
			v[BasicBlock::SEL] = v[BasicBlock::SWL];
			v[BasicBlock::NEL] = v[BasicBlock::NWL];
			v[BasicBlock::SEH] = v[BasicBlock::SWH];
			v[BasicBlock::NEH] = v[BasicBlock::NWH];
		}
	}


	// south:
	cylCells[TerrainBlock::BASE1]    = moduleBase().cellNrs[TerrainBlock::BASE1];
	cylCells[TerrainBlock::BASE2]    = cylinderRadialCells;
	cylCells[TerrainBlock::UP]       = moduleBase().cellNrs[TerrainBlock::UP];
	gradingList[TerrainBlock::BASE2] = 1./cylinderRadialGrading;
	gradingList[TerrainBlock::BASE1] = 1;
	for(label i = 0; i < moduleBase().blockNrs[TerrainBlock::BASE1]; i++){

		// get point labels:
		v[BasicBlock::NWL] = moduleBase().pointAdr_ijv[key(i,0,BasicBlock::SWL)];
		v[BasicBlock::NEL] = moduleBase().pointAdr_ijv[key(i,0,BasicBlock::SEL)];
		v[BasicBlock::NWH] = moduleBase().pointAdr_ijv[key(i,0,BasicBlock::SWH)];
		v[BasicBlock::NEH] = moduleBase().pointAdr_ijv[key(i,0,BasicBlock::SEH)];

		// get section info:
		labelList secionInfo_NWL = cylinderConnections_pI_secSubsec[v[BasicBlock::NWL]];
		labelList secionInfo_NEL = cylinderConnections_pI_secSubsec[v[BasicBlock::NEL]];

		// loop over radius:
		for(label rI = 0; rI < cylinderRadialBlocks; rI++){

			// get opposite point labels:
			v[BasicBlock::SWL] = cylinderPointAdr[key(secionInfo_NWL[0],secionInfo_NWL[1],rI + 1) + ".L"];
			v[BasicBlock::SEL] = cylinderPointAdr[key(secionInfo_NEL[0],secionInfo_NEL[1],rI + 1) + ".L"];
			v[BasicBlock::SWH] = cylinderPointAdr[key(secionInfo_NWL[0],secionInfo_NWL[1],rI + 1) + ".H"];
			v[BasicBlock::SEH] = cylinderPointAdr[key(secionInfo_NEL[0],secionInfo_NEL[1],rI + 1) + ".H"];

			// create block:
			moduleBase().blocks[moduleBase().blockCounter] =
				TerrainBlock(
					SplineBlock
					(
						&moduleBase().points,
						v,
						cylCells,
						&moduleBase().splines,
						"simpleGrading",
						gradingList
					)
				);
			cylinderBlockAdr.set(key(BasicBlock::SOUTH,i,rI),moduleBase().blockCounter);
			moduleBase().blockCounter++;

			// rotate:
			v[BasicBlock::NWL] = v[BasicBlock::SWL];
			v[BasicBlock::NEL] = v[BasicBlock::SEL];
			v[BasicBlock::NWH] = v[BasicBlock::SWH];
			v[BasicBlock::NEH] = v[BasicBlock::SEH];
		}
	}

	// east:
	cylCells[TerrainBlock::BASE2]    = moduleBase().cellNrs[TerrainBlock::BASE2];
	cylCells[TerrainBlock::BASE1]    = cylinderRadialCells;
	cylCells[TerrainBlock::UP]       = moduleBase().cellNrs[TerrainBlock::UP];
	gradingList[TerrainBlock::BASE1] = cylinderRadialGrading;
	gradingList[TerrainBlock::BASE2] = 1;
	for(label j = 0; j < moduleBase().blockNrs[TerrainBlock::BASE2]; j++){

		// get point labels:
		v[BasicBlock::SWL] = moduleBase().pointAdr_ijv[key(moduleBase().blockNrs[TerrainBlock::BASE1] - 1,j,BasicBlock::SEL)];
		v[BasicBlock::NWL] = moduleBase().pointAdr_ijv[key(moduleBase().blockNrs[TerrainBlock::BASE1] - 1,j,BasicBlock::NEL)];
		v[BasicBlock::SWH] = moduleBase().pointAdr_ijv[key(moduleBase().blockNrs[TerrainBlock::BASE1] - 1,j,BasicBlock::SEH)];
		v[BasicBlock::NWH] = moduleBase().pointAdr_ijv[key(moduleBase().blockNrs[TerrainBlock::BASE1] - 1,j,BasicBlock::NEH)];

		// get section info:
		labelList secionInfo_SWL = cylinderConnections_pI_secSubsec[v[BasicBlock::SWL]];
		labelList secionInfo_NWL = cylinderConnections_pI_secSubsec[v[BasicBlock::NWL]];

		// loop over radius:
		for(label rI = 0; rI < cylinderRadialBlocks; rI++){

			// get opposite point labels:
			v[BasicBlock::SEL] = cylinderPointAdr[key(secionInfo_SWL[0],secionInfo_SWL[1],rI + 1) + ".L"];
			v[BasicBlock::NEL] = cylinderPointAdr[key(secionInfo_NWL[0],secionInfo_NWL[1],rI + 1) + ".L"];
			v[BasicBlock::SEH] = cylinderPointAdr[key(secionInfo_SWL[0],secionInfo_SWL[1],rI + 1) + ".H"];
			v[BasicBlock::NEH] = cylinderPointAdr[key(secionInfo_NWL[0],secionInfo_NWL[1],rI + 1) + ".H"];

			// create block:
			moduleBase().blocks[moduleBase().blockCounter] =
				TerrainBlock(
					SplineBlock
					(
						&moduleBase().points,
						v,
						cylCells,
						&moduleBase().splines,
						"simpleGrading",
						gradingList
					)
				);
			cylinderBlockAdr.set(key(BasicBlock::EAST,j,rI),moduleBase().blockCounter);
			moduleBase().blockCounter++;

			// rotate:
			v[BasicBlock::SWL] = v[BasicBlock::SEL];
			v[BasicBlock::NWL] = v[BasicBlock::NEL];
			v[BasicBlock::SWH] = v[BasicBlock::SEH];
			v[BasicBlock::NWH] = v[BasicBlock::NEH];
		}
	}

	// north:
	cylCells[TerrainBlock::BASE1]    = moduleBase().cellNrs[TerrainBlock::BASE1];
	cylCells[TerrainBlock::BASE2]    = cylinderRadialCells;
	cylCells[TerrainBlock::UP]       = moduleBase().cellNrs[TerrainBlock::UP];
	gradingList[TerrainBlock::BASE2] = cylinderRadialGrading;
	gradingList[TerrainBlock::BASE1] = 1;
	for(label i = 0; i < moduleBase().blockNrs[TerrainBlock::BASE1]; i++){

		// get point labels:
		v[BasicBlock::SWL] = moduleBase().pointAdr_ijv[key(i,moduleBase().blockNrs[TerrainBlock::BASE2] - 1,BasicBlock::NWL)];
		v[BasicBlock::SEL] = moduleBase().pointAdr_ijv[key(i,moduleBase().blockNrs[TerrainBlock::BASE2] - 1,BasicBlock::NEL)];
		v[BasicBlock::SWH] = moduleBase().pointAdr_ijv[key(i,moduleBase().blockNrs[TerrainBlock::BASE2] - 1,BasicBlock::NWH)];
		v[BasicBlock::SEH] = moduleBase().pointAdr_ijv[key(i,moduleBase().blockNrs[TerrainBlock::BASE2] - 1,BasicBlock::NEH)];

		// get section info:
		labelList secionInfo_SWL = cylinderConnections_pI_secSubsec[v[BasicBlock::SWL]];
		labelList secionInfo_SEL = cylinderConnections_pI_secSubsec[v[BasicBlock::SEL]];

		// loop over radius:
		for(label rI = 0; rI < cylinderRadialBlocks; rI++){

			// get opposite point labels:
			v[BasicBlock::NWL] = cylinderPointAdr[key(secionInfo_SWL[0],secionInfo_SWL[1],rI + 1) + ".L"];
			v[BasicBlock::NEL] = cylinderPointAdr[key(secionInfo_SEL[0],secionInfo_SEL[1],rI + 1) + ".L"];
			v[BasicBlock::NWH] = cylinderPointAdr[key(secionInfo_SWL[0],secionInfo_SWL[1],rI + 1) + ".H"];
			v[BasicBlock::NEH] = cylinderPointAdr[key(secionInfo_SEL[0],secionInfo_SEL[1],rI + 1) + ".H"];

			// create block:
			moduleBase().blocks[moduleBase().blockCounter] =
				TerrainBlock(
					SplineBlock
					(
						&moduleBase().points,
						v,
						cylCells,
						&moduleBase().splines,
						"simpleGrading",
						gradingList
					)
				);
			cylinderBlockAdr.set(key(BasicBlock::NORTH,i,rI),moduleBase().blockCounter);
			moduleBase().blockCounter++;

			// rotate:
			v[BasicBlock::SWL] = v[BasicBlock::NWL];
			v[BasicBlock::SEL] = v[BasicBlock::NEL];
			v[BasicBlock::SWH] = v[BasicBlock::NWH];
			v[BasicBlock::SEH] = v[BasicBlock::NEH];
		}
	}

	Info << "      added " << moduleBase().blockCounter - oldBlockCounter << " blocks. Total blocks = " << moduleBase().blockCounter << endl;

	return true;
}

bool TerrainManagerModuleCylinder::setCylinderPatches(){

	// init:
	word groundName = moduleBase().patches[BasicBlock::GROUND].getName();
	word groundType = moduleBase().patches[BasicBlock::GROUND].getType();
	word skyName    = moduleBase().patches[BasicBlock::SKY].getName();
	word skyType    = moduleBase().patches[BasicBlock::SKY].getType();
	moduleBase().patches         = Foam::List<Patch>(cylinderSectionNr + 2);
	for(label sectionI = 0; sectionI < cylinderSectionNr; sectionI++){
		moduleBase().addPatch(sectionI,"section_" + blib::String(sectionI),"patch");
	}
	moduleBase().addPatch(cylinderSectionNr,skyName,skyType);
	moduleBase().addPatch(cylinderSectionNr + 1,groundName,groundType);

	// south:
	labelList v(8);
	for(label i = 0; i < moduleBase().blockNrs[TerrainBlock::BASE1]; i++){

		// get point labels:
		v[BasicBlock::NWL] = moduleBase().pointAdr_ijv[key(i,0,BasicBlock::SWL)];
		v[BasicBlock::NEL] = moduleBase().pointAdr_ijv[key(i,0,BasicBlock::SEL)];
		v[BasicBlock::NWH] = moduleBase().pointAdr_ijv[key(i,0,BasicBlock::SWH)];
		v[BasicBlock::NEH] = moduleBase().pointAdr_ijv[key(i,0,BasicBlock::SEH)];

		// get section info:
		labelList secionInfo_NWL = cylinderConnections_pI_secSubsec[v[BasicBlock::NWL]];
		labelList secionInfo_NEL = cylinderConnections_pI_secSubsec[v[BasicBlock::NEL]];
		label sectionI           = secionInfo_NEL[0];

		// grab block:
		word key_block             = key(BasicBlock::SOUTH,i,cylinderRadialBlocks - 1);
		const TerrainBlock & block = moduleBase().blocks[cylinderBlockAdr[key_block]];

		// add to moduleBase().patches:
		moduleBase().patches[sectionI].addPatch(&block,BasicBlock::SOUTH,key_block);
		moduleBase().patches[cylinderSectionNr].addPatch(&block,BasicBlock::SKY,key_block);
		moduleBase().patches[cylinderSectionNr + 1].addPatch(&block,BasicBlock::GROUND,key_block);
	}

	// east:
	for(label j = 0; j < moduleBase().blockNrs[TerrainBlock::BASE2]; j++){

		// get point labels:
		v[BasicBlock::SWL] = moduleBase().pointAdr_ijv[key(moduleBase().blockNrs[TerrainBlock::BASE1] - 1,j,BasicBlock::SEL)];
		v[BasicBlock::NWL] = moduleBase().pointAdr_ijv[key(moduleBase().blockNrs[TerrainBlock::BASE1] - 1,j,BasicBlock::NEL)];
		v[BasicBlock::SWH] = moduleBase().pointAdr_ijv[key(moduleBase().blockNrs[TerrainBlock::BASE1] - 1,j,BasicBlock::SEH)];
		v[BasicBlock::NWH] = moduleBase().pointAdr_ijv[key(moduleBase().blockNrs[TerrainBlock::BASE1] - 1,j,BasicBlock::NEH)];

		// get section info:
		labelList secionInfo_SWL = cylinderConnections_pI_secSubsec[v[BasicBlock::SWL]];
		labelList secionInfo_NWL = cylinderConnections_pI_secSubsec[v[BasicBlock::NWL]];
		label sectionI           = secionInfo_NWL[0];

		// grab block:
		word key_block             = key(BasicBlock::EAST,j,cylinderRadialBlocks - 1);
		const TerrainBlock & block = moduleBase().blocks[cylinderBlockAdr[key_block]];

		// add to moduleBase().patches:
		moduleBase().patches[sectionI].addPatch(&block,BasicBlock::EAST,key_block);
		moduleBase().patches[cylinderSectionNr].addPatch(&block,BasicBlock::SKY,key_block);
		moduleBase().patches[cylinderSectionNr + 1].addPatch(&block,BasicBlock::GROUND,key_block);
	}

	// north:
	for(label i = 0; i < moduleBase().blockNrs[TerrainBlock::BASE1]; i++){

		// get point labels:
		v[BasicBlock::SWL] = moduleBase().pointAdr_ijv[key(i,moduleBase().blockNrs[TerrainBlock::BASE2] - 1,BasicBlock::NWL)];
		v[BasicBlock::SEL] = moduleBase().pointAdr_ijv[key(i,moduleBase().blockNrs[TerrainBlock::BASE2] - 1,BasicBlock::NEL)];
		v[BasicBlock::SWH] = moduleBase().pointAdr_ijv[key(i,moduleBase().blockNrs[TerrainBlock::BASE2] - 1,BasicBlock::NWH)];
		v[BasicBlock::SEH] = moduleBase().pointAdr_ijv[key(i,moduleBase().blockNrs[TerrainBlock::BASE2] - 1,BasicBlock::NEH)];

		// get section info:
		labelList secionInfo_SWL = cylinderConnections_pI_secSubsec[v[BasicBlock::SWL]];
		labelList secionInfo_SEL = cylinderConnections_pI_secSubsec[v[BasicBlock::SEL]];
		label sectionI           = secionInfo_SWL[0];

		// grab block:
		word key_block             = key(BasicBlock::NORTH,i,cylinderRadialBlocks - 1);
		const TerrainBlock & block = moduleBase().blocks[cylinderBlockAdr[key_block]];

		// add to moduleBase().patches:
		moduleBase().patches[sectionI].addPatch(&block,BasicBlock::NORTH,key_block);
		moduleBase().patches[cylinderSectionNr].addPatch(&block,BasicBlock::SKY,key_block);
		moduleBase().patches[cylinderSectionNr + 1].addPatch(&block,BasicBlock::GROUND,key_block);
	}

	// west:
	for(label j = 0; j < moduleBase().blockNrs[TerrainBlock::BASE2]; j++){

		// get point labels:
		v[BasicBlock::SEL] = moduleBase().pointAdr_ijv[key(0,j,BasicBlock::SWL)];
		v[BasicBlock::NEL] = moduleBase().pointAdr_ijv[key(0,j,BasicBlock::NWL)];
		v[BasicBlock::SEH] = moduleBase().pointAdr_ijv[key(0,j,BasicBlock::SWH)];
		v[BasicBlock::NEH] = moduleBase().pointAdr_ijv[key(0,j,BasicBlock::NWH)];

		// get section info:
		labelList secionInfo_SEL = cylinderConnections_pI_secSubsec[v[BasicBlock::SEL]];
		labelList secionInfo_NEL = cylinderConnections_pI_secSubsec[v[BasicBlock::NEL]];
		label sectionI           = secionInfo_SEL[0];

		// grab block:
		word key_block             = key(BasicBlock::WEST,j,cylinderRadialBlocks - 1);
		const TerrainBlock & block = moduleBase().blocks[cylinderBlockAdr[key_block]];

		// add to moduleBase().patches:
		moduleBase().patches[sectionI].addPatch(&block,BasicBlock::WEST,key_block);
		moduleBase().patches[cylinderSectionNr].addPatch(&block,BasicBlock::SKY,key_block);
		moduleBase().patches[cylinderSectionNr + 1].addPatch(&block,BasicBlock::GROUND,key_block);
	}

	// center:
	for(label i = 0; i < moduleBase().blockNrs[TerrainBlock::BASE1];i++){
		for(label j = 0; j < moduleBase().blockNrs[TerrainBlock::BASE2];j++){

			// grab block:
			word key_block             = key(i,j);
			const TerrainBlock & block = moduleBase().blocks[moduleBase().blockAdr_ij[key_block]];

			// add to moduleBase().patches:
			moduleBase().patches[cylinderSectionNr].addPatch(&block,BasicBlock::SKY,key_block);
			moduleBase().patches[cylinderSectionNr + 1].addPatch(&block,BasicBlock::GROUND,key_block);
		}
	}


	Info << "      created " << moduleBase().patches.size() << " patches." << endl;


	return true;
}

bool TerrainManagerModuleCylinder::setCylinderSplines(){

	Info << "      setting cylinder splines" << endl;

	// west:
	labelList v(8);
	for(label j = 0; j < moduleBase().blockNrs[TerrainBlock::BASE2]; j++){

		// get point labels:
		v[BasicBlock::SEL] = moduleBase().pointAdr_ijv[key(0,j,BasicBlock::SWL)];
		v[BasicBlock::SEH] = moduleBase().pointAdr_ijv[key(0,j,BasicBlock::SWH)];

		// grab moduleBase().blocks:
		word key_cylBlock             = key(BasicBlock::WEST,j,0);
	    TerrainBlock & cylBlock       = moduleBase().blocks[cylinderBlockAdr[key_cylBlock]];
		const TerrainBlock & terBlock = moduleBase().blocks[moduleBase().blockAdr_ij[key(0,j)]];

		// get moduleBase().points:
		point p0_L         = terBlock.getVertex(BasicBlock::SEL);
		point p0_H         = terBlock.getVertex(BasicBlock::SEH);
		const point & p1_L = moduleBase().points[v[BasicBlock::SEL]];
		const point & p1_H = moduleBase().points[v[BasicBlock::SEH]];
		const point & p2_L = cylBlock.getVertex(BasicBlock::SWL);
		const point & p2_H = cylBlock.getVertex(BasicBlock::SWH);

		// calc deltas L:
		Foam::vector deltaA = p1_L - p0_L;
		if(j == 0){
			const point & p0B_L  = terBlock.getVertex(BasicBlock::NWL);
			Foam::vector deltaA2 = p1_L - p0B_L;
			deltaA = 0.5 * (mag(deltaA) + mag(deltaA2)) * 0.5 * (deltaA/mag(deltaA) + deltaA2/mag(deltaA2));
			p0_L   = p1_L - deltaA;
		}
		Foam::vector deltaB = (p1_L - p2_L) * mag(deltaA) / mag(p1_L - p2_L);

		// create path L:
		PointLinePath path_L;
		path_L.addPoint(p0_L);
		path_L.addPoint(p1_L);
		point p_temp = p1_L + (1. - cylinderLinearFraction) * (p2_L - p1_L);
		path_L.addPoint(p_temp);
		p_temp += mag(deltaA) * (p_temp - p1_L) / mag(p_temp - p1_L);
		path_L.addPoint(p_temp);
		path_L.addPoint(p2_L + deltaB);
		path_L.addPoint(p2_L);

		// st spline L:
		pointField pts(cylinderRadialCells - 1);
		scalar s0 = path_L.getPointS(1);
		forAll(pts,pI){
			scalar s = s0 + (pI + 1) * (1. - s0)/(pts.size() + 2);
			pts[pI]  = path_L[s];
		}
		cylBlock.setSpline(SplineBlock::SEL_SWL,pts);

		// calc deltas H:
		deltaA = p1_H - p0_H;
		if(j == 0){
			const point & p0B_H = terBlock.getVertex(BasicBlock::NWH);
			Foam::vector deltaA2 = p1_H - p0B_H;
			deltaA = 0.5 * (mag(deltaA) + mag(deltaA2)) * 0.5 * (deltaA/mag(deltaA) + deltaA2/mag(deltaA2));
			p0_H   = p1_H - deltaA;
		}
		deltaB = (p1_H - p2_H) * mag(deltaA) / mag(p1_H - p2_H);

		// create path H:
		PointLinePath path_H;
		path_H.addPoint(p0_H);
		path_H.addPoint(p1_H);
		p_temp = p1_H + (1. - cylinderLinearFraction) * (p2_H - p1_H);
		path_H.addPoint(p_temp);
		p_temp += mag(deltaA) * (p_temp - p1_H) / mag(p_temp - p1_H);
		path_H.addPoint(p_temp);
		path_H.addPoint(p2_H + deltaB);
		path_H.addPoint(p2_H);

		// st spline H:
		s0 = path_H.getPointS(1);
		forAll(pts,pI){
			scalar s = s0 + (pI + 1) * (1. - s0)/(pts.size() + 2);
			pts[pI]  = path_H[s];
		}
		cylBlock.setSpline(SplineBlock::SEH_SWH,pts);

		// outer arc spline L:
		Foam::vector n = (p2_L - cylinderCentre)/ mag(p2_L - cylinderCentre);
		pointField ptsArc(cylBlock.getCells(TerrainBlock::BASE2) - 1);
		scalar deltaAngle = getAngleRad(n,cylBlock.getVertex(BasicBlock::NWL) - cylinderCentre) / (ptsArc.size() + 2);
		forAll(ptsArc,pI){
			moduleBase().cooSys->rotate(n,deltaAngle,-moduleBase().get_e(TerrainBlock::UP));
			ptsArc[pI] = cylinderCentre + cylinderRadius * n;
		}
		cylBlock.setSpline(SplineBlock::SWL_NWL,ptsArc);

		// outer arc spline H:
		n = (p2_L - cylinderCentre)/ mag(p2_L - cylinderCentre);
		forAll(ptsArc,pI){
			moduleBase().cooSys->rotate(n,deltaAngle,-moduleBase().get_e(TerrainBlock::UP));
			ptsArc[pI]  = cylinderCentre + cylinderRadius * n;
			ptsArc[pI] += ( (p2_H - ptsArc[pI]) & moduleBase().get_e(TerrainBlock::UP) ) * moduleBase().get_e(TerrainBlock::UP);
		}
		cylBlock.setSpline(SplineBlock::SWH_NWH,ptsArc);
	}

	// north:
	for(label i = 0; i <= moduleBase().blockNrs[TerrainBlock::BASE1]; i++){

		// get point labels:
		label i2 = (i == moduleBase().blockNrs[TerrainBlock::BASE1]) ? i - 1 : i;
		v[BasicBlock::SWL] = moduleBase().pointAdr_ijv[key(i2,moduleBase().blockNrs[TerrainBlock::BASE2] - 1,BasicBlock::NWL)];
		v[BasicBlock::SWH] = moduleBase().pointAdr_ijv[key(i2,moduleBase().blockNrs[TerrainBlock::BASE2] - 1,BasicBlock::NWH)];
		if(i != i2){
			v[BasicBlock::SWL] = moduleBase().pointAdr_ijv[key(i2,moduleBase().blockNrs[TerrainBlock::BASE2] - 1,BasicBlock::NEL)];
			v[BasicBlock::SWH] = moduleBase().pointAdr_ijv[key(i2,moduleBase().blockNrs[TerrainBlock::BASE2] - 1,BasicBlock::NEH)];
		}

		// grab moduleBase().blocks:
		word key_cylBlock             = key(BasicBlock::NORTH,i2,0);
		TerrainBlock & cylBlock       = moduleBase().blocks[cylinderBlockAdr[key_cylBlock]];
		const TerrainBlock & terBlock = moduleBase().blocks[moduleBase().blockAdr_ij[key(i2,moduleBase().blockNrs[TerrainBlock::BASE2] - 1)]];

		// get moduleBase().points:
		point p0_L         = terBlock.getVertex(i == i2 ? BasicBlock::SWL : BasicBlock::SEL);
		point p0_H         = terBlock.getVertex(i == i2 ? BasicBlock::SWH : BasicBlock::SEH);
		const point & p1_L = moduleBase().points[v[BasicBlock::SWL]];
		const point & p1_H = moduleBase().points[v[BasicBlock::SWH]];
		const point & p2_L = cylBlock.getVertex(i == i2 ? BasicBlock::NWL : BasicBlock::NEL);
		const point & p2_H = cylBlock.getVertex(i == i2 ? BasicBlock::NWH : BasicBlock::NEH);

		// calc deltas L:
		Foam::vector deltaA = p1_L - p0_L;
		if(i == 0){
			const point & p0B_L = terBlock.getVertex(BasicBlock::NEL);
			Foam::vector deltaA2 = p1_L - p0B_L;
			deltaA = 0.5 * (mag(deltaA) + mag(deltaA2)) * 0.5 * (deltaA/mag(deltaA) + deltaA2/mag(deltaA2));
			p0_L   = p1_L - deltaA;
		} else if(i != i2){
			const point & p0B_L = terBlock.getVertex(BasicBlock::NWL);
			Foam::vector deltaA2 = p1_L - p0B_L;
			deltaA = 0.5 * (mag(deltaA) + mag(deltaA2)) * 0.5 * (deltaA/mag(deltaA) + deltaA2/mag(deltaA2));
			p0_L   = p1_L - deltaA;
		}
		Foam::vector deltaB = (p1_L - p2_L) * mag(deltaA) / mag(p1_L - p2_L);

		// create path L:
		PointLinePath path_L;
		path_L.addPoint(p0_L);
		path_L.addPoint(p1_L);
		point p_temp = p1_L + (1. - cylinderLinearFraction) * (p2_L - p1_L);
		path_L.addPoint(p_temp);
		p_temp += mag(deltaA) * (p_temp - p1_L) / mag(p_temp - p1_L);
		path_L.addPoint(p_temp);
		path_L.addPoint(p2_L + deltaB);
		path_L.addPoint(p2_L);

		// st spline L:
		pointField pts(cylinderRadialCells - 1);
		scalar s0 = path_L.getPointS(1);
		forAll(pts,pI){
			scalar s = s0 + (pI + 1) * (1. - s0)/(pts.size() + 2);
			pts[pI]  = path_L[s];
		}
		cylBlock.setSpline(i == i2 ? SplineBlock::SWL_NWL : SplineBlock::SEL_NEL,pts);

		// calc deltas H:
		deltaA = p1_H - p0_H;
		if(i == 0){
			const point & p0B_H = terBlock.getVertex(BasicBlock::NEH);
			Foam::vector deltaA2 = p1_H - p0B_H;
			deltaA = 0.5 * (mag(deltaA) + mag(deltaA2)) * 0.5 * (deltaA/mag(deltaA) + deltaA2/mag(deltaA2));
			p0_H   = p1_H - deltaA;
		} else if(i != i2){
			const point & p0B_H = terBlock.getVertex(BasicBlock::NWH);
			Foam::vector deltaA2 = p1_H - p0B_H;
			deltaA = 0.5 * (mag(deltaA) + mag(deltaA2)) * 0.5 * (deltaA/mag(deltaA) + deltaA2/mag(deltaA2));
			p0_H   = p1_H - deltaA;
		}
		deltaB = (p1_H - p2_H) * mag(deltaA) / mag(p1_H - p2_H);

		// create path H:
		PointLinePath path_H;
		path_H.addPoint(p0_H);
		path_H.addPoint(p1_H);
		p_temp = p1_H + (1. - cylinderLinearFraction) * (p2_H - p1_H);
		path_H.addPoint(p_temp);
		p_temp += mag(deltaA) * (p_temp - p1_H) / mag(p_temp - p1_H);
		path_H.addPoint(p_temp);
		path_H.addPoint(p2_H + deltaB);
		path_H.addPoint(p2_H);

		// st spline H:
		s0 = path_H.getPointS(1);
		forAll(pts,pI){
			scalar s = s0 + (pI + 1) * (1. - s0)/(pts.size() + 2);
			pts[pI]  = path_H[s];
		}
		cylBlock.setSpline(i == i2 ? SplineBlock::SWH_NWH : SplineBlock::SEH_NEH,pts);

		// outer arc spline L:
		if(i == i2){
			Foam::vector n = (p2_L - cylinderCentre)/ mag(p2_L - cylinderCentre);
			pointField ptsArc(cylBlock.getCells(TerrainBlock::BASE1) - 1);
			scalar deltaAngle = getAngleRad(n,cylBlock.getVertex(BasicBlock::NEL) - cylinderCentre) / (ptsArc.size() + 2);
			forAll(ptsArc,pI){
				moduleBase().cooSys->rotate(n,deltaAngle,-moduleBase().get_e(TerrainBlock::UP));
				ptsArc[pI] = cylinderCentre + cylinderRadius * n;
			}
			cylBlock.setSpline(SplineBlock::NWL_NEL,ptsArc);
		}

		// outer arc spline H:
		if(i == i2){
			Foam::vector n = (p2_L - cylinderCentre)/ mag(p2_L - cylinderCentre);
			pointField ptsArc(cylBlock.getCells(TerrainBlock::BASE1) - 1);
			scalar deltaAngle = getAngleRad(n,cylBlock.getVertex(BasicBlock::NEL) - cylinderCentre) / (ptsArc.size() + 2);
			forAll(ptsArc,pI){
				moduleBase().cooSys->rotate(n,deltaAngle,-moduleBase().get_e(TerrainBlock::UP));
				ptsArc[pI]  = cylinderCentre + cylinderRadius * n;
				ptsArc[pI] += ( (p2_H - ptsArc[pI]) & moduleBase().get_e(TerrainBlock::UP) ) * moduleBase().get_e(TerrainBlock::UP);
			}
			cylBlock.setSpline(SplineBlock::NWH_NEH,ptsArc);
		}
	}

	// east:
	for(label j = 0; j < moduleBase().blockNrs[TerrainBlock::BASE2]; j++){

		// get point labels:
		v[BasicBlock::SWL] = moduleBase().pointAdr_ijv[key(moduleBase().blockNrs[TerrainBlock::BASE1] - 1,j,BasicBlock::SEL)];
		v[BasicBlock::SWH] = moduleBase().pointAdr_ijv[key(moduleBase().blockNrs[TerrainBlock::BASE1] - 1,j,BasicBlock::SEH)];

		// grab moduleBase().blocks:
		word key_cylBlock             = key(BasicBlock::EAST,j,0);
		TerrainBlock & cylBlock       = moduleBase().blocks[cylinderBlockAdr[key_cylBlock]];
		const TerrainBlock & terBlock = moduleBase().blocks[moduleBase().blockAdr_ij[key(moduleBase().blockNrs[TerrainBlock::BASE1] - 1,j)]];

		// get moduleBase().points:
		const point & p0_L = terBlock.getVertex(BasicBlock::SWL);
		const point & p0_H = terBlock.getVertex(BasicBlock::SWH);
		const point & p1_L = moduleBase().points[v[BasicBlock::SWL]];
		const point & p1_H = moduleBase().points[v[BasicBlock::SWH]];
		const point & p2_L = cylBlock.getVertex(BasicBlock::SEL);
		const point & p2_H = cylBlock.getVertex(BasicBlock::SEH);

		if(j > 0){

			// calc deltas L:
			Foam::vector deltaA = p1_L - p0_L;
			Foam::vector deltaB = (p1_L - p2_L) * mag(deltaA) / mag(p1_L - p2_L);

			// create path L:
			PointLinePath path_L;
			path_L.addPoint(p0_L);
			path_L.addPoint(p1_L);
			point p_temp = p1_L + (1. - cylinderLinearFraction) * (p2_L - p1_L);
			path_L.addPoint(p_temp);
			p_temp += mag(deltaA) * (p_temp - p1_L) / mag(p_temp - p1_L);
			path_L.addPoint(p_temp);
			path_L.addPoint(p2_L + deltaB);
			path_L.addPoint(p2_L);

			// st spline L:
			pointField pts(cylinderRadialCells - 1);
			scalar s0 = path_L.getPointS(1);
			forAll(pts,pI){
				scalar s = s0 + (pI + 1) * (1. - s0)/(pts.size() + 2);
				pts[pI]  = path_L[s];
			}
			cylBlock.setSpline(SplineBlock::SWL_SEL,pts);

			// calc deltas H:
			deltaA = p1_H - p0_H;
			deltaB = (p1_H - p2_H) * mag(deltaA) / mag(p1_H - p2_H);

			// create path H:
			PointLinePath path_H;
			path_H.addPoint(p0_H);
			path_H.addPoint(p1_H);
			p_temp = p1_H + (1. - cylinderLinearFraction) * (p2_H - p1_H);
			path_H.addPoint(p_temp);
			p_temp += mag(deltaA) * (p_temp - p1_H) / mag(p_temp - p1_H);
			path_H.addPoint(p_temp);
			path_H.addPoint(p2_H + deltaB);
			path_H.addPoint(p2_H);

			// st spline H:
			s0 = path_H.getPointS(1);
			forAll(pts,pI){
				scalar s = s0 + (pI + 1) * (1. - s0)/(pts.size() + 2);
				pts[pI]  = path_H[s];
			}
			cylBlock.setSpline(SplineBlock::SWH_SEH,pts);
		}

		// outer arc spline L:
		Foam::vector n = (p2_L - cylinderCentre)/ mag(p2_L - cylinderCentre);
		pointField ptsArc(cylBlock.getCells(TerrainBlock::BASE2) - 1);
		scalar deltaAngle = getAngleRad(n,cylBlock.getVertex(BasicBlock::NEL) - cylinderCentre) / (ptsArc.size() + 2);
		forAll(ptsArc,pI){
			moduleBase().cooSys->rotate(n,deltaAngle,moduleBase().get_e(TerrainBlock::UP));
			ptsArc[pI] = cylinderCentre + cylinderRadius * n;
		}
		cylBlock.setSpline(SplineBlock::SEL_NEL,ptsArc);

		// outer arc spline H:
		n = (p2_L - cylinderCentre)/ mag(p2_L - cylinderCentre);
		forAll(ptsArc,pI){
			moduleBase().cooSys->rotate(n,deltaAngle,moduleBase().get_e(TerrainBlock::UP));
			ptsArc[pI]  = cylinderCentre + cylinderRadius * n;
			ptsArc[pI] += ( (p2_H - ptsArc[pI]) & moduleBase().get_e(TerrainBlock::UP) ) * moduleBase().get_e(TerrainBlock::UP);
		}
		cylBlock.setSpline(SplineBlock::SEH_NEH,ptsArc);

	}

	// south:
	for(label i = 0; i <= moduleBase().blockNrs[TerrainBlock::BASE1]; i++){

		// get point labels:
		label i2 = (i == moduleBase().blockNrs[TerrainBlock::BASE1]) ? i - 1 : i;
		v[BasicBlock::NWL] = moduleBase().pointAdr_ijv[key(i2,0,BasicBlock::SWL)];
		v[BasicBlock::NWH] = moduleBase().pointAdr_ijv[key(i2,0,BasicBlock::SWH)];
		if(i != i2){
			v[BasicBlock::NWL] = moduleBase().pointAdr_ijv[key(i2,0,BasicBlock::SEL)];
			v[BasicBlock::NWH] = moduleBase().pointAdr_ijv[key(i2,0,BasicBlock::SEH)];
		}

		// grab moduleBase().blocks:
		word key_cylBlock             = key(BasicBlock::SOUTH,i2,0);
		TerrainBlock & cylBlock       = moduleBase().blocks[cylinderBlockAdr[key_cylBlock]];
		const TerrainBlock & terBlock = moduleBase().blocks[moduleBase().blockAdr_ij[key(i2,0)]];

		// get moduleBase().points:
		point p0_L         = terBlock.getVertex(i == i2 ? BasicBlock::NWL : BasicBlock::NEL);
		point p0_H         = terBlock.getVertex(i == i2 ? BasicBlock::NWH : BasicBlock::NEH);
		const point & p1_L = moduleBase().points[v[BasicBlock::NWL]];
		const point & p1_H = moduleBase().points[v[BasicBlock::NWH]];
		const point & p2_L = cylBlock.getVertex(i == i2 ? BasicBlock::SWL : BasicBlock::SEL);
		const point & p2_H = cylBlock.getVertex(i == i2 ? BasicBlock::SWH : BasicBlock::SEH);

		if(i > 0){
			// calc deltas L:
			Foam::vector deltaA = p1_L - p0_L;
			if(i != i2){
				const point & p0B_L = terBlock.getVertex(BasicBlock::SWL);
				Foam::vector deltaA2 = p1_L - p0B_L;
				deltaA = 0.5 * (mag(deltaA) + mag(deltaA2)) * 0.5 * (deltaA/mag(deltaA) + deltaA2/mag(deltaA2));
				p0_L   = p1_L - deltaA;
			}
			Foam::vector deltaB = (p1_L - p2_L) * mag(deltaA) / mag(p1_L - p2_L);

			// create path L:
			PointLinePath path_L;
			path_L.addPoint(p0_L);
			path_L.addPoint(p1_L);
			point p_temp = p1_L + (1. - cylinderLinearFraction) * (p2_L - p1_L);
			path_L.addPoint(p_temp);
			p_temp += mag(deltaA) * (p_temp - p1_L) / mag(p_temp - p1_L);
			path_L.addPoint(p_temp);
			path_L.addPoint(p2_L + deltaB);
			path_L.addPoint(p2_L);

			// st spline L:
			pointField pts(cylinderRadialCells - 1);
			scalar s0 = path_L.getPointS(1);
			forAll(pts,pI){
				scalar s = s0 + (pI + 1) * (1. - s0)/(pts.size() + 2);
				pts[pI]  = path_L[s];
			}
			cylBlock.setSpline(i == i2 ? SplineBlock::NWL_SWL : SplineBlock::NEL_SEL,pts);

			// calc deltas H:
			deltaA = p1_H - p0_H;
			if(i != i2){
				const point & p0B_H = terBlock.getVertex(BasicBlock::SWH);
				Foam::vector deltaA2 = p1_H - p0B_H;
				deltaA = 0.5 * (mag(deltaA) + mag(deltaA2)) * 0.5 * (deltaA/mag(deltaA) + deltaA2/mag(deltaA2));
				p0_H   = p1_H - deltaA;
			}
			deltaB = (p1_H - p2_H) * mag(deltaA) / mag(p1_H - p2_H);

			// create path H:
			PointLinePath path_H;
			path_H.addPoint(p0_H);
			path_H.addPoint(p1_H);
			p_temp = p1_H + (1. - cylinderLinearFraction) * (p2_H - p1_H);
			path_H.addPoint(p_temp);
			p_temp += mag(deltaA) * (p_temp - p1_H) / mag(p_temp - p1_H);
			path_H.addPoint(p_temp);
			path_H.addPoint(p2_H + deltaB);
			path_H.addPoint(p2_H);

			// st spline H:
			s0 = path_H.getPointS(1);
			forAll(pts,pI){
				scalar s = s0 + (pI + 1) * (1. - s0)/(pts.size() + 2);
				pts[pI]  = path_H[s];
			}
			cylBlock.setSpline(i == i2 ? SplineBlock::NWH_SWH : SplineBlock::NEH_SEH,pts);
		}

		// outer arc spline L:
		if(i == i2){
			Foam::vector n = (p2_L - cylinderCentre)/ mag(p2_L - cylinderCentre);
			pointField ptsArc(cylBlock.getCells(TerrainBlock::BASE2) - 1);
			scalar deltaAngle = getAngleRad(n,cylBlock.getVertex(BasicBlock::SEL) - cylinderCentre) / (ptsArc.size() + 2);
			forAll(ptsArc,pI){
				moduleBase().cooSys->rotate(n,deltaAngle,moduleBase().get_e(TerrainBlock::UP));
				ptsArc[pI] = cylinderCentre + cylinderRadius * n;
			}
			cylBlock.setSpline(SplineBlock::SWL_SEL,ptsArc);
		}

		// outer arc spline H:
		if(i == i2){
			Foam::vector n = (p2_L - cylinderCentre)/ mag(p2_L - cylinderCentre);
			pointField ptsArc(cylBlock.getCells(TerrainBlock::BASE2) - 1);
			scalar deltaAngle = getAngleRad(n,cylBlock.getVertex(BasicBlock::SEL) - cylinderCentre) / (ptsArc.size() + 2);
			forAll(ptsArc,pI){
				moduleBase().cooSys->rotate(n,deltaAngle,moduleBase().get_e(TerrainBlock::UP));
				ptsArc[pI]  = cylinderCentre + cylinderRadius * n;
				ptsArc[pI] += ( (p2_H - ptsArc[pI]) & moduleBase().get_e(TerrainBlock::UP) ) * moduleBase().get_e(TerrainBlock::UP);
			}
			cylBlock.setSpline(SplineBlock::SWH_SEH,ptsArc);
		}
	}

	return true;
}

void TerrainManagerModuleCylinder::cylinderFindSectionConnections(){

	// preapre:
	labelList deltaNSection(cylinderSectionNr,label(-1));
	deltaNSection[0] = 0;
	label nFirst     = 0;

	// connect first section start by shortest distance:
	{
		label sectionI = 0;

		// prepare:
		scalar minDistL = -1;

		//find:
		for(label n = 0; n < moduleBase().walkBoxMaximum(); n++){

			// dist:
			scalar dist = cylinderCalcSectionDistance(sectionI,n);

			// compare:
			if(minDistL < 0 || dist < minDistL){
				minDistL        = dist;
				nFirst          = n;
			}
		}
	}

	// distribute other section corners naively:
	for(label sectionI = 1; sectionI < cylinderSectionNr; sectionI++){
		label nFree             = moduleBase().walkBoxMaximum() - (deltaNSection[sectionI - 1] + 1);
		scalar delta            = scalar(nFree) / scalar(cylinderSectionNr - sectionI + 1);
		scalar rest             = delta - label(delta);
		deltaNSection[sectionI] = deltaNSection[sectionI - 1] + label(delta) + (rest >= 0.5 ? 1 : 0);
	}

	// optimize:
	Info << "      optimizing inner-outer section connections." << flush;
	scalarList distances(cylinderSectionNr,scalar(-1));
	cylinderCalcSectionDistances(nFirst,deltaNSection,distances);
	cylinderIterateSectionConnections(nFirst,deltaNSection,distances);
	Info << endl;

	// perform connection:
	for(label sectionI = 0; sectionI < cylinderSectionNr; sectionI++){

		// grab outer moduleBase().points:
		word key_outer_L        = key(sectionI,0,cylinderRadialBlocks) + ".L";
		word key_outer_H        = key(sectionI,0,cylinderRadialBlocks) + ".H";

		// get point inner L:
		label n           = ( nFirst + deltaNSection[sectionI] ) % moduleBase().walkBoxMaximum();
		labelList ijvL    = moduleBase().walkBox(n,"L",false);
		word key_inner_L  = key(ijvL[0],ijvL[1],ijvL[2]);
		labelList ijvH    = moduleBase().walkBox(n,"H",false);
		word key_inner_H  = key(ijvH[0],ijvH[1],ijvH[2]);

		// connect:
		cylinderConnections_innerOuterKeys.set(key_inner_L,key_outer_L);
		cylinderConnections_innerOuterKeys.set(key_inner_H,key_outer_H);
		labelList secsubsec(2,0);
		secsubsec[0] = sectionI;
		cylinderConnections_pI_secSubsec.set(moduleBase().pointAdr_ijv[key_inner_L],secsubsec);
	}

}

scalar TerrainManagerModuleCylinder::cylinderCalcSectionDistance(
			label sectionI,
			label n
			){

	// grab outer point:
	word key_outer_L        = key(sectionI,0,cylinderRadialBlocks) + ".L";
	const point & p_outer_L = moduleBase().points[cylinderPointAdr[key_outer_L]];

	// get point inner L:
	labelList ijvL          = moduleBase().walkBox(n,"L",false);
	word key_inner_L        = key(ijvL[0],ijvL[1],ijvL[2]);
	const point & p_inner_L = moduleBase().points[moduleBase().pointAdr_ijv[key_inner_L]];

	return mag(p_outer_L - p_inner_L);
}

void TerrainManagerModuleCylinder::cylinderCalcSectionDistances(
			label nFirst,
			const labelList & deltaNSection,
			scalarList & distances
			){
	forAll(deltaNSection,sectionI){
		label n = (nFirst + deltaNSection[sectionI]) % moduleBase().walkBoxMaximum();
		distances[sectionI] = cylinderCalcSectionDistance(sectionI,n);
	}
}

void TerrainManagerModuleCylinder::cylinderIterateSectionConnections(
		label nFirst,
		labelList & deltaNSection,
		scalarList & distances
		){

	Info <<"." << flush;

	// prepare:
	bool news = false;
	HashSet< label > nList(deltaNSection);

	// loop:
	forAll(deltaNSection,sectionI){

		// define ns:
		label n0        = (nFirst + deltaNSection[sectionI]) % moduleBase().walkBoxMaximum();
		label nMinus    = (moduleBase().walkBoxMaximum() + n0 - 1) % moduleBase().walkBoxMaximum();
		label nPlus     = (n0 + 1) % moduleBase().walkBoxMaximum();
		label delNMinus = (moduleBase().walkBoxMaximum() + nMinus - nFirst) % moduleBase().walkBoxMaximum();
		label delNPlus  = (moduleBase().walkBoxMaximum() + nPlus - nFirst) % moduleBase().walkBoxMaximum();

		// calc plus minus distances:
		scalar distMinus = -1;
		scalar distPlus  = -1;
		if(!nList.found(delNMinus)){
			distMinus = cylinderCalcSectionDistance(sectionI,nMinus);
		}
		if(!nList.found(delNPlus)){
			distPlus = cylinderCalcSectionDistance(sectionI,nPlus);
		}

		// evaluate:
		scalar newn    = -1;
		scalar newDist = distances[sectionI];
		if(distMinus > 0 && distMinus < newDist){
			newn    = delNMinus;
			newDist = distMinus;
		}
		if(distPlus > 0 && distPlus < newDist){
			newn    = delNPlus;
			newDist = distPlus;
		}

		// update:
		if(newn > 0){
			nList.erase(deltaNSection[sectionI]);
			deltaNSection[sectionI] = newn;
			distances[sectionI]     = newDist;
			news                    = true;
			nList.set(newn);
		}
	}

	// if news, repeat:
	if(news) cylinderIterateSectionConnections(nFirst,deltaNSection,distances);
}

} /* iwesol */
} /* Foam */
