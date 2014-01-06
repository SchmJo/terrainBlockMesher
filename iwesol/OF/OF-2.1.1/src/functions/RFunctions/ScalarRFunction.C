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

#include "ScalarRFunction.H"
#include "fvCFD.H"
#include "Plot.h"

namespace Foam
{

namespace oldev
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

ScalarRFunction::ScalarRFunction(
			const dictionary & dict
			):
			jlib::RRFunction
			(
					readLabel(dict.lookup("interpolOrder")),
					word(dict.lookup("name"))
			){

	// read dictionary:
	scalarListList xy(dict.lookup("data"));
	forAll(xy,dataI){
		add(xy[dataI][0],xy[dataI][1]);
	}

}

ScalarRFunction::ScalarRFunction(
		int interpolOrder,
		const std::string & name
		):
		jlib::RRFunction(
				interpolOrder,
				name
				){
}

ScalarRFunction::ScalarRFunction(
		const scalarList & xvals,
		const scalarList & yvals,
		int interpolOrder,
		const std::string & name
		):
		jlib::RRFunction(
				interpolOrder,
				 name
				){
	forAll(xvals,xI){
		add(xvals[xI],yvals[xI]);
	}
}

// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

void ScalarRFunction::writePDF(
		const std::string & resultsFolder,
		const std::string & xLabel,
		const std::string & yLabel,
		const std::string & legend,
		label points
		) const {
	mkDir(resultsFolder);
	setPlotPoints(points);
	jlib::Plot plot
	(
			name(),
			resultsFolder,
			xLabel,
			yLabel
	);
	plot.addLine(this,0,1,legend);
	plot.write();
}


// ************************************************************************* //

} // End namespace oldev

} // End namespace Foam
