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

TerrainManager::TerrainManager(
		const dictionary & dict,
		CoordinateSystem * cooSys
		):
		BlockManager(dict,cooSys),
		landscape(0),
		splineNormalDistFactor(0),
		mode_upwardSplines(0),
		zeroLevel(0),
		f_constant_A(1.),
		f_constant_B(2.),
		cylinderModule(this),
		modificationModule(this),
		gradingModule(this){
	init(dict);
}

TerrainManager::TerrainManager(
		const dictionary & dict,
		CoordinateSystem * cooSys,
		searchableSurface const * landscape
		):
		BlockManager(dict,cooSys),
		landscape(landscape),
		splineNormalDistFactor(0),
		mode_upwardSplines(0),
		zeroLevel(0),
		f_constant_A(1.),
		f_constant_B(2.),
		cylinderModule(this),
		modificationModule(this),
		gradingModule(this){
	init(dict);
}

TerrainManager::~TerrainManager() {
}

void TerrainManager::init(const dictionary & dict){

	// Read dictionary:
	p_corner               = point(dict.lookup("p_corner"));
	p_corner_stl           = p_corner;
	dimensions             = scalarList(dict.lookup("dimensions"));
	dimensions_stl         = dimensions;
	p_above                = point(dict.lookup("p_above"));
	maxDistProj            = readScalar(dict.lookup("maxDistProj"));

	// option for stl inside the domain box:
	if(dict.found("stlInsideBox")){
		if(dict.subDict("stlInsideBox").found("zeroLevel")){
			zeroLevel = readScalar(dict.subDict("stlInsideBox").lookup("zeroLevel"));
		}
		if(dict.subDict("stlInsideBox").found("f_constant_A")){
			f_constant_A = readScalar(dict.subDict("stlInsideBox").lookup("f_constant_A"));
		}
		if(dict.subDict("stlInsideBox").found("f_constant_B")){
			f_constant_B = readScalar(dict.subDict("stlInsideBox").lookup("f_constant_B"));
		}
		if(dict.subDict("stlInsideBox").found("p_corner_inside_stl")){
			p_corner_stl = point(dict.subDict("stlInsideBox").lookup("p_corner_inside_stl"));
		}
		if(dict.subDict("stlInsideBox").found("dimensions_inside_stl")){
			dimensions_stl = scalarList(dict.subDict("stlInsideBox").lookup("dimensions_inside_stl"));
		}
	}

    // module orography modifications:
    if(dict.found("terrainModification")){
    	Info << "   loading orography modification module" << endl;
    	if(!modificationModule.load(dict.subDict("terrainModification"))){
    		Info << "\n   TerrainManager: Error while loading orography modification module." << endl;
    		throw;
    	}
    }

    // module block grading:
    if(dict.found("blockGrading")){
    	Info << "   loading block grading module" << endl;
    	if(!gradingModule.load(dict.subDict("blockGrading"))){
    		Info << "\n   TerrainManager: Error while loading block grading module." << endl;
    		throw;
    	}
    }

    // module outer cylinder:
    if(dict.found("outerCylinder")){
    	Info << "   loading cylinder module" << endl;
    	if(!cylinderModule.load(dict.subDict("outerCylinder"))){
    		Info << "\n   TerrainManager: Error while loading cylinder module." << endl;
    		throw;
    	}
    }

	// option for othogonalization of upward splines:
	if(dict.found("orthogonalizeUpwardSplines")){
		dictionary upsDict = dict.subDict("orthogonalizeUpwardSplines");
		splineNormalDistFactor = readScalar(upsDict.lookup("splineNormalDistFactor"));
		if(upsDict.found("ignoreBoundary")) mode_upwardSplines = 2;
		else mode_upwardSplines = 1;
	}

	// add patches:
	patches.resize(6);
	addPatch(BasicBlock::WEST,word(dict.lookup("patch_name_west")),word(dict.lookup("patch_type_west")));
	addPatch(BasicBlock::EAST,word(dict.lookup("patch_name_east")),word(dict.lookup("patch_type_east")));
	addPatch(BasicBlock::NORTH,word(dict.lookup("patch_name_north")),word(dict.lookup("patch_type_north")));
	addPatch(BasicBlock::SOUTH,word(dict.lookup("patch_name_south")),word(dict.lookup("patch_type_south")));
	addPatch(BasicBlock::SKY,word(dict.lookup("patch_name_sky")),word(dict.lookup("patch_type_sky")));
	addPatch(BasicBlock::GROUND,word(dict.lookup("patch_name_ground")),word(dict.lookup("patch_type_ground")));

	// only one block in up direction:
	blockNrs[TerrainBlock::UP] = 1;
}

