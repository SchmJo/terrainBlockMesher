/**
 * @file PlotLineInfo.h
 * @brief Definition/Implementation of struct PlotLineInfo
 *
 */

#ifndef PLOTLINEINFO_H_
#define PLOTLINEINFO_H_

#include "Plotable.h"

namespace jlib{

/**
 * @class PlotLineInfo
 * @brief A info struct for lines in class Plot.
 *
 */
/// data of a plot line
class PlotLineInfo{

public:

	/// The origin of first axis data
	Plotable const * source_x;

	/// The origin of second axis data
	Plotable const * source_y;

	/// The column of first axis data
	int column_x;

	/// The column of second axis data
	int column_y;

	/// the name of the plot line
	std::string name;
};

}

#endif /* PLOTLINEINFO_H_ */
