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
    Foam::iwesol::GroundObject

Description
    See below.

SourceFiles
    GroundObject.C

References
	[1] J. Schmidt, C. Peralta, B. Stoevesandt, "Automated Generation of
	    Structured Meshes for Wind Energy Applications", Proceedings of the
	    Open Source CFD International Conference, 2012, London, UK

\*---------------------------------------------------------------------------*/

#ifndef GROUNDOBJECT_H_
#define GROUNDOBJECT_H_

#include "dictionary.H"
#include "point.H"

namespace Foam{
namespace iwesol{


/**
 * @class Foam::iwesol::GroundObject
 * @brief An interface for ground objects
 *
 */
class GroundObject {

public:

	/// Constructor
	GroundObject(const word & addType = "add", scalar resolution = 0.001);

	/// Constructor
	GroundObject(const dictionary & dict);

	/// Destructor
	virtual ~GroundObject();

	/// GroundObject: Returns height at point (here this must me in the same plane, no projection)
	virtual scalar getHeight(const point & p) { return 0; }

	/// GroundObject: Returns true if point is inside
	virtual bool isInside(const point & p){ return getHeight(p) != 0; }

	/// returns resolution
	scalar getResolution() const { return resolution; }

	/// Returns the add type
	const word & getAddType() const { return addType; }


protected:

	/// The add type
	word addType;

	/// The resolution
	scalar resolution;


};

} /* iwesol */
} /* Foam */

#endif /* GROUNDOBJECT_H_ */
