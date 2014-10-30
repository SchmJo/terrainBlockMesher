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
    Foam::oldev::STLLandscape

Description

SourceFiles
    STLLandscapeI.H
    STLLandscape.C
    STLLandscapeIO.C

\*---------------------------------------------------------------------------*/

#ifndef STLLandscape_H
#define STLLandscape_H

#include "STLProjecting.H"
#include "Globals.H"
#include "ScalarBlendingFunction.H"
#include "Box.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

namespace oldev
{

/*---------------------------------------------------------------------------*\
                         Class STLLandscape Declaration
\*---------------------------------------------------------------------------*/

class STLLandscape
:
    public STLProjecting
{
    // Private data

    	/// the domain box
    	Box const * dBox_;

    	/// the stl box
    	Box const * sBox_;

    	/// the zero level
    	scalar zeroLevel_;

    	/// the blending function
    	const ScalarBlendingFunction * blending_;


public:

    // Constructors

        /// Construct from components
        STLLandscape
        (
    			searchableSurface const * stl = 0,
    			const ScalarBlendingFunction * blending = 0,
    			Box const * domainBox = 0,
    			Box const * stlBox = 0,
    			scalar zeroLevel = 0
        );

    // Member Functions

        // Access

    	/// return domain box
    	inline const Box & domainBox() const { return *dBox_; }

    	/// return stl box
    	inline const Box & stlBox() const { return *sBox_; }

    	/// checks if point is inside
    	inline bool isInside(const point & p) const { return dBox_->isInside(p); }

    	/// checks if point is inside stl
    	inline bool isInsideSTL(const point & p) const  { return sBox_->isInside(p); }

    	/// gets nearest point of outside box and stl. returns success.
    	bool getNearestPoints(
    			const point & p,
    			point & p_boundary,
    			point & p_stl
    			) const;


        // Edit

    	/// attach a point to stl. returns success.
    	bool attachPoint(point & p, const point & p_projectTo) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace oldev

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
