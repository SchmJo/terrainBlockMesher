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
    Foam::oldev::OptimizerGroundObject

Description

SourceFiles
    OptimizerGroundObjectI.H
    OptimizerGroundObject.C
    OptimizerGroundObjectIO.C

\*---------------------------------------------------------------------------*/

#ifndef OptimizerGroundObject_H
#define OptimizerGroundObject_H

#include "GroundObject.H"
#include "Box.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

namespace oldev
{

/*---------------------------------------------------------------------------*\
                         Class OptimizerGroundObject Declaration
\*---------------------------------------------------------------------------*/

class OptimizerGroundObject
:
    public GroundObject
{
    // Private data

    	/// the range box
    	Box box_;


    // Private Member Functions

        /// Disallow default bitwise copy construct
        OptimizerGroundObject(const OptimizerGroundObject&);

        /// Disallow default bitwise assignment
        void operator=(const OptimizerGroundObject&);


protected:

    // Member functions:

        /// initialize range
        inline void initRangeBox
        (
			const Box & rangeBox
		);


public:

    // run time selection macro
    declareRunTimeSelectionTable
    (
    	autoPtr,
    	OptimizerGroundObject,
    	dict,
    	(
    			const dictionary & dict
    	),
    	(dict)
    );


    // Constructors

    	/// Constructor
    	OptimizerGroundObject
    	(
    			const word & addType = "add",
    			const Box & rangeBox = Box(CoordinateSystem(),scalarList(3,scalar(1)))
    	);

    	/// Constructor
    	OptimizerGroundObject
    	(
    			const dictionary & dict,
    			const Box & rangeBox = Box(CoordinateSystem(),scalarList(3,scalar(1)))
    	);


    // Selectors

    	/// selector
    	static autoPtr< OptimizerGroundObject > New
    	(
    			const dictionary & dict
    	);


    /// Destructor
    virtual ~OptimizerGroundObject();


    // Member Functions

        // Access

    	/// OptimizerGroundObject: the number of constraints
    	virtual label nrConstraints() const = 0;

    	/// GroundObject: returns dictionary
    	virtual dictionary exportDict() const;

    	/// OptimizationEvaluator: Calculate constraint number i. return success.
    	inline bool calcConstraint
    	(
    			label i,
    			const UList< scalar > & parameters,
    			scalar & constraintValue
    	) const;

    	/// GroundObject: Returns true if point is inside
    	bool isInside(const point & p) const;

    	/// returns type name; automatically specified by TypeName-macro
    	virtual const word& type() const = 0;

    	/// returns range box
    	inline const Box & rangeBox() const { return box_; }

    	/// returns coordinate system
    	inline const CoordinateSystem & coordinateSystem() const { return box_.coordinateSystem(); }

    	/// return normalized point
    	inline point normalize(const point & p) const;

    	/// return un-normalized point
    	inline point unnormalize(const point & c) const;


        // Check

        // Edit

    	/// OptimizerGroundObject: Calculate constraint number i. return success.
    	virtual bool calcPointConstraint
    	(
    			label i,
    			const point & p,
    			scalar & constraintValue
    	) const = 0;

    	/// OptimizerGroundObject: returns scale of constraints
    	virtual scalarList calcConstraintScales() const = 0;

    	/// evaluate constraints for a point
    	inline bool calcPointConstraints
    	(
    			const point & p,
    			UList< scalar > & results
    	) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace oldev

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "OptimizerGroundObjectI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
