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
    Foam::iwesol::ClassModule

Description
    See below.

SourceFiles

References
	[1] J. Schmidt, C. Peralta, B. Stoevesandt, "Automated Generation of
	    Structured Meshes for Wind Energy Applications", Proceedings of the
	    Open Source CFD International Conference, 2012, London, UK

\*---------------------------------------------------------------------------*/

#ifndef ClassModule_H_
#define ClassModule_H_

#include "dictionary.H"

namespace Foam{
namespace iwesol{


/**
 * @class Foam::iwesol::ClassModule
 * @brief A generic module for a class.
 */
template< class Class >
class ClassModule {

public:

	/// Constructor
	ClassModule(
			Class * moduleBase
			);

	/// Destructor
	virtual ~ClassModule(){}

	/// ClassModule: load the module
	virtual bool load(
			const dictionary & dict
			) { flag_ready = true; return ready(); }

	/// checks if module is loaded
	inline bool ready() const { return m_moduleBase != 0 && flag_ready; }


protected:

	/// the underlying base
	inline Class & moduleBase() { return *m_moduleBase; }


private:

	/// the underlying base
	Class * m_moduleBase;

	/// flag is loading is complete
	bool flag_ready;


};

template< class Class >
ClassModule< Class >::ClassModule(
		Class * moduleBase
		):
		m_moduleBase(moduleBase),
		flag_ready(false){
}

} /* iwesol */
} /* Foam */

#endif /* ClassModule_H_ */
