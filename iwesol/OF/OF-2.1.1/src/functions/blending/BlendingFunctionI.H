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

// * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * //

template< class Data, class Point >
BlendingFunction< Data, Point >::BlendingFunction
(
			const word & transitionType
):
			transition_
			(
					TransitionFunction::New(transitionType)
			){
}

template< class Data, class Point >
BlendingFunction< Data, Point >::BlendingFunction
(
			const dictionary & dict
):
			transition_
			(
					TransitionFunction::New(dict.subDict("transitionFunction"))
			){
}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template< class Data, class Point >
inline Data BlendingFunction< Data, Point >::blend
(
			const Point & point0,
			const Point & point1,
			const Data & data0,
			const Data & data1,
			scalar & s
) const{

	// transform s:
	sTransformation
	(
			point0,
			point1,
			data0,
			data1,
			s
	);

	// calc:
	scalar t = transition_()(s);
	return (1. - t) * data0 + t * data1;
}


// ************************************************************************* //

} // End namespace oldev

} // End namespace Foam
