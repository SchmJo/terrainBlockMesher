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

#include "OvalKMHill.H"
#include "Globals.H"

#include "mathematicalConstants.H"

namespace Foam{
namespace iwesol{


OvalKMHill::OvalKMHill(const dictionary & dict):
	KasmiMassonHill(dict){

	b           = readScalar(dict.lookup("coRadius"));
	n_length    = Foam::vector(dict.lookup("direction_l"));
	n_length[2] = 0;
	n_width     = Foam::vector(-n_length[1],n_length[0],0);
	normalize(n_length);
	normalize(n_width);

}

OvalKMHill::~OvalKMHill() {
}

KasmiMassonHill & OvalKMHill::getKMHill(const scalar & angleRad){

	// prepare:
	const scalar & pi = Foam::constant::mathematical::pi;

	// reduce to angle resolution:
	const scalar angleres = resolution / (a > b ? a : b);
	const label anglen    = angleRad / angleres;
	const scalar hAngle   = anglen * angleres;

	// check if known (zero angle has been calculated in constructor):
	const word k = hkey(hAngle);
	if(k.compare(hkey(0)) == 0) return *this;
	if(angleHills.found(k)) return angleHills[k];

	// reduce to first sector (angles between 0 and pi/2):
	if(hAngle < 0)  return getKMHill(angleRad + 2. * pi);
	if(hAngle > pi) return getKMHill(angleRad - pi);
	if(hAngle > pi/2) return getKMHill(pi/2 - (angleRad - pi/2));

	// calculate length at given angle:
	scalar hlength = 0;
	if(hAngle == 0){
		hlength = a;
	} else if(mag(hAngle - pi/2) < angleres){
		hlength = b;
	} else {
		const scalar l1  = b;
		const scalar l2  = a;
		const scalar l1c = l1 * Foam::cos(hAngle);
		const scalar l2s = l2 * Foam::sin(hAngle);
		hlength    = l1 * l2 / Foam::sqrt( l1c * l1c + l2s * l2s );
	}

	// calculate new hill:
	KasmiMassonHill kmh(
				p_center,
				maxHeight,
				hlength,
				addType,
				resolution);

	// store:
	angleHills.set(k,kmh);

	return angleHills[k];
}

scalar OvalKMHill::getHeight(const point & p){

	point q    = p - p_center;
	scalar nq  = mag(q);
	scalar ang = nq > blib::CHOP ? getAngleRad(q,n_length) : 0.;
	return getHeight(ang,nq);
}

} /* iwesol */
} /* Foam */
