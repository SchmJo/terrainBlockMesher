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

namespace Foam{
namespace iwesol{


KasmiMassonHill::KasmiMassonHill(const dictionary & dict){

	p_center    = point(dict.lookup("p_center"));
	maxHeight   = readScalar(dict.lookup("height"));
	a		    = readScalar(dict.lookup("radius"));
	resolution  = readScalar(dict.lookup("resolution"));
	addType     = word(dict.lookup("addType"));

	init();
}

KasmiMassonHill::KasmiMassonHill(
		const point & p_center,
		const scalar & maxHeight,
		const scalar & length,
		const word & addType,
		const scalar & resolution
		):
	GroundObject(addType,resolution),
	p_center(p_center),
	maxHeight(maxHeight),
	a(length){

	init();
}

KasmiMassonHill::~KasmiMassonHill() {
}

void KasmiMassonHill::init(){

	// calc aspect ratio:
	aspectRatio = a / maxHeight;

	// calc m:
	m = 1. / aspectRatio + Foam::sqrt( 1. / ( aspectRatio * aspectRatio ) + 1. );
}

void KasmiMassonHill::calcAll(){

	// init and fill height list:
	heightTable = HashTable<scalar>( a / resolution + 1);
	for(label i = 0; i < heightTable.size(); i++){
		scalar d = i * resolution;
		heightTable.set(hkey(d), calcHeight(d, 0., a));
	}
}

scalar KasmiMassonHill::calcHeight(
		const scalar & dist,
		const scalar & xi0,
		const scalar & xi1
		){

	// equal:
	if(xi0 == xi1) return zofxi(xi0);

	// calc x values at boundaries:
	scalar x0 = xofxi(xi0);
	scalar x1 = xofxi(xi1);

	// if close enough, return mean height:
	scalar d0 = mag(x0 - dist);
	scalar d1 = mag(x1 - dist);

	if(d0 <= resolution && d1 <= resolution){

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

scalar KasmiMassonHill::getHeight(const scalar & dist){

	// simple cases:
	if(dist >= a) return 0.;
	if(dist < resolution) return maxHeight;

	// chop to resolution:
	label hi     = dist / resolution;
	scalar hdist = hi * resolution;

	// check if already calculated, calc otherwise:
	word k = hkey(hdist);
	if(!heightTable.found(k)){
		heightTable.set(k,calcHeight(hdist, 0., a));
	}

	return heightTable[k];
}

} /* iwesol */
} /* Foam */
