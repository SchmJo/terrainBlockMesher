/**
 * @file PolarPlotWriter.cpp
 * @brief This file defines/implements the class PolarPlotWriter.
 */

#include "PolarPlotWriter.h"
#include "Plot.h"
using namespace std;

namespace jlib {

const int PolarPlotWriter::RADIANS = 2;
const int PolarPlotWriter::DEGREES = 3;

const int PolarPlotWriter::EAST    = 4;
const int PolarPlotWriter::NORTH   = 5;

const int PolarPlotWriter::CLOCKWISE         = 6;
const int PolarPlotWriter::COUNTER_CLOCKWISE = 7;

PolarPlotWriter::PolarPlotWriter(
		Plot const * plot,
		const double & tickInterval,
		int angleMode,
		int startAngle,
		int rotDirection
		):
		PlotWriter(plot),
		m_tickInterval(tickInterval),
		m_angleMode(angleMode),
		m_startAngle(startAngle),
		m_rotDirection(rotDirection){
	update();
}

Outputable::OData PolarPlotWriter::getOData(
			const std::string & type) const{

	Outputable::OData out(1);
	IO::OFILE::Data   dat;

	// set basic info:
	dat.name   = m_plot->getName();
	dat.type   = type;
	dat.origin = "Plot";

	////////////////////////////////////////////////////////////
	//
	// 		gnuplot data file output:
	//
	if(type.compare(IO::OFILE::TYPE::GNUPLOT_DATA) == 0){
		return PlotWriter::getOData(type);
	}

	////////////////////////////////////////////////////////////
	//
	// 		gnuplot script file output:
	//
	if(type.compare(IO::OFILE::TYPE::GNUPLOT_SCRIPT) == 0){

		// set header:
		dat.header = IO::OFILE::PREAMBLE::GNUPLOT_SCRIPT;

		// prepare:
		string base = "./" + m_plot->m_name;

		// set pre- and post-data:
		dat.pre_data  = "# Plot '" + m_plot->getName() + "'";
		dat.pre_data += "\nset terminal epslatex color colortext standalone header '\\usepackage{amsmath}'";
		dat.pre_data += "\nset output '" + base + ".tex'";
		dat.pre_data += "\nunset border";
		if(m_angleMode == DEGREES )dat.pre_data += "\nset angle degrees";
		dat.pre_data += "\nset xrange [" + String(-m_maxVal - m_tickInterval)
				+ ":"  + String(m_maxVal + m_tickInterval) + "]";
		dat.pre_data += "\nset yrange [" + String(-m_maxVal - m_tickInterval)
				+ ":"  + String(m_maxVal + m_tickInterval) + "]";
		dat.pre_data += "\nset size square";
		dat.pre_data += "\nset polar";
		dat.pre_data += "\nset grid polar";
		dat.pre_data += "\nset xtics axis";
		dat.pre_data += "\nset ytics axis";
		dat.pre_data += "\nset style line 1 lt 1 lw 25";
		dat.pre_data += "\nset xtics scale 0";
		dat.pre_data += "\nset xtics (";
		double tick = m_tickInterval;
		while(tick < m_maxVal){
			if(tick > m_tickInterval) dat.pre_data += ",";
			dat.pre_data += String('"') + String('"') + " " + String(tick);
			tick += m_tickInterval;
		}
		dat.pre_data += ", " + String('"') + String('"') + " " + String(tick) + ")";
		dat.pre_data += "\nset ytics 0, " + String(m_tickInterval) + ", " + String(tick);

		// data:
		dat.data = "\nplot";
		for(int i = 0; i < int(m_plot->size()); i++){
			const PlotLineInfo & pli = m_plot->m_lines[i];
			int adr_x                = m_plot->m_adr[m_plot->key(i,"X")];
			int adr_y                = m_plot->m_adr[m_plot->key(i,"Y")];
			string angleCol          = "$" + String(adr_x + 1);
			if(m_rotDirection == COUNTER_CLOCKWISE){
				angleCol = "+" + angleCol;
			} else {
				angleCol = "-" + angleCol;
			}
			if(m_startAngle == NORTH){
				if(m_angleMode == DEGREES){
					angleCol = "90. " + angleCol;
				} else {
					angleCol = "1.57079632679 " + angleCol;
				}
			}
			dat.data +=" '" + base + ".dat' u (" + angleCol + "):" + String(adr_y + 1)
					+ " with impulses ls 1 "
					+ "ti '" + pli.name + "'";
			if(i < int(m_plot->size()) - 1) dat.data += ",";
		}
	}

	////////////////////////////////////////////////////////////
	//
	// 		shell script file output:
	//
	if(type.compare(IO::OFILE::TYPE::SHELL_SCRIPT) == 0){
		return PlotWriter::getOData(type);
	}

	out[0] = dat;
	return out;
}

void PolarPlotWriter::update(){

	// determine data maximum:
	m_maxVal = 0;
	for(int l = 0; l < int(m_plot->size()); l++){

		// grab data column:
		int colNr                = m_plot->m_adr[m_plot->key(l,"Y")];
		const Plot::column & col = m_plot->m_data[colNr];

		// loop over data:
		for(int i = 0; i < int(col.size()); i++){

			// update maximum:
			const double & val = col[i];
			if( (l == 0 && i == 0) || val > m_maxVal){
				m_maxVal = val;
			}
		}
	}
}

} /* namespace jlib */
