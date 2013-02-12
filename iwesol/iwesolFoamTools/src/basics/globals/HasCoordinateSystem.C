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

#include "HasCoordinateSystem.H"

namespace Foam{
namespace iwesol{

HasCoordinateSystem::HasCoordinateSystem(CoordinateSystem * cooSys):
	cooSys(cooSys){
}

HasCoordinateSystem::HasCoordinateSystem(const CoordinateSystem & hcooSys):
	_cooSys(hcooSys){
	cooSys = &_cooSys;
}

HasCoordinateSystem::HasCoordinateSystem(const dictionary & dict):
	_cooSys(dict){
	cooSys = &_cooSys;
}

HasCoordinateSystem::HasCoordinateSystem(const HasCoordinateSystem & other):
		cooSys(other.cooSys),
		_cooSys(other._cooSys){
	if(other.cooSys == &other._cooSys) cooSys = &_cooSys;
	sendRegistration();
}

HasCoordinateSystem::~HasCoordinateSystem() {
}

HasCoordinateSystem & HasCoordinateSystem::operator=(const HasCoordinateSystem & rhs){
	if(this != &rhs){
		cooSys  = rhs.cooSys;
		_cooSys = rhs._cooSys;

		if(rhs.cooSys == &rhs._cooSys) cooSys = &_cooSys;
		sendRegistration();
	}
	return *this;
}

} /* iwesol */
} /* Foam */
