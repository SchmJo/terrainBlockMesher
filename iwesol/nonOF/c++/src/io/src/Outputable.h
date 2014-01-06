/**
 * @file Outputable.h
 * @brief This file defines/implements the class Outputable.
 *
 */

#ifndef OUTPUTABLE_H_
#define OUTPUTABLE_H_

#include "IO.h"

namespace jlib{

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

}

#endif /* OUTPUTABLE_H_ */
