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
    Foam::oldev::GroundObject

Description

SourceFiles
    GroundObjectI.H
    GroundObject.C
    GroundObjectIO.C

\*---------------------------------------------------------------------------*/

#ifndef GroundObject_H
#define GroundObject_H

#include "PointLinePath.H"
#include "runTimeSelectionTables.H"
#include "Plot.h"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

namespace oldev
{

/*---------------------------------------------------------------------------*\
                         Class GroundObject Declaration
\*---------------------------------------------------------------------------*/

class GroundObject
{
    // Private data

    	/// The add type
    	word addType_;

    	/// The resolution
    	scalar resolution_;


public:

    // run time selection macro
    declareRunTimeSelectionTable
    (
        autoPtr,
        GroundObject,
        dict,
        (
        		const dictionary & dict
        ),
        (dict)
    );


    // Constructors

    	/// Constructor
    	GroundObject
    	(
    			const word & addType = "add",
    			const scalar & resolution = 0.001
    	);

    	/// Constructor
    	GroundObject
    	(
    			const dictionary & dict
    	);


    // Selectors

        /// Selector
    	static autoPtr< GroundObject > New
    	(
    			const dictionary & dict
    	);


    /// Destructor
    virtual ~GroundObject();


    // Member Functions

        // Access

    	/// GroundObject: Returns height at point (here this must me in the same plane, no projection)
    	virtual scalar height(const point & p) const { return 0; }

    	/// GroundObject: Returns true if point is inside
    	virtual bool isInside(const point & p) const { return height(p) != 0; }

    	/// GroundObject: Returns 2D ground circumference plot
    	virtual jlib::Plot boundaryPlot
    	(
    			const word & plotName,
    			const fileName & folder
    	) const { return jlib::Plot(plotName,folder); }

    	/// GroundObject: returns dictionary
    	virtual dictionary exportDict() const;

    	/// Returns the add type
    	inline const word & addType() const { return addType_; }

    	/// returns resolution
    	inline const scalar & resolution() const { return resolution_; }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace oldev

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
