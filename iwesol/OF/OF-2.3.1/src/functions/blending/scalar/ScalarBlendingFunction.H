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
    Foam::oldev::ScalarBlendingFunction

Description

SourceFiles
    ScalarBlendingFunction.C

\*---------------------------------------------------------------------------*/

#ifndef ScalarBlendingFunction_H
#define ScalarBlendingFunction_H

#include "BlendingFunction.H"
#include "runTimeSelectionTables.H"
#include "dictionary.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

namespace oldev
{

/*---------------------------------------------------------------------------*\
                         Class ScalarBlendingFunction Declaration
\*---------------------------------------------------------------------------*/

class ScalarBlendingFunction
:
    public BlendingFunction< scalar >
{
public:

    /// run time selection macro
    declareRunTimeSelectionTable
    (
    		autoPtr,
    		ScalarBlendingFunction,
    		type,
    		(const word & transitionType),
    		(transitionType)
    );

    /// run time selection macro
    declareRunTimeSelectionTable
    (
    		autoPtr,
    		ScalarBlendingFunction,
    		dict,
    		(const dictionary & dict),
    		(dict)
    );


    // Constructors

    	/// Constructor
    	ScalarBlendingFunction
    	(
    			const word & transitionType = "linear"
    	);

    	/// Constructor
    	ScalarBlendingFunction
    	(
    			const dictionary & dict
    	);

    // Selectors

    	/// selector
    	static autoPtr< ScalarBlendingFunction > New
    	(
    			const word & scalarBlendingType,
    			const word & transitionType
    	);

    	/// selector
    	static autoPtr< ScalarBlendingFunction > New
    	(
    			const dictionary & dict
    	);


    // Member Functions

        // Access

    	/// exports s transformation as scalar R function
    	ScalarRFunction exportSTransformation(
    			const point & point0,
    			const point & point1,
    			const scalar & data0,
    			const scalar & data1,
    			label steps,
    			label interpolOrder = 4,
    			const std::string & name = "sTransformation"
    			) const;

    	/// exports blending as scalar R function
    	ScalarRFunction exportBlending(
    			const point & point0,
    			const point & point1,
    			const scalar & data0,
    			const scalar & data1,
    			label steps,
    			label interpolOrder = 4,
    			const std::string & name = "blending"
    			) const;


        // Write

    	/// writes PDFs
    	void writePDFs(
    			const word & baseName,
    			const word & resultsFolder,
    			const point & point0,
    			const point & point1,
    			const scalar & data0,
    			const scalar & data1,
    			label steps,
    			label stepsPlot = 0,
    			label interpolOrder = 4
    			) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace oldev

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
