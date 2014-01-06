/**
 * @file LogPlotWriter.h
 * @brief Definition/Implementation of class LogPlotWriter.
 *
 */

#ifndef LOGPLOTWRITER_H_
#define LOGPLOTWRITER_H_

#include "PlotWriter.h"

/**
 * @class jlib::LogPlotWriter
 * @brief Writes a log plot
 */
namespace jlib {

class LogPlotWriter:
	public PlotWriter{

public:

	/// Constructor
	LogPlotWriter(
			Plot const * plot,
			bool setLogX,
			bool setLogY
			);

	/// Outputable: Provides the data that will be written to the output file.
	Outputable::OData getOData(
			const std::string & type = IO::OFILE::TYPE::EMPTY) const;

private:

	/// flag for x log
	bool m_flag_xLog;

	/// flag for y log
	bool m_flag_yLog;
};

}

#endif /* LOGPLOTWRITER_H_ */
