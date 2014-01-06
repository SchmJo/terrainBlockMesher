/**
 * @file PolarPlot.cpp
 * @brief This file defines/implements the class PolarPlot.
 */

#include "PolarPlot.h"

namespace jlib {

PolarPlot::PolarPlot(
		const std::string & name,
		const std::string & writeToFolder,
		const std::string & label_data
		):
		Plot
		(
				name,
				writeToFolder,
				"angle",
				label_data
		){
}

} /* namespace jlib */
