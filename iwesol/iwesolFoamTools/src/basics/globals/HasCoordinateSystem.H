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
    Foam::iwesol::HasCoordinateSystem

Description
    See below.

SourceFiles
    HasCoordinateSystem.C

References
	[1] J. Schmidt, C. Peralta, B. Stoevesandt, "Automated Generation of
	    Structured Meshes for Wind Energy Applications", Proceedings of the
	    Open Source CFD International Conference, 2012, London, UK

\*---------------------------------------------------------------------------*/

#ifndef HASCOORDINATESYSTEM_H_
#define HASCOORDINATESYSTEM_H_

#include "CoordinateSystem.H"

namespace Foam{
namespace iwesol{

/**
 * @class HasCoordinateSystem
 * @brief An interface for classes that need coordinates.
 *
 */
class HasCoordinateSystem {

public:

	/// Constructor.
	HasCoordinateSystem(CoordinateSystem * cooSys);

	/// Constructor.
	HasCoordinateSystem(const CoordinateSystem & cooSys = CoordinateSystem());

	/// Constructor.
	HasCoordinateSystem(const dictionary & dict);

	/// Copy constructor
	HasCoordinateSystem(const HasCoordinateSystem & other);

	/// Destructor.
	virtual ~HasCoordinateSystem();

	/// HasCoordinateSystem: Specify coordinates that should be registered
	virtual List<point*> getRegCoordinates() { return List<point*>(); }

	/// operator =
	HasCoordinateSystem & operator=(const HasCoordinateSystem & rhs);

	/// Returns the coordinate system
	inline CoordinateSystem * getCoordinateSystem() const { return cooSys; }

	/// Returns the coordinate system
	inline const CoordinateSystem & getCoordinateSystemC() const { return *cooSys; }

	/// Returns basis axis
	inline const Foam::vector & get_e(label i) const { return cooSys->e(i); }

	/// Returns origin
	inline const point & getOrigin() const { return cooSys->origin(); }


protected:

	/// The underlying coordinate system
	mutable CoordinateSystem * cooSys;

	/// Perform the registration
	void sendRegistration();

	/// reset
	inline void resetCoordinateSystem(const CoordinateSystem & newCS) const { *cooSys = newCS; }


private:

	/// The coordinate system data;
	CoordinateSystem _cooSys;


};

inline void HasCoordinateSystem::sendRegistration(){
	if(cooSys){
		cooSys->regCoordinates(getRegCoordinates());
	}
}

} /* iwesol */
} /* Foam */

#endif /* HASCOORDINATESYSTEM_H_ */
