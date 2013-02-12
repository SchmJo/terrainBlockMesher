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
    Foam::iwesol::TerrainManagerModuleOrographyModifications

Description
    See below.

SourceFiles
    TerrainManagerModuleOrographyModifications.C

Reference

\*---------------------------------------------------------------------------*/

#ifndef TERRAINMANAGERMODULEOROGRAPHYMODIFICATIONS_H_
#define TERRAINMANAGERMODULEOROGRAPHYMODIFICATIONS_H_

#include "point.H"

#include "ClassModule.H"

namespace Foam{
namespace iwesol{

class TerrainManager;

/**
 * @class Foam::iwesol::TerrainManagerModuleOrographyModifications
 * @brief The module for the terrain mesher which modifies the orography.
 */
class TerrainManagerModuleOrographyModifications:
	public ClassModule< TerrainManager > {

public:

	/// Constructor
	TerrainManagerModuleOrographyModifications(
			TerrainManager * terrainManager
			);

	/// ClassModule: load the module
	bool load(
			const dictionary & dict
			);

	/// Starts post-calc action, as defined by modifyDict
	bool calc();


private:

	/// the task dictionaries for modifying the orography
	List<dictionary> modifyTasks;

	/// add a KasmiMassonHill
	void addKasmiMassonHill(const dictionary & dict);

	/// add a convex polygon
	void addConvexPolygon(const dictionary & dict);

	/// add an OvalKMHill
	void addOvalKMHill(const dictionary & dict);

	/// makes the mesh cyclic
	void makeCyclic(label patchIA, label patchIB,const scalar & depth);

	/// moves a ground point
	void moveGroundPoint(point & p, scalar height0, scalar height, const word & addType);


};

} /* iwesol */
} /* Foam */

#endif /* TERRAINMANAGERMODULEOROGRAPHYMODIFICATIONS_H_ */
