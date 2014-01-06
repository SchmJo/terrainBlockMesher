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

#include "ScalarBlendingFunction.H"
#include "addToRunTimeSelectionTable.H"

namespace Foam
{

namespace oldev
{

defineRunTimeSelectionTable(ScalarBlendingFunction,type);
defineRunTimeSelectionTable(ScalarBlendingFunction,dict);


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

ScalarBlendingFunction::ScalarBlendingFunction(
		const word & transitionType
		):
		BlendingFunction< scalar >
		(
				transitionType
		){
}

ScalarBlendingFunction::ScalarBlendingFunction(
		const dictionary & dict
		):
		BlendingFunction< scalar >
		(
				dict
		){
}


// * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * * //

autoPtr< ScalarBlendingFunction > ScalarBlendingFunction::New(
		const word & type,
		const word & transitionType
		){

	Info<< "Selecting scalar blending function type '" << type << "'" << endl;

	typeConstructorTable::iterator cstrIter =
			typeConstructorTablePtr_->find(type);

	if (cstrIter == typeConstructorTablePtr_->end()){
	        FatalErrorIn
	        (
	             "ScalarBlendingFunction::New"
	             "("
	                 "const word&"
	             ")"
	        )   << "Unknown scalar blending function type '"
	            << type << "'" << nl << nl
	            << "Valid types:" << endl
	            << typeConstructorTablePtr_->sortedToc()
	            << exit(FatalError);
	}

	return autoPtr< ScalarBlendingFunction >(cstrIter()(transitionType));
}

autoPtr< ScalarBlendingFunction > ScalarBlendingFunction::New(
		const dictionary & dict
		){

	word type(dict.lookup("type"));
	Info<< "Selecting scalar blending function type '" << type << "'" << endl;

	dictConstructorTable::iterator cstrIter =
			dictConstructorTablePtr_->find(type);

	if (cstrIter == dictConstructorTablePtr_->end()){
	        FatalErrorIn
	        (
	             "ScalarBlendingFunction::New"
	             "("
	                 "const dictionary & dict"
	             ")"
	        )   << "Unknown scalar blending function type '"
	            << type << "'" << nl << nl
	            << "Valid types:" << endl
	            << dictConstructorTablePtr_->sortedToc()
	            << exit(FatalError);
	}

	return autoPtr< ScalarBlendingFunction >(cstrIter()(dict));
}



// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //


ScalarRFunction ScalarBlendingFunction::exportSTransformation(
			const point & point0,
			const point & point1,
			const scalar & data0,
			const scalar & data1,
			label steps,
			label interpolOrder,
			const std::string & name
			) const{

	// prepare:
	scalarList x(steps);
	scalarList y(steps);

	// fill:
	forAll(x,n){
		x[n] = scalar(n) / scalar(steps - 1);
		y[n] = x[n];
		sTransformation
		(
				point0,
				point1,
				data0,
				data1,
				y[n]
		);
	}

	return ScalarRFunction
	(
			x,
			y,
			interpolOrder,
			name
	);
}

ScalarRFunction ScalarBlendingFunction::exportBlending(
			const point & point0,
			const point & point1,
			const scalar & data0,
			const scalar & data1,
			label steps,
			label interpolOrder,
			const std::string & name
			) const{

	// prepare:
	scalarList x(steps);
	scalarList y(steps);

	// fill:
	forAll(x,n){
		scalar s = scalar(n) / scalar(steps - 1);
		x[n] = scalar(n) / scalar(steps - 1);
		y[n] = blend
		(
				point0,
				point1,
				data0,
				data1,
				s
		);
	}

	return ScalarRFunction
	(
			x,
			y,
			interpolOrder,
			name
	);
}

void ScalarBlendingFunction::writePDFs(
			const word & baseName,
			const word & resultsFolder,
			const point & p0,
			const point & p1,
			const scalar & d0,
			const scalar & d1,
			label steps,
			label stepsPlot,
			label interpolOrder
			) const{
	transition().scalarRFunction
			(
					steps,
					interpolOrder,
					baseName + "_dataTransitionFunction"
			).writePDF(
			resultsFolder,
			"s",
			"f(s)",
			baseName + " data transition function",
			stepsPlot
			);
	OStringStream oss;
	oss << p0 << " --> " << p1;
	exportSTransformation
			(
					p0,p1,d0,d1,
					steps,interpolOrder,
					baseName + "_sTransformation"
			).writePDF
			(
					"results",
					"s",
					"t(s)",
					"s trafo: " + oss.str(),
					stepsPlot
			);
	exportBlending
			(
					p0,p1,d0,d1,
					steps,interpolOrder,
					baseName + "_blending"
			).writePDF
			(
					"results",
					"s",
					"f(s)",
					"blending: " + oss.str(),
					stepsPlot
			);
}


// ************************************************************************* //

} // End namespace oldev

} // End namespace Foam
