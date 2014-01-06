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

#include "TerrainManagerModuleBlockGrading.H"
#include "TerrainManager.H"

namespace Foam
{

namespace oldev
{


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

TerrainManagerModuleBlockGrading::TerrainManagerModuleBlockGrading(
		TerrainManager * terrainManager
		):
		ClassModule< TerrainManager >(terrainManager),
		gradingTasks_(3){
}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

bool TerrainManagerModuleBlockGrading::load
(
		const dictionary & dict
){

    // block grading base1:
    if(dict.found("direction1")){

        // get the modify orography tasks:
        gradingTasks_[TerrainManager::BASE1] = getDictTasks(dict.subDict("direction1"),"region");
        Info << "   found " << gradingTasks_[TerrainManager::BASE1].size() << " regions for block grading in direction 1." << endl;
    }
    if(dict.found("direction2")){

        // get the modify orography tasks:
        gradingTasks_[TerrainManager::BASE2] = getDictTasks(dict.subDict("direction2"),"region");
        Info << "   found " << gradingTasks_[TerrainManager::BASE2].size() << " regions for block grading in direction 2." << endl;
    }

	return ClassModule< TerrainManager >::load(dict);
}

void TerrainManagerModuleBlockGrading::getDeltaLists(scalarListList & blockGrading,scalarListList & cellGrading){

	// prepare:
	blockGrading                           = scalarListList(3);
	blockGrading[TerrainManager::BASE1]    = scalarList(moduleBase().blockNrs_[TerrainManager::BASE1] + 1,scalar(0));
	blockGrading[TerrainManager::BASE2]    = scalarList(moduleBase().blockNrs_[TerrainManager::BASE2] + 1,scalar(0));
	cellGrading                            = scalarListList(3);
	cellGrading[TerrainManager::BASE1]     = scalarList(moduleBase().blockNrs_[TerrainManager::BASE1] + 1,scalar(0));
	cellGrading[TerrainManager::BASE2]     = scalarList(moduleBase().blockNrs_[TerrainManager::BASE2] + 1,scalar(0));
	scalarListList weights(3);
	weights[TerrainManager::BASE1] = scalarList(moduleBase().blockNrs_[TerrainManager::BASE1],scalar(1));
	weights[TerrainManager::BASE2] = scalarList(moduleBase().blockNrs_[TerrainManager::BASE2],scalar(1));


	// calc weights:
	for(label dir = 0; dir < 2; dir++){
		if(gradingTasks_[dir].size() > 0){

			// prepare:
			scalarList positions(moduleBase().blockNrs_[dir]); // end points of blocks, first at 0

			// read weight regions:
			List<gradingRegion> regions(gradingTasks_[dir].size());
			scalar widthTot    = 0;
			label blockTot     = 0;
			label currentBlock = 0;
			forAll(gradingTasks_[dir],rI){
				const dictionary & regionDict = gradingTasks_[dir][rI].subDict("region");
				regions[rI].start             = widthTot;
				regions[rI].firstBlock        = currentBlock;
				if(rI < gradingTasks_[dir].size() - 1){
					regions[rI].width  = readScalar(regionDict.lookup("width"));
					regions[rI].blocks = readLabel(regionDict.lookup("blocks"));
					widthTot          += regions[rI].width;
					blockTot          += regions[rI].blocks;
					regions[rI].end    = widthTot;
				} else {
					regions[rI].width  = moduleBase().domainBox_.lengths()[dir] - widthTot;
					regions[rI].blocks = moduleBase().blockNrs_[dir] - blockTot;
					regions[rI].end    = moduleBase().domainBox_.lengths()[dir];
				}
				regions[rI].type       = word(regionDict.lookup("type"));
				currentBlock          += regions[rI].blocks;
				regions[rI].lastBlock  = currentBlock - 1;

			}


			// first set all linear:
			currentBlock = 0;
			forAll(gradingTasks_[dir],rI){

				// grab region:
				const gradingRegion & region = regions[rI];

				// set all linear::
				scalar delta = scalar(region.width) / scalar(region.blocks);
				scalar p0    = currentBlock > 0 ? positions[currentBlock - 1] : 0;
				for(label i = 0; i < region.blocks; i++){
					positions[currentBlock] = p0 + (i + 1) * delta;
					currentBlock++;
				}
			}

			// now the interpolation:
			currentBlock = 0;
			forAll(gradingTasks_[dir],rI){

				// grab region:
				const gradingRegion & region = regions[rI];

				// consider type interpolate:
				if(region.type.compare("interpolating") == 0 && region.blocks > 1){

					// prepare:
					scalarList posL(positions);
					scalarList posR(positions);

					// find goal delta values:
					scalar deltaLin  = scalar(region.width) / scalar(region.blocks);
					scalar goalLeft  = deltaLin;
					scalar goalRight = deltaLin;
					if(region.firstBlock == 1) {
						goalLeft = positions[region.firstBlock - 1];
					} else if(region.firstBlock > 1){
						goalLeft = positions[region.firstBlock - 1] - positions[region.firstBlock - 2];
					}
					if(region.lastBlock < positions.size() - 1) {
						goalRight = positions[region.lastBlock + 1] - positions[region.lastBlock];
					}

					// interpolate:

					// from left:
					label jmax      = region.blocks;
					for(label i = 0; i < region.blocks - 1; i++){

						scalar posLin          = positions[currentBlock] + i * deltaLin;
						scalar posCon          = currentBlock + i == 0 ? 0 : posL[currentBlock + i - 1];
						posCon                += goalLeft;

						label j                = i + 1;
						posL[currentBlock + i] = ( j * posLin + (jmax - j) * posCon ) / jmax;
					}

					// from right:
					jmax    = region.blocks;
					for(label i = region.blocks - 2; i >= 0; i--){

						scalar posLin          = positions[currentBlock] + i * deltaLin;
						scalar posCon          = posR[currentBlock + i + 1] - goalRight;

						label j                = i + 1;
						posR[currentBlock + i] = ( (jmax - j) * posLin + j * posCon ) / jmax;
					}

					// average:
					for(label i = 0; i < region.blocks - 1; i++){

						positions[currentBlock + i] = 0.5 * (
								posL[currentBlock + i] + posR[currentBlock + i]
								);
					}
				}

				// count blocks:
				currentBlock += region.blocks;
			}

			// translate to weights:
			forAll(positions,pI){
				scalar p1 = positions[pI];
				scalar p0 = pI == 0 ? 0 : positions[pI - 1];
				weights[dir][pI] = (p1 - p0) / moduleBase().domainBox_.lengths()[dir];
			}
		}
	}


	// calc deltas:
	scalarList sum(3,scalar(0));
	forAll(weights[TerrainManager::BASE1],wI) sum[TerrainManager::BASE1] += weights[TerrainManager::BASE1][wI];
	forAll(weights[TerrainManager::BASE2],wI) sum[TerrainManager::BASE2] += weights[TerrainManager::BASE2][wI];
	scalar pc = 0;
	forAll(weights[TerrainManager::BASE1],wI){
		blockGrading[TerrainManager::BASE1][wI + 1] =
				moduleBase().domainBox_.lengths()[TerrainManager::BASE1] * weights[TerrainManager::BASE1][wI] / sum[TerrainManager::BASE1];
		pc += blockGrading[TerrainManager::BASE1][wI];
	}
	forAll(weights[TerrainManager::BASE2],wI){
		blockGrading[TerrainManager::BASE2][wI + 1] =
				moduleBase().domainBox_.lengths()[TerrainManager::BASE2] * weights[TerrainManager::BASE2][wI] / sum[TerrainManager::BASE2];
	}

	// calc cell gradings:
	scalarListList cellGradingL(cellGrading);
	scalarListList cellGradingR(cellGrading);
	for(label dir = 0; dir < 2; dir++){

		// cell grading from the left:
		if(gradingTasks_[dir].size() > 0){

			// read weight regions:
			List<gradingRegion> regions(gradingTasks_[dir].size());
			scalar widthTot    = 0;
			label blockTot     = 0;
			label currentBlock = 0;
			forAll(gradingTasks_[dir],rI){
				const dictionary & regionDict = gradingTasks_[dir][rI].subDict("region");
				regions[rI].start             = widthTot;
				regions[rI].firstBlock        = currentBlock;
				if(rI < gradingTasks_[dir].size() - 1){
					regions[rI].width  = readScalar(regionDict.lookup("width"));
					regions[rI].blocks = readLabel(regionDict.lookup("blocks"));
					widthTot          += regions[rI].width;
					blockTot          += regions[rI].blocks;
					regions[rI].end    = widthTot;
				} else {
					regions[rI].width  = moduleBase().domainBox_.lengths()[dir] - widthTot;
					regions[rI].blocks = moduleBase().blockNrs_[dir] - blockTot;
					regions[rI].end    = moduleBase().domainBox_.lengths()[dir];
				}
				regions[rI].type       = word(regionDict.lookup("type"));
				currentBlock          += regions[rI].blocks;
				regions[rI].lastBlock  = currentBlock - 1;

			}

			currentBlock   = 0;
			scalar lastCellWidth = blockGrading[dir][1] / moduleBase().cellNrs_[dir];
			forAll(gradingTasks_[dir],rI){

				// grab region:
				const gradingRegion & region = regions[rI];

				for(label regionBlockCounter = 0; regionBlockCounter < region.blocks; regionBlockCounter++){

					// next block cell width:
					scalar thisCellWidth = blockGrading[dir][currentBlock + 1] / moduleBase().cellNrs_[dir];

					// calc current grading:
					cellGradingL[dir][currentBlock] = thisCellWidth / lastCellWidth;

					// rotate:
					lastCellWidth = thisCellWidth;

					// count blocks:
					currentBlock++;
				}
			}
		}

		// cell grading from the right:
		if(gradingTasks_[dir].size() > 0){

			// read weight regions:
			List<gradingRegion> regions(gradingTasks_[dir].size());
			scalar widthTot    = 0;
			label blockTot     = 0;
			label currentBlock = 0;
			forAll(gradingTasks_[dir],rI){
				const dictionary & regionDict = gradingTasks_[dir][rI].subDict("region");
				regions[rI].start             = widthTot;
				regions[rI].firstBlock        = currentBlock;
				if(rI < gradingTasks_[dir].size() - 1){
					regions[rI].width  = readScalar(regionDict.lookup("width"));
					regions[rI].blocks = readLabel(regionDict.lookup("blocks"));
					widthTot          += regions[rI].width;
					blockTot          += regions[rI].blocks;
					regions[rI].end    = widthTot;
				} else {
					regions[rI].width  = moduleBase().domainBox_.lengths()[dir] - widthTot;
					regions[rI].blocks = moduleBase().blockNrs_[dir] - blockTot;
					regions[rI].end    = moduleBase().domainBox_.lengths()[dir];
				}
				regions[rI].type       = word(regionDict.lookup("type"));
				currentBlock          += regions[rI].blocks;
				regions[rI].lastBlock  = currentBlock - 1;

			}

			currentBlock         = moduleBase().blockNrs_[dir] - 1;
			scalar lastCellWidth = blockGrading[dir][currentBlock + 1] / moduleBase().cellNrs_[dir];
			for(label rI = gradingTasks_[dir].size() - 1; rI >=0; rI--){

				// grab region:
				const gradingRegion & region = regions[rI];

				for(label regionBlockCounter = region.blocks - 1; regionBlockCounter >= 0 ; regionBlockCounter--){

					// next block cell width:
					scalar thisCellWidth = blockGrading[dir][currentBlock + 1] / moduleBase().cellNrs_[dir];

					// calc current grading:
					cellGradingR[dir][currentBlock] =  lastCellWidth / thisCellWidth;

					// rotate:
					lastCellWidth = thisCellWidth;

					// count blocks:
					currentBlock--;
				}
			}
		}

		// average:
		for(label bI = 0; bI < moduleBase().blockNrs_[dir]; bI++){
			cellGrading[dir][bI] = gradingTasks_[dir].size() > 0 ?
					(0.5 * ( cellGradingL[dir][bI] + cellGradingR[dir][bI] )) :
					1;
		}
	}
}


// ************************************************************************* //

} // End namespace oldev

} // End namespace Foam
