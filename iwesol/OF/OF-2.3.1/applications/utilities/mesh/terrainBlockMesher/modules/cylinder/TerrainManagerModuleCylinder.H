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
    Foam::oldev::TerrainManagerModuleCylinder

Description


SourceFiles
    TerrainManagerModuleCylinder.C
    TMMCCalc.C
    TMMCSet.C

Author
    Jonas Schmidt (jonas.schmidt@iwes.fraunhofer.de)

Modified by


Test location


Tested by


Used by


Limitations


Future improvements


Reference


\*---------------------------------------------------------------------------*/

#ifndef TerrainManagerModuleCylinder_H
#define TerrainManagerModuleCylinder_H

#include "ClassModule.H"
#include "ScalarBlendingFunction.H"
#include "labelList.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

namespace oldev
{

class TerrainManager;

/*---------------------------------------------------------------------------*\
                         Class TerrainManagerModuleCylinder Declaration
\*---------------------------------------------------------------------------*/

class TerrainManagerModuleCylinder:
public ClassModule< TerrainManager >
{
    // Private data

    	/// the cylinder centre
    	point cylinderCentre_;

    	/// the cylinder radius
    	scalar cylinderRadius_;

    	/// the cylinder radial blocks
    	label cylinderRadialBlocks_;

    	/// the cylinder radial cells
    	label cylinderRadialCells_;

    	/// the cylinder radial grading
    	label cylinderRadialGrading_;

    	/// the total number of pure cylinder blocks
    	label cylinderBlockNr_;

    	/// the total number of pure cylinder points
    	label cylinderPointNr_;

    	/// the direction of the first cylinder sector
    	Foam::vector cylinderFirstSectionStartDir_;

    	/// the number of cylinder sectors
    	label cylinderSectionNr_;

    	/// the subsections
    	labelList cylinderSubSections_;

    	/// the cylinder point addressing. key = "sectionI.subsectionI.radialI.LH"
    	HashTable< label > cylinderPointAdr_;

    	/// the cylinder block addressing. key = "faceI.ij.radialI"
    	HashTable< label > cylinderBlockAdr_;

    	/// inner point to cylinder connections. key = inner point key, value = outer point key
    	HashTable< word > cylinderConnections_innerOuterKeys_;

    	/// inner point to cylinder connections. key = absolute pointI of inner point, value = (sectionI,subSectionI)
    	HashTable< labelList, label > cylinderConnections_pI_secSubsec_;

    	/// the outer spline blending function
    	autoPtr< ScalarBlendingFunction > blending_;


    // Private Member Functions

    	/// init the cylinder
    	void initCylinder(const dictionary & dict);

    	/// find inner-outer corner connections
    	void cylinderFindSectionConnections();

    	/// iterate section connections
    	void cylinderIterateSectionConnections
    	(
    			label nFirst,
    			labelList & deltaNSection,
    			scalarList & distances
    	);

    	/// calculate inner-outer distances
    	void cylinderCalcSectionDistances
    	(
    			label nFirst,
    			const labelList & deltaNSection,
    			scalarList & distances
    	);

    	/// calculate single inner-outer distance
    	scalar cylinderCalcSectionDistance
    	(
    			label sectionI,
    			label n
    	);

    	/// calculate points
    	bool calcCylinderPoints();

    	/// calculate blocks
    	bool calcCylinderBlocks();

    	/// set Patches
    	bool setCylinderPatches();

    	/// set splines
    	bool setCylinderSplines();

        /// Disallow default bitwise copy construct
        TerrainManagerModuleCylinder(const TerrainManagerModuleCylinder&);

        /// Disallow bitwise assignment operator
        void operator=(const TerrainManagerModuleCylinder&);


public:

    // Constructors

    	/// Constructor
    	TerrainManagerModuleCylinder
    	(
    			TerrainManager * terrainManager
    	);


    /// Destructor
    ~TerrainManagerModuleCylinder();


    // Member Functions

        // Edit

		/// ClassModule: load the module
		bool load
		(
				const dictionary & dict
		);

		/// provide storage
		void reserveStorageCylinder();

		/// calculate
		bool calc();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace oldev

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
