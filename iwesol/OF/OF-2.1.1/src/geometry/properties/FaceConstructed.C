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

#include "FaceConstructed.H"
#include "STLsurfaceFormat.H"
#include "surfZone.H"

namespace Foam
{

namespace oldev
{


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

FaceConstructed::FaceConstructed(
		const double & resolution
		):
		resolution_(resolution){
}

FaceConstructed::FaceConstructed(
		const dictionary & dict
		):
		resolution_(readScalar(dict.lookup("resolution"))){
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

FaceConstructed::~FaceConstructed()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

void FaceConstructed::writeSTL(
		const fileName & file,
		bool binary
		) const{
	Info << "Writing file " << file << endl;
	List< surfZone > lsz
	(
			1,
			surfZone
			(
					"zone0",
					nFaces(),
					startFaceIndex(),
					0
			)
	);
	if(binary){
		Foam::fileFormats::STLsurfaceFormat< face >::writeBinary(
				file,
				MeshedSurfaceProxy< face >
				(
						points(),
						faces(),
						lsz
				)
			);
	} else {
		Foam::fileFormats::STLsurfaceFormat< face >::writeAscii(
				file,
				MeshedSurfaceProxy< face >
				(
						points(),
						faces(),
						lsz
				)
			);
	}
}

bool FaceConstructed::isInside(const point & p) const{
	for(label fi = 0; fi < nFaces(); fi++){
		scalar d = ( p - Cf()[fi] ) & ( Sf()[fi] / mag(Sf()[fi]) );
		if(mag(d) > resolution_){
			if( d >= 0 ) return false;
		} else {
			return false;
		}
	}
	return true;
}

bool FaceConstructed::isNotOutside(const point & p) const{
	for(label fi = 0; fi < nFaces(); fi++){
		scalar d = ( p - Cf()[fi] ) & ( Sf()[fi] / mag(Sf()[fi]) );
		if(mag(d) > resolution_){
			if( d > 0 ) return false;
		}
	}
	return true;
}

bool FaceConstructed::isBoundary(const point & p) const{
	bool cand = false;
	for(label fi = 0; fi < nFaces(); fi++){
		scalar proj  = ( p - Cf()[fi] ) & ( Sf()[fi] / mag(Sf()[fi]) );
		scalar mproj = mag(proj);
		if(mproj > resolution_){
			if( proj > 0 ) return false;
		} else {
			cand = true;
		}
	}
	return cand;
}



// ************************************************************************* //

} // End namespace oldev

} // End namespace Foam

