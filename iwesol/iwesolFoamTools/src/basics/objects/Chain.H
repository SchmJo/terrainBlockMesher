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
    Foam::iwesol::Chain

Description
    See below.

SourceFiles
    Chain.C

References
	[1] J. Schmidt, C. Peralta, B. Stoevesandt, "Automated Generation of
	    Structured Meshes for Wind Energy Applications", Proceedings of the
	    Open Source CFD International Conference, 2012, London, UK

\*---------------------------------------------------------------------------*/

#ifndef CHAIN_H_
#define CHAIN_H_

#include "PartlyPointedList.h"

#include "pointField.H"

namespace Foam{
namespace iwesol{

/**
 * @class Foam::iwesol::Chain
 * @brief A line of points, stored here or pointed to
 */
class Chain:
	public blib::PartlyPointedList<point>{

public:

	/// Constructor
	Chain();

	/// Constructor
	Chain(
			const pointField & points
	);

	/// Constructor
	Chain(
			const List<point*> & points
	);

	/// Constructor
	Chain(
			Chain * c
	);

	/// Constructor
	Chain(
			point * first,
			point * last,
			const pointField & inBetween
	);

	/// Constructor
	Chain(
			point * first,
			point * last,
			label pointsInBetween = 0
	);

	/// Destructor
	virtual ~Chain();

	/// Chain: Print to console
	virtual void print() const;

	/// Chain: Sets the Dirichlet flag
	virtual void setDirichlet(label i, bool fixed = true) { dirichletFlags[i] = fixed; }

	/// Returns point
	inline point & getPoint(label i, bool reversed = false) { return (*this)[reversed ? size() - 1 - i : i]; }

	/// Returns velocity
	inline const point & getPoint(label i, bool reversed = false) const { return (*this)[reversed ? size() - 1 - i : i]; }

	/// Returns point
	inline point & getFirstPoint(bool reversed = false) { return reversed ? last() : first(); }

	/// Returns point
	inline const point & getFirstPoint(bool reversed = false) const { return reversed ? last() : first(); }

	/// Returns point
	inline point & getLastPoint(bool reversed = false) { return reversed ? first() : last(); }

	/// Returns point
	inline const point & getLastPoint(bool reversed = false) const { return reversed ? first() : last(); }

	/// returns the points as a copy
	pointField getPoints() const;

	/// returns inner points as a copy
	pointField getInnerPoints() const;

	/// returns the pointers
	List<point*> getPointers();

	/// returns the Dirichlet flag
	inline bool isDirichlet(label i, bool reversed = false) const { return dirichletFlags[reversed ? size() - 1 - i : i]; }


protected:

	/// Returns the partly pointed list
	inline blib::PartlyPointedList<point> & getPartlyPointedList() { return *this; }

	/// The Dirichlet condition flags
	List<bool> dirichletFlags;
};

} /* iwesol */
} /* Foam */

#endif /* CHAIN_H_ */
