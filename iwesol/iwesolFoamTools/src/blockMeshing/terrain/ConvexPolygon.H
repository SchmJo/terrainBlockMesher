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
    Foam::iwesol::ConvexPolygon

Description
    See below.

SourceFiles
    ConvexPolygon.C

References
	[1] J. Schmidt, C. Peralta, B. Stoevesandt, "Automated Generation of
	    Structured Meshes for Wind Energy Applications", Proceedings of the
	    Open Source CFD International Conference, 2012, London, UK

\*---------------------------------------------------------------------------*/

#ifndef ConvexPolygon_H_
#define ConvexPolygon_H_

#include "pointField.H"

#include "GroundObject.H"

namespace Foam{
namespace iwesol{

/**
 * @class Foam::iwesol::ConvexPolygon
 * @brief A convex polygon for landscape manipulation
 *
 */
class ConvexPolygon:
	public virtual GroundObject{

public:

	/// Constructor
	ConvexPolygon(
			const pointField & points,
			const point & insidePoint,
			scalar height = 0,
			const word & addType = "add",
			scalar resolution = 0.001
			);

	/// Constructor
	ConvexPolygon(const dictionary & dict);

	/// Constructor
	ConvexPolygon(){}

	/// Destructor
	virtual ~ConvexPolygon();

	/// GroundObject: Returns height at point (here this must me in the same plane, no projection)
	virtual scalar getHeight(const point & p) { return isInside(p) ? height : 0; }

	/// GroundObject: Returns true if point is inside
	virtual bool isInside(const point & p);

	/// return a point inside
	const point & getInsidePoint() const { return p_inside; }

	/// the size
	label size() const { return points.size(); }

	/// returns a point
	point & operator[](label i) { return points[i]; }

	/// returns a point
	const point & operator[](label i) const { return points[i]; }

	/// returns the maximal distance
	scalar getMaxDistance() const { return maxDist; }


private:

	/// The points
	pointField points;

	/// a point inside
	point p_inside;

	/// The height
	scalar height;

	/// checks if point is ok wrt to one of the lines
	bool onOneSide(const point & p,label pA, label iB) const;

	/// The max distance
	scalar maxDist;

	/// called by constructors
	void init();

};

} /* iwesol */
} /* Foam */

#endif /* ConvexPolygon_H_ */
