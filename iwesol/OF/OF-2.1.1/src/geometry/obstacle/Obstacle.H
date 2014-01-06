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
    Foam::oldev::Obstacle

Description

SourceFiles
    Obstacle.C

\*---------------------------------------------------------------------------*/

#ifndef Obstacle_H
#define Obstacle_H

#include "fvMesh.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

namespace oldev
{

/*---------------------------------------------------------------------------*\
                         Class Obstacle Declaration
\*---------------------------------------------------------------------------*/

class Obstacle
{
    // Private data

		/// the obstacle obstacleType
		label obstacleType_;

		/// the resolution
		scalar res_;


public:

    // Static data members

    	/// the obstacle obstacleTypes
    	static const label NONE, WALL, WALL_KICK;


    // Constructors

        /// Construct from components
        Obstacle
        (
    			scalar resolution = 0.001,
    			label obstacleType = NONE
        );


    /// Destructor
    virtual ~Obstacle();


    // Member Functions

        // Access

		/// Obstacle: Returns true if the point lies inside/beside
		virtual bool isInside(const point & p) const = 0;

		/// Obstacle: Returns true if the point lies on the boundary
		virtual bool isBoundary(const point & p) const = 0;

		/// Returns true if the point lies outside
		inline bool isOutside(const point & p) const { return !(isInside(p) || isBoundary(p)); }

		/// Returns obstacle obstacleType
		inline label getObstacleType() const { return obstacleType_; }

		/// Returns the resolution
		inline const scalar & resolution() const { return res_; }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace oldev

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
