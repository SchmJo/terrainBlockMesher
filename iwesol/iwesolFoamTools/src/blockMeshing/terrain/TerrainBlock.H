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
    Foam::iwesol::TerrainBlock

Description
    See below.

SourceFiles
    TerrainBlock.C

References
	[1] J. Schmidt, C. Peralta, B. Stoevesandt, "Automated Generation of
	    Structured Meshes for Wind Energy Applications", Proceedings of the
	    Open Source CFD International Conference, 2012, London, UK

\*---------------------------------------------------------------------------*/

#ifndef TERRAINBLOCK_H_
#define TERRAINBLOCK_H_

#include "SplineBlock.H"
#include "STLLandscape.H"

namespace Foam{
namespace iwesol{

/**
 * @class Foam::iwesol::TerrainBlock
 * @brief A class for nested blocks that follow an stl landscape.
 *
 */
class TerrainBlock:
	public SplineBlock,
	public STLLandscape{

public:

	/// definition of directions:
	static const label BASE1, BASE2, UP;

	/// returns direction label for spline index
	static label getSplineDirection(label splineI);

	/// Constructor.
	TerrainBlock();

	/// Constructor.
	TerrainBlock(const SplineBlock & block): SplineBlock(block){}

	/// Constructor.
	TerrainBlock(searchableSurface const * landscape,
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
			scalar zeroLevel = 0,
			scalar f_pref = 1,
			scalar f_expo = 2,
			scalar maxProjDist = 100000,
			const std::string & gradingCommand = "simpleGrading",
			const scalarList & gradingFactors = scalarList(3,1.)
			);

	/// Destructor.
	virtual ~TerrainBlock();

	/// Constructs the mesh
	void calc();


protected:

	/// The spline point numbers
	labelList splinePointNrs;

	/// Move vertices onto landscape, returns success.
	bool attachVertices();

	/// Calculate the vertex-vertex splines, returns success.
	bool calcSplines();


private:

	/// The coordinate system - third direction is assumed to be normal to terrain
	CoordinateSystem const * cooSys;

	/// The maximal projection distance
	scalar maxProjDist;

	/// A point well above the surface
	point p_above;

	/// Initializes
	void init();

};

} /* iwesol */
} /* Foam */


#endif /* TERRAINBLOCK_H_ */
