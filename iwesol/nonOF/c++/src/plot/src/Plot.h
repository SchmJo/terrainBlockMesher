/*---------------------------------------------------------------------------*\
                               |
  _____        _______ ____    | IWESOL: IWES Open Library
 |_ _\ \      / / ____/ ___|   |
  | | \ \ /\ / /|  _| \___ \   | Copyright: Fraunhofer Institute for Wind
  | |  \ V  V / | |___ ___) |  | Energy and Energy System Technology IWES
 |___|  \_/\_/  |_____|____/   |
                               | http://www.iwes.fraunhofer.de
                               |
-------------------------------------------------------------------------------
License
    This file is part of IWESOL.

    IWESOL is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    IWESOL is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with IWESOL.  If not, see <http://www.gnu.org/licenses/>.

Class

Description

SourceFiles

References

\*---------------------------------------------------------------------------*/

#ifndef PLOT_H_
#define PLOT_H_

#include "Outputable.h"
#include "PlotLineInfo.h"
#include "HashTable.h"
#include "JString.h"
#include "LogPlotWriter.h"
#include "PlotTableWriter.h"
#include "PolarPlotWriter.h"
#include "PolarPlotPythonWriter.h"
#include "PlotHistogramWriter.h"
#include "GnuplotFitInfo.h"

namespace jlib {

/**
 * @class Plot
 * @brief Basic plot data.
 *
 */
class Plot{

public:

	/// declare friendship with writers
	friend class PlotWriter;
	friend class LogPlotWriter;
	friend class PlotTableWriter;
	friend class PolarPlotWriter;
	friend class PolarPlotPythonWriter;
	friend class PlotHistogramWriter;

	/// defining a column
	typedef std::vector<double> column;

	/// Constructor
	Plot(
		const std::string & name = "",
		const std::string & writeToFolder = ".",
		const std::string & label_x = "x-Axis",
		const std::string & label_y = "y-Axis",
		const double & xRangeMin = 0,
		const double & xRangeMax = 0,
		const double & yRangeMin = 0,
		const double & yRangeMax = 0
		);

	/// Destructor
	virtual ~Plot();

	/// Writes pdf
	inline void write() const { PlotWriter(this).write(); }

	/// writes log plot
	void writeLogPlot(
			bool setLogX,
			bool setLogY
			) const;

	/// Writes histogram pdf
	inline void writeHistogram(
			const double & binWidth
			) const;

	/// Writes a table into a txt file
	inline void writeTable(
			int colWidth_x = 13,
			int colWidth_y = 13,
			int precision_x = 2,
			int precision_y = 2,
			bool scientific_x = false,
			bool scientific_y = true
			) const;

	/// Writes polar plot pdf
	inline void writePolar(
			const double & tickInterval,
			int angleMode = PolarPlotWriter::DEGREES,
			int startAngle = PolarPlotWriter::EAST,
			int rotDirection = PolarPlotWriter::COUNTER_CLOCKWISE
			) const;

	/// Writes polar plot pdf
	inline void writePolarPython(
			int sectors,
			bool flag_12axes
			) const;

	/// add a line
	void addLine(const PlotLineInfo & line);

	/// add a line
	void addLine(
			Plotable const * source,
			int column_x,
			int column_y,
			std::string name
			);

	/// add a line
	void addLine(
			Plotable const * source_x,
			Plotable const * source_y,
			int column_x,
			int column_y,
			std::string name
			);

	/// Returns the plot name
	const std::string & getName() const { return m_name; }

	/// Returns the number of lines
	unsigned int size() const { return m_lines.size(); }

	/// Checks if empty
	bool empty() const { return m_lines.empty(); }

	/// request a marker
	inline void requestMarker(
			const double & x,
			const double & y,
			const std::string & markerLabel = ""
			);

	/// requests a title from the writer
	void requestTitle(const std::string & title) { m_title = title; }

	/// requests a vertical line from the writer
	void requestVerticalLine(const double & xLine) { m_vLines.push_back(xLine); }

	/** requests to fit a curve from the writer.
	 *
	 *  fitFunction_of_x is the right-hand-side of a function definition f(x):
	 *
	 *           f(x) = fitFunction_of_x
	 *
	 *   eg. fitFunction_of_x = "A * x + B". The parameter list in this case
	 *   would be fitParameterList = ("A", "B"). The fit parameters can be addressed
	 *   after plotting by the method fitResult(lineI).
	 */
	inline void requestFit(
			int lineI,
			const GnuplotFitInfo & fitInfo,
			const double & fitRangeMin = 0,
			const double & fitRangeMax = 0
			);

