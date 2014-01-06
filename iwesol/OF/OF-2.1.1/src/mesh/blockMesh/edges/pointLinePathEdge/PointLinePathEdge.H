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
    Foam::oldev::PointLinePathEdge

Description

SourceFiles
    PointLinePathEdgeI.H
    PointLinePathEdge.C
    PointLinePathEdgeIO.C

\*---------------------------------------------------------------------------*/

#ifndef PointLinePathEdge_H
#define PointLinePathEdge_H

#include "curvedEdge.H"
#include "PointLinePath.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

namespace oldev
{


/*---------------------------------------------------------------------------*\
                         Class PointLinePathEdge Declaration
\*---------------------------------------------------------------------------*/

class PointLinePathEdge
:
	public curvedEdge,
	public PointLinePath
{

public:

    /// Runtime type information
    TypeName("pointLinePath");


    // Constructors

        /// Construct from components
        PointLinePathEdge
        (
                const pointField & points,
                const label start,
                const label end,
                const pointField & internalPoints
        );

        /// Construct from Istream
        PointLinePathEdge
        (
    			const pointField & points,
    			Istream & is
        );


    // Member Functions

        // Access

        /// Return the point position corresponding to the curve parameter
        //  0 <= lambda <= 1
        inline Foam::point position(const scalar s) const { return PointLinePath::operator[](s); }

        /// Return the length of the curve
        inline scalar length() const { return PointLinePath::length(); }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace oldev

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
