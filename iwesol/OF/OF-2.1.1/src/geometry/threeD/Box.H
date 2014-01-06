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
    Foam::oldev::Box

Description

SourceFiles
    BoxI.H
    Box.C
    BoxIO.C

\*---------------------------------------------------------------------------*/

#ifndef Box_H
#define Box_H

#include "Object3D.H"
#include "FaceConstructed.H"
#include "CoordinateSystem.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

namespace oldev
{

/*---------------------------------------------------------------------------*\
                         Class Box Declaration
\*---------------------------------------------------------------------------*/

class Box
:
	public Object3D,
	public FaceConstructed
{
    // Private data

    	/// the coordinate system, defines wall directions
    	CoordinateSystem cooSys_;

    	/// length axes 1,2,3
    	scalarList lengths_;

    	/// the point list
    	pointField points_;

    	/// the face list
    	faceList faces_;

    	/// the face centres
    	pointField faceCentres_;

    	/// the face area vectors
    	vectorField faceAreaVectors_;


    // Private Member Functions

    	/// called by constructor
    	void init();


public:

    // The OF run time selection type name
    TypeName("Box");

    // Static data members

		/// directions
		static const label X, Y, Z;

		/// corner point conventions: front-left-base etc.
		static const label FLB, FRB, BRB, BLB, FLT, FRT, BRT, BLT;

		/// wall side conventions
		static const label FRONT, RIGHT, BACK, LEFT, BASE, TOP;


    // Constructors

		/// Constructor
		Box
		(
				scalar resolution = 0.001,
				label obstacleType = Obstacle::NONE
		);

		/// Constructor
		Box
		(
			const dictionary & dict
		);

		/// Constructor
		Box
		(
				const CoordinateSystem & cooSys,
				const scalarList & sideLengths,
				scalar resolution = 0.001,
				label obstacleType = Obstacle::NONE
		);

		/// Constructor
		Box
		(
				const CoordinateSystem & cooSys,
				const scalar & lengthX,
				const scalar & lengthY,
				const scalar & lengthZ,
				scalar resolution = 0.001,
				label obstacleType = Obstacle::NONE
		);


    // Member Functions

        // Access

		/// Object3D: returns a surrounding box
		inline const Box & surroundingBox() const { return *this; }

		/// returns resolution
		inline const scalar & resolution() const { return Object3D::resolution(); }

		/// Obstacle: Returns true if the point lies inside/beside
		inline bool isInside(const point & p) const { return FaceConstructed::isInside(p); }

		/// Obstacle: Returns true if the point lies on the boundary
		inline bool isBoundary(const point & p) const { return FaceConstructed::isBoundary(p); }

		/// returns entries -1,0,1 for x,y,z, telling of point left,not out,right of boundary
		inline labelList isOutsideBox(const point & p) const;

		// FaceConstructed: return faces
		inline const faceList & faces() const { return faces_; }

		/// FaceConstructed: return points
		inline const pointField & points() const { return points_; }

		/// FaceConstructed: return start face in face list
		inline label startFaceIndex() const { return 0; }

		/// FaceConstructed: return number of faces
		inline label nFaces() const { return faces_.size(); }

		/// FaceConstructed: return face centres
		inline const pointField & Cf() const { return faceCentres_; }

		/// FaceConstructed: return face area vectors
		inline const vectorField & Sf() const { return faceAreaVectors_; }

		/// returns coordinate system
		const CoordinateSystem & coordinateSystem() const { return cooSys_; }

		/// returns centre point
		inline const point centre() const;

		/// returns the volume
		inline scalar volume() const { return lengths_[0] * lengths_[1] * lengths_[2]; }

		/// returns min point
		inline const point & pMin() const { return points_[FLB]; }

		/// returns max point
		inline const point & pMax() const { return points_[BRT]; }

		/// returns the side lengths
		inline const scalarList & lengths() const { return lengths_; }

		/// returns sub boxes. out[X][Y][Z] is a Box.
		List< List< List< Box > > > subBoxes(const labelList & divisionsXYZ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace oldev

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "BoxI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
