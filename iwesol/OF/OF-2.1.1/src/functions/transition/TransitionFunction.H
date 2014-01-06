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
    Foam::oldev::TransitionFunction

Description

SourceFiles
    TransitionFunction.C

\*---------------------------------------------------------------------------*/

#ifndef TransitionFunction_H
#define TransitionFunction_H

#include "scalar.H"
#include "runTimeSelectionTables.H"
#include "dictionary.H"
#include "ScalarRFunction.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

namespace oldev
{

/*---------------------------------------------------------------------------*\
                         Class TransitionFunction Declaration
\*---------------------------------------------------------------------------*/

class TransitionFunction
{
public:

    /// run time selection macro
    declareRunTimeSelectionTable
    (
    	autoPtr,
    	TransitionFunction,
    	type,
    		(),
    		()
    	);

    /// run time selection macro
    declareRunTimeSelectionTable
    (
    	autoPtr,
    	TransitionFunction,
    	dict,
    	(const dictionary & dict),
    	(dict)
    );

    // Constructors

        /// Construct null
        TransitionFunction();


    // Selectors

    	/// selector
    	static autoPtr< TransitionFunction > New
    	(
    			const word & type
    	);

    	/// selector
    	static autoPtr< TransitionFunction > New
    	(
    			const dictionary & dict
    	);


    /// Destructor
    virtual ~TransitionFunction();


    // Member Functions

        // Access

		/// TransitionFunction: the function [0,1] -> R, with f(0) = 0, f(1) = 1
		virtual scalar operator()(const scalar & s) const = 0;

		/// exports scalar R function
		ScalarRFunction scalarRFunction
		(
			label steps,
			int interpolOrder = 4,
			const std::string & name = "TransitionFunction"
		) const;


        // Check

		/// checks if f(0) = 0 and f(1) = 1
		inline bool check() const { return (*this)(0) == 0 && (*this)(1) == 0; }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace oldev

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
