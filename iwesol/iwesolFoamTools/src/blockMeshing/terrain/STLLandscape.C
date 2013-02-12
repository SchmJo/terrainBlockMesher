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

\*---------------------------------------------------------------------------*/

#include "STLLandscape.H"
#include "mathematicalConstants.H"
#include "Globals.H"

namespace Foam{
namespace iwesol{

STLLandscape::STLLandscape(
		CoordinateSystem * cooSys,
		searchableSurface const * stl,
		const point & p_SWL,
		const scalarList & dimensions,
		const point & p_SWL_stl,
		const scalarList & dimensions_stl,
		scalar zeroLevel,
		scalar f_pref,
		scalar f_expo
		):
	STLProjecting(stl),
	HasCoordinateSystem(cooSys),
	p_SWL(p_SWL),
	p_SWL_stl(p_SWL_stl),
	dimensions(dimensions),
	dimensions_stl(dimensions_stl),
	zeroLevel(zeroLevel),
	f_A(f_pref),
	f_B(f_expo){
}

STLLandscape::~STLLandscape() {
}

bool STLLandscape::getNearestPoints(
		const point & p,
		point & p_boundary,
		point & p_stl
		) const{

	// prepare:
	const point c2c_delta_SWL = p_SWL - p_SWL_stl;
	const point p_NEL         = p_SWL + dimensions[0] * get_e(0) + dimensions[1] * get_e(1);
	const point p_NEL_stl     = p_SWL_stl + dimensions_stl[0] * get_e(0) + dimensions_stl[1] * get_e(1);
	const point c2c_delta_NEL = p_NEL - p_NEL_stl;
	const point p_SEL         = p_SWL + dimensions[0] * get_e(0);
	const point p_SEL_stl     = p_SWL_stl + dimensions_stl[0] * get_e(0);
	const point c2c_delta_SEL = p_SEL - p_SEL_stl;
	const point p_NWL         = p_SWL + dimensions[1] * get_e(1);
	const point p_NWL_stl     = p_SWL_stl + dimensions_stl[1] * get_e(1);
	const point c2c_delta_NWL = p_NWL - p_NWL_stl;

	// get delta vectors:
	const point c_delta     = chop(cooSys->point2coord(p - p_SWL));
	const point c_delta_stl = chop(cooSys->point2coord(p - p_SWL_stl));

	// check if outside box:
	for(label i = 0; i < 2; i++){
		if(c_delta[i] < 0 || chop(dimensions[i] - c_delta[i])  < 0 ) return false;
	}

	// check if outside stl box:
	labelList isOut(2);
	for(label i = 0; i < 2; i++){
		isOut[i] = ( c_delta_stl[i] >= 0 &&  chop(dimensions_stl[i] - c_delta_stl[i]) >= 0  ) ? 0 :
				 ( c_delta_stl[i] < 0 ? -1 : 1 );
	}
	if(!isOut[0] && !isOut[1]) return false;

	// prepare:
	const scalar maxrad_SWL = getAngleRad(-get_e(0),c2c_delta_SWL);
	const scalar maxrad_SEL = getAngleRad(get_e(0),c2c_delta_SEL);
	const scalar maxrad_NWL = getAngleRad(-get_e(0),c2c_delta_NWL);
	const scalar maxrad_NEL = getAngleRad(get_e(0),c2c_delta_NEL);

	// calc neighbors:
	if(isOut[0] < 0 && !isOut[1]){
		p_boundary = p_SWL     + c_delta[1] * get_e(1)     + c_delta[2] * get_e(2);
		p_stl      = p_SWL_stl + c_delta_stl[1] * get_e(1) + c_delta_stl[2] * get_e(2);
	}
	if(isOut[1] < 0 && !isOut[0]){
		p_boundary = p_SWL     + c_delta[0] * get_e(0)     + c_delta[2] * get_e(2);
		p_stl      = p_SWL_stl + c_delta_stl[0] * get_e(0) + c_delta_stl[2] * get_e(2);
	}
	//
	if(isOut[0] > 0 && !isOut[1]){
		p_boundary = p_SWL     + c_delta[1] * get_e(1)     + c_delta[2] * get_e(2)     + dimensions[0] * get_e(0);
		p_stl      = p_SWL_stl + c_delta_stl[1] * get_e(1) + c_delta_stl[2] * get_e(2) + dimensions_stl[0] * get_e(0);
	}
	if(isOut[1] > 0 && !isOut[0]){
		p_boundary = p_SWL     + c_delta[0] * get_e(0)     + c_delta[2] * get_e(2)     + dimensions[1] * get_e(1);
		p_stl      = p_SWL_stl + c_delta_stl[0] * get_e(0) + c_delta_stl[2] * get_e(2) + dimensions_stl[1] * get_e(1) ;
	}
	//
	if(isOut[0] < 0 && isOut[1] < 0){
		p_stl          = p_SWL_stl + c_delta_stl[2] * get_e(2);
		point p_SWLh   = p_SWL + c_delta[2] * get_e(2);
		Foam::vector n = p - p_stl;
		normalize(n);

		scalar angleRad = getAngleRad(-get_e(0),n);
		if(angleRad <= maxrad_SWL){
			scalar d   = mag(p_SWLh - p_stl) * Foam::cos(maxrad_SWL);
			point q    = p_stl - d * get_e(0);
			p_boundary = q - d * Foam::tan(angleRad) * get_e(1);

		} else {
			//angleRad   = getAngleRad(-get_e(1),n);
			scalar d   = mag(p_SWLh - p_stl) * Foam::sin(maxrad_SWL);
			point q    = p_stl - d * get_e(1);
			p_boundary = q - d / Foam::tan(angleRad) * get_e(0);
		}
	}
	//
	else if(isOut[0] > 0 && isOut[1] < 0){
		p_stl          = p_SEL_stl + c_delta_stl[2] * get_e(2);
		point p_SELh   = p_SEL + c_delta[2] * get_e(2);
		Foam::vector n = p - p_stl;
		normalize(n);

		scalar angleRad = getAngleRad(get_e(0),n);
		if(angleRad <= maxrad_SEL){
			scalar d   = mag(p_SELh - p_stl) * Foam::cos(maxrad_SEL);
			point q    = p_stl + d * get_e(0);
			p_boundary = q - d * Foam::tan(angleRad) * get_e(1);
		} else {
			//angleRad   = getAngleRad(-get_e(1),n);
			scalar d   = mag(p_SELh - p_stl) * Foam::sin(maxrad_SEL);
			point q    = p_stl - d * get_e(1);
			p_boundary = q + d / Foam::tan(angleRad) * get_e(0);
		}
	}
	//
	else if(isOut[0] > 0 && isOut[1] > 0){
		p_stl          = p_NEL_stl + c_delta_stl[2] * get_e(2);
		point p_NELh   = p_NEL + c_delta[2] * get_e(2);
		Foam::vector n = p - p_stl;
		normalize(n);

		scalar angleRad = getAngleRad(get_e(0),n);
		if(angleRad <= maxrad_NEL){
			scalar d   = mag(p_NELh - p_stl) * Foam::cos(maxrad_NEL);
			point q    = p_stl + d * get_e(0);
			p_boundary = q + d * Foam::tan(angleRad) * get_e(1);
		} else {
			//angleRad   = getAngleRad(-get_e(1),n);
			scalar d   = mag(p_NELh - p_stl) * Foam::sin(maxrad_NEL);
			point q    = p_stl + d * get_e(1);
			p_boundary = q + d / Foam::tan(angleRad) * get_e(0);
		}
	}
	//
	else if(isOut[0] < 0 && isOut[1] > 0){
		p_stl          = p_NWL_stl + c_delta_stl[2] * get_e(2);
		point p_NWLh   = p_NWL + c_delta[2] * get_e(2);
		Foam::vector n = p - p_stl;
		normalize(n);

		scalar angleRad = getAngleRad(-get_e(0),n);
		if(angleRad <= maxrad_NWL){
			scalar d   = mag(p_NWLh - p_stl) * Foam::cos(maxrad_NWL);
			point q    = p_stl - d * get_e(0);
			p_boundary = q + d * Foam::tan(angleRad) * get_e(1);
		} else {
			//angleRad   = getAngleRad(-get_e(1),n);
			scalar d   = mag(p_NWLh - p_stl) * Foam::sin(maxrad_NWL);
			point q    = p_stl + d * get_e(1);
			p_boundary = q - d / Foam::tan(angleRad) * get_e(0);
		}
	}
	return true;
}

bool STLLandscape::attachPoint(point & p, const point & p_projectTo){

	// get neighbor points at boundary or stl:
	point p_boundary(0,0,0);
	point p_stl(0,0,0);

	if(!getNearestPoints(p,p_boundary,p_stl)){
		// maybe the point is inside the stl, then do standard:
		return STLProjecting::attachPoint(p,p_projectTo);
	}

	// project p_stl:
	if(!STLProjecting::attachPoint(p_stl,p_stl + dot(p_projectTo - p_stl,get_e(2)) * get_e(2))) return false;

	// interpolate:
	p_boundary       += ( zeroLevel - dot(p_boundary,get_e(2)) ) * get_e(2);
	scalar height     = dot(p_stl - p_boundary, get_e(2));
	point p_temp      = p - p_stl;
	p_temp[2]         = 0;
	scalar d_stl      = mag(p_temp);
	p_temp            = p_boundary - p_stl;
	p_temp[2]         = 0;
	scalar d_tot      = mag(p_temp);
	scalar s          = d_stl / d_tot;
	if(s > 1) s = 1; // this corrects a bug due to precision
	p[2]              = zeroLevel + height * f_interpolate_terrain(s);

	return true;
}

scalar STLLandscape::f_interpolate_terrain(scalar s) const{

	if(s == 0) return 1;
	if(s == 1) return 0;

	scalar u = f_A * s / (s - 1.);
	scalar v = f_B * (1. - s) / s;

	return 0.5 + Foam::atan(u + v) / Foam::constant::mathematical::pi;
}


} /* iwesol */
} /* Foam */
