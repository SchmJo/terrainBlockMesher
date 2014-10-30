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
#include "Globals.H"
#include "PointLinePath.H"

namespace Foam
{

namespace oldev
{

bool TerrainManagerModuleCylinder::calc(){

	// check request:
	if(!ready()) return false;

	Info << "   calculating outer Cylinder" << endl;

	// reerve storage:
	reserveStorageCylinder();

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
	Foam::vector nFirstSectionStart     = cylinderFirstSectionStartDir_ / mag(cylinderFirstSectionStartDir_);
	scalar sectionDeltaAngle            = 2. * Foam::constant::mathematical::pi / cylinderSectionNr_;
	label oldPointCounter               = moduleBase().pointCounter();
	cylinderConnections_innerOuterKeys_ = HashTable< word >(cylinderBlockNr_); // key = inner point key, value = outer point key
	cylinderConnections_pI_secSubsec_   = HashTable< labelList, label >(cylinderBlockNr_); // key = absolute pointI of inner point, value = (sectionI,subSectionI)

	// set outer section moduleBase().points:
	Foam::vector n(nFirstSectionStart);
	for(label sectionI = 0; sectionI < cylinderSectionNr_; sectionI++){

		// calc point:
		label lp1 = moduleBase().addPoint
		(
				cylinderCentre_ + cylinderRadius_ * n
		);
		label lp2 = moduleBase().addPoint
		(
				cylinderCentre_ + cylinderRadius_ * n
				+ moduleBase().domainBox_.lengths()[TerrainManager::UP]
				* moduleBase().coordinateSystem().e(TerrainManager::UP)
		);

		// memorize:
		cylinderPointAdr_.set(key(sectionI,0,cylinderRadialBlocks_) + ".L",lp1);
		cylinderPointAdr_.set(key(sectionI,0,cylinderRadialBlocks_) + ".H",lp2);

		// rotate:
		moduleBase().coordinateSystem().rotate
		(
				n,
				sectionDeltaAngle,
				-moduleBase().coordinateSystem().e(TerrainManager::UP)
		);
	}

	// connect section border moduleBase().points() by shortest distance:
	cylinderFindSectionConnections();

	// count sub sections:
	cylinderSubSections_ = labelList(cylinderSectionNr_,label(0));
	label sectionI      = -1;
	label reserve       = 0;
	for(label n = 0; n < moduleBase().walkBoxMaximum(); n++){

		// get key L:
		labelList ijvL   = moduleBase().walkBox(n,"L",false);
		word key_inner_L = key(ijvL[0],ijvL[1],ijvL[2]);
		label pI_inner_L = moduleBase().pointAdr_ijv_[key_inner_L];

		// check if cylinderConnections_pI_secSubsec_:
		if(cylinderConnections_pI_secSubsec_.found(pI_inner_L)){
			sectionI = cylinderConnections_pI_secSubsec_[pI_inner_L][0];
		}

		// else add to section:
		else if(sectionI >= 0){
			cylinderSubSections_[sectionI]++;
		}

		// else remember for the last:
		else {
			reserve++;
		}
	}
	cylinderSubSections_[sectionI] += reserve;
	Info << "      cylinder subsections: " << flush;
	label secSum = 0;
	forAll(cylinderSubSections_,sI){
		Info << cylinderSubSections_[sI] << " " << flush;
		secSum += cylinderSubSections_[sI];
	}
	Info << "." << endl;
	Info << "      sum = " << secSum << ", innerPointSum = " << moduleBase().walkBoxMaximum()<<", sections = " <<cylinderSectionNr_<< endl;

	// create outer sub section moduleBase().points():
	n = nFirstSectionStart;
	for(sectionI = 0; sectionI < cylinderSectionNr_; sectionI++){

		// check:
		if(cylinderSubSections_[sectionI] != 0){

			// prepare sub section rotation:
			scalar subSectionDeltaAngle  = scalar(sectionDeltaAngle) / scalar(cylinderSubSections_[sectionI] + 1);
			Foam::vector nSub            = n;

			// loop over sub sections:
			for(label subSectionI = 0; subSectionI < cylinderSubSections_[sectionI]; subSectionI++){

				// rotate:
				moduleBase().coordinateSystem().rotate
				(
						nSub,
						subSectionDeltaAngle,
						-moduleBase().coordinateSystem().e(TerrainManager::UP)
				);

				// calc point:
				label lp1 = moduleBase().addPoint
				(
						cylinderCentre_ + cylinderRadius_ * nSub
				);
				label lp2 = moduleBase().addPoint
				(
						cylinderCentre_ + cylinderRadius_ * nSub
						+ moduleBase().domainBox_.lengths()[TerrainManager::UP]
						* moduleBase().coordinateSystem().e(TerrainManager::UP)
				);

				// memorize:
				cylinderPointAdr_.set(key(sectionI,subSectionI + 1,cylinderRadialBlocks_) + ".L",lp1);
				cylinderPointAdr_.set(key(sectionI,subSectionI + 1,cylinderRadialBlocks_) + ".H",lp2);
			}
		}

		// rotate n:
		moduleBase().coordinateSystem().rotate
		(
				n,
				sectionDeltaAngle,
				-moduleBase().coordinateSystem().e(TerrainManager::UP)
		);
	}

	// connect sub section moduleBase().points():
	sectionI                = -1;
	reserve                 = 0;
	label subSectionCounter = 0;
	for(label n = 0; n < moduleBase().walkBoxMaximum(); n++){

		// get key L:
		labelList ijvL = moduleBase().walkBox(n,"L",false);
		word key_inner_L = key(ijvL[0],ijvL[1],ijvL[2]);
		label pI_inner_L = moduleBase().pointAdr_ijv_[key_inner_L];

		// check if cylinderConnections_pI_secSubsec_:
		if(cylinderConnections_pI_secSubsec_.found(pI_inner_L)){
			sectionI          = cylinderConnections_pI_secSubsec_[pI_inner_L][0];
			subSectionCounter = 0;
		}

		// else connect:
		else if(sectionI >= 0){

			// get key H:
			labelList ijvH = moduleBase().walkBox(n,"H",false);
			word key_inner_H = key(ijvH[0],ijvH[1],ijvH[2]);

			// get outer point keys:
			word key_outer_L = key(sectionI,subSectionCounter + 1,cylinderRadialBlocks_) + ".L";
			word key_outer_H = key(sectionI,subSectionCounter + 1,cylinderRadialBlocks_) + ".H";

			// connect:
			cylinderConnections_innerOuterKeys_.set(key_inner_L,key_outer_L);
			cylinderConnections_innerOuterKeys_.set(key_inner_H,key_outer_H);
			labelList secsubsec(2,0);
			secsubsec[0] = sectionI;
			secsubsec[1] = subSectionCounter + 1;
			cylinderConnections_pI_secSubsec_.set(pI_inner_L,secsubsec);

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
		label pI_inner_L = moduleBase().pointAdr_ijv_[key_inner_L];

		// get key H:
		labelList ijvH = moduleBase().walkBox(n,"H",false);
		word key_inner_H = key(ijvH[0],ijvH[1],ijvH[2]);

		// get outer point keys:
		word key_outer_L = key(sectionI,subSectionCounter + n + 1,cylinderRadialBlocks_) + ".L";
		word key_outer_H = key(sectionI,subSectionCounter + n + 1,cylinderRadialBlocks_) + ".H";

		// connect:
		cylinderConnections_innerOuterKeys_.set(key_inner_L,key_outer_L);
		cylinderConnections_innerOuterKeys_.set(key_inner_H,key_outer_H);
		labelList secsubsec(2,0);
		secsubsec[0] = sectionI;
		secsubsec[1] = subSectionCounter + n + 1;
		cylinderConnections_pI_secSubsec_.set(pI_inner_L,secsubsec);
	}


	// add inner cylinder moduleBase().points():
	for(label n = 0; n < moduleBase().walkBoxMaximum(); n++){

		// get inner point L:
		labelList ijvL          = moduleBase().walkBox(n,"L",false);
		word key_inner_L        = key(ijvL[0],ijvL[1],ijvL[2]);
		const point & p_inner_L = moduleBase().points()[moduleBase().pointAdr_ijv_[key_inner_L]];
		sectionI                = cylinderConnections_pI_secSubsec_[moduleBase().pointAdr_ijv_[key_inner_L]][0];
		label subSectionI       = cylinderConnections_pI_secSubsec_[moduleBase().pointAdr_ijv_[key_inner_L]][1];

		// get outer point L:
		word key_outer_L        = cylinderConnections_innerOuterKeys_[key_inner_L];
		const point & p_outer_L = moduleBase().points()[cylinderPointAdr_[key_outer_L]];

		// get inner point H:
		labelList ijvH          = moduleBase().walkBox(n,"H",false);
		word key_inner_H        = key(ijvH[0],ijvH[1],ijvH[2]);
		const point & p_inner_H = moduleBase().points()[moduleBase().pointAdr_ijv_[key_inner_H]];

		// get outer point H:
		word key_outer_H        = cylinderConnections_innerOuterKeys_[key_inner_H];
		const point & p_outer_H = moduleBase().points()[cylinderPointAdr_[key_outer_H]];

		// linear interpolate L:
		List< point > innerRadialPointsL = getLinearList< point >(
				p_inner_L,
				p_outer_L,
				cylinderRadialBlocks_ - 1,
				false,
				false
				);

		// linear interpolate H:
		List< point > innerRadialPointsH = getLinearList< point >(
				p_inner_H,
				p_outer_H,
				cylinderRadialBlocks_ - 1,
				false,
				false
				);

		// add moduleBase().points():
		forAll(innerRadialPointsL,irpI){

			// L:
			label lp1 = moduleBase().addPoint(innerRadialPointsL[irpI]);
			cylinderPointAdr_.set(key(sectionI,subSectionI,irpI + 1) + ".L",lp1);

			// H:
			label lp2 = moduleBase().addPoint(innerRadialPointsH[irpI]);
			cylinderPointAdr_.set(key(sectionI,subSectionI,irpI + 1) + ".H",lp2);
		}

	}

	Info << "      added " << moduleBase().pointCounter() - oldPointCounter << " points. Total points = " << moduleBase().pointCounter() << endl;

	return true;
}

bool TerrainManagerModuleCylinder::calcCylinderBlocks(){

	// prepare:
	label oldBlockCounter = moduleBase().blockCounter();
	labelList v(8);
	Vector< label > cylCells;
	scalarList gradingList(moduleBase().gradingFactors_);


	// west:
	cylCells[TerrainManager::BASE2]    = moduleBase().cellNrs_[TerrainManager::BASE2];
	cylCells[TerrainManager::BASE1]    = cylinderRadialCells_;
	cylCells[TerrainManager::UP]       = moduleBase().cellNrs_[TerrainManager::UP];
	gradingList[TerrainManager::BASE1] = 1./cylinderRadialGrading_;
	gradingList[TerrainManager::BASE2] = 1;
	for(label j = 0; j < moduleBase().blockNrs_[TerrainManager::BASE2]; j++){

		// get point labels:
		v[Block::SEL] = moduleBase().pointAdr_ijv_[key(0,j,Block::SWL)];
		v[Block::NEL] = moduleBase().pointAdr_ijv_[key(0,j,Block::NWL)];
		v[Block::SEH] = moduleBase().pointAdr_ijv_[key(0,j,Block::SWH)];
		v[Block::NEH] = moduleBase().pointAdr_ijv_[key(0,j,Block::NWH)];

		// get section info:
		labelList secionInfo_SEL = cylinderConnections_pI_secSubsec_[v[Block::SEL]];
		labelList secionInfo_NEL = cylinderConnections_pI_secSubsec_[v[Block::NEL]];

		// loop over radius:
		for(label rI = 0; rI < cylinderRadialBlocks_; rI++){

			// get opposite point labels:
			v[Block::SWL] = cylinderPointAdr_[key(secionInfo_SEL[0],secionInfo_SEL[1],rI + 1) + ".L"];
			v[Block::NWL] = cylinderPointAdr_[key(secionInfo_NEL[0],secionInfo_NEL[1],rI + 1) + ".L"];
			v[Block::SWH] = cylinderPointAdr_[key(secionInfo_SEL[0],secionInfo_SEL[1],rI + 1) + ".H"];
			v[Block::NWH] = cylinderPointAdr_[key(secionInfo_NEL[0],secionInfo_NEL[1],rI + 1) + ".H"];

			// create block:
			label lb = moduleBase().addBlock
			(
					v,
					cylCells,
					gradingList
			);
			cylinderBlockAdr_.set(key(Block::WEST,j,rI),lb);

			// rotate:
			v[Block::SEL] = v[Block::SWL];
			v[Block::NEL] = v[Block::NWL];
			v[Block::SEH] = v[Block::SWH];
			v[Block::NEH] = v[Block::NWH];
		}
	}


	// south:
	cylCells[TerrainManager::BASE1]    = moduleBase().cellNrs_[TerrainManager::BASE1];
	cylCells[TerrainManager::BASE2]    = cylinderRadialCells_;
	cylCells[TerrainManager::UP]       = moduleBase().cellNrs_[TerrainManager::UP];
	gradingList[TerrainManager::BASE2] = 1./cylinderRadialGrading_;
	gradingList[TerrainManager::BASE1] = 1;
	for(label i = 0; i < moduleBase().blockNrs_[TerrainManager::BASE1]; i++){

		// get point labels:
		v[Block::NWL] = moduleBase().pointAdr_ijv_[key(i,0,Block::SWL)];
		v[Block::NEL] = moduleBase().pointAdr_ijv_[key(i,0,Block::SEL)];
		v[Block::NWH] = moduleBase().pointAdr_ijv_[key(i,0,Block::SWH)];
		v[Block::NEH] = moduleBase().pointAdr_ijv_[key(i,0,Block::SEH)];

		// get section info:
		labelList secionInfo_NWL = cylinderConnections_pI_secSubsec_[v[Block::NWL]];
		labelList secionInfo_NEL = cylinderConnections_pI_secSubsec_[v[Block::NEL]];

		// loop over radius:
		for(label rI = 0; rI < cylinderRadialBlocks_; rI++){

			// get opposite point labels:
			v[Block::SWL] = cylinderPointAdr_[key(secionInfo_NWL[0],secionInfo_NWL[1],rI + 1) + ".L"];
			v[Block::SEL] = cylinderPointAdr_[key(secionInfo_NEL[0],secionInfo_NEL[1],rI + 1) + ".L"];
			v[Block::SWH] = cylinderPointAdr_[key(secionInfo_NWL[0],secionInfo_NWL[1],rI + 1) + ".H"];
			v[Block::SEH] = cylinderPointAdr_[key(secionInfo_NEL[0],secionInfo_NEL[1],rI + 1) + ".H"];

			// create block:
			label bl = moduleBase().addBlock
			(
					v,
					cylCells,
					gradingList
			);
			cylinderBlockAdr_.set(key(Block::SOUTH,i,rI),bl);

			// rotate:
			v[Block::NWL] = v[Block::SWL];
			v[Block::NEL] = v[Block::SEL];
			v[Block::NWH] = v[Block::SWH];
			v[Block::NEH] = v[Block::SEH];
		}
	}

	// east:
	cylCells[TerrainManager::BASE2]    = moduleBase().cellNrs_[TerrainManager::BASE2];
	cylCells[TerrainManager::BASE1]    = cylinderRadialCells_;
	cylCells[TerrainManager::UP]       = moduleBase().cellNrs_[TerrainManager::UP];
	gradingList[TerrainManager::BASE1] = cylinderRadialGrading_;
	gradingList[TerrainManager::BASE2] = 1;
	for(label j = 0; j < moduleBase().blockNrs_[TerrainManager::BASE2]; j++){

		// get point labels:
		v[Block::SWL] = moduleBase().pointAdr_ijv_[key(moduleBase().blockNrs_[TerrainManager::BASE1] - 1,j,Block::SEL)];
		v[Block::NWL] = moduleBase().pointAdr_ijv_[key(moduleBase().blockNrs_[TerrainManager::BASE1] - 1,j,Block::NEL)];
		v[Block::SWH] = moduleBase().pointAdr_ijv_[key(moduleBase().blockNrs_[TerrainManager::BASE1] - 1,j,Block::SEH)];
		v[Block::NWH] = moduleBase().pointAdr_ijv_[key(moduleBase().blockNrs_[TerrainManager::BASE1] - 1,j,Block::NEH)];

		// get section info:
		labelList secionInfo_SWL = cylinderConnections_pI_secSubsec_[v[Block::SWL]];
		labelList secionInfo_NWL = cylinderConnections_pI_secSubsec_[v[Block::NWL]];

		// loop over radius:
		for(label rI = 0; rI < cylinderRadialBlocks_; rI++){

			// get opposite point labels:
			v[Block::SEL] = cylinderPointAdr_[key(secionInfo_SWL[0],secionInfo_SWL[1],rI + 1) + ".L"];
			v[Block::NEL] = cylinderPointAdr_[key(secionInfo_NWL[0],secionInfo_NWL[1],rI + 1) + ".L"];
			v[Block::SEH] = cylinderPointAdr_[key(secionInfo_SWL[0],secionInfo_SWL[1],rI + 1) + ".H"];
			v[Block::NEH] = cylinderPointAdr_[key(secionInfo_NWL[0],secionInfo_NWL[1],rI + 1) + ".H"];

			// create block:
			label bl = moduleBase().addBlock
			(
					v,
					cylCells,
					gradingList
			);
			cylinderBlockAdr_.set(key(Block::EAST,j,rI),bl);

			// rotate:
			v[Block::SWL] = v[Block::SEL];
			v[Block::NWL] = v[Block::NEL];
			v[Block::SWH] = v[Block::SEH];
			v[Block::NWH] = v[Block::NEH];
		}
	}

	// north:
	cylCells[TerrainManager::BASE1]    = moduleBase().cellNrs_[TerrainManager::BASE1];
	cylCells[TerrainManager::BASE2]    = cylinderRadialCells_;
	cylCells[TerrainManager::UP]       = moduleBase().cellNrs_[TerrainManager::UP];
	gradingList[TerrainManager::BASE2] = cylinderRadialGrading_;
	gradingList[TerrainManager::BASE1] = 1;
	for(label i = 0; i < moduleBase().blockNrs_[TerrainManager::BASE1]; i++){

		// get point labels:
		v[Block::SWL] = moduleBase().pointAdr_ijv_[key(i,moduleBase().blockNrs_[TerrainManager::BASE2] - 1,Block::NWL)];
		v[Block::SEL] = moduleBase().pointAdr_ijv_[key(i,moduleBase().blockNrs_[TerrainManager::BASE2] - 1,Block::NEL)];
		v[Block::SWH] = moduleBase().pointAdr_ijv_[key(i,moduleBase().blockNrs_[TerrainManager::BASE2] - 1,Block::NWH)];
		v[Block::SEH] = moduleBase().pointAdr_ijv_[key(i,moduleBase().blockNrs_[TerrainManager::BASE2] - 1,Block::NEH)];

		// get section info:
		labelList secionInfo_SWL = cylinderConnections_pI_secSubsec_[v[Block::SWL]];
		labelList secionInfo_SEL = cylinderConnections_pI_secSubsec_[v[Block::SEL]];

		// loop over radius:
		for(label rI = 0; rI < cylinderRadialBlocks_; rI++){

			// get opposite point labels:
			v[Block::NWL] = cylinderPointAdr_[key(secionInfo_SWL[0],secionInfo_SWL[1],rI + 1) + ".L"];
			v[Block::NEL] = cylinderPointAdr_[key(secionInfo_SEL[0],secionInfo_SEL[1],rI + 1) + ".L"];
			v[Block::NWH] = cylinderPointAdr_[key(secionInfo_SWL[0],secionInfo_SWL[1],rI + 1) + ".H"];
			v[Block::NEH] = cylinderPointAdr_[key(secionInfo_SEL[0],secionInfo_SEL[1],rI + 1) + ".H"];

			// create block:
			label bl = moduleBase().addBlock
			(
					v,
					cylCells,
					gradingList
			);
			cylinderBlockAdr_.set(key(Block::NORTH,i,rI),bl);

			// rotate:
			v[Block::SWL] = v[Block::NWL];
			v[Block::SEL] = v[Block::NEL];
			v[Block::SWH] = v[Block::NWH];
			v[Block::SEH] = v[Block::NEH];
		}
	}

	Info << "      added " << moduleBase().blockCounter() - oldBlockCounter
			<< " blocks. Total blocks = " << moduleBase().blockCounter() << endl;

	return true;
}

scalar TerrainManagerModuleCylinder::cylinderCalcSectionDistance(
			label sectionI,
			label n
			){

	// grab outer point:
	word key_outer_L        = key(sectionI,0,cylinderRadialBlocks_) + ".L";
	const point & p_outer_L = moduleBase().points()[cylinderPointAdr_[key_outer_L]];

	// get point inner L:
	labelList ijvL          = moduleBase().walkBox(n,"L",false);
	word key_inner_L        = key(ijvL[0],ijvL[1],ijvL[2]);
	const point & p_inner_L = moduleBase().points()[moduleBase().pointAdr_ijv_[key_inner_L]];

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

void TerrainManagerModuleCylinder::cylinderFindSectionConnections(){

	// preapre:
	labelList deltaNSection(cylinderSectionNr_,label(-1));
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
	for(label sectionI = 1; sectionI < cylinderSectionNr_; sectionI++){
		label nFree             = moduleBase().walkBoxMaximum() - (deltaNSection[sectionI - 1] + 1);
		scalar delta            = scalar(nFree) / scalar(cylinderSectionNr_ - sectionI + 1);
		scalar rest             = delta - label(delta);
		deltaNSection[sectionI] = deltaNSection[sectionI - 1] + label(delta) + (rest >= 0.5 ? 1 : 0);
	}

	// optimize:
	Info << "      optimizing inner-outer section connections." << flush;
	scalarList distances(cylinderSectionNr_,scalar(-1));
	cylinderCalcSectionDistances(nFirst,deltaNSection,distances);
	cylinderIterateSectionConnections(nFirst,deltaNSection,distances);
	Info << endl;

	// perform connection:
	for(label sectionI = 0; sectionI < cylinderSectionNr_; sectionI++){

		// grab outer moduleBase().points():
		word key_outer_L        = key(sectionI,0,cylinderRadialBlocks_) + ".L";
		word key_outer_H        = key(sectionI,0,cylinderRadialBlocks_) + ".H";

		// get point inner L:
		label n           = ( nFirst + deltaNSection[sectionI] ) % moduleBase().walkBoxMaximum();
		labelList ijvL    = moduleBase().walkBox(n,"L",false);
		word key_inner_L  = key(ijvL[0],ijvL[1],ijvL[2]);
		labelList ijvH    = moduleBase().walkBox(n,"H",false);
		word key_inner_H  = key(ijvH[0],ijvH[1],ijvH[2]);

		// connect:
		cylinderConnections_innerOuterKeys_.set(key_inner_L,key_outer_L);
		cylinderConnections_innerOuterKeys_.set(key_inner_H,key_outer_H);
		labelList secsubsec(2,0);
		secsubsec[0] = sectionI;
		cylinderConnections_pI_secSubsec_.set(moduleBase().pointAdr_ijv_[key_inner_L],secsubsec);
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



} // End namespace oldev

} // End namespace Foam

