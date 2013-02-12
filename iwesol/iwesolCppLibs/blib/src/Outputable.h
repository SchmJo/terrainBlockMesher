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
License
    This file is part of IWESOL.

    IWESOL is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    IWESOL is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with IWESOL. If not, see <http://www.gnu.org/licenses/>.

Class
    blib::Outputable

Description
    See below

SourceFiles
    Outputable.C

References
	[1] J. Schmidt, C. Peralta, B. Stoevesandt, "Automated Generation of
	    Structured Meshes for Wind Energy Applications", Proceedings of the
	    Open Source CFD International Conference, 2012, London, UK

\*---------------------------------------------------------------------------*/

#ifndef OUTPUTABLE_H_
#define OUTPUTABLE_H_

#include <vector>

#include "IO.h"

namespace blib{

/**
 * @class Outputable
 * @brief An interface for classes which can write data to files via
 * the class OManager.
 *
 * This includes writing simple functions
 * 		float f(const float x)
 * 			or
 * 		float f(const int i)
 * via function pointers.
 *
 */
class Outputable {

public:

	/// Define output data std::vector
	typedef std::vector<IO::OFILE::Data> OData;

	/// Define a function pointer.
	typedef float (Outputable::*FctPtr_ffc)(const float f);

	/// Define a function pointer.
	typedef float (Outputable::*FctPtr_fic)(const int i);

	/// Constructor.
	Outputable();

	/// Destructor.
	virtual ~Outputable();

	/// Outputable: Provides the data that will be written to the output file.
	virtual OData getOData(
			const std::string & type = IO::OFILE::TYPE::EMPTY) const = 0;


};

} /* blib */

#endif /* OUTPUTABLE_H_ */
