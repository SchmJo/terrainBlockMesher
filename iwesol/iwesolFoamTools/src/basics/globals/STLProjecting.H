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
    Foam::iwesol::STLProjecting

Description
    See below.

SourceFiles
    STLProjecting.C

References
	[1] J. Schmidt, C. Peralta, B. Stoevesandt, "Automated Generation of
	    Structured Meshes for Wind Energy Applications", Proceedings of the
	    Open Source CFD International Conference, 2012, London, UK

\*---------------------------------------------------------------------------*/

#ifndef STLPROJECTING_H_
#define STLPROJECTING_H_

#include "searchableSurface.H"

namespace Foam{
namespace iwesol{

/**
 * @class Foam::iwesol::STLProjecting
 * @brief An interface for projection of points onto an stl.
 *
 */
class STLProjecting {

public:

	/// get the surface point on an stl between two points (hit point). Returns success.
	static bool getSTLHit(
			searchableSurface const * stl,
			const point & p_start,
			const point & p_end,
			point & surfacePoint
			);

	/// Constructor.
	STLProjecting();

	/// Constructor.
	STLProjecting(searchableSurface const * stl);

	/// Destructor.
	virtual ~STLProjecting();

	/// Returns the underlying stl
	searchableSurface const * getSTL() const { return stl; }


protected:

	/// the stl.
	searchableSurface const * stl;

	/// attach a point to stl. returns success.
	virtual bool attachPoint(point & p, const point & p_projectTo);

	/// project a point to stl, using a direction vector. returns success.
	virtual bool projectPoint(point & p, const Foam::vector & dir_proj, scalar maxDist = 100000);


};

} /* iwesol */
} /* Foam */

#endif /* STLPROJECTING_H_ */
