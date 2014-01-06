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

\*---------------------------------------------------------------------------*/

#include "OptimizerGroundObject.H"
#include "addToRunTimeSelectionTable.H"

namespace Foam
{

namespace oldev
{

defineRunTimeSelectionTable(OptimizerGroundObject,dict);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

OptimizerGroundObject::OptimizerGroundObject(
		const word & addType,
		const Box & rangeBox
		):
		GroundObject(addType),
		box_(rangeBox){
}

OptimizerGroundObject::OptimizerGroundObject(
		const dictionary & dict,
		const Box & rangeBox
		):
		GroundObject(dict),
		box_(rangeBox){
}


// * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * * //

autoPtr< OptimizerGroundObject > OptimizerGroundObject::New(
		const dictionary & dict
		){

	word type(dict.lookup("type"));
	Info<< "Selecting optimizer ground object type '" << type << "'" << endl;

	dictConstructorTable::iterator cstrIter =
			dictConstructorTablePtr_->find(type);

	if (cstrIter == dictConstructorTablePtr_->end()){
	        FatalErrorIn
	        (
	             "GroundObject::New"
	             "("
	                 "const dictionary& "
	             ")"
	        )   << "Unknown optimizer ground object type '"
	            << type << "'" << nl << nl
	            << "Valid types:" << endl
	            << dictConstructorTablePtr_->sortedToc()
	            << exit(FatalError);
	}

	return autoPtr< OptimizerGroundObject >(cstrIter()(dict));
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

OptimizerGroundObject::~OptimizerGroundObject()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

bool OptimizerGroundObject::isInside(
		const point & p
		) const{

	// check bounds:
	if(box_.isOutside(p)) return false;

	// get constraint values:
	scalarList c(nrConstraints());
	if(!calcPointConstraints(p,c)){
		Info << "\nOptimizerGroundObject: Error during constraints evaluation." << endl;
		throw;
	}

	// check all constraints:
	forAll(c,cI) if(c[cI] > 0) {
		return false;
	}

	return true;
}

dictionary OptimizerGroundObject::exportDict() const{

	dictionary out = GroundObject::exportDict();

	out.add("type",type());

	return out;
}


// ************************************************************************* //

} // End namespace oldev

} // End namespace Foam
