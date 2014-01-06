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

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

namespace oldev
{

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline const point Box::centre() const{
	point out(0,0,0);
	forAll(points_,pI) out += points_[pI];
	return out / scalar(points_.size());
}

inline labelList Box::isOutsideBox(const point & p) const{
	labelList isOut(3,0);
	point c_delta = coordinateSystem().point2coord(p);
	for(label i = 0; i < 2; i++){
		if(mag(c_delta[i]) > resolution()){
			if(c_delta[i] < 0) isOut[i] = -1;
		}
		scalar ldist = lengths()[i] - c_delta[i];
		if(mag(ldist) > resolution()){
			if(ldist < 0 ) isOut[i] = 1;
		}
	}
	return isOut;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


// ************************************************************************* //

} // End namespace oldev

} // End namespace Foam
