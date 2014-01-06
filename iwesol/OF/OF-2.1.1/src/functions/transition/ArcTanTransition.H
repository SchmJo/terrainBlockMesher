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
    Foam::oldev::ArcTanTransition

Description

SourceFiles
    ArcTanTransition.C

\*---------------------------------------------------------------------------*/

#ifndef ArcTanTransition_H
#define ArcTanTransition_H

#include "TransitionFunction.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

namespace oldev
{

/*---------------------------------------------------------------------------*\
                         Class ArcTanTransition Declaration
\*---------------------------------------------------------------------------*/

class ArcTanTransition
:
    public TransitionFunction
{

    // Private member data

    	/// the centre
    	scalar s0_;

    	/// the argument multiplicator
    	scalar argMult_;

    	/// the core begin
    	scalar coreLow_;

    	/// the core end
    	scalar coreHigh_;


public:

    // The OF run time selection type name
    TypeName("arctan");


    // Constructors

        /// Construct null
        ArcTanTransition();

    	/// Constructor
    	ArcTanTransition
    	(
    			const dictionary & dict
    	);


    // Member Functions

        // Access

    	/// TransitionFunction: the function [0,1] -> R, with f(0) = 0, f(1) = 1
    	scalar operator()(const scalar & s) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace oldev

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
