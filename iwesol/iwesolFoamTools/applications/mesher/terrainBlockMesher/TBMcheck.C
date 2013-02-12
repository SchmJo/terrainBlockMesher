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

#include "TerrainManager.H"

namespace Foam{
namespace iwesol{

bool TerrainManager::check(scalar tolerance) const{

	// check if splines make sense:
	Info << "   Checking spline definitions" << endl;
	forAll(blocks,bI){
		if(!blocks[bI].check(tolerance)){
			Info << "\nTerrainManager: Error in block " << bI << endl;
			return false;
		}
	}

	// check block shared points:
	Info << "   Checking common vertices" << endl;
	for(int i = 0; i < blockNrs[0]; i++){
		for(int j = 0; j < blockNrs[1]; j++){

			// grab block:
			const TerrainBlock & blockA = blocks[blockAdr_ij[key(i,j)]];

			// check east neighbor:
			if(i < blockNrs[0] - 1){
				const TerrainBlock & blockB = blocks[blockAdr_ij[key(i + 1,j)]];
				if(
						mag(blockA.getVertex(BasicBlock::SEL) - blockB.getVertex(BasicBlock::SWL)) > tolerance ||
						mag(blockA.getVertex(BasicBlock::SEH) - blockB.getVertex(BasicBlock::SWH)) > tolerance ||
						mag(blockA.getVertex(BasicBlock::NEL) - blockB.getVertex(BasicBlock::NWL)) > tolerance ||
						mag(blockA.getVertex(BasicBlock::NEH) - blockB.getVertex(BasicBlock::NWH)) > tolerance
				){
					Info << "\nTerrainManager: Error: Block vertex mismatch at west-east face between blocks" << endl;
					Info << "   blockA = " << blockA.getVertexPoints() << endl;
					Info << "   blockB = " << blockB.getVertexPoints() << endl;
					return false;
				}
			}

			// check west neighbor:
			if(i > 0){
				const TerrainBlock & blockB = blocks[blockAdr_ij[key(i - 1,j)]];
				if(
						mag(blockB.getVertex(BasicBlock::SEL) - blockA.getVertex(BasicBlock::SWL)) > tolerance ||
						mag(blockB.getVertex(BasicBlock::SEH) - blockA.getVertex(BasicBlock::SWH)) > tolerance ||
						mag(blockB.getVertex(BasicBlock::NEL) - blockA.getVertex(BasicBlock::NWL)) > tolerance ||
						mag(blockB.getVertex(BasicBlock::NEH) - blockA.getVertex(BasicBlock::NWH)) > tolerance
				){
					Info << "\nTerrainManager: Error: Block vertex mismatch at west-east face between blocks" << endl;
					Info << "   blockA = " << blockA.getVertexPoints() << endl;
					Info << "   blockB = " << blockB.getVertexPoints() << endl;
					return false;
				}
			}

			// check north neighbor:
			if(j < blockNrs[0] - 1){
				const TerrainBlock & blockB = blocks[blockAdr_ij[key(i,j + 1)]];
				if(
						mag(blockA.getVertex(BasicBlock::NWL) - blockB.getVertex(BasicBlock::SWL)) > tolerance ||
						mag(blockA.getVertex(BasicBlock::NEL) - blockB.getVertex(BasicBlock::SEL)) > tolerance ||
						mag(blockA.getVertex(BasicBlock::NWH) - blockB.getVertex(BasicBlock::SWH)) > tolerance ||
						mag(blockA.getVertex(BasicBlock::NEH) - blockB.getVertex(BasicBlock::SEH)) > tolerance
				){
					Info << "\nTerrainManager: Error: Block vertex mismatch at north-south face between blocks" << endl;
					Info << "   blockA = " << blockA.getVertexPoints() << endl;
					Info << "   blockB = " << blockB.getVertexPoints() << endl;
					return false;
				}
			}

			// check south neighbor:
			if(j > 0){
				const TerrainBlock & blockB = blocks[blockAdr_ij[key(i,j - 1)]];
				if(
						mag(blockB.getVertex(BasicBlock::NWL) - blockA.getVertex(BasicBlock::SWL)) > tolerance ||
						mag(blockB.getVertex(BasicBlock::NEL) - blockA.getVertex(BasicBlock::SEL)) > tolerance ||
						mag(blockB.getVertex(BasicBlock::NWH) - blockA.getVertex(BasicBlock::SWH)) > tolerance ||
						mag(blockB.getVertex(BasicBlock::NEH) - blockA.getVertex(BasicBlock::SEH)) > tolerance
				){
					Info << "\nTerrainManager: Error: Block vertex mismatch at north-south face between blocks" << endl;
					Info << "   blockA = " << blockA.getVertexPoints() << endl;
					Info << "   blockB = " << blockB.getVertexPoints() << endl;
					return false;
				}
			}

		}
	}

	// check periodicity:
	labelList patchChecked(patches.size(),0);
	forAll(patches,paI){
		if(!patchChecked[paI]){

			// grab patch:
			const Patch & patch = patches[paI];

			// if cyclic, find neighbor:
			if(patch.isCyclic()){

				Info << "   Checking periodicity of patches '" << patch.getName()
						<< "' and '" << patch.getCyclicPartner() << "'" << endl;

				// grab partner:
				const Patch & npatch = getPatch(patch.getCyclicPartner());

				// check partner type:
				if(!npatch.isCyclic() || npatch.getCyclicPartner().compare(patch.getName()) != 0){
					Info << "\nTerrainManager: Error: Patch '" << patch.getName()
							<< "' is cyclic , but neighbor patch '" << npatch.getName() << "' does not say so." << endl;
					return false;
				}

				// grab faces:
				const Foam::List<Patch::PatchFace> & faces_p  = patch.getFacesData();
				const Foam::List<Patch::PatchFace> & faces_np = npatch.getFacesData();

				// check face numbers:
				if(faces_p.size() != faces_np.size()){
					Info << "\nTerrainManager: Error: Patches '" << patch.getName()
							<< "' and '" << npatch.getName() << "' have unequal number of faces." << endl;
					Info << "Patch '" << patch.getName()<< "': " << faces_p.size() << endl;
					Info << "Patch '" << npatch.getName()<< "': " << faces_np.size() << endl;
					return false;
				}

				// prepare patch one:
				labelList fdata_p = unkey_l(faces_p[0].blockKey);
				label fixDir_p    = BasicBlock::getConstantDirectionFace(fdata_p[0]);
				label freeDir_p   = fixDir_p == BasicBlock::X ?  BasicBlock::Y : BasicBlock::X;
				label fixVal_p    = fdata_p[1 + fixDir_p];

				// prepare patch two:
				label npaI         = faces_np[0].blockFaceLabel;
				labelList fdata_np = unkey_l(faces_np[0].blockKey);
				label fixDir_np    = BasicBlock::getConstantDirectionFace(fdata_np[0]);
				label fixVal_np    = fdata_np[1 + fixDir_np];

				// check if parallel patches:
				if(fixDir_p != fixDir_np){
					Info << "\nTerrainManager: Error: Patches '" << patch.getName()
								<< "' and '" << npatch.getName() << "' are not opposite sides." << endl;
					return false;
				}
				const Foam::vector n_fix = cooSys->e(fixDir_p);

				// loop over faces:
				forAll(faces_p,fpI){

					// grab face data one:
					const Patch::PatchFace & f_p = faces_p[fpI];

					// get face data:
					word k_p = f_p.blockKey;
					fdata_p  = unkey_l(k_p);
					if(fdata_p[0] != paI){
						Info << "\nTerrainManager: Error: Patch '" << patch.getName()
									<< "' has inconsistent patch face." << endl;
						Info << "   face key: " << k_p << endl;
						Info << "   face orientation label: " << fdata_p[0] << ", expected: " << paI << endl;
						return false;
					}

					// check constant direction:
					label hval_p = fdata_p[1 + fixDir_p];
					if(hval_p != fixVal_p){
						Info << "\nTerrainManager: Error: Patch '" << patch.getName()
									<< "' has inconsistent face indexes." << endl;
						return false;
					}

					// get partner key and face:
					label searchVal = fdata_p[1 + freeDir_p];
					word k_np = (fixDir_np == BasicBlock::X) ?
							key(npaI,fixVal_np,searchVal):
							key(npaI,searchVal,fixVal_np);
					if(!npatch.found(k_np)){
						Info << "\nTerrainManager: Error: Patch '" << npatch.getName()
									<< "' has no face with key '" << k_np
									<< "', needed by face with key '" << k_p
									<< "' of patch '" << patch.getName() << "'." << endl;
						return false;
					}
					const Patch::PatchFace & f_np = npatch.getFace(k_np);

					// grab blocks:
					const SplineBlock * block_p  = dynamic_cast<const SplineBlock *>(f_p.block);
					const SplineBlock * block_np = dynamic_cast<const SplineBlock *>(f_np.block);

					// get spline indices of faces:
					labelList slist_p  = SplineBlock::getFaceSplinesI(paI);
					labelList slist_np = SplineBlock::getFaceSplinesI(npaI);

					// loop over face splines:
					for(int i = 0; i < 8; i++){

						if(block_p->hasSpline(slist_p[i])){

							// grab spline one:
							const Spline * spl_p = &(block_p->getSpline(slist_p[i]));

							// find spline two:
							const Spline * spl_np = 0;
							labelList cdir_p = SplineBlock::getConstantDirectionsEdge(slist_p[i]);
							for(int j = 0; j < 8; j++){
								if(block_np->hasSpline(slist_np[j])){
									labelList cdir_np = SplineBlock::getConstantDirectionsEdge(slist_np[i]);
									if((cdir_p[0] == cdir_np[0] && cdir_p[1] == cdir_np[1]) ||
										(cdir_p[0] == cdir_np[1] && cdir_p[1] == cdir_np[0])){

										// grab spline two:
										spl_np = &(block_np->getSpline(slist_np[j]));
										break;
									}
								}
							}
							if(spl_np == 0){
								Info << "\nTerrainManager: Error: No partner spline for spline spl_p of patch '" << patch.getName()
											<< "' found in patch '" << npatch.getName() << "'." << endl;
								Info << "   Spline spl_p: key '" << k_p << "', " << spl_p->size() << " points" << endl;
								Info << "   " << spl_p->getPoints() << endl;
								return false;
							}

							// check if spline point numbers agree:
							if(spl_p->size() != spl_np->size()){
								Info << "\nTerrainManager: Error: Spline spl_p of patch '" << patch.getName()
											<< "' and spline spl_np of patch '" << npatch.getName() << "' have different number of points." << endl;
								Info << "   Spline spl_p: key '" << k_p << "', " << spl_p->size() << " points" << endl;
								Info << "   " << spl_p->getPoints() << endl;
								Info << "   Spline spl_np: key '" << k_np << "', " << spl_np->size() << " points" << endl;
								Info << "   " << spl_np->getPoints() << endl;
								return false;
							}

							// compare points:
							for(label i = 0; i < label(spl_p->size()); i++){

								// calc difference:
								point q  = spl_p->getPoint(i) - spl_np->getPoint(i);
								q       -= dot(q,n_fix) * n_fix;
								scalar d = mag(q);

								// check:
								if(d > tolerance){
									Info << "\nTerrainManager: Error: Spline spl_p of patch '" << patch.getName()
												<< "' and spl_np of patch '" << npatch.getName() << "' have non-matching points." << endl;
									Info << "   Spline spl_p: key '" << k_p << "', " << spl_p->size() << " points" << endl;
									Info << "   " << spl_p->getPoints() << endl;
									Info << "   Spline spl_np: key '" << k_np << "', " << spl_np->size() << " points" << endl;
									Info << "   " << spl_np->getPoints() << endl;
									Info << "   points with number " << i << " disagree by " << d << ", tolerance = " << tolerance << endl;
									return false;
								}

							}

						}
					}
				}
				patchChecked[npaI] = 1;
			}
			patchChecked[paI] = 1;
		}
	}

	return true;
}


} /* iwesol */
} /* Foam */
