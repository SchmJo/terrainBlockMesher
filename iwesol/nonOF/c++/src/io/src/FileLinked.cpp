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
    along with IWESOL.  If not, see <http://www.gnu.org/licenses/>.

Class

Description

SourceFiles

References

\*---------------------------------------------------------------------------*/
#include <sstream>
#include <fstream>
#include <iostream>
using namespace std;

#include "FileLinked.h"
#include "IO.h"

namespace jlib{

FileLinked::FileLinked(const string & filename_in,
		const string & filename_out) :
	flag_out_append(false), flag_out_cout(false),fname_in(filename_in), fname_out(filename_out) {
	init();
}

FileLinked::~FileLinked() {
}

void FileLinked::init() {

	check();

}


bool FileLinked::check() {

	if (!IO::fileExists(fname_in) && fname_out.empty()) {
		cerr << "## " << "FileLinked" << ": Input file '" << fname_in
				<< "' not found." << endl;
		return false;
	}

	if(fname_out.empty()){
		flag_out_cout = true;
		return true;
	}

	if (fname_out.compare(fname_in) != 0) {

		flag_out_cout = false;

		if (IO::fileExists(fname_out)) {

			cerr << "## " << "FileLinked" << ": Output file '" << fname_out
					<< "' exists and will be overwritten." << endl;
			flag_out_append = false;
			return true;

		} else
			flag_out_append = false;

	} else {
		cerr << "## " << "FileLinked" << ": Output to console." << endl;
		flag_out_cout = true;
	}

	return true;
}

}

