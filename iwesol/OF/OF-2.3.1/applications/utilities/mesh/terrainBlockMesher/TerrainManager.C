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
		cylinderModule_(this),
		modificationModule_(this),
		gradingModule_(this),
		blendingFunction_
		(
				new ScalarBlendingFunction()
		){

	// Read dictionary:
	p_above_     = point(dict.lookup("p_above"));
	maxDistProj_ = readScalar(dict.lookup("maxDistProj"));

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
    	if(!modificationModule_.load(dict.subDict("terrainModification"))){
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

    // module outer cylinder:
    if(dict.found("outerCylinder")){
    	Info << "   loading cylinder module" << endl;
    	if(!cylinderModule_.load(dict.subDict("outerCylinder"))){
    		Info << "\n   TerrainManager: Error while loading cylinder module." << endl;
    		throw;
    	}
    }

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

labelList TerrainManager::walkBox
(
		label n,
		const word & HL,
		bool counterClockWise
) const{

	// preapre:
	labelList out(3,0);
	label counter = 0;
	label face    = counterClockWise ? Block::SOUTH : Block::WEST;
	bool isHigh   = HL.compare("H") == 0 ? true : false;

	// walk counter clock wise:
	if(counterClockWise){
		while(counter < n){

			// west:
			if(face == Block::WEST){
				out[2] = isHigh ? Block::NWH : Block::NWL;
				out[Block::Y]--;
				if(out[Block::Y] < 0){
					Info << "\nBLockManager: Error: walkBox overshoot." << endl;
					Info << "   n    = " << n << endl;
					Info << "   nmax = " << walkBoxMaximum() << endl;
					throw;
				}
			}

			// north:
			if(face == Block::NORTH){
				out[2] = isHigh ? Block::NEH : Block::NEL;
				out[Block::X]--;
				if(out[Block::X] < 0){
					out[Block::X]++;
					out[2] = isHigh ? Block::NWH : Block::NWL;
					face   = Block::WEST;
				}
			}

			// east:
			if(face == Block::EAST){
				out[2] = isHigh ? Block::SEH : Block::SEL;
				out[Block::Y]++;
				if(out[Block::Y] == blockNrs_[Block::Y] ){
					out[Block::Y]--;
					out[2] = isHigh ? Block::NEH : Block::NEL;
					face   = Block::NORTH;
				}
			}

			// south:
			if(face == Block::SOUTH){
				out[2] = isHigh ? Block::SWH : Block::SWL;
				out[Block::X]++;
				if(out[Block::X] == blockNrs_[Block::X] ){
					out[Block::X]--;
					out[2] = isHigh ? Block::SEH : Block::SEL;
					face   = Block::EAST;
				}
			}

			counter++;
		}
	}

	// else walk clockwise:
	else {
		while(counter < n){

			// south:
			if(face == Block::SOUTH){
				out[2] = isHigh ? Block::SEH : Block::SEL;
				out[Block::X]--;
				if(out[Block::X] < 0 ){
					Info << "\nBLockManager: Error: walkBox overshoot." << endl;
					Info << "   n    = " << n << endl;
					Info << "   nmax = " << walkBoxMaximum() << endl;
					throw;
				}
			}

			// east:
			if(face == Block::EAST){
				out[2] = isHigh ? Block::NEH : Block::NEL;
				out[Block::Y]--;
				if(out[Block::Y] < 0 ){
					out[Block::Y]++;
					out[2] = isHigh ? Block::SEH : Block::SEL;
					face   = Block::SOUTH;
				}
			}

			// north:
			if(face == Block::NORTH){
				out[2] = isHigh ? Block::NWH : Block::NWL;
				out[Block::X]++;
				if(out[Block::X] == blockNrs_[Block::X]){
					out[Block::X]--;
					out[2] = isHigh ? Block::NEH : Block::NEL;
					face   = Block::EAST;
				}
			}

			// west:
			if(face == Block::WEST){
				out[2] = isHigh ? Block::SWH : Block::SWL;
				out[Block::Y]++;
				if(out[Block::Y] == blockNrs_[Block::Y]){
					out[Block::Y]--;
					out[2] = isHigh ? Block::NWH : Block::NWL;
					face   = Block::NORTH;
				}
			}

			counter++;
		}
	}

	return out;
}


// ************************************************************************* //

} // End namespace oldev

} // End namespace Foam
