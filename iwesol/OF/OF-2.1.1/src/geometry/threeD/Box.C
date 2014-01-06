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

\*---------------------------------------------------------------------------*/

#include "Box.H"
#include "addToRunTimeSelectionTable.H"
#include "Globals.H"

namespace Foam
{

namespace oldev
{

defineTypeNameAndDebug(Box, 0);
addToRunTimeSelectionTable(Object3D,Box,dict);

// * * * * * * * * * * * * * * * * Static data  * * * * * * * * * * * * * * //

const label Box::X = 0;
const label Box::Y = 1;
const label Box::Z = 2;

const label Box::FLB = 0;
const label Box::FRB = 1;
const label Box::BRB = 2;
const label Box::BLB = 3;
const label Box::FLT = 4;
const label Box::FRT = 5;
const label Box::BRT = 6;
const label Box::BLT = 7;

const label Box::FRONT = 0;
const label Box::RIGHT = 1;
const label Box::BACK  = 2;
const label Box::LEFT  = 3;
const label Box::BASE  = 4;
const label Box::TOP   = 5;


// * * * * * * * * * * * * * * * * Private member functions  * * * * * * *  * //

void Box::init(){

	// construct wall base points:
	points_[FRB] = chop(points_[FLB] + lengths_[X] * cooSys_.e(X));
	points_[BRB] = chop(points_[FRB] + lengths_[Y] * cooSys_.e(Y));
	points_[BLB] = chop(points_[FLB] + lengths_[Y] * cooSys_.e(Y));

	// construct wall top points:
	points_[FLT] = chop(points_[FLB] + lengths_[Z] * cooSys_.e(Z));
	points_[FRT] = chop(points_[FLT] + lengths_[X] * cooSys_.e(X));
	points_[BRT] = chop(points_[FRT] + lengths_[Y] * cooSys_.e(Y));
	points_[BLT] = chop(points_[FLT] + lengths_[Y] * cooSys_.e(Y));

	// construct faces:
	faces_[FRONT] = createFace
			(
					FLB,
					FRB,
					FRT,
					FLT
			);
	faces_[RIGHT] = createFace
			(
					FRB,
					BRB,
					BRT,
					FRT
			);
	faces_[BACK]  = createFace
			(
					BRB,
					BLB,
					BLT,
					BRT
			);
	faces_[LEFT]  = createFace
			(
					BLB,
					FLB,
					FLT,
					BLT
			);
	faces_[BASE]  = createFace
			(
					FLB,
					BLB,
					BRB,
					FRB
			);
	faces_[TOP] = createFace
			(
					FLT,
					FRT,
					BRT,
					BLT
			);

	// calc face centres:
	forAll(faces_,fI){
		faceCentres_[fI] = faces_[fI].centre(points_);
	}

	// calc face area vectors:
	forAll(faceAreaVectors_,fI){
		faceAreaVectors_[fI] = faces_[fI].normal(points_);
	}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Box::Box
(
			scalar resolution,
			label obstacleType
):
		Object3D
		(
				resolution,
				obstacleType
		),
		FaceConstructed
		(
				resolution
		){
}

Box::Box
(
		const dictionary & dict
):
		Object3D
		(
				dict
		),
		FaceConstructed
		(
				dict
		),
		cooSys_(dict.subDict("coordinates")),
		lengths_(dict.lookup("sideLengthList")),
		points_(8),
		faces_(6),
		faceCentres_(6),
		faceAreaVectors_(6){

	points_[FLB] = chop(cooSys_.origin());
	init();
}

Box::Box
(
			const CoordinateSystem & cooSys,
			const scalarList & sideLengths,
			scalar resolution,
			label obstacleType
):
		Object3D
		(
				resolution,
				obstacleType
		),
		FaceConstructed
		(
				resolution
		),
		cooSys_(cooSys),
		lengths_(sideLengths),
		points_(8),
		faces_(6),
		faceCentres_(6),
		faceAreaVectors_(6){

	points_[FLB] = chop(cooSys_.origin());
	init();
}

Box::Box
(
			const CoordinateSystem & cooSys,
			const scalar & lengthX,
			const scalar & lengthY,
			const scalar & lengthZ,
			scalar resolution,
			label obstacleType
):
		Object3D
		(
				resolution,
				obstacleType
		),
		FaceConstructed
		(
				resolution
		),
		cooSys_(cooSys),
		lengths_(3),
		points_(8),
		faces_(6),
		faceCentres_(6),
		faceAreaVectors_(6){
	lengths_[0] = lengthX;
	lengths_[1] = lengthY;
	lengths_[2] = lengthZ;
	points_[FLB] = chop(cooSys_.origin());
	init();
}

// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

List< List< List< Box > > > Box::subBoxes(
		const labelList & divisionsXYZ
		) const{

	// prepare:
	List< List< List< Box > > > out
	(
			divisionsXYZ[0],
			List< List< Box > >
			(
					divisionsXYZ[1],
					List< Box >
					(
							divisionsXYZ[2]
					)
			)
	);
	scalarList delta(3);
	forAll(delta,dirI) {
		delta[dirI] = lengths_[dirI] / scalar(divisionsXYZ[dirI]);
	}

	// create sub boxes:
	for(label X = 0; X < divisionsXYZ[0]; X++){
		for(label Y = 0; Y < divisionsXYZ[1]; Y++){
			for(label Z = 0; Z < divisionsXYZ[2]; Z++){

				point pstart = points_[FLB]
				               + X * delta[0] * cooSys_.e(0)
				               + Y * delta[1] * cooSys_.e(1)
				               + Z * delta[2] * cooSys_.e(2);

				//Info << "XYZ " << point(X,Y,Z) << ": pstart = " << pstart << ", delta = " << delta << endl;

				CoordinateSystem cs
				(
						pstart,
						cooSys_.axes()
				);
				out[X][Y][Z] = Box
						(
								cs,
								delta,
								resolution()
						);
			}
		}
	}

	return out;
}


// * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * * //



// ************************************************************************* //

} // End namespace oldev

} // End namespace Foam
