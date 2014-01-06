/**
 * @file PolarPlotPythonWriter.h
 * @brief This file defines/implements the class PolarPlotPythonWriter.
 */

#ifndef POLARPLOTPYTHONWRITER_H_
#define POLARPLOTPYTHONWRITER_H_

#include "PlotWriter.h"

namespace jlib {

/**
 * @class jlib::PolarPlotPythonWriter
 * @brief Writes a polar plot via python.
 *
 * x-data is interpreted as angles in degrees, in mathematical sense.
 * y-data is interpreted as polar data.
 */
class PolarPlotPythonWriter:
	public PlotWriter{

public:

	/// Constructor
	PolarPlotPythonWriter(
			Plot const * plot,
			int sectors,
			bool flag_12axes
			);

	/// Outputable: Provides the data that will be written to the output file.
	Outputable::OData getOData(
			const std::string & type = IO::OFILE::TYPE::EMPTY) const;

	/// PlotWriter: create the plot
	void write() const;

	/// load data files
	void load();


private:

	/// the number of sectors
	int m_sectors;

	/// the axes style: 12 or 8
	bool m_flag_12axes;

	/// Write the python script
	void writePython() const;
};

} /* namespace jlib */
#endif /* POLARPLOTPYTHONWRITER_H_ */
