/**
 * @file OManager.h
 * @brief This file defines/implements the class OManager.
 *
 */

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
