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
    Foam::oldev::ConvexPolygon

Description

SourceFiles
    ConvexPolygonI.H
    ConvexPolygon.C
    ConvexPolygonIO.C

\*---------------------------------------------------------------------------*/

#ifndef ConvexPolygon_H
#define ConvexPolygon_H

#include "OptimizerGroundObject.H"
#include "Plotable.h"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

namespace oldev
{

/*---------------------------------------------------------------------------*\
                         Class ConvexPolygon Declaration
\*---------------------------------------------------------------------------*/

class ConvexPolygon
:
	public OptimizerGroundObject,
	public jlib::Plotable
{
    // Private data

    	/// The points
    	pointField points_;

    	/// a point inside
    	point p_inside_;

    	/// the resolution
    	scalar res_;

    	/// The height
    	scalar height_;

    	/// The max distance
    	scalar maxDist_;

    	/// the lines
    	vectorField linesf_;

    	/// the sign of the inside point
    	scalarField insideSigns_;


    // Private Member Functions

    	/// called by constructors
    	void init();


public:

    // The OF run time selection type name
    TypeName("convexPolygon");


    // Constructors

    	/// Constructor
    	ConvexPolygon
    	(
    			const pointField & points,
    			const point & insidePoint,
    			const scalar & resolution = 0.001,
    			const scalar & height = 0,
    			const word & addType = "add"
    	);

    	/// Constructor
    	ConvexPolygon(const dictionary & dict);

    	/// Constructor
    	ConvexPolygon(){}


    // Member Functions

        // Access

    	/// OptimizationEvaluator: Returns the number of geometric constraints
    	inline label nrConstraints() const { return size(); }

    	/// OptimizerGroundObject: returns scale of constraints
    	scalarList calcConstraintScales() const;

    	/// GroundObject: returns dictionary
    	dictionary exportDict() const;

    	/// GroundObject: Returns height at point (here this must me in the same plane, no projection)
    	scalar getHeight(const point & p) const { return isInside(p) ? height_ : 0; }

    	/// GroundObject: Returns 2D ground circumference plot
    	jlib::Plot boundaryPlot
    	(
    			const word & plotName,
    			const fileName & folder
    	) const;

    	/// Plotable: Returns the number of columns
    	inline int getPlotColumns() const { return 3; }

    	/// Plotable: Returns columns no. i as vector of doubles
    	std::vector<double> getPlotColumn(int i) const;

    	/// Plotable: Returns label of columns no. i
    	inline std::string getPlotColumnLabel(int i) const;

    	/// return a point inside
    	const point & getInsidePoint() const { return p_inside_; }

    	/// returns resolution
    	const scalar & resolution() const { return res_; }

    	/// returns the lines
    	const vectorField & lines() const { return linesf_; }

    	/// the size
    	label size() const { return points_.size(); }

    	/// returns the maximal distance
    	scalar getMaxDistance() const { return maxDist_; }


        // Edit

    	/// OptimizerGroundObject: Calculate constraint number i. return success.
    	bool calcPointConstraint
    	(
    			label lineI,
    			const point & p,
    			scalar & constraintValue
    	) const;


    // Member Operators

    	/// returns a point
    	point & operator[](label i) { return points_[i]; }

    	/// returns a point
    	const point & operator[](label i) const { return points_[i]; }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace oldev

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "ConvexPolygonI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
