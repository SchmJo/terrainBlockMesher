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

#include "TerrainManagerModuleOrographyModifications.H"
#include "TerrainManager.H"
#include "Globals.H"

namespace Foam
{

namespace oldev
{

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //



// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

TerrainManagerModuleOrographyModifications::TerrainManagerModuleOrographyModifications
(
		TerrainManager * terrainManager
):
		ClassModule< TerrainManager >(terrainManager){
}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

bool TerrainManagerModuleOrographyModifications::load
(
			const dictionary & dict
){

    // get the modify orography tasks:
    modifyTasks_ = getDictTasks(dict);
    Info << "   found " << modifyTasks_.size() << " orography modification requests." << endl;

	return ClassModule< TerrainManager >::load(dict);
}

bool TerrainManagerModuleOrographyModifications::calc(){

	// ensure ground splines:
	{
		label eg = moduleBase().blockNrs()[TerrainManager::BASE1]
		           * ( moduleBase().blockNrs()[TerrainManager::BASE2] + 1 ) +
		           moduleBase().blockNrs()[TerrainManager::BASE2]
		           * ( moduleBase().blockNrs()[TerrainManager::BASE1] + 1 );
		Info << "   creating " << eg - moduleBase().edgeCounter()
			 << " trivial ground edges" << endl;
		if(moduleBase().edgeCounter() != eg){
			forAll(moduleBase().blocks(),bI){
				moduleBase().ensureEdge(bI,Block::SWL_SEL);
				moduleBase().ensureEdge(bI,Block::SEL_NEL);
				moduleBase().ensureEdge(bI,Block::NEL_NWL);
				moduleBase().ensureEdge(bI,Block::NWL_SWL);
			}
			Info << "   new total number of edges: " << moduleBase().edgeCounter() << endl;
		}
	}

	forAll(modifyTasks_,mI){

		Info << "   Task " << mI + 1 << ": ";

		// grab task:
		const dictionary & modifyDict = modifyTasks_[mI];
/*
	    // delete holes:
	    if(modifyDict.found("deleteHoles")){

	    	Info << "Deleting holes" << endl;
	        deleteHoles(modifyDict.subDict("deleteHoles"));
	    }
*/

	    // add a KasmiMassonHill:
	    if(modifyDict.found("KasmiMassonHill")){

	    	Info << "Adding Kasmi-Masson-Hill" << endl;
	        addKasmiMassonHill(modifyDict.subDict("KasmiMassonHill"));
	    }

	    // add an OvalKMHill:
	    if(modifyDict.found("OvalKMHill")){

	    	Info << "Adding oval Kasmi-Masson-Hill" << endl;
	        addOvalKMHill(modifyDict.subDict("OvalKMHill"));
	    }

	    // add an ConvexPolygon:
	    if(modifyDict.found("ConvexPolygon")){

	    	Info << "Adding convex polygon" << endl;
	        addConvexPolygon(modifyDict.subDict("ConvexPolygon"));
	    }
/*
	    // make mesh cyclic:
	    if(modifyDict.found("makeCyclicNorthSouth")){

	    	const scalar depth = readScalar(modifyDict.subDict("makeCyclicNorthSouth").lookup("adjDepth"));
	    	Info << "Enforcing cyclic boundaries: "
	    			<< moduleBase().patches()[Block::NORTH].name() << " <--> "
	    			<< moduleBase().patches()[Block::SOUTH].name() << ", depth = " << depth << endl;
	    	makeCyclic(Block::NORTH,Block::SOUTH,depth);
	    }
	    if(modifyDict.found("makeCyclicWestEast")){

	    	const scalar depth = readScalar(modifyDict.subDict("makeCyclicWestEast").lookup("adjDepth"));
	    	Info << "Enforcing cyclic boundaries: "
	    			<< moduleBase().patches()[Block::WEST].name() << " <--> "
	    			<< moduleBase().patches()[Block::EAST].name() << ", depth = " << depth << endl;
	    	makeCyclic(Block::WEST,Block::EAST,depth);
	    }
*/
	}

	return true;
}

void TerrainManagerModuleOrographyModifications::moveGroundPoint
(
		point & p,
		scalar height0,
		scalar height,
		const word & addType
){

	// prepare:
	const Foam::vector & n_up = moduleBase().coordinateSystem().e(TerrainManager::UP);

	// add type 'add':
	if(addType.compare("add") == 0){

		if( height != 0 ) p += height * n_up;
	}

	// add type 'max':
	else if(addType.compare("max") == 0){

		const scalar h0 = p & n_up;
		if( height0 + height > h0 ){
			p += (height0 + height - h0) * n_up;
		}
	}

	// add type 'average':
	else if(addType.compare("average") == 0){

		if( height != 0 ){
			const scalar h0 = p & n_up;
			scalar h        = 0.5 * ( 2 * h0 + height0 + height );
			p              += (h - h0) * n_up;
		}

	}

	// add type 'hill':
	else if(addType.compare("hill") == 0){

		if( height != 0.){
			const scalar h0 = p & n_up;
			p              += (height0 + height - h0) * n_up;
		}
	}

}


// ************************************************************************* //

} // End namespace oldev

} // End namespace Foam
