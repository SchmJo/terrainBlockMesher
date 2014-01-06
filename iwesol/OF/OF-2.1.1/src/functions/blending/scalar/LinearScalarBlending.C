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

#include "LinearScalarBlending.H"
#include "addToRunTimeSelectionTable.H"

namespace Foam
{

namespace oldev
{

defineTypeNameAndDebug(LinearScalarBlending, 0);
addToRunTimeSelectionTable(ScalarBlendingFunction,LinearScalarBlending,type);
addToRunTimeSelectionTable(ScalarBlendingFunction,LinearScalarBlending,dict);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

LinearScalarBlending::LinearScalarBlending
(
		const word & transitionType
):
		ScalarBlendingFunction
		(
				transitionType
		){
}

LinearScalarBlending::LinearScalarBlending
(
			const dictionary & dict
):
			ScalarBlendingFunction
			(
					dict
			){
}


// ************************************************************************* //

} // End namespace oldev

} // End namespace Foam