bool TerrainManager::calc() {

	// fill pointField and create blocks:
	initAll();

	// modify orography:
	if(modificationModule.ready() && !modificationModule.calc()){
		Info << "\n   TerrainManager: Error during orography modification." << endl;
		return false;
	}

	// calculate outer cylinder
	if(cylinderModule.ready() && !cylinderModule.calc()){
		Info << "\n   TerrainManager: Error during outer cylinder calculation." << endl;
		return false;
	}

	// calculate up-splines
	if(!calcUpSplines()){
		Info << "\n   TerrainManager: Error during up-spline calculation." << endl;
		return false;
	}


	return true;
}


void TerrainManager::initAll(){

	// prepare:
	points.resize( (blockNrs[TerrainBlock::BASE1] + 1) *  (blockNrs[TerrainBlock::BASE2] + 1) * 2);
	blocks.resize( blockNrs[TerrainBlock::BASE1] *  blockNrs[TerrainBlock::BASE2]);
	if(cylinderModule.ready()) cylinderModule.reserveStorageCylinder();

	//scalar delta0 = dimensions[TerrainBlock::BASE1] / blockNrs[TerrainBlock::BASE1];
	//scalar delta1 = dimensions[TerrainBlock::BASE2] / blockNrs[TerrainBlock::BASE2];
	scalar deltaz = dimensions[TerrainBlock::UP];

	// get delta values.
	scalarListList deltaLL(3);
	scalarListList cellGradingLL(3);
	if(gradingModule.ready()){
		gradingModule.getDeltaLists(deltaLL,cellGradingLL);
	} else {
		for(label d = 0; d < 2; d++){
			deltaLL[d]              = scalarList(blockNrs[d] + 1,dimensions[d] / blockNrs[d]);
			cellGradingLL[d]        = scalarList(blockNrs[d] + 1,gradingFactors[d]);
			deltaLL[d][0]           = 0;
			cellGradingLL[d].last() = 0;
		}
	}


	// find all block vertices:
	pointCounter = 0;
	blockCounter = 0;
	point p_j0 = p_corner;
	for(int i = 0; i < blockNrs[TerrainBlock::BASE1]; i++){

		// point at j = 0:
		p_j0 += deltaLL[TerrainBlock::BASE1][i] * cooSys->e(TerrainBlock::BASE1);

		// set cell grading:
		scalarList gradingF(gradingFactors);
		gradingF[TerrainBlock::BASE1] = cellGradingLL[TerrainBlock::BASE1][i];

		point p = p_j0;
		for(int j = 0; j < blockNrs[TerrainBlock::BASE2]; j++){

			// find vertex SWL:
			p += deltaLL[TerrainBlock::BASE2][j] * cooSys->e(TerrainBlock::BASE2);

			// set cell grading:
			gradingF[TerrainBlock::BASE2] = cellGradingLL[TerrainBlock::BASE2][j];

			// prepare:
			labelList vI(8);

			// add vertices:
			label up = 0;
			for(int v = 0; v < 8; v++){

				// find deltap to vertex v:
				Foam::vector dp(0,0,0);
				if(    v == BasicBlock::SEL
					|| v == BasicBlock::NEL
					|| v == BasicBlock::SEH
					|| v == BasicBlock::NEH
				) {
					dp += deltaLL[TerrainBlock::BASE1][i + 1] * cooSys->e(TerrainBlock::BASE1);
				}
				if(    v == BasicBlock::NWL
					|| v == BasicBlock::NEL
					|| v == BasicBlock::NWH
					|| v == BasicBlock::NEH
				) {
					dp += deltaLL[TerrainBlock::BASE2][j + 1] * cooSys->e(TerrainBlock::BASE2);
				}
				if(    v == BasicBlock::SWH
					|| v == BasicBlock::SEH
					|| v == BasicBlock::NWH
					|| v == BasicBlock::NEH
				) {
					up  = 1;
					dp += deltaz * cooSys->e(TerrainBlock::UP);
				}

				// add point:
				vI[v] = _addPoint(p + dp,up);

				// remember address by i,j,v key:
				pointAdr_ijv.set(key(i,j,v),vI[v]);

			}

			// create block:
			blocks[blockCounter] = TerrainBlock(
					landscape,
					&points,
					vI,
					cellNrs,
					cooSys,
					p_above,
					&splines,
					splinePointNrs,
					p_corner,
					dimensions,
					p_corner_stl,
					dimensions_stl,
					zeroLevel,
					f_constant_A,
					f_constant_B,
					maxDistProj,
					gradingCommand,
					gradingF
					);

			// remember block address by i,j key:
			blockAdr_ij.set(key(i,j),blockCounter);

			// order splines:
			for(int s = 0; s < 24; s++){
				blocks[blockCounter].orderSpline(s,cooSys->e(SplineBlock::getDirectionEdge(s)));
			}

			// contribute to patches:
			contributeToPatches(i, j, blocks[blockCounter]);

			// update counter:
			blockCounter++;

		}
		Info << "   blocks = " << blockCounter << endl;
	}

	// resize:
	// points.resize(pointCounter);
	// blocks.resize(blockCounter);

	Info << "   added " << pointCounter << " points" << endl;
	Info << "   created " << blockCounter << " blocks" << endl;

}

