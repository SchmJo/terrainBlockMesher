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

\*---------------------------------------------------------------------------*/

#include "PointLinePathEdge.H"
#include "addToRunTimeSelectionTable.H"

namespace Foam
{

namespace oldev
{

defineTypeNameAndDebug(PointLinePathEdge, 0);
addToRunTimeSelectionTable(curvedEdge,PointLinePathEdge,Istream);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

PointLinePathEdge::PointLinePathEdge
(
        const pointField & points,
        const label start,
        const label end,
        const pointField & internalPoints
):
		curvedEdge
		(
				points,
				start,
				end
		),
		PointLinePath
		(
				"edge_" + Foam::name(start) + "_" + Foam::name(end),
				4
		){

	// init path:
	addPoint(points[start]);
	forAll(internalPoints,pI){
		addPoint(internalPoints[pI]);
	}
	addPoint(points[end]);
}

PointLinePathEdge::PointLinePathEdge
(
			const pointField & points,
			Istream & is
):
			curvedEdge
			(
					points,
					is
			),
			PointLinePath
			(
					"edge_" + Foam::name(start_) + "_" + Foam::name(end_),
					4
			){

	// read points:
	pointField allPoints
	(
			appendEndPoints(points, start_, end_, pointField(is))
	);

	// init path:
	forAll(allPoints,pI){
		addPoint(allPoints[pI]);
	}

	// post care for stream:
    token t(is);
    is.putBack(t);

    // discard unused start/end tangents
    if (t == token::BEGIN_LIST)
    {
        vector tangent0Ignored(is);
        vector tangent1Ignored(is);
    }
}


// ************************************************************************* //

} // End namespace oldev

} // End namespace Foam
