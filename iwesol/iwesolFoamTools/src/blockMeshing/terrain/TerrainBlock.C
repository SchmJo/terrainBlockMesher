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

#include "TerrainBlock.H"
//#include "Globals.H"

namespace Foam{
namespace iwesol{

const label TerrainBlock::BASE1 = 0;
const label TerrainBlock::BASE2 = 1;
const label TerrainBlock::UP    = 2;

label TerrainBlock::getSplineDirection(label splineI){

	if(splineI == SWL_SEL || splineI == SEL_SWL) return BASE1;
	if(splineI == NWL_NEL || splineI == NEL_NWL) return BASE1;
	if(splineI == SWH_SEH || splineI == SEH_SWH) return BASE1;
	if(splineI == NWH_NEH || splineI == NEH_NWH) return BASE1;

	if(splineI == SWL_NWL || splineI == NWL_SWL) return BASE2;
	if(splineI == SEL_NEL || splineI == NEL_SEL) return BASE2;
	if(splineI == SWH_NWH || splineI == NWH_SWH) return BASE2;
	if(splineI == SEH_NEH || splineI == NEH_SEH) return BASE2;

	if(splineI == SWL_SWH || splineI == SWH_SWL) return UP;
	if(splineI == SEL_SEH || splineI == SEH_SEL) return UP;
	if(splineI == NWL_NWH || splineI == NWH_NWL) return UP;
	if(splineI == NEL_NEH || splineI == NEH_NEL) return UP;

	return -1;
}

TerrainBlock::TerrainBlock(){
}


TerrainBlock::TerrainBlock(
		searchableSurface const * landscape,
		pointField* globalPoints,
		const labelList & verticesI,
		const labelList & cells,
		CoordinateSystem * cooSys,
		const point & p_above,
		HashTable<Spline>* globalSplines,
		const labelList & splinePointNrs,
		const point & p_SWL,
		const scalarList & dimensions,
		const point & p_SWL_stl,
		const scalarList & dimensions_stl,
		scalar zeroLevel,
		scalar f_pref,
		scalar f_expo,
		scalar maxProjDist,
		const std::string & gradingCommand,
		const scalarList & gradingFactors
		):
		SplineBlock(
				globalPoints,
				verticesI,
				cells,
				globalSplines,
				gradingCommand,
				gradingFactors
				),
		STLLandscape(
				cooSys,
				landscape,
				p_SWL,
				dimensions,
				p_SWL_stl,
				dimensions_stl,
				zeroLevel,
				f_pref,
				f_expo
				),
		splinePointNrs(splinePointNrs),
		cooSys(cooSys),
		maxProjDist(maxProjDist),
		p_above(p_above){
	init();
}

TerrainBlock::~TerrainBlock() {
}

void TerrainBlock::init(){

	// attach ground vertices to landscape height:
	if(!attachVertices()){
		Info << "\nTerrainBlock: Error attaching points.\n" << endl;
		throw;
	}

	// calculate the vertex-vertex splines:
	if(!calcSplines()){
		Info << "\nTerrainBlock: Error calculating splines.\n" << endl;
		throw;
	}

}

bool TerrainBlock::attachVertices(){

	// prepare:
	const Foam::vector & n_up = cooSys->e(UP);

	point & p1 = getVertex(BasicBlock::SWL);
	p1        += dot(p_above - p1,n_up)  * n_up;
	if(!attachPoint(p1,p1 - maxProjDist * n_up)){
		Info << "TerrainBlock: Cannot attach point SWL = " << p1 << " to STL.\n" << endl;
		return false;
	}

	point & p2 = getVertex(BasicBlock::NWL);
	p2        += dot(p_above - p2,n_up) * n_up;
	if(!attachPoint(p2,p2 - maxProjDist * n_up)){
		Info << "TerrainBlock: Cannot attach point NWL = " << p2 << " to STL.\n" << endl;
		return false;
	}

	point & p3 = getVertex(BasicBlock::SEL);
	p3        += dot(p_above - p3,n_up) * n_up;
	if(!attachPoint(p3,p3 - maxProjDist * n_up)){
		Info << "TerrainBlock: Cannot attach point SEL = " << p3 << " to STL.\n" << endl;
		return false;
	}

	point & p4 = getVertex(BasicBlock::NEL);
	p4        += dot(p_above - p4,n_up) * n_up;
	if(!attachPoint(p4,p4 - maxProjDist * n_up)){
		Info << "TerrainBlock: Cannot attach point NEL = " << p4 << " to STL.\n" << endl;
		return false;
	}
	return true;
}

bool TerrainBlock::calcSplines(){

	// prepare:
	const Foam::vector & n_up = cooSys->e(UP);

	// loop over ground spline labels:
	for(label i = 0; i < 4; i++){

		// prepare:
		const label splinePoints = splinePointNrs[getSplineDirection(i)];
		pointField spline(splinePoints);

		// grab spline end points:
		const labelList verticesI = getSplineVerticesI(i);
		point & pointA = getVertex(verticesI[0]);
		point & pointB = getVertex(verticesI[1]);

		// calc delta in x,y:
		point delta = (pointB - pointA) / (splinePoints + 1);

		// loop over spline points:
		for(label u = 0; u < splinePoints; u++){

			point splinePoint = pointA + (1 + u) * delta;

			// make sure point is above surface:
			splinePoint += dot(p_above - splinePoint,n_up) * n_up;

			// project to stl:
			if(!attachPoint(splinePoint,splinePoint - maxProjDist * n_up)){
				Info << "TerrainBlock: Error: Cannot project point p = " << splinePoint << " onto stl.\n" << endl;
				return false;
			}

			// add:
			spline[u] = splinePoint;

		}

		// set it:
		setSpline(i,spline);

	}

	return true;
}


} /* iwesol */
} /* Foam */
