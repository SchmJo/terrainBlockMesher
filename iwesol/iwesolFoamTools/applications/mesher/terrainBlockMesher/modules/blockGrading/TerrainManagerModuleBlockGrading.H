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
    Foam::iwesol::TerrainManagerModuleBlockGrading

Description
    See below.

SourceFiles
    TerrainManagerModuleBlockGrading.C

Reference

\*---------------------------------------------------------------------------*/

#ifndef TerrainManagerModuleBlockGrading_H_
#define TerrainManagerModuleBlockGrading_H_

#include "scalarList.H"

#include "ClassModule.H"

namespace Foam{
namespace iwesol{

class TerrainManager;

/**
 * @class Foam::iwesol::TerrainManagerModuleBlockGrading
 * @brief The terrainBlockMesher module for an outer cylinder around the rectangular landscape.
 */
class TerrainManagerModuleBlockGrading:
	public ClassModule< TerrainManager >{

public:

	/// Constructor
	TerrainManagerModuleBlockGrading(
			TerrainManager * terrainManager
			);

	/// ClassModule: load the module
	bool load(
			const dictionary & dict
			);

	/** Calculates the block grading lists and cell gradings.
	 *  blockGrading[i][j]: i = direction, j = blockNr, blockGrading = block width
	 */
	void getDeltaLists(scalarListList & blockGrading,scalarListList & cellGrading);


private:

	/// the task dictionaries for block grading
	List< List<dictionary> > gradingTasks;

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


};

} /* iwesol */
} /* Foam */

#endif /* TerrainManagerModuleBlockGrading_H_ */