	/// request a data band
	void requestBand(
			Plotable const * source_x,
			Plotable const * source_bandMin,
			Plotable const * source_bandMax,
			int column_x,
			int column_bandMin,
			int column_bandMax
			);

	/// gives the parameter after fit. returns success of fit.
	inline bool fitResults(
			int lineI,
			std::vector< double > & fitParameterList
			) const;

	/// returns the requested language, for values see PlotWriter
	inline int language() const { return m_language; }

	/// sets the requested language, for values see PlotWriter
	inline void setLanguage(int language) { m_language = language; }


private:

	/// The name
	std::string m_name;

	/// The requested language
	int m_language;

	/// The requested title
	std::string m_title;

	/// Requested vertical line positions
	std::vector< double > m_vLines;

	/// The folder
	std::string m_folder;

	/// The x axis label
	std::string m_label_x;

	/// The y axis label
	std::string m_label_y;

	/// the x range (min,max)
	std::vector< double > m_xRange;

	/// the y range (min,max)
	std::vector< double > m_yRange;

	/// The line info list
	std::vector<PlotLineInfo> m_lines;

	/// The data
	std::vector<column> m_data;

	/// The names of the data columns
	std::vector<std::string> m_dataNames;

	/// the markers
	std::vector< std::vector< double > > m_markers;

	/// the marker names
	std::vector< std::string > m_markerNames;

	/// the column addresses of the x and y axes. key = "iLine.X" or "iLine.Y"
	HashTable< int, std::string > m_adr;

	/// the fitting requests
	HashTable< GnuplotFitInfo, int > m_fitRequests;

	/// the fitting results
	mutable HashTable< std::vector< double >, int > m_fitResults;

	/// the fitting ranges (xMin,xMax)
	HashTable< std::vector< double >, int > m_fitRanges;

	/// requested band
	std::vector< PlotLineInfo > m_source_bandMin;

	/// requested band source
	std::vector< PlotLineInfo > m_source_bandMax;

	/// requested band x data link
	std::vector< int > m_xadr_band;

	/// the key for m_adr
	inline std::string key(int i, const std::string & XorY) const;

	/// adds a column, if new data content. returns data position.
	int m_addCol(const column & c, const std::string & cname);

	/// checks if a column exists already, if so returns position, else -1
	int m_colExists(const column & c);

};

inline std::string Plot::key(int i, const std::string & XorY) const{
	return jlib::String(i) + XorY;
}

inline void Plot::writeTable(
			int colWidth_x,
			int colWidth_y,
			int precision_x,
			int precision_y,
			bool scientific_x,
			bool scientific_y
			) const{
	PlotTableWriter
	(
			this,
			colWidth_x,
			colWidth_y,
			precision_x,
			precision_y,
			scientific_x,
			scientific_y
	).write();
}

inline void Plot::writeHistogram(
		const double & binWidth
		) const {
	PlotHistogramWriter(this,binWidth).write();
}

inline void Plot::writeLogPlot(
			bool setLogX,
			bool setLogY
			) const{
	LogPlotWriter(this,setLogX,setLogY).write();
}

inline void Plot::writePolar(
		const double & tickInterval,
		int angleMode,
		int startAngle,
		int rotDirection
		) const {
	PolarPlotWriter
	(
			this,
			tickInterval,
			angleMode,
			startAngle,
			rotDirection
	).write();
}

inline void Plot::writePolarPython(
		int sectors,
		bool flag_12axes
		) const {
	PolarPlotPythonWriter
	(
			this,
			sectors,
			flag_12axes
	).write();
}

inline void Plot::requestFit(
			int lineI,
			const GnuplotFitInfo & fitInfo,
			const double & fitRangeMin,
			const double & fitRangeMax
			){
	m_fitRequests.set(lineI,fitInfo);
	std::vector< double > range(2);
	range[0] = fitRangeMin;
	range[1] = fitRangeMax;
	m_fitRanges.set(lineI,range);
}

inline bool Plot::fitResults(
		int lineI,
		std::vector< double > & fitParameterList
		) const{
	if(!m_fitResults.found(lineI)) return false;
	fitParameterList = m_fitResults[lineI];
	return true;
}

inline void Plot::requestMarker(
		const double & x,
		const double & y,
		const std::string & markerLabel
		){

	std::vector< double > marker(2,x);
	marker[1] = y;
	m_markers.push_back(marker);
	m_markerNames.push_back(markerLabel);
}


} /* namespace jFoam */
#endif /* PLOT_H_ */
