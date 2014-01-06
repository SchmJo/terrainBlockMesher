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
    Foam::oldev::Object3D

Description

SourceFiles
    Object3D.C

\*---------------------------------------------------------------------------*/

#ifndef Object3D_H
#define Object3D_H

#include "runTimeSelectionTables.H"
#include "Obstacle.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

namespace oldev
{

// foward declarations
class Box;

/*---------------------------------------------------------------------------*\
                         Class Object3D Declaration
\*---------------------------------------------------------------------------*/

class Object3D
:
    public Obstacle
{
public:

	// run time selection macro
	declareRunTimeSelectionTable
	(
		autoPtr,
		Object3D,
		dict,
		(
				const dictionary & dict
		),
		(dict)
	);


    // Constructors

    	/// Constructor
    	Object3D
    	(
    			scalar resolution = 0.001,
    			label obstacleType = Obstacle::NONE
    	);

    	/// Constructor
    	Object3D
    	(
    			const dictionary & dict
    	);


    // Selectors

        /// Selector
		static autoPtr< Object3D > New
		(
				const dictionary & dict
		);


    /// Destructor
    virtual ~Object3D();


    // Member Functions

        // Access

		/// Object3D: returns a surrounding box
		virtual const Box & surroundingBox() const = 0;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace oldev

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
