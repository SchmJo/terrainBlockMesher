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

#include "TransitionFunction.H"
#include "addToRunTimeSelectionTable.H"

namespace Foam
{

namespace oldev
{

defineRunTimeSelectionTable(TransitionFunction,type);
defineRunTimeSelectionTable(TransitionFunction,dict);


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

TransitionFunction::TransitionFunction() {
}

// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

TransitionFunction::~TransitionFunction()
{}

// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

autoPtr< TransitionFunction > TransitionFunction::New(
		const word & type
		){

	Info<< "Selecting transition function type '" << type << "'" << endl;

	typeConstructorTable::iterator cstrIter =
			typeConstructorTablePtr_->find(type);

	if (cstrIter == typeConstructorTablePtr_->end()){
	        FatalErrorIn
	        (
	             "TransitionFunction::New"
	             "("
	                 "const word&"
	             ")"
	        )   << "Unknown transition function type '"
	            << type << "'" << nl << nl
	            << "Valid types:" << endl
	            << typeConstructorTablePtr_->sortedToc()
	            << exit(FatalError);
	}

	return autoPtr< TransitionFunction >(cstrIter()());
}

autoPtr< TransitionFunction > TransitionFunction::New(
		const dictionary & dict
		){

	word type(dict.lookup("type"));
	Info<< "Selecting transition function type '" << type << "'" << endl;

	dictConstructorTable::iterator cstrIter =
			dictConstructorTablePtr_->find(type);

	if (cstrIter == dictConstructorTablePtr_->end()){
	        FatalErrorIn
	        (
	             "TransitionFunction::New"
	             "("
	                 "const dictionary & dict"
	             ")"
	        )   << "Unknown transition function type '"
	            << type << "'" << nl << nl
	            << "Valid types:" << endl
	            << dictConstructorTablePtr_->sortedToc()
	            << exit(FatalError);
	}

	return autoPtr< TransitionFunction >(cstrIter()(dict));
}

ScalarRFunction TransitionFunction::scalarRFunction(
			label steps,
			int interpolOrder,
			const std::string & name
			) const{

	// prepare:
	scalarList x(steps);
	scalarList y(steps);

	// fill:
	forAll(x,n){
		x[n] = scalar(n) / scalar(steps - 1);
		y[n] = (*this)(x[n]);
	}

	return ScalarRFunction
	(
			x,
			y,
			interpolOrder,
			name
	);
}


// ************************************************************************* //

} // End namespace oldev

} // End namespace Foam
