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
    Foam::iwesol::SplineBlock

Description
    See below.

SourceFiles
    SplineBlock.C

References
	[1] J. Schmidt, C. Peralta, B. Stoevesandt, "Automated Generation of
	    Structured Meshes for Wind Energy Applications", Proceedings of the
	    Open Source CFD International Conference, 2012, London, UK

\*---------------------------------------------------------------------------*/

#ifndef SPLINEBLOCK_H_
#define SPLINEBLOCK_H_

#include "CoordinateSystem.H"
#include "BasicBlock.H"
#include "Spline.H"

namespace Foam{
namespace iwesol{

/**
 * @class SplineBlock
 * @brief Class for a block with splines.
 *
 */
class SplineBlock: public BasicBlock {

public:

	/// Define spline labels
	static const label SWL_SEL, SWL_NWL, SEL_NEL, NEL_NWL;
	static const label SEL_SWL, NWL_SWL, NEL_SEL, NWL_NEL;

	static const label SEH_NEH, SWH_NWH, NEH_SEH, NWH_SWH;
	static const label SEL_SEH, NEL_NEH, SEH_SEL, NEH_NEL;

	static const label SWH_SEH, SEH_SWH, NWH_NEH, NEH_NWH;
	static const label SWL_SWH, SWH_SWL, NWL_NWH, NWH_NWL;

	/// Returns spline label
	static label getSplineLabel(label vertex1, label vertex2);

	/// Returns spline vertices labels
	static labelList getSplineVerticesI(label splineLabel);

	/// Returns the spline indices of a face
	static labelList getFaceSplinesI(label faceI);

	/// Returns the constant directions of an edge (ie a spline)
	static labelList getConstantDirectionsEdge(label splineLabel);

	/// Returns the direction of a spline
	static label getDirectionEdge(label splineLabel);

	/// Checks if an edge belongs to a face
	static bool edgeBelongsToFace(label splineLabel, label faceLabel);

	/// Returns label of opposite spline
	static label switchedOrientationLabel(label splineLabel);

	/// returns the sign of spline direction
	static label getEdgeDirectionSign(label splineLabel);

	/// returns the signed spline direction
	static label getSignedEdgeDirection(label splineLabel);

	/// returns the label of the face the spline starts from
	static label splineStartFace(label splineLabel);

	/// returns the label of the face the spline ends at
	static label splineEndFace(label splineLabel);

	/** returns spline points for a spline between two points,
	 * with given start delta vector
	 */
	static pointField getSplinePoints(
			const point & pA,
			const point & pB,
			const Foam::vector & deltaA,
			label splinePointNr
			);

	/** returns spline points for a spline between two points,
	 * with given start and end delta vectors (pointing away from vertex points
	 */
	static pointField getSplinePoints(
			const point & pA,
			const point & pB,
			const Foam::vector & deltaA,
			const Foam::vector & deltaB,
			label splinePointNr
			);

	/// Constructor.
	SplineBlock();

	/// Constructor.
	SplineBlock(pointField* globalPoints,
			const labelList & verticesI,
			const labelList & cells,
			HashTable<Spline>* globalSplines,
			const std::string & gradingCommand = "simpleGrading",
			const scalarList & gradingFactors = scalarList(3,1.)
			);

	/// Constructor.
	SplineBlock(pointField* globalPoints,
			const label & p_SWL,const label & p_SWH,
			const label & p_NWL,const label & p_NWH,
			const label & p_SEL,const label & p_SEH,
			const label & p_NEL,const label & p_NEH,
			const label cells_x,
			const label cells_y,
			const label cells_z,
			HashTable<Spline>* globalSplines,
			const std::string & gradingCommand = "simpleGrading",
			const scalarList & gradingFactors = scalarList(3,1.)
			);

	/// Destructor.
	virtual ~SplineBlock();

	/// Returns a spline
	const Spline & getSpline(label i) const;

	/// Returns a spline
	Spline & getSpline(label i);

	/// Returns a copy of the spline, if empty the spline with end points.
	Spline getSplineCopy(label i) const;

	/// Returns spline points
	inline label getSplinePointsNr(label i) const { return getSpline(i).size(); }

	/// Returns the spline list
	inline const labelList & getSplineIndexList() const { return splineIndexList; }

	/// Returns the dict entry spline for blockMeshDict/edges
	std::string dictEntry_splines() const;

	/// Returns true if the spline is set
	bool hasSpline(label i) const;

	/** Checks consistency:
	 *     Requires that first and last spline points are vertex points,
	 *     and that each spline is only defined once
	 */
	bool check(scalar tolerance = 0.000001) const;

	/// orders a spline. returns true if spline ends have been exchanged.
	bool orderSpline(label splineLabel, const Foam::vector & direction);

	/// Sets and linearly interpolated spline
	void setSpline(label i, label splinePoints);

	/// Sets spline points
	void setSpline(label i, const pointField & splinePoints);

	/// Sets spline points
	void setSpline(label i, const Spline & s);

	/// Set spline between two points, starting with a given vector
	void setSpline(label i, const Foam::vector & delta0, label splinePointNr);

	/// Set spline between two points, with given start and end delta vectors (pointing away from vertex points)
	void setSpline(
			label i,
			const Foam::vector & deltaA,
			const Foam::vector & deltaB,
			label splinePointNr
			);

	/// Set dummy splines, just for making connections visible
	void setDummySplines();

	/// gets spline before. returns 0 for no neighbor, 1 for neighbor with trivial spline, 2 for full spline.
	label getSplineBefore(
			label iSpline,
			Spline & splineBefore
			);

	/// gets spline after. returns 0 for no neighbor, 1 for neighbor with trivial spline, 2 for full spline.
	label getSplineAfter(
				label iSpline,
				Spline & splineAfter
				);

	/// returns tangent to a spline, at start or end point
	Foam::vector getSplineTangent(label iSpline, bool atStartPoint, bool ignoreEmptyNeighborSplines = false) const;

	/// returns local coordinate system of a spline point
	CoordinateSystem getLocalSplineCoordinateSystem(
			label iSpline,
			scalar s
			);

	/// checks if spline exists, otherwise create with linear points. returns spline.
	Spline & ensureSpline(label iSpline);

	/// flips a spline
	Spline & flipSpline(label iSpline);

	/// deletes a spline
	void eraseSpline(label iSpline);


protected:

	/// List of spline labels in global spline list
	labelList splineIndexList;


private:

	/// The global spline list
	HashTable<Spline>* globalSplines;

};

} /* iwesol */
} /* Foam */

#endif /* SPLINEBLOCK_H_ */
