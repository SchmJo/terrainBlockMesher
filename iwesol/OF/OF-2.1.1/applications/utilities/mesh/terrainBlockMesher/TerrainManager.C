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
#include "Globals.H"
#include "ConvexPolygon.H"
#include "PointLinePath.H"

namespace Foam
{

namespace oldev
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

const int TerrainManager::BASE1 = 0;
const int TerrainManager::BASE2 = 1;
const int TerrainManager::UP    = 2;


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void TerrainManager::initAll(){

	// prepare:
	resizePoints( (blockNrs_[BASE1] + 1) *  (blockNrs_[BASE2] + 1) * 2 );
	resizeBlocks( blockNrs_[BASE1] *  blockNrs_[BASE2] );
	if(stl_){
		resizeEdges
		(
				(blockNrs_[BASE1] + 1) *  blockNrs_[BASE2] +
				(blockNrs_[BASE2] + 1) *  blockNrs_[BASE1]
		);
	}
	//if(cylinderModule.ready()) cylinderModule.reserveStorageCylinder();

	//scalar delta0 = domainBox_.lengths()[BASE1] / blockNrs_[BASE1];
	//scalar delta1 = domainBox_.lengths()[BASE2] / blockNrs_[BASE2];
	scalar deltaz = domainBox_.lengths()[UP];

	// get delta values.
	scalarListList deltaLL(3);
	scalarListList cellGradingLL(3);

	if(gradingModule_.ready()){
		gradingModule_.getDeltaLists(deltaLL,cellGradingLL);
	} else {
		for(label d = 0; d < 2; d++){
			deltaLL[d]              = scalarList(blockNrs_[d] + 1, domainBox_.lengths()[d] / scalar(blockNrs_[d]));
			cellGradingLL[d]        = scalarList(blockNrs_[d] + 1, gradingFactors_[d]);
			deltaLL[d][0]           = 0;
			cellGradingLL[d].last() = 0;
		}
	}

	Info << "\nTerrainManager: creating " << points().size()
			<< " points, " << blocks().size() << " blocks"
			<< " and " << edges().size() << " edges" << endl;

	// find all block vertices:
	point p_j0 = domainBox_.pMin();
	label proc = -1;

	for(int i = 0; i < blockNrs_[BASE1]; i++){

		// point at j = 0:
		p_j0 += deltaLL[BASE1][i] * coordinateSystem().e(BASE1);

		// set cell grading:
		scalarList gradingF(gradingFactors_);
		gradingF[BASE1] = cellGradingLL[BASE1][i];

		point p = p_j0;
		for(int j = 0; j < blockNrs_[BASE2]; j++){

			// find vertex SWL:
			p += deltaLL[BASE2][j] * coordinateSystem().e(BASE2);

			// set cell grading:
			gradingF[BASE2] = cellGradingLL[BASE2][j];

			// prepare:
			labelList vI(8,-1);

			// add vertices:
			label up = 0;
			for(int v = 0; v < 8; v++){

				// find deltap to vertex v:
				Foam::vector dp(0,0,0);
				if(    v == Block::SEL
					|| v == Block::NEL
					|| v == Block::SEH
					|| v == Block::NEH
				) {
					dp += deltaLL[BASE1][i + 1] * coordinateSystem().e(BASE1);
				}
				if(    v == Block::NWL
					|| v == Block::NEL
					|| v == Block::NWH
					|| v == Block::NEH
				) {
					dp += deltaLL[BASE2][j + 1] * coordinateSystem().e(BASE2);
				}
				if(    v == Block::SWH
					|| v == Block::SEH
					|| v == Block::NWH
					|| v == Block::NEH
				) {
					up  = 1;
					dp += deltaz * coordinateSystem().e(UP);
				}

				// add point:
				vI[v] = getAddedPoint(p + dp,up);

				// remember address by i,j,v key:
				pointAdr_ijv_.set(key(i,j,v),vI[v]);
			}

			// create block:
			label blockI = addBlock
			(
					vI,
					cellNrs_,
					gradingF
			);

			// project points to landscape_:
			if(landscape_.valid()){

				// attach ground vertices to landscape_ height:
				if(!attachVertices(blockI)){
					Info << "\nTerrainManager: Error attaching points.\n" << endl;
					throw;
				}

				// calculate the vertex-vertex splines:
				if(!calcLandscapeSplines(blockI)){
					Info << "\nTerrainManager: Error calculating splines.\n" << endl;
					throw;
				}
			}

			// remember block address by i,j key:
			blockAdr_ij_.set(key(i,j),blockCounter() - 1);
			labelList ij(2,i);
			ij[1] = j;
			blockAdrI_i_.set(blockCounter() - 1,ij);

			// contribute to patches:
			contributeToPatches(i, j);

			// report:
			label hproc = scalar( 100. * blockCounter() ) / scalar( blocks().size() );
			if( hproc > proc ){
				proc = hproc;
				Info << " " << proc << "%" << flush;
			}
		}
	}
	Info << endl;

	// resize:
	// points.resize(pointCounter);
	// blocks.resize(blockCounter());

	Info << "   created " << pointCounter() << " points" << endl;
	Info << "   created " << blockCounter() << " blocks" << endl;
	Info << "   created " << edgeCounter() << " edges" << endl;
}

bool TerrainManager::attachVertices(label blockI){

	// prepare:
	const Foam::vector & n_up = coordinateSystem().e(UP);

	point & p1 = getPoint(blockVertex(blockI,Block::SWL));
	p1        += ( ( p_above_ - p1) & n_up )  * n_up;
	if(!landscape_().attachPoint(p1,p1 - maxDistProj_ * n_up)){
		Info << "TerrainManager: Cannot attach point SWL = " << p1 << " to STL.\n" << endl;
		return false;
	}

	point & p2 = getPoint(blockVertex(blockI,Block::NWL));
	p2        += ( ( p_above_ - p2 ) & n_up ) * n_up;
	if(!landscape_().attachPoint(p2,p2 - maxDistProj_ * n_up)){
		Info << "TerrainManager: Cannot attach point NWL = " << p2 << " to STL.\n" << endl;
		return false;
	}

	point & p3 = getPoint(blockVertex(blockI,Block::SEL));
	p3        += ( ( p_above_ - p3 ) & n_up ) * n_up;
	if(!landscape_().attachPoint(p3,p3 - maxDistProj_ * n_up)){
		Info << "TerrainManager: Cannot attach point SEL = " << p3 << " to STL.\n" << endl;
		return false;
	}

	point & p4 = getPoint(blockVertex(blockI,Block::NEL));
	p4        += ( ( p_above_ - p4 ) & n_up ) * n_up;
	if(!landscape_().attachPoint(p4,p4 - maxDistProj_ * n_up)){
		Info << "TerrainManager: Cannot attach point NEL = " << p4 << " to STL.\n" << endl;
		return false;
	}
	return true;
}

bool TerrainManager::calcLandscapeSplines(label blockI){

	// prepare:
	const Foam::vector & n_up = coordinateSystem().e(UP);
	labelList groundSplines   = Block::getFaceEdgesI(Block::GROUND);

	// loop over ground spline labels:
	forAll(groundSplines,gsI){

		// get spline label:
		label i = groundSplines[gsI];
		if(i > 11) continue;

		// check if already set:
		if( edges().foundInBlock(blockI,i) ) continue;
		if( edges().foundInBlock(blockI,Block::switchedOrientationLabel(i)) ) continue;

		// prepare:
		const label splinePoints = splinePointNrs_[Block::getDirectionEdge(i)];
		pointField spline(splinePoints);

		// grab spline end points:
		const labelList verticesI = Block::getEdgeVerticesI(i);
		point & pointA = getPoint(blockVertex(blockI,verticesI[0]));
		point & pointB = getPoint(blockVertex(blockI,verticesI[1]));

		// calc delta in x,y:
		point delta = (pointB - pointA) / scalar(splinePoints + 1);

		// loop over spline points:
		for(label u = 0; u < splinePoints; u++){

			point splinePoint = pointA + (1 + u) * delta;

			// make sure point is above surface:
			splinePoint += dot(p_above_ - splinePoint,n_up) * n_up;

			// project to stl_:
			if(!landscape_().attachPoint(splinePoint,splinePoint - maxDistProj_ * n_up)){
				Info << "TerrainManager: Error: Cannot project point p = " << splinePoint << " onto stl_.\n" << endl;
				return false;
			}

			// add:
			spline[u] = splinePoint;
		}

		// set it:
		setEdge(blockI,i,spline);
	}

	return true;
}

void TerrainManager::contributeToPatches(label i, label j){

	contributeToPatch
	(
			Block::SKY,
			blockAdr_ij_[key(i,j)],
			Block::SKY
	);
	contributeToPatch
	(
			Block::GROUND,
			blockAdr_ij_[key(i,j)],
			Block::GROUND
	);
	if(i == 0){
		contributeToPatch
		(
				Block::WEST,
				blockAdr_ij_[key(i,j)],
				Block::WEST
		);
	}
	if(i == blockNrs_[BASE1] - 1){
		contributeToPatch
		(
				Block::EAST,
				blockAdr_ij_[key(i,j)],
				Block::EAST
		);
	}
	if(j == 0){
		contributeToPatch
		(
				Block::SOUTH,
				blockAdr_ij_[key(i,j)],
				Block::SOUTH
		);
	}
	if(j == blockNrs_[BASE2] - 1){
		contributeToPatch
		(
				Block::NORTH,
				blockAdr_ij_[key(i,j)],
				Block::NORTH
		);
	}
}

word TerrainManager::hkey(const point & p, label upDown) const{

	return jlib::BLIB::d2S(p[BASE1],8,"X")
	+ "." + jlib::BLIB::d2S(p[BASE2],8,"X")
	+ "." + jlib::String(upDown);
}

label TerrainManager::getAddedPoint(const point & p, label upDown){

	// prepare:
	label pI     = -1;
	const word k = hkey(p,upDown);

	// check if existent:
	if(pointAdr_.found(k)){
		pI = pointAdr_[k];
	}

	// add new point:
	else {

		pointAdr_.set(k,pointCounter());
		pI = pointCounter();
		addPoint(p);
	}

	return pI;
}


bool TerrainManager::calcUpSplines(){

	// check:
	if(mode_upwardSplines_ <= 0) return true;

	Info << "   calculating orthogonal up-splines" << endl;

	// resize:
	{
		label newEdges = points().size() / 2;
		if(mode_upwardSplines_ == 2){
			newEdges -= 2 * ( blockNrs_[BASE1] + blockNrs_[BASE2] );
		}
		resizeEdges( edges().size() + newEdges );
		Info << "     expected upward edges         : " << newEdges << endl;
		Info << "     expected total number of edges: " << edges().size() << endl;
	}

	// calc SOUTH-NORTH paths:
	Info << "        ...collecting ground splines" << endl;
	List<PointLinePath> paths_SN(blockNrs_[BASE1] + 1);
	for(label i = 0; i <= blockNrs_[BASE1]; i++){

		// prepare:
		const label hi     = i < blockNrs_[BASE1] ? i : i - 1;
		const label sp     = i < blockNrs_[BASE1] ? Block::SWL_NWL : Block::SEL_NEL;
		const label iBlock = blockAdr_ij_[key(hi,0)];

		// set path:
		paths_SN[i] = getInterBlockPath(
						iBlock,
						sp,
						Block::NORTH
						);
	}

	// calc WEST-EAST paths:
	List<PointLinePath> paths_WE(blockNrs_[BASE2] + 1);
	for(label j = 0; j <= blockNrs_[BASE2]; j++){

		// prepare:
		const label hj     = j < blockNrs_[BASE2] ? j : j - 1;
		const label sp     = j < blockNrs_[BASE2] ? Block::SWL_SEL : Block::NWL_NEL;
		const label iBlock = blockAdr_ij_[key(0,hj)];

		// set path:
		paths_WE[j] = getInterBlockPath(
						iBlock,
						sp,
						Block::EAST
						);
	}

	// compute orthogonal splines:
	Info << "        ...computing upward splines" << flush;
	label imin = 0;
	label imax = blockNrs_[BASE1];
	label jmin = 0;
	label jmax = blockNrs_[BASE2];
	if(mode_upwardSplines_ == 2){
		imin = 1; jmin = 1; imax--; jmax--;
	}
	for(label i = imin; i <= imax; i++){

		Info << "." << flush;

		// prepare:
		const label hi         = i < blockNrs_[BASE1] ? i : i - 1;
		PointLinePath & pathSN = paths_SN[i];
		for(label j = jmin; j <= jmax; j++){

			// prepare:
			const label hj           = j < blockNrs_[BASE2] ? j : j - 1;
			PointLinePath & pathWE   = paths_WE[j];
			const label iBlock       = blockAdr_ij_[key(hi,hj)];
			label sp                 = Block::SWL_SWH;
			if(i == blockNrs_[BASE1]){
				if(j < blockNrs_[BASE2]){
					sp = Block::SEL_SEH;
				} else {
					sp = Block::NEL_NEH;
				}
			} else if(j == blockNrs_[BASE2]){
				sp = Block::NWL_NWH;
			}

			// get tangent WE:
			const scalar si  = pathWE.getPointS(i * cellNrs_[BASE1]);
			Foam::vector nWE = pathWE.getTangent(si);
			nWE             /= mag(nWE);

			// get tangent SN:
			const scalar sj  = pathSN.getPointS(j * cellNrs_[BASE2]);
			Foam::vector nSN = pathSN.getTangent(sj);
			nSN             /= mag(nSN);

			// calc out normal:
			Foam::vector n_up = nWE ^ nSN;

			// set spline:
			setInterBlockSpline(
						splineNormalDist_ * n_up,
						-splineNormalDist_ * coordinateSystem().e(UP),
						iBlock,
						Block::SKY,
						sp
					);
		}
	}
	Info << endl;

	Info << "     total number of edges: " << edgeCounter() << endl;

	return true;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

TerrainManager::TerrainManager(
		const Time & runTime,
		const dictionary & dict,
		const searchableSurface * const stl_
		):
		BlockMeshManager(runTime),
		stl_(stl_),
		blockNrs_(dict.lookup("blocks")),
		cellNrs_(dict.lookup("cells")),
		domainBox_
		(
				CoordinateSystem
				(
						point(dict.lookup("p_corner")),
						List< Foam::vector >(dict.subDict("coordinates").lookup("baseVectors"))
				),
				scalarList(dict.lookup("dimensions")),
				dict.lookupOrDefault< scalar >("boxResolution",0.0001)
		),
		stlBox_(domainBox_),
		splineNormalDist_(0),
		mode_upwardSplines_(0),
		zeroLevel_(0),
		gradingFactors_(dict.lookup("gradingFactors")),
		//cylinderModule(this),
		modificationModule(this),
		gradingModule_(this),
		blendingFunction_
		(
				new ScalarBlendingFunction()
		){

	// Read dictionary:
	p_above_                = point(dict.lookup("p_above"));
	maxDistProj_            = readScalar(dict.lookup("maxDistProj"));

	// option for stl_ inside the domain box:
	if(dict.found("stlInsideBox")){
		if(dict.subDict("stlInsideBox").found("zeroLevel")){
			zeroLevel_ = readScalar(dict.subDict("stlInsideBox").lookup("zeroLevel"));
		}
		stlBox_ = Box
		(
				CoordinateSystem
				(
						point(dict.subDict("stlInsideBox").lookup("p_corner_inside_stl")),
						coordinateSystem().axes()
				),
				scalarList(dict.subDict("stlInsideBox").lookup("dimensions_inside_stl")),
				dict.subDict("stlInsideBox").lookupOrDefault< scalar >("boxResolution",0.0001)
		);
		blendingFunction_ =
				ScalarBlendingFunction::New
				(
					dict.subDict("stlInsideBox").subDict("blendingFunction")
				);
		if(dict.subDict("stlInsideBox").subDict("blendingFunction").found("writePDF")){
			const dictionary & writeDict = dict.subDict("stlInsideBox").subDict("blendingFunction").subDict("writePDF");
			blendingFunction_().writePDFs
			(
					word(writeDict.lookup("baseName")),
					fileName(writeDict.lookup("resultsFolder")),
					point(writeDict.lookup("probePoint0")),
					point(writeDict.lookup("probePoint1")),
					0,1,
					readLabel(writeDict.lookup("steps")),
					readLabel(writeDict.lookup("plotPoints")),
					readLabel(writeDict.lookup("interpolOrder"))
			);
		} else {
			Info << "TerrainManager: keyword 'writePDF' not found in sub dictionary 'blendingFunction'." << endl;
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
    	if(!gradingModule_.load(dict.subDict("blockGrading"))){
    		Info << "\n   TerrainManager: Error while loading block grading module." << endl;
    		throw;
    	}
    }
/*
    // module outer cylinder:
    if(dict.found("outerCylinder")){
    	Info << "   loading cylinder module" << endl;
    	if(!cylinderModule.load(dict.subDict("outerCylinder"))){
    		Info << "\n   TerrainManager: Error while loading cylinder module." << endl;
    		throw;
    	}
    }
    */

	// option for othogonalization of upward splines:
	if(dict.found("orthogonalizeUpwardSplines")){
		dictionary upsDict = dict.subDict("orthogonalizeUpwardSplines");
		splineNormalDist_ = readScalar(upsDict.lookup("splineNormalDist"));
		if(upsDict.found("ignoreBoundary")) mode_upwardSplines_ = 2;
		else mode_upwardSplines_ = 1;
	}

	// add patches:
	patchesRef().resize(6);
	patchesRef().set
	(
			Block::WEST,
			new BlockMeshPatch
			(
					*this,
					word(dict.lookup("patch_name_west")),
					word(dict.lookup("patch_type_west"))
			)
	);
	patchesRef().set
	(
			Block::EAST,
			new BlockMeshPatch
			(
					*this,
					word(dict.lookup("patch_name_east")),
					word(dict.lookup("patch_type_east"))
			)
	);
	patchesRef().set
	(
			Block::SOUTH,
			new BlockMeshPatch
			(
					*this,
					word(dict.lookup("patch_name_south")),
					word(dict.lookup("patch_type_south"))
			)
	);
	patchesRef().set
	(
			Block::NORTH,
			new BlockMeshPatch
			(
					*this,
					word(dict.lookup("patch_name_north")),
					word(dict.lookup("patch_type_north"))
			)
	);
	patchesRef().set
	(
			Block::GROUND,
			new BlockMeshPatch
			(
					*this,
					word(dict.lookup("patch_name_ground")),
					word(dict.lookup("patch_type_ground"))
			)
	);
	patchesRef().set
	(
			Block::SKY,
			new BlockMeshPatch
			(
					*this,
					word(dict.lookup("patch_name_sky")),
					word(dict.lookup("patch_type_sky"))
			)
	);

	// only one block in up direction:
	blockNrs_[UP] = 1;

	// set spline point numbers:
	splinePointNrs_.resize(3);
	splinePointNrs_[0] = cellNrs_[0] - 1;
	splinePointNrs_[1] = cellNrs_[1] - 1;
	splinePointNrs_[2] = cellNrs_[2] - 1;

	// init landscape_:
	if(stl_){
		landscape_.set
		(
				new STLLandscape
				(
						stl_,
						&(blendingFunction_()),
						&domainBox_,
						&stlBox_,
						zeroLevel_
				)
		);
	}

	// output boxes:
	domainBox_.writeSTL("domainBox.stl");
	stlBox_.writeSTL("stlBox.stl");
}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

bool TerrainManager::calc() {

	// fill pointField and create blocks:
	initAll();


	// modify orography:
	if(modificationModule.ready() && !modificationModule.calc()){
		Info << "\n   TerrainManager: Error during orography modification." << endl;
		return false;
	}
/*
	// calculate outer cylinder
	if(cylinderModule.ready() && !cylinderModule.calc()){
		Info << "\n   TerrainManager: Error during outer cylinder calculation." << endl;
		return false;
	}
*/
	// calculate up-splines
	if(!calcUpSplines()){
		Info << "\n   TerrainManager: Error during up-spline calculation." << endl;
		return false;
	}

	return true;
}


// ************************************************************************* //

} // End namespace oldev

} // End namespace Foam
