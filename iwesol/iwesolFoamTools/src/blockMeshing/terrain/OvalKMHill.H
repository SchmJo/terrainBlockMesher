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
    Foam::iwesol::OvalKMHill

Description
    See below.

SourceFiles
    OvalKMHill.C

References
	[1] J. Schmidt, C. Peralta, B. Stoevesandt, "Automated Generation of
	    Structured Meshes for Wind Energy Applications", Proceedings of the
	    Open Source CFD International Conference, 2012, London, UK

\*---------------------------------------------------------------------------*/

#ifndef OVALKMHILL_H_
#define OVALKMHILL_H_

#include "KasmiMassonHill.H"

namespace Foam{
namespace iwesol{

/**
 * @class Foam::iwesol::OvalKMHill.H
 * @brief Class for an oval hill, based on KasmiMassonHill in each direction.
 *
 */
class OvalKMHill:
	public KasmiMassonHill {

public:

	/// Construct from sictionary
	OvalKMHill(const dictionary & dict);

	/// Construct from sictionary
	OvalKMHill(){}

	/// Destructor
	virtual ~OvalKMHill();

	/// GroundObject: Returns height at point (here this must me in the same plane, no projection)
	scalar getHeight(const point & p);

	/// Returns the width
	inline scalar getCoRadius() const { return b; }

	/// Returns the length direction
	inline const Foam::vector & getLengthDirection() const { return n_length; }

	/// Returns the width direction
	inline const Foam::vector & getWidthDirection() const { return n_width; }

	/// Returns the KasmiMassonHill at an angle. zero angle corresponds to length direction.
	KasmiMassonHill & getKMHill(const scalar & angleRad);

	/// Returns the height at a given angle and distance from the center. calculates if not known yet.
	scalar getHeight(const scalar & angleRad, const scalar & dist);


protected:

	/// the width
	scalar b;

	/// the length direction
	Foam::vector n_length;

	/// the width direction
	Foam::vector n_width;


private:

	/// The data at an angle.
	HashTable<KasmiMassonHill> angleHills;


};

inline scalar OvalKMHill::getHeight(
		const scalar & angleRad,
		const scalar & dist
		){
	KasmiMassonHill & kmh = getKMHill(angleRad);
	return kmh.getHeight(dist);
}

} /* iwesol */
} /* Foam */

#endif /* OVALKMHILL_H_ */
