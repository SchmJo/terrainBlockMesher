/**
 * @file IO.h
 * @brief Basic Input/Output functions.
 *
 */

#ifndef IO_H_
#define IO_H_

#include <string>
#include <sstream>
#include <iostream>
#include <vector>

namespace jlib{

namespace IO{

	namespace OFILE{

		/// A piece of data to be written to a file
		struct Data{

			/// the name of the piece of data.
			std::string name;

			/// the file type.
			std::string type;

			/// the origin of the data.
			std::string origin;

			/// contribution to file header.
			std::string header;

			/// pre-data entry.
			std::string pre_data;

			/// pure data entry.
			std::string data;

			/// post-data entry.
			std::string post_data;

			/// contribution to file closure.
			std::string closure;

		};

		namespace TYPE{

			/// the empty file type.
			const std::string EMPTY      = "";

			/// the text file type.
			const std::string TXT        = "txt";

			/// the ascii vtk file type.
			const std::string VTK_ASCII  = "vtka";

			/// the binary vtk file type.
			const std::string VTK_BIN    = "vtkb";

			/// OpenFoam data
			const std::string OPEN_FOAM  = "of";

			/// gnuplot data
			const std::string GNUPLOT_DATA = "gnud";

			/// gnuplot script
			const std::string GNUPLOT_SCRIPT = "gnus";

			/// shell script
			const std::string SHELL_SCRIPT = "sh";

			/// python script
			const std::string PYTHON_SCRIPT = "py";
		}

		namespace PREAMBLE{

			/// the standard preamble of gnuplot files.
			const std::string GNUPLOT_DATA =
					"# gnuplot data file\n";

			/// the standard preamble of gnuplot files.
			const std::string GNUPLOT_SCRIPT =
					"# gnuplot script file\n";

			/// the standard preamble of python files.
			const std::string PYTHON_SCRIPT =
					"#!/usr/bin/env python \n# -*- coding: utf-8 -*-\n";

			/// the standard preamble of shell scripts.
			const std::string SHELL_SCRIPT =
					"#!/bin/sh\n";

			/// the standard preamble of vtka files.
			const std::string VTK_ASCII =
				"# vtk DataFile Version 3.0\nvtk output\nASCII\n\n";

			/// the OpenFoam preamble
			const std::string OPEN_FOAM =
"/*--------------------------------*- C++ -*----------------------------------*\\  \n\
| =========                 |                                                 |    \n\
| \\\\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox           |  \n\
|  \\\\    /   O peration     | Version:  2.0.1                                 |  \n\
|   \\\\  /    A nd           | Web:      www.OpenFOAM.com                      |  \n\
|    \\\\/     M anipulation  |                                                 |  \n\
\\ *---------------------------------------------------------------------------*/  \n\
FoamFile\n\
{\n\
    version 2.0;\n\
    format  ascii;\n\
    class   dictionary;\n\
    object  testDict;\n\
}\n\
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * // \n\
 \n";

			/// the OpenFoam preamble
			const std::string OPEN_FOAM_HALF1 =
"/*--------------------------------*- C++ -*----------------------------------*\\  \n\
| =========                 |                                                 |    \n\
| \\\\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox           |  \n\
|  \\\\    /   O peration     | Version:  2.0.1                                 |  \n\
|   \\\\  /    A nd           | Web:      www.OpenFOAM.com                      |  \n\
|    \\\\/     M anipulation  |                                                 |  \n\
\\ *---------------------------------------------------------------------------*/  \n\
FoamFile\n\
{\n\
    version 2.0;\n\
    format  ascii;\n\
    class   dictionary;\n\
    object  ";


			/// the OpenFoam preamble
			const std::string OPEN_FOAM_HALF2 =
";\n\
}\n\
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * // \n\
 \n";

		}
	}

	namespace FILEPOS{

		/// the current word.
		const std::string CURRENT_WORD = "FilePosCurrentWord";

	}


	/// checks if a file exists
	bool fileExists(const std::string & filename);

	/** Reads the next word from a stream, returns success.
	 * The file position after this operation is on the blank
	 * AFTER the word.
	 */
	bool readWord(std::ifstream* file, std::string & word);

	/** Reads the next words and the file positions after them
	 * from a stream, returns success.
	 */
	bool readWords(std::ifstream* file, std::string & words,
			std::vector<long> & postWordPos, const int nrOfWords);

	/** Reads the ßprevious word from a stream, returns success.
	 * The file position after this operation is on the blank
	 * BEFORE the word.
	 */
	bool readWordB(std::ifstream* file, std::string & word);

	/** Reads the previous words and the file positions before them
	 * from a stream, returns success.
	 */
	bool readWordsB(std::ifstream* file, std::string & words,
			std::vector<long> & preWordPos, const int nrOfWords);

	/// Returns the size of a file.
	long getFileSize(std::ifstream* file);

	/// Finds std::strings in a file and returns the corresponding line, or parts
	std::string find(std::istream& file /*! Input stream */,
	                const std::string & term   /*! search terms, seperated by blanks */,
	                const std::string & sep = ":"   /*! seperators, seperated by blanks */,
	                bool restonly = true /*! return only the rest, not the search terms */,
	                int returnhowmany = 1 /*! how many words should be returned, all if 0 */,
	                bool beginning = false /*! reset current file position */,
	                int skip = 0/* offset relative to file position specified by 'beginning' */,
	                bool autoUpcase = true /* converts to upcase first */
	                );

	/// Reads a file completely into a std::vector of std::strings
	std::vector<std::string> readAll(const std::string &filename);

	/// replaces German ',' by '.' and parses number
	double readReplaceGermanComma(std::istream & is);

	/// counts character in a stream, eg '\n'. leaves stream at begin position.
	int countChars(std::istream & is,const char & c);

	/// forwards stream to position
	bool forward(std::istream & is, const char & c, int howMany = 1);

	/// forwards stream to position
	bool forwardWord(std::istream & is, int howMany = 1);

} // end namespace IO


}

#endif /* IO_H_ */
