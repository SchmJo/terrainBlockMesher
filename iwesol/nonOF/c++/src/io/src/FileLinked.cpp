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

