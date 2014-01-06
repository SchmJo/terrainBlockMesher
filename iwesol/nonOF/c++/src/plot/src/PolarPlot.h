/**
 * @file PolarPlot.h
 * @brief This file defines/implements the class PolarPlot.
 */

#ifndef POLARPLOT_H_
#define POLARPLOT_H_

#include "Plot.h"

namespace jlib {

/**
 * @class jlib::PolarPlot
 * @brief A plot in polar coordinates.
 *
 * Connection with class Plot: x-axis is angle wrt. (1 0 0), y-axis is data.
 */
class PolarPlot:
	public Plot{

public:

	/// Constructor
	PolarPlot(
			const std::string & name = "",
			const std::string & writeToFolder = ".",
			const std::string & label_data = "frequency"
			);


private:

	/// disabled
	void addLine(const PlotLineInfo & line);

	// disabled
	void addLine(
				Plotable const * source,
				int column_x,
				int column_y,
				std::string name
				);

	/// disabled
	void addLine(
				Plotable const * source_x,
				Plotable const * source_y,
				int column_x,
				int column_y,
				std::string name
				);

};

} /* namespace jlib */
#endif /* POLARPLOT_H_ */
