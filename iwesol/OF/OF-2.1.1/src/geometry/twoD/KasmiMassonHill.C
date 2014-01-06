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

#include "KasmiMassonHill.H"
#include "addToRunTimeSelectionTable.H"

namespace Foam
{

namespace oldev
{

defineTypeNameAndDebug(KasmiMassonHill, 0);
addToRunTimeSelectionTable(GroundObject,KasmiMassonHill,dict);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

KasmiMassonHill::KasmiMassonHill
(
		const dictionary & dict
):
	GroundObject(dict){

	p_center_  = point(dict.lookup("p_center"));
	maxHeight_ = readScalar(dict.lookup("height"));
	a_		   = readScalar(dict.lookup("radius"));

	init();
}

KasmiMassonHill::KasmiMassonHill
(
		const point & p_center,
		scalar maxHeight,
		scalar length,
		const word & addType,
		scalar resolution
):
	GroundObject(addType,resolution),
	p_center_(p_center),
	maxHeight_(maxHeight),
	a_(length){

	init();
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

KasmiMassonHill::~KasmiMassonHill()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

void KasmiMassonHill::init(){

	// calc aspect ratio:
	aspectRatio_ = a_ / maxHeight_;

	// calc m_:
	m_ = 1. / aspectRatio_ + Foam::sqrt( 1. / ( aspectRatio_ * aspectRatio_ ) + 1. );

}

void KasmiMassonHill::calcAll(){

	// init and fill height list:
	heightTable_ = HashTable<scalar>( a_ / resolution() + 1);
	for(label i = 0; i < heightTable_.size(); i++){
		scalar d = i * resolution();
		heightTable_.set(hkey(d), calcHeight(d, 0., a_));
	}

}

scalar KasmiMassonHill::calcHeight(scalar dist, scalar xi0, scalar xi1) const{

	// equal:
	if(xi0 == xi1) return zofxi(xi0);

	// calc x values at boundaries:
	scalar x0 = xofxi(xi0);
	scalar x1 = xofxi(xi1);

	// if close enough, return mean height:
	scalar d0 = mag(x0 - dist);
	scalar d1 = mag(x1 - dist);

	if(d0 <= resolution() && d1 <= resolution()){

		// calc heights
		scalar z0 = zofxi(xi0);
		scalar z1 = zofxi(xi1);

		return d1 / (d0 + d1) * z0 + d0 / (d0 + d1) * z1;

	}

	// calc middle xi and x there:
	scalar xi = 0.5 * (xi0 + xi1);
	scalar x  = xofxi(xi);

	// call again:
	if(x <= dist) return calcHeight(dist,xi,xi1);
	return calcHeight(dist,xi0,xi);

}

scalar KasmiMassonHill::height(scalar dist) const{

	// simple cases:
	if(dist >= a_) return 0.;
	if(dist < resolution()) return maxHeight_;

	// chop to resolution():
	label hi     = dist / resolution();
	scalar hdist = hi * resolution();

	// check if already calculated, calc otherwise:
	word k = hkey(hdist);
	if(!heightTable_.found(k)){
		heightTable_.set(k,calcHeight(hdist, 0., a_));
	}

	return heightTable_[k];

}

// ************************************************************************* //

} // End namespace oldev

} // End namespace Foam
