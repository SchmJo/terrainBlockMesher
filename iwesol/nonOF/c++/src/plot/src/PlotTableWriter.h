/**
 * @file PlotTableWriter.h
 * @brief This file defines/implements the class PlotTableWriter.
 */

#ifndef PLOTTABLEWRITER_H_
#define PLOTTABLEWRITER_H_

#include "PlotWriter.h"

namespace jlib {

class Plot;

/**
 * @class jlib::PlotTableWriter
 * @brief A class writing a table file (.txt) of a plot.
 */
class PlotTableWriter:
	public PlotWriter{

public:

	/// Constructor
	PlotTableWriter(
			Plot const * plot,
			int colWidth_x = 13,
			int colWidth_y = 13,
			int precision_x = 2,
			int precision_y = 2,
			bool scientific_x = false,
			bool scientific_y = true
			);

	/// Outputable: Provides the data that will be written to the output file.
	Outputable::OData getOData(
			const std::string & type = IO::OFILE::TYPE::EMPTY) const;

	/// PlotWriter: create the plot
	void write() const;

	/// set parameters
	inline void setParameters(
			int colWidth_x = 13,
			int colWidth_y = 13,
			int precision_x = 2,
			int precision_y = 2,
			bool scientific_x = false,
			bool scientific_y = true
			);


private:

	/// the columns width, x data
	int m_colWidth_x;

	/// the columns width, y data
	int m_colWidth_y;

	/// the precision, x data
	int m_precision_x;

	/// the precision, y data
	int m_precision_y;

	/// flag for scientific numbers, x data
	bool m_scientific_x;

	/// flag for scientific numbers, y data
	bool m_scientific_y;

	/// PlotWriter: write the gnuplot file
	inline void writeGnuplot() const {}

	/// PlotWriter: write the shell script
	inline void writeScript() const {}

	/// PlotWriter: run the shell script
	inline void runScript() const {}
};

inline void PlotTableWriter::setParameters(
			int colWidth_x,
			int colWidth_y,
			int precision_x,
			int precision_y,
			bool scientific_x,
			bool scientific_y
			){
	m_colWidth_x   = colWidth_x;
	m_colWidth_y   = colWidth_y;
	m_precision_x  = precision_x;
	m_precision_y  = precision_y;
	m_scientific_x = scientific_x;
	m_scientific_y = scientific_y;
}

} /* namespace jlib */
#endif /* PLOTTABLEWRITER_H_ */
