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

    You should have received a_ copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

namespace Foam
{

namespace oldev
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline scalar KasmiMassonHill::xofxi(scalar xi) const{

	return 0.5 * xi * (
			1. + a_ * a_ / ( xi * xi + m_ * m_ * ( a_ * a_ - xi * xi ) )
		);
}

inline scalar KasmiMassonHill::zofxi(scalar xi) const{

	return 0.5 * m_ * Foam::sqrt(a_ * a_ - xi * xi) * (
			1. - a_ * a_ / ( xi * xi + m_ * m_ * ( a_ * a_ - xi * xi ) )
		);
}

inline word KasmiMassonHill::hkey(scalar d) const{
	return jlib::BLIB::d2S(d,8,"X");
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


// ************************************************************************* //

} // End namespace oldev

} // End namespace Foam
