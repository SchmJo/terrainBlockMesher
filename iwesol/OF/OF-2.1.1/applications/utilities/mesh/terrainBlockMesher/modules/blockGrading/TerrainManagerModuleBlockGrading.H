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
    Foam::TerrainManagerModuleBlockGrading

Description

SourceFiles
    TerrainManagerModuleBlockGrading.C

\*---------------------------------------------------------------------------*/

#ifndef TerrainManagerModuleBlockGrading_H
#define TerrainManagerModuleBlockGrading_H

#include "ClassModule.H"
#include "scalarList.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

namespace oldev
{

class TerrainManager;

/*---------------------------------------------------------------------------*\
                         Class TerrainManagerModuleBlockGrading Declaration
\*---------------------------------------------------------------------------*/

class TerrainManagerModuleBlockGrading
:
    public ClassModule< TerrainManager >
{

    /// Helper struct for weightRegions. TODO: class?
    struct gradingRegion{
    	scalar width;
    	scalar start;
    	scalar end;
    	label blocks;
    	label firstBlock;
    	label lastBlock;
    	word type;
    };

    // Private data

    	/// the task dictionaries for block grading
    	List< List<dictionary> > gradingTasks_;


    // Private Member Functions

        /// Disallow default bitwise copy construct
        TerrainManagerModuleBlockGrading(const TerrainManagerModuleBlockGrading&);

        /// Disallow default bitwise assignment
        void operator=(const TerrainManagerModuleBlockGrading&);


public:

    // Constructors

        /// Construct from components
        TerrainManagerModuleBlockGrading(TerrainManager * terrainManager);


    // Member Functions

        // Access

		/** Calculates the block grading lists and cell gradings.
		 *  blockGrading[i][j]: i = direction, j = blockNr, blockGrading = block width
		 */
		void getDeltaLists
		(
				scalarListList & blockGrading,
				scalarListList & cellGrading
		);


        // Edit

		/// ClassModule: load the module
		bool load(
			const dictionary & dict
		);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace oldev

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
