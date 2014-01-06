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

#ifndef OMANAGER_H_
#define OMANAGER_H_

#include "FileLinked.h"
#include "Outputable.h"
#include "IO.h"

namespace jlib{

/**
 * @class OManager
 * @brief This class organizes and writes output files.
 *
 * Classes that can write to files should be derived from the interface
 * Outputable.
 */
class OManager: public FileLinked {


public:

	/// Links to the data origins.
	std::vector<Outputable const *> oLinks;

	/// Constructor.
	OManager(const std::string & filename_out, const std::string & fileType = IO::OFILE::TYPE::EMPTY);

	/// Destructor.
	virtual ~OManager();

	/// resets the content.
	void clear() { content = ""; }

	/** Collect pre, data and post information from the linked objects.
	 * Add a header using addHeader.
	 */
	void collectAll(const std::string & nonstdcaseheader = "");

	/** Collect data from the linked objects, including pre-
	 * and post-data entries.
	 */
	void collectData();

	/// Collect header entries from the linked objects.
	void collectHeader();

	/// Collect closure entries from the linked objects.
	void collectClosure();

	/// adds data to the output content.
	void addToContent(const std::string & data) { content += data; }

	/// prepends the output content by data.
	void addContentTo(const std::string & data) { content = data + content; }

	/// writes the previously collected data content to file.
	void write();

	/// writes a table (float x, float f(const float x)) to content.
	void readFunc_ffc(Outputable* obj,
			Outputable::FctPtr_ffc f, float xmin, float xmax, int points);

	/// prints the currently collected data content.
	void printContent() const { std::cout<<getContent()<<std::endl; }

	/// adds a data link.
	void addOLink(Outputable const * link) { oLinks.push_back(link); }

	/// resets case
	void set(const std::string & filename_out, const std::string & fileType = IO::OFILE::TYPE::EMPTY){
		setFileNameOut(filename_out); setType(fileType);
	}

	/// sets the file type.
	void setType(const std::string & fileType) { type = fileType; }

	/// adds a header. if argument is given, this is set, otherwise standard from IO.h.
	void addHeader(const std::string & nonstdcaseheader = "");

	/// sets the output file.
	void setFileNameOut(const std::string & fname) { fname_out = fname; }

	/// Returns the file type.
	std::string getType() const { return type;}

	/// Returns the currently collected data content.
	std::string getContent() const { return content; }


protected:

	/// data content to be written to file.
	std::string content;

	/// the file type.
	std::string type;

};

}

#endif /* OMANAGER_H_ */
