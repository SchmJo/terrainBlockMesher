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

Class
    Foam::iwesol::STLLandscape

Description
    See below.

SourceFiles
    STLLandscape.C

References
	[1] J. Schmidt, C. Peralta, B. Stoevesandt, "Automated Generation of
	    Structured Meshes for Wind Energy Applications", Proceedings of the
	    Open Source CFD International Conference, 2012, London, UK

\*---------------------------------------------------------------------------*/

#ifndef STLLANDSCAPE_H_
#define STLLANDSCAPE_H_

#include "STLProjecting.H"
#include "HasCoordinateSystem.H"
#include "Globals.H"

namespace Foam{
namespace iwesol{

/**
 * @class Foam::iwesol::STLLandscape
 * @brief A STL landscape, extended to a zero level.
 *
 */
class STLLandscape:
	public STLProjecting,
	public HasCoordinateSystem{

public:

	/// Constructor
	STLLandscape(){}

	/// Constructor
	STLLandscape(
			CoordinateSystem * cooSys,
			searchableSurface const * stl,
			const point & p_SWL,
			const scalarList & dimensions,
			const point & p_SWL_stl,
			const scalarList & dimensions_stl,
			scalar zeroLevel = 0,
			scalar f_pref = 1.,
			scalar f_expo = 2.
			);

	/// Destructor
	virtual ~STLLandscape();

	/// checks if point is inside
	bool isInside(const point & p) const;

	/// checks if point is inside stl
	bool isInsideSTL(const point & p) const;

	/// gets nearest point on outside box and stl. returns success.
	bool getNearestPoints(
			const point & p,
			point & p_boundary,
			point & p_stl
			) const;

	/// the function used for interpolation, 0 <= s <= 1, 0 <= f <= 1, f(0) = 1, f(1) = 0
	//virtual scalar f_interpolate_terrain(scalar s) const { return Foam::pow(1 - s,f_pref) * Foam::exp(-Foam::pow(s,f_expo)); }
	virtual scalar f_interpolate_terrain(scalar s) const;


protected:

	/// attach a point to stl. returns success.
	bool attachPoint(point & p, const point & p_projectTo);


private:

	/// The outer lower SWL point
	point p_SWL;

	/// The stl lower SWL point
	point p_SWL_stl;

	/// The outer dimensions
	scalarList dimensions;

	/// The outer stl dimensions
	scalarList dimensions_stl;

	/// the zero level
	scalar zeroLevel;

	/// interpolation function prefactor
	scalar f_A;

	/// interpolation function exponent
	scalar f_B;

};

inline bool STLLandscape::isInside(const point & p) const{
	return isInsideBox(p,p_SWL,getCoordinateSystemC(),dimensions);
}

inline bool STLLandscape::isInsideSTL(const point & p) const{
	return isInsideBox(p,p_SWL_stl,getCoordinateSystemC(),dimensions_stl);
}

} /* iwesol */
} /* Foam */

#endif /* STLLANDSCAPE_H_ */
