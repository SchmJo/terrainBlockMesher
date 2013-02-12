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
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Class
    blib::FileLinked

Description
    See below

SourceFiles
    FileLinked.C

References
	[1] J. Schmidt, C. Peralta, B. Stoevesandt, "Automated Generation of
	    Structured Meshes for Wind Energy Applications", Proceedings of the
	    Open Source CFD International Conference, 2012, London, UK

\*---------------------------------------------------------------------------*/

#ifndef FILELINKED_H_
#define FILELINKED_H_

#include <string>

namespace blib{

/**
 * @class blib::FileLinked
 * @brief An interface connecting classes to files.
 */
class FileLinked {

public:

	/// Link to input and output files
	FileLinked(const std::string & filename_in,
			const std::string & filename_out = "");

	virtual ~FileLinked();

	/// Checks existence of files
	bool check();

	/// Retruns the input file name
	inline std::string getFileNameIn() const {
		return fname_in;
	}

	/// Returns the output file name
	inline std::string getFileNameOut() const {
		return fname_out;
	}


protected:

	/// Append an existing file
	bool flag_out_append;

	/// Ouput to console
	bool flag_out_cout;

	/// The input file name
	std::string fname_in;

	/// The output file name
	std::string fname_out;

	/// Called by the constructors.
	virtual void init();
};

}

#endif /* FILELINKED_H_ */
