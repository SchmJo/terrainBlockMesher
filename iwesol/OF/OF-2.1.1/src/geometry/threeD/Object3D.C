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

#include "Object3D.H"
#include "addToRunTimeSelectionTable.H"

namespace Foam
{

namespace oldev
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Object3D::Object3D(
			scalar resolution,
			label obstacleType
			):
			Obstacle
			(
					resolution,
					obstacleType
			){
}

Object3D::Object3D(
		const dictionary & dict
		):
		Obstacle
		(
			readScalar(dict.lookup("resolution")),
			Obstacle::WALL
		){
}

// * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * * //

defineRunTimeSelectionTable(Object3D,dict);

autoPtr<Object3D> Object3D::New(
		const dictionary & dict
		){

	word type(dict.lookup("type"));
	Info<< "Selecting Object3D of type '" << type << "'" << endl;

	dictConstructorTable::iterator cstrIter =
			dictConstructorTablePtr_->find(type);

	if (cstrIter == dictConstructorTablePtr_->end()){
	        FatalErrorIn
	        (
	             "Object3D::New"
	             "("
	        		 "const dictionary & "
	             ")"
	        )   << "Unknown type '"
	            << type << "'" << nl << nl
	            << "Valid types:" << endl
	            << dictConstructorTablePtr_->sortedToc()
	            << exit(FatalError);
	}

	return autoPtr<Object3D>(cstrIter()(dict));
}

// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Object3D::~Object3D()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //


// ************************************************************************* //

} // End namespace oldev

} // End namespace Foam
