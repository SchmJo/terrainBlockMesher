/**
 * @file FileLinked.h
 * @class FileLinked
 * @brief An interface connecting classes to files.
 */

#ifndef FILELINKED_H_
#define FILELINKED_H_

#include <string>

namespace jlib{

class FileLinked {

public:

	/// Link to input and output files
	FileLinked(const std::string & filename_in,
			const std::string & filename_out = "");

	virtual ~FileLinked();

	/// Checks existence of files
	bool check();

	/// Retruns the input file name
	std::string getFileNameIn() const {
		return fname_in;
	}

	/// Returns the output file name
	std::string getFileNameOut() const {
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
