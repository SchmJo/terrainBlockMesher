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
    Foam::oldev::TerrainManagerModuleOrographyModifications

Description

SourceFiles
    TerrainManagerModuleOrographyModificationsI.H
    TerrainManagerModuleOrographyModifications.C
    TerrainManagerModuleOrographyModificationsIO.C

\*---------------------------------------------------------------------------*/

#ifndef TerrainManagerModuleOrographyModifications_H
#define TerrainManagerModuleOrographyModifications_H

#include "ClassModule.H"
#include "point.H"
#include "labelList.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

namespace oldev
{

class TerrainManager;

/*---------------------------------------------------------------------------*\
                         Class TerrainManagerModuleOrographyModifications Declaration
\*---------------------------------------------------------------------------*/

class TerrainManagerModuleOrographyModifications
:
    public ClassModule< TerrainManager >
{
    // Private data

    	/// the task dictionaries for modifying the orography
    	List< dictionary > modifyTasks_;


    // Private Member Functions

    	/// deletes holes
    	//void deleteHoles(const dictionary & dict);

    	/// add a KasmiMassonHill
    	void addKasmiMassonHill(const dictionary & dict);

    	/// add a convex polygon
    	void addConvexPolygon(const dictionary & dict);

    	/// add an OvalKMHill
    	void addOvalKMHill(const dictionary & dict);

    	/// makes the mesh cyclic
/*    	void makeCyclic
    	(
    			label patchIA,
    			label patchIB,
    			const scalar & depth
    	);
*/
    	/// moves a ground point
    	void moveGroundPoint
    	(
    			point & p,
    			scalar height0,
    			scalar height,
    			const word & addType
    	);

    	/// identifies holes. returns number of hole points found.
/*    	label identifyHoles
    	(
    			List< point* > pathPoints,
    			labelList & isHole,
    			const scalar & holeTolerance,
    			const scalar & holeMinDepth,
    			label dirSign = 1
    	);
*/
        //- Disallow default bitwise copy construct
        TerrainManagerModuleOrographyModifications
        (
        		const TerrainManagerModuleOrographyModifications&
        );

        //- Disallow default bitwise assignment
        void operator=
        (
        		const TerrainManagerModuleOrographyModifications&
        );


public:


    // Constructors

        //- Construct from components
        TerrainManagerModuleOrographyModifications
        (
        		TerrainManager * terrainManager
        );


    // Member Functions

        // Access

    	/// ClassModule: load the module
    	bool load
    	(
    			const dictionary & dict
    	);

    	/// Starts post-calc action, as defined by modifyDict
    	bool calc();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace oldev

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
