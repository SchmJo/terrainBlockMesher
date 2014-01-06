/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Class
    Foam::oldev::BlockConventions

Description

SourceFiles
    BlockConventions.C

\*---------------------------------------------------------------------------*/

#ifndef BlockConventions_H
#define BlockConventions_H

#include "labelList.H"
#include "scalarList.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

namespace oldev
{


/*---------------------------------------------------------------------------*\
                         Class BlockConventions Declaration
\*---------------------------------------------------------------------------*/

class BlockConventions
{

public:

    // Static data members

    	/// define box corner labels: eg south-west-low = SWL
    	static const label SWL, SWH, NWL, NWH, SEL, SEH, NEL, NEH;

    	/// define x, y, z labels
    	static const label X, Y, Z;

    	/// define face labels
    	static const label NONE, WEST, EAST, NORTH, SOUTH, GROUND, SKY;

    	/// Define edge labels
    	static const label SWL_SEL, SWL_NWL, SEL_NEL, NEL_NWL;
    	static const label SEL_SWL, NWL_SWL, NEL_SEL, NWL_NEL;

    	static const label SEH_NEH, SWH_NWH, NEH_SEH, NWH_SWH;
    	static const label SEL_SEH, NEL_NEH, SEH_SEL, NEH_NEL;

    	static const label SWH_SEH, SEH_SWH, NWH_NEH, NEH_NWH;
    	static const label SWL_SWH, SWH_SWL, NWL_NWH, NWH_NWL;


    // Static functions

		/// transform grading factors into expandRatios
		static scalarList gradingFactors2ExpandRatios
		(
			const scalarList & gradingFactors
		);

		/// create labelList from labels
		static labelList vertices2vertexList
		(
			const label & p_SWL,const label & p_SWH,
			const label & p_NWL,const label & p_NWH,
			const label & p_SEL,const label & p_SEH,
			const label & p_NEL,const label & p_NEH
		);

		/// returns the constant direction of a face
		static label getConstantDirectionFace(label faceID);

		/// returns the opposite face
		static label getOppositeFace(label faceID);

		/// Returns edge label
		static label getEdgeLabel(label vertex1, label vertex2);

		/// Returns edge vertices labels
		static labelList getEdgeVerticesI(label edgeID);

		/// Returns the edge indices of a face
		static labelList getFaceEdgesI(label faceI);

		/// Returns the constant directions of an edge (ie a edge)
		static labelList getConstantDirectionsEdge(label edgeID);

		/// Returns the direction of a edge
		static label getDirectionEdge(label edgeID);

		/// Checks if an edge belongs to a face
		static bool edgeBelongsToFace(label edgeID, label faceID);

		/// Returns label of opposite edge
		static label switchedOrientationLabel(label edgeID);

		/// returns the sign of edge direction
		static label getEdgeDirectionSign(label edgeID);

		/// returns the signed edge direction
		static label getSignedEdgeDirection(label edgeID);

		/// returns the label of the face the edge starts from
		static label edgeStartFace(label edgeID);

		/// returns the label of the face the edge ends at
		static label edgeEndFace(label edgeID);

		/// maps block labels to model labels
		static labelList mapToHexModel(const labelList & vertices);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace oldev

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
