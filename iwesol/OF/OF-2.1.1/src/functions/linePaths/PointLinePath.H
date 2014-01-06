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
    Foam::oldev::PointLinePath

Description

SourceFiles
    PointLinePath.C

\*---------------------------------------------------------------------------*/

#ifndef PointLinePath_H
#define PointLinePath_H

#include "Outputable.h"
#include "ObjectLinePath.H"
#include "Globals.H"
#include "PointDistance.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

namespace oldev
{

/*---------------------------------------------------------------------------*\
                         Class PointLinePath Declaration
\*---------------------------------------------------------------------------*/

class PointLinePath
:
    public ObjectLinePath< Foam::point, PointDistance >,
	public jlib::Outputable
{
    // Private data

    	/// the number of points for output, zero means as many as size
    	label outputPoints_;


public:


    // Constructors

    	/// Constructor
    	PointLinePath
    	(
    			const std::string & name = "PointLinePath",
    			int interpolOrder = 4
    	);

    	/// Constructor
    	PointLinePath
    	(
    			const Foam::point & p0,
    			const Foam::point & p1,
    			const std::string & name = "PointLinePath",
    			int interpolOrder = 4
    	);

    	/// Constructor
    	PointLinePath
    	(
    			const Foam::point & p,
    			const std::string & name = "PointLinePath",
    			int interpolOrder = 4
    	);

    	/// Constructor
    	PointLinePath
    	(
    			const pointField & points,
    			const std::string & name = "PointLinePath",
    			int interpolOrder = 4
    	);

    	/// Constructor
    	PointLinePath
    	(
    			const dictionary & dict,
    			const word & keyWord = "points"
    	);

    // Member Functions

        // Access

    	/// return out points
    	inline label outputPoints() const { return outputPoints_; }

    	/// Returns all points
    	inline pointField getPoints() const { return pointField(getList()); }


        // Edit

    	/// sets output point number, zero means as many as size
    	inline void setOutputPoints(label outputPoints) { outputPoints_ = outputPoints; }

    	/// smoothen strong derivatives, given relative tolerance
    	void smoothen(const scalar & tolerance);


        // Write

    	/// Outputable: Provides the data that will be written to the output file.
    	virtual jlib::Outputable::OData getOData
    	(
    			const std::string & type = jlib::IO::OFILE::TYPE::EMPTY
    	) const;

    	/// Writes vtk files
    	void writeVTK(const std::string & folderName = "") const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace oldev

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
