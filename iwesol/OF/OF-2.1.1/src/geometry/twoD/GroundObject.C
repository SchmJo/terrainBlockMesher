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

#include "GroundObject.H"
#include "addToRunTimeSelectionTable.H"

namespace Foam
{

namespace oldev
{

defineRunTimeSelectionTable(GroundObject,dict);


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

GroundObject::GroundObject(
		const word & addType,
		const scalar & resolution
		):
		addType_(addType),
		resolution_(resolution){
}

GroundObject::GroundObject(
		const dictionary & dict
		):
		addType_("add"),
		resolution_(readScalar(dict.lookup("resolution"))){

	if(dict.found("addType")){
		addType_ = word(dict.lookup("addType"));
	} else {
		Info << "GroundObject: keyword 'addType' not found in dictionary, choosing '" << addType_ << "'." << endl;
	}
}


// * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * * //

autoPtr< GroundObject > GroundObject::New(
		const dictionary & dict
		){

	word type(dict.lookup("type"));
	Info<< "Selecting ground object type '" << type << "'" << endl;

	dictConstructorTable::iterator cstrIter =
			dictConstructorTablePtr_->find(type);

	if (cstrIter == dictConstructorTablePtr_->end()){
	        FatalErrorIn
	        (
	             "GroundObject::New"
	             "("
	                 "const dictionary& "
	             ")"
	        )   << "Unknown ground object type '"
	            << type << "'" << nl << nl
	            << "Valid types:" << endl
	            << dictConstructorTablePtr_->sortedToc()
	            << exit(FatalError);
	}

	return autoPtr< GroundObject >(cstrIter()(dict));
}

// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

GroundObject::~GroundObject()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //


dictionary GroundObject::exportDict() const{

	dictionary out;

	out.add("add",addType_);

	return out;
}


// ************************************************************************* //

} // End namespace oldev

} // End namespace Foam
