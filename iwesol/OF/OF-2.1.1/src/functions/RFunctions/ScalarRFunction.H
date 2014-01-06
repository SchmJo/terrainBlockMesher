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

Class
    Foam::oldev::ScalarRFunction

Description

SourceFiles
    ScalarRFunctionI.H
    ScalarRFunction.C
    ScalarRFunctionIO.C

\*---------------------------------------------------------------------------*/

#ifndef ScalarRFunction_H
#define ScalarRFunction_H

#include "dictionary.H"
#include "scalarList.H"
#include "RRFunction.h"
#include "Plotable.h"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

namespace oldev
{

/*---------------------------------------------------------------------------*\
                         Class ScalarRFunction Declaration
\*---------------------------------------------------------------------------*/

class ScalarRFunction
:
    public jlib::RRFunction
{
public:

    // Constructors

    	/// Constructor
    	ScalarRFunction
    	(
    			const dictionary & dict
    	);

    	/// Constructor
    	ScalarRFunction
    	(
    			int interpolOrder = 4,
    			const std::string & name = "ScalarRFunction"
    	);

    	/// Constructor
    	ScalarRFunction
    	(
    			const scalarList & xvals,
    			const scalarList & yvals,
    			int interpolOrder = 4,
    			const std::string & name = "ScalarRFunction"
    	);


    // Member Functions

        // Access

    	/// returns point list
    	inline scalarList getPointList() const;

    	/// returns data list
    	inline scalarList getDataList() const;


        // Write

    	/// write pdf output. zero points corresponds to data points.
    	void writePDF
    	(
    			const std::string & resultsFolder,
    			const std::string & xLabel,
    			const std::string & yLabel,
    			const std::string & legend,
    			label points = 0
    	) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace oldev

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