bool TerrainManager::calcTopology(){
	if(topologyReady()) return true;
	Info << "   finding mesh topology" << endl;
	return BlockManager::calcTopology();
}

void TerrainManager::contributeToPatches(label i, label j, const TerrainBlock & block){

	patches[BasicBlock::SKY].addPatch(
			&block, BasicBlock::SKY,key(BasicBlock::SKY,i,j)
			);
	patches[BasicBlock::GROUND].addPatch(
			&block, BasicBlock::GROUND,key(BasicBlock::GROUND,i,j)
			);
	if(i == 0)
		patches[BasicBlock::WEST].addPatch(
				&block, BasicBlock::WEST,key(BasicBlock::WEST,i,j)
				);
	if(i == blockNrs[TerrainBlock::BASE1] - 1)
		patches[BasicBlock::EAST].addPatch(
				&block, BasicBlock::EAST,key(BasicBlock::EAST,i,j)
				);
	if(j == 0)
		patches[BasicBlock::SOUTH].addPatch(
				&block, BasicBlock::SOUTH,key(BasicBlock::SOUTH,i,j)
				);
	if(j == blockNrs[TerrainBlock::BASE2] - 1)
		patches[BasicBlock::NORTH].addPatch(
				&block, BasicBlock::NORTH,key(BasicBlock::NORTH,i,j)
				);

}

word TerrainManager::hkey(const point & p, label upDown) const{

	return key(p[TerrainBlock::BASE1],8,0.001,"X")
	+ "." + key(p[TerrainBlock::BASE2],8,0.001,"X")
	+ "." + key(upDown);

}

label TerrainManager::_addPoint(const point & p, label upDown){

	// prepare:
	label pI = -1;
	const word k   = hkey(p,upDown);

	// check if existent, else add:
	if(pointAdr.found(k)){

		pI = pointAdr[k];

	} else {

		// add point:
		points[pointCounter] = p;
		pointAdr.set(k,pointCounter);

		// update labels:
		pI = pointCounter;
		pointCounter++;

	}

	return pI;

}


