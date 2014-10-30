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
#include "addToRunTimeSelectionTable.H"

namespace Foam
{

namespace oldev
{

defineTypeNameAndDebug(OvalKMHill, 0);
addToRunTimeSelectionTable(GroundObject,OvalKMHill,dict);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

OvalKMHill::OvalKMHill
(
		const dictionary & dict
):
	KasmiMassonHill(dict){

	b_           = readScalar(dict.lookup("coRadius"));
	n_length_    = Foam::vector(dict.lookup("direction_l"));
	n_length_[2] = 0;
	n_width_     = Foam::vector(-n_length_[1],n_length_[0],0);
	normalize(n_length_);
	normalize(n_width_);

}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

const KasmiMassonHill & OvalKMHill::getKMHill(scalar angleRad) const{

	// prepare:
	const scalar & pi = Foam::constant::mathematical::pi;

	// reduce to angle resolution():
	const scalar angleres = resolution() / (radius() > b_ ? radius() : b_);
	const label anglen    = angleRad / angleres;
	const scalar hAngle   = anglen * angleres;

	// check if known (zero angle has been calculated in constructor):
	const word k = hkey(hAngle);
	if(k.match(hkey(0))) return *this;
	if(angleHills_.found(k)) return angleHills_[k];

	// reduce to first sector (angles between 0 and pi/2):
	if(hAngle < 0)  return getKMHill(angleRad + 2. * pi);
	if(hAngle > pi) return getKMHill(angleRad - pi);
	if(hAngle > pi/2) return getKMHill(pi/2 - (angleRad - pi/2));

	// calculate length at given angle:
	scalar hlength = 0;
	if(hAngle == 0){
		hlength = radius();
	} else if(mag(hAngle - pi/2) < angleres){
		hlength = b_;
	} else {
		const scalar l1  = b_;
		const scalar l2  = radius();
		const scalar l1c = l1 * Foam::cos(hAngle);
		const scalar l2s = l2 * Foam::sin(hAngle);
		hlength          = l1 * l2 / Foam::sqrt( l1c * l1c + l2s * l2s );
	}

	// calculate new hill:
	KasmiMassonHill kmh
	(
				p_center_,
				maxHeight(),
				hlength,
				addType(),
				resolution()
	);

	// store:
	angleHills_.set(k,kmh);

	return angleHills_[k];
}

scalar OvalKMHill::height(const point & p) const{

	point q    = p - p_center_;
	scalar nq  = norm(q);
	scalar ang = nq > CHOP ? getAngleRad(q,n_length_) : 0.;
	return height(ang,nq);
}

// ************************************************************************* //

} // End namespace oldev

} // End namespace Foam
