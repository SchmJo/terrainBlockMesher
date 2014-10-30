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

namespace Foam
{

namespace oldev
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline void OptimizerGroundObject::initRangeBox(
			const Box & rangeBox
			){
	box_  = rangeBox;
}

inline point OptimizerGroundObject::normalize(
		const point & p
		) const{
	point c = coordinateSystem().point2coord(p);
	forAll(c,dI) c[dI] /= box_.lengths()[dI];
	return c;
}

inline point OptimizerGroundObject::unnormalize(
		const point & c
		) const{
	point p = c;
	forAll(p,dI) p[dI] *= box_.lengths()[dI];
	coordinateSystem().coord2point(p);
	return p;
}

inline bool OptimizerGroundObject::calcConstraint(
			label i,
			const UList< scalar > & parameters,
			scalar & constraintValue
			) const{
	point p
	(
			parameters[0],
			parameters[1],
			0
	);
	if(parameters.size() > 2) p[2] = parameters[2];
	if
	(
			!calcPointConstraint
			(
					i, p, constraintValue
			)
	) return false;

	return true;
}


inline bool OptimizerGroundObject::calcPointConstraints(
			const point & p,
			UList< scalar > & results
			) const{

	for(label cI = 0; cI < nrConstraints(); cI++){
		if
		(
				!calcPointConstraint
				(
						cI, p, results[cI]
				)
		) return false;
	}
	return true;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


// ************************************************************************* //

} // End namespace oldev

} // End namespace Foam
