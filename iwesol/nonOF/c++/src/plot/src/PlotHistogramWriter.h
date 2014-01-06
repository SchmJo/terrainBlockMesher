/**
 * @file PlotHistogramWriter.h
 * @brief This file defines/implements the class PlotHistogramWriter.
 */

#ifndef PLOTHISTOGRAMWRITER_H_
#define PLOTHISTOGRAMWRITER_H_

#include "PlotWriter.h"

namespace jlib {

/**
 * @class jlib::PlotHistogramWriter
 * @brief Histogram style pdf writer, via gnuplot.
 */
class PlotHistogramWriter:
	public PlotWriter{

public:

	/// Constructor
	PlotHistogramWriter(
			Plot const * plot,
			const double & binWidth
			);

	/// Destructor
	virtual ~PlotHistogramWriter(){}

	/// Outputable: Provides the data that will be written to the output file.
	Outputable::OData getOData(
			const std::string & type = IO::OFILE::TYPE::EMPTY) const;

	/// PlotWriter: optional last step in write process
	void writePostCare() const;


private:

	/// the bin width
	double m_binWidth;

};

} /* namespace jlib */
#endif /* PLOTHISTOGRAMWRITER_H_ */
