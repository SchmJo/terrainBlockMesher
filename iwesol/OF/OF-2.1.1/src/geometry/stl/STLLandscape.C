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

#include "STLLandscape.H"
#include "mathematicalConstants.H"
#include "Globals.H"

namespace Foam
{

namespace oldev
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

STLLandscape::STLLandscape(
		searchableSurface const * stl,
		const ScalarBlendingFunction * blending_,
		Box const * dBox_,
		Box const * sBox_,
		scalar zeroLevel_
		):
	STLProjecting(stl),
	dBox_(dBox_),
	sBox_(sBox_),
	zeroLevel_(zeroLevel_),
	blending_(blending_){
}



// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

bool STLLandscape::getNearestPoints(
		const point & p,
		point & p_boundary,
		point & p_stl
		) const{

	// prepare:
	const point & p_SWL       = dBox_->points()[Box::FLB];
	const point & p_SWL_stl   = sBox_->points()[Box::FLB];
	const point c2c_delta_SWL = p_SWL - p_SWL_stl;
	const point & p_NEL       = dBox_->points()[Box::BRB];
	const point & p_NEL_stl   = sBox_->points()[Box::BRB];
	const point c2c_delta_NEL = p_NEL - p_NEL_stl;
	const point & p_SEL       = dBox_->points()[Box::FRB];
	const point & p_SEL_stl   = sBox_->points()[Box::FRB];
	const point c2c_delta_SEL = p_SEL - p_SEL_stl;
	const point & p_NWL       = dBox_->points()[Box::BLB];
	const point & p_NWL_stl   = sBox_->points()[Box::BLB];
	const point c2c_delta_NWL = p_NWL - p_NWL_stl;

	// get delta vectors:
	point c_delta     = dBox_->coordinateSystem().point2coord(p);
	point c_delta_stl = sBox_->coordinateSystem().point2coord(p);

	// check if outside box:
	for(label i = 0; i < 2; i++){
		if(mag(c_delta[i]) > dBox_->resolution()){
			if(c_delta[i] < 0 ) return false;
		}
		scalar ldist = dBox_->lengths()[i] - c_delta[i];
		if(mag(ldist) > dBox_->resolution()){
			if(ldist < 0 ) return false;
		}
	}

	// check if outside stl box:
	labelList isOut = sBox_->isOutsideBox(p);
	if(!isOut[0] && !isOut[1]) return false;

	// prepare:
	const scalar maxrad_SWL = getAngleRad(-dBox_->coordinateSystem().e(0),c2c_delta_SWL / mag(c2c_delta_SWL));
	const scalar maxrad_SEL = getAngleRad(dBox_->coordinateSystem().e(0),c2c_delta_SEL / mag(c2c_delta_SEL));
	const scalar maxrad_NWL = getAngleRad(-dBox_->coordinateSystem().e(0),c2c_delta_NWL / mag(c2c_delta_NWL));
	const scalar maxrad_NEL = getAngleRad(dBox_->coordinateSystem().e(0),c2c_delta_NEL / mag(c2c_delta_NEL));

	// calc neighbors:
	if(isOut[0] < 0 && !isOut[1]){
		p_boundary = p_SWL     + c_delta[1] * dBox_->coordinateSystem().e(1)     + c_delta[2] * dBox_->coordinateSystem().e(2);
		p_stl      = p_SWL_stl + c_delta_stl[1] * sBox_->coordinateSystem().e(1) + c_delta_stl[2] * sBox_->coordinateSystem().e(2);
	}
	if(isOut[1] < 0 && !isOut[0]){
		p_boundary = p_SWL     + c_delta[0] * dBox_->coordinateSystem().e(0)     + c_delta[2] * dBox_->coordinateSystem().e(2);
		p_stl      = p_SWL_stl + c_delta_stl[0] * sBox_->coordinateSystem().e(0) + c_delta_stl[2] * sBox_->coordinateSystem().e(2);
	}
	//
	if(isOut[0] > 0 && !isOut[1]){
		p_boundary = p_SWL     + c_delta[1] * dBox_->coordinateSystem().e(1)     + c_delta[2] * dBox_->coordinateSystem().e(2)     + dBox_->lengths()[0] * dBox_->coordinateSystem().e(0);
		p_stl      = p_SWL_stl + c_delta_stl[1] * sBox_->coordinateSystem().e(1) + c_delta_stl[2] * sBox_->coordinateSystem().e(2) + sBox_->lengths()[0] * dBox_->coordinateSystem().e(0);
	}
	if(isOut[1] > 0 && !isOut[0]){
		p_boundary = p_SWL     + c_delta[0] * dBox_->coordinateSystem().e(0)     + c_delta[2] * dBox_->coordinateSystem().e(2)     + dBox_->lengths()[1] * dBox_->coordinateSystem().e(1);
		p_stl      = p_SWL_stl + c_delta_stl[0] * sBox_->coordinateSystem().e(0) + c_delta_stl[2] * sBox_->coordinateSystem().e(2) + sBox_->lengths()[1] * dBox_->coordinateSystem().e(1) ;
	}
	//
	if(isOut[0] < 0 && isOut[1] < 0){
		p_stl          = p_SWL_stl + c_delta_stl[2] * sBox_->coordinateSystem().e(2);
		point p_SWLh   = p_SWL + c_delta[2] * dBox_->coordinateSystem().e(2);
		Foam::vector n = p - p_stl;
		n             /= mag(n);

		scalar angleRad = getAngleRad(-dBox_->coordinateSystem().e(0),n);
		if(angleRad <= maxrad_SWL){
			scalar d   = mag(p_SWLh - p_stl) * Foam::cos(maxrad_SWL);
			point q    = p_stl - d * sBox_->coordinateSystem().e(0);
			p_boundary = q - d * Foam::tan(angleRad) * dBox_->coordinateSystem().e(1);

		} else {
			//angleRad   = getAngleRad(-dBox_->coordinateSystem().e(1),n);
			scalar d   = mag(p_SWLh - p_stl) * Foam::sin(maxrad_SWL);
			point q    = p_stl - d * sBox_->coordinateSystem().e(1);
			p_boundary = q - d / Foam::tan(angleRad) * dBox_->coordinateSystem().e(0);
		}
	}
	//
	else if(isOut[0] > 0 && isOut[1] < 0){
		p_stl          = p_SEL_stl + c_delta_stl[2] * sBox_->coordinateSystem().e(2);
		point p_SELh   = p_SEL + c_delta[2] * dBox_->coordinateSystem().e(2);
		Foam::vector n = p - p_stl;
		n             /= mag(n);

		scalar angleRad = getAngleRad(dBox_->coordinateSystem().e(0),n);
		if(angleRad <= maxrad_SEL){
			scalar d   = mag(p_SELh - p_stl) * Foam::cos(maxrad_SEL);
			point q    = p_stl + d * sBox_->coordinateSystem().e(0);
			p_boundary = q - d * Foam::tan(angleRad) * dBox_->coordinateSystem().e(1);
		} else {
			//angleRad   = getAngleRad(-dBox_->coordinateSystem().e(1),n);
			scalar d   = mag(p_SELh - p_stl) * Foam::sin(maxrad_SEL);
			point q    = p_stl - d * sBox_->coordinateSystem().e(1);
			p_boundary = q + d / Foam::tan(angleRad) * dBox_->coordinateSystem().e(0);
		}
	}
	//
	else if(isOut[0] > 0 && isOut[1] > 0){
		p_stl          = p_NEL_stl + c_delta_stl[2] * dBox_->coordinateSystem().e(2);
		point p_NELh   = p_NEL + c_delta[2] * dBox_->coordinateSystem().e(2);
		Foam::vector n = p - p_stl;
		n             /= mag(n);

		scalar angleRad = getAngleRad(dBox_->coordinateSystem().e(0),n);
		if(angleRad <= maxrad_NEL){
			scalar d   = mag(p_NELh - p_stl) * Foam::cos(maxrad_NEL);
			point q    = p_stl + d * sBox_->coordinateSystem().e(0);
			p_boundary = q + d * Foam::tan(angleRad) * dBox_->coordinateSystem().e(1);
		} else {
			//angleRad   = getAngleRad(-dBox_->coordinateSystem().e(1),n);
			scalar d   = mag(p_NELh - p_stl) * Foam::sin(maxrad_NEL);
			point q    = p_stl + d * sBox_->coordinateSystem().e(1);
			p_boundary = q + d / Foam::tan(angleRad) * dBox_->coordinateSystem().e(0);
		}
	}
	//
	else if(isOut[0] < 0 && isOut[1] > 0){
		p_stl          = p_NWL_stl + c_delta_stl[2] * sBox_->coordinateSystem().e(2);
		point p_NWLh   = p_NWL + c_delta[2] * dBox_->coordinateSystem().e(2);
		Foam::vector n = p - p_stl;
		n             /= mag(n);

		scalar angleRad = getAngleRad(-dBox_->coordinateSystem().e(0),n);
		if(angleRad <= maxrad_NWL){
			scalar d   = mag(p_NWLh - p_stl) * Foam::cos(maxrad_NWL);
			point q    = p_stl - d * sBox_->coordinateSystem().e(0);
			p_boundary = q + d * Foam::tan(angleRad) * dBox_->coordinateSystem().e(1);
		} else {
			//angleRad   = getAngleRad(-dBox_->coordinateSystem().e(1),n);
			scalar d   = mag(p_NWLh - p_stl) * Foam::sin(maxrad_NWL);
			point q    = p_stl + d * sBox_->coordinateSystem().e(1);
			p_boundary = q - d / Foam::tan(angleRad) * dBox_->coordinateSystem().e(0);
		}
	}

	return true;
}

bool STLLandscape::attachPoint(point & p, const point & p_projectTo) const{

	// get neighbor points at boundary or stl:
	point p_boundary(0,0,0);
	point p_stl(0,0,0);
	if(!getNearestPoints(p,p_boundary,p_stl)){

		// maybe the point is inside the stl, then do standard:
		return STLProjecting::attachPoint(p,p_projectTo);
	}

	// project p_stl:
	if
	(
			!STLProjecting::attachPoint
			(
					p_stl,
					p_stl + dot
					(
							p_projectTo - p_stl,
							sBox_->coordinateSystem().e(2)) * sBox_->coordinateSystem().e(2)
					)
	) return false;

	// interpolate:
	p_boundary       += ( zeroLevel_ - ( p_boundary & (dBox_->coordinateSystem().e(2)) ) ) * dBox_->coordinateSystem().e(2);

	//scalar height     = dot(p_stl - p_boundary, dBox_->coordinateSystem().e(2));
	point p_temp      = p - p_stl;
	p_temp[2]         = 0;
	scalar d_stl      = mag(p_temp);
	p_temp            = p_boundary - p_stl;
	p_temp[2]         = 0;
	scalar d_tot      = mag(p_temp);
	scalar s          = d_stl / d_tot;
	if(s > 1) s = 1; // this corrects a bug due to precision

	p[2] = blending_->blend
			(
					p_stl,
					p_boundary,
					p_stl[2],
					p_boundary[2],
					s
			);

	return true;
}


// * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * * //



// ************************************************************************* //

} // End namespace oldev

} // End namespace Foam