bool TerrainManager::calcUpSplines(){

	// check:
	if(mode_upwardSplines <= 0) return true;

	// calc topology:
	if(!calcTopology()){
		Info << "\n   TerrainManager: Error during topology calculation." << endl;
		return false;
	}

	Info << "   calculating orthogonal up-splines" << endl;

	// prepare:
	const scalar splineDeltaFactor = splineNormalDistFactor *
						dimensions[TerrainBlock::UP] / (blockNrs[TerrainBlock::UP] * cellNrs[TerrainBlock::UP]);

	// calc SOUTH-NORTH paths:
	Info << "      ...collecting ground splines" << endl;
	List<PointLinePath> paths_SN(blockNrs[TerrainBlock::BASE1] + 1);
	for(label i = 0; i <= blockNrs[TerrainBlock::BASE1]; i++){

		// prepare:
		const label hi     = i < blockNrs[TerrainBlock::BASE1] ? i : i - 1;
		const label sp     = i < blockNrs[TerrainBlock::BASE1] ? SplineBlock::SWL_NWL : SplineBlock::SEL_NEL;
		const label iBlock = blockAdr_ij[key(hi,0)];

		// set path:
		paths_SN[i] = getInterBlockPath(
						iBlock,
						sp,
						BasicBlock::NORTH
						);
	}

	// calc WEST-EAST paths:
	List<PointLinePath> paths_WE(blockNrs[TerrainBlock::BASE2] + 1);
	for(label j = 0; j <= blockNrs[TerrainBlock::BASE2]; j++){

		// prepare:
		const label hj     = j < blockNrs[TerrainBlock::BASE2] ? j : j - 1;
		const label sp     = j < blockNrs[TerrainBlock::BASE2] ? SplineBlock::SWL_SEL : SplineBlock::NWL_NEL;
		const label iBlock = blockAdr_ij[key(0,hj)];

		// set path:
		paths_WE[j] = getInterBlockPath(
						iBlock,
						sp,
						BasicBlock::EAST
						);
	}

	// compute orthogonal splines:
	Info << "      ...computing upward splines" << flush;
	label imin = 0;
	label imax = blockNrs[TerrainBlock::BASE1];
	label jmin = 0;
	label jmax = blockNrs[TerrainBlock::BASE2];
	if(mode_upwardSplines == 2){
		imin = 1; jmin = 1; imax--; jmax--;
	}
	for(label i = imin; i <= imax; i++){

		Info << "." << flush;

		// prepare:
		const label hi               = i < blockNrs[TerrainBlock::BASE1] ? i : i - 1;
		PointLinePath & pathSN = paths_SN[i];

		for(label j = jmin; j <= jmax; j++){

			// prepare:
			const label hj         = j < blockNrs[TerrainBlock::BASE2] ? j : j - 1;
			PointLinePath & pathWE = paths_WE[j];
			const label iBlock           = blockAdr_ij[key(hi,hj)];
			label sp               = SplineBlock::SWL_SWH;
			if(i == blockNrs[TerrainBlock::BASE1]){
				if(j < blockNrs[TerrainBlock::BASE2]){
					sp = SplineBlock::SEL_SEH;
				} else {
					sp = SplineBlock::NWL_NWH;
				}
			} else if(j == blockNrs[TerrainBlock::BASE2]){
				sp = SplineBlock::NWL_NWH;
			}

			// get tangent WE:
			const scalar si        = pathWE.getPointS(i * getCellNr(TerrainBlock::BASE1));
			Foam::vector nWE 		= pathWE.getTangent(si,2);
			normalize(nWE);

			// get tangent SN:
			const scalar sj        = pathSN.getPointS(j * getCellNr(TerrainBlock::BASE2));
			Foam::vector nSN 		= pathSN.getTangent(sj,2);
			normalize(nSN);

			// calc out normal:
			Foam::vector n_up = nWE ^ nSN;

			// set spline:
			setInterBlockSpline(
						splineDeltaFactor * n_up,
						-get_e(TerrainBlock::UP),
						iBlock,
						BasicBlock::SKY,
						sp
					);
		}
	}
	Info << endl;

	return true;
}

} /* iwesol */
} /* Foam */

