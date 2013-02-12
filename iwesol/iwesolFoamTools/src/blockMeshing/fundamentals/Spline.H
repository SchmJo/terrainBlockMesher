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
    Foam::iwesol::Spline

Description
    See below.

SourceFiles
    Spline.C

References
	[1] J. Schmidt, C. Peralta, B. Stoevesandt, "Automated Generation of
	    Structured Meshes for Wind Energy Applications", Proceedings of the
	    Open Source CFD International Conference, 2012, London, UK

\*---------------------------------------------------------------------------*/

#ifndef SPLINE_H_
#define SPLINE_H_

#include "Chain.H"
#include "PointLinePath.H"

namespace Foam{
namespace iwesol{

/**
 * @class Spline
 * @brief A simple class for a spline, eventually based on class PointLinePath.
 *
 */
class Spline:
	public Chain{

public:

	/// Constructor.
	Spline();

	/// Constructor.
	Spline(
			pointField * globalPoints,
			label vertexA,
			label vertexB,
			const pointField & splinePointsBetween
			);

	/// Constructor.
	Spline(
			pointField * globalPoints,
			label vertexA,
			label vertexB
			);

	/// Destructor.
	virtual ~Spline();

	/// Returns vertex A
	inline label getVertexA() const { return vA; }

	/// Returns vertex B
	inline label getVertexB() const { return vB; }

	/// Returns a single spline point
	inline const point & getPoint(label i) const { return (*this)[i]; }

	/// Returns a single spline point
	inline point & getPoint(label i){ return (*this)[i]; }

	/// Returns first spline point (vertex)
	inline point & getFirstPoint(){ return (*this)[0]; }

	/// Returns first spline point (vertex)
	inline const point & getFirstPoint() const { return (*this)[0]; }

	/// Returns last spline point (vertex)
	inline point & getLastPoint(){ return (*this)[size() - 1]; }

	/// Returns last spline point (vertex)
	inline const point & getLastPoint() const { return (*this)[size() - 1]; }

	/// Returns the dict entry spline for blockMeshDict/edges
	std::string dictEntry() const;

	/// orders the spline wrt a given direction. returns true if switched vA and vB.
	bool order(const Foam::vector & direction);

	/// returns the point line path
	PointLinePath getPointLinePath(
			bool withFirst = true,
			bool withLast = true
			) const;

	/// returns point s list
	scalarList getPointSList(
			bool withFirst = true,
			bool withLast = true
			) const;

	/// mixes spline with linear line, negative mixSize corresponds to all points
	void mixWithLinear(bool atFirst = true, label mixSize = -1);


private:

	/// The global point list
	pointField * globalPoints;

	/// vertex A
	label vA;

	/// vertex B
	label vB;
};

} /* iwesol */
} /* Foam */

#endif /* SPLINE_H_ */
