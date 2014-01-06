/**
 * @file LogPlotWriter.cpp
 * @brief Definition/Implementation of class LogPlotWriter.
 *
 */

#include "LogPlotWriter.h"
#include "Plot.h"
using namespace std;

namespace jlib {

LogPlotWriter::LogPlotWriter(
		Plot const * plot,
		bool setLogX,
		bool setLogY
		):
		PlotWriter(plot),
		m_flag_xLog(setLogX),
		m_flag_yLog(setLogY){
}

Outputable::OData LogPlotWriter::getOData(
			const std::string & type) const{


	////////////////////////////////////////////////////////////
	//
	// 		gnuplot script file output:
	//
	if(type.compare(IO::OFILE::TYPE::GNUPLOT_SCRIPT) == 0){

		Outputable::OData out(1);
		IO::OFILE::Data   dat;

		// set basic info:
		dat.name   = m_plot->getName();
		dat.type   = type;
		dat.origin = "Plot";

		// set header:
		dat.header = IO::OFILE::PREAMBLE::GNUPLOT_SCRIPT;

		// prepare:
		string base = "./" + m_plot->m_name;

		// set pre- and post-data:
		dat.pre_data  = "# Plot '" + m_plot->getName() + "'";
		dat.pre_data += "\nset terminal epslatex color colortext standalone header '\\usepackage{amsmath}'";
		dat.pre_data += "\nset output '" + base + ".tex'";
		dat.pre_data += "\nset xlabel '" + m_plot->m_label_x + "'";
		dat.pre_data += "\nset ylabel '" + m_plot->m_label_y + "'";
		dat.pre_data += "\nset key under";
		if(m_flag_xLog) dat.pre_data += "\nset logscale x";
		if(m_flag_yLog) dat.pre_data += "\nset logscale y";
		if(m_plot->m_xRange[0] != m_plot->m_xRange[1]){
			dat.pre_data += "\nset xrange [" + String(m_plot->m_xRange[0])
					+ ":" + String(m_plot->m_xRange[1]) + "]";
		}
		if(m_plot->m_yRange[0] != m_plot->m_yRange[1]){
			dat.pre_data += "\nset yrange [" + String(m_plot->m_yRange[0])
					+ ":" + String(m_plot->m_yRange[1]) + "]";
		}
		dat.pre_data += "\nset style line 223 lt 3 lw 1 lc rgb " + String('"') + "black" + String('"');
		for(int i = 0; i < int(m_plot->m_vLines.size());i++){
			dat.pre_data += "\nset arrow from " + String(m_plot->m_vLines[i])
					  + ",graph(0,0) to " + String(m_plot->m_vLines[i])
					  + ",graph(1,1) nohead ls 223";
		}
		if(m_plot->m_title.size() > 0) {
			dat.pre_data += "\nset title '" + m_plot->m_title + "'";
			if(m_plot->size() == 1 && m_plot->m_fitRequests.size() == 0) dat.pre_data += "\nunset key";
		}

		if(m_plot->m_fitRequests.size() > 0){
			for(int i = 0; i < int(m_plot->m_fitRequests.size()); i++){
				int tc                    = m_plot->m_fitRequests.toc()[i];
				const GnuplotFitInfo & fr = m_plot->m_fitRequests[tc];
				int adr_x                 = m_plot->m_adr[m_plot->key(tc,"X")];
				int adr_y                 = m_plot->m_adr[m_plot->key(tc,"Y")];
				const std::vector< double > range = m_plot->m_fitRanges[tc];
				if(range[0] != range[1]){
					dat.pre_data += "\nset xrange [" + String(range[0]) + ":" + String(range[1]) + "]";
				}
				dat.pre_data += "\n" + fr.gnuplot_defineFunction();
				dat.pre_data += "\n" + fr.gnuplot_fit
						(
								"'" + base + ".dat' u "
								+ String(adr_x + 1) + ":"
								+ String(adr_y + 1)
						);
				dat.pre_data += "\n" + fr.gnuplot_defineLegend(m_plot->m_language);
				if(range[0] != range[1]){
					if(m_plot->m_xRange[0] != m_plot->m_xRange[1]){
						dat.pre_data += "\nset xrange [" + String(m_plot->m_xRange[0])
								+ ":" + String(m_plot->m_xRange[1]) + "]";
					} else {
						dat.pre_data += "\nset xrange [*:*]";
					}
				}
			}
		}

		// marker:
		if(m_plot->m_markers.size() > 0){
			for(unsigned int i = 0; i < m_plot->m_markers.size(); i++){
				dat.pre_data += "\nset label " + String(i + 1)
						+ " '" + m_plot->m_markerNames[i]
						+ "' at " + String(m_plot->m_markers[i][0]) + ","
						+ String(m_plot->m_markers[i][1])
						+ " point pointtype 2 lw 2 front";
			}
		}

		// data:
		dat.data = "\nplot";
		for(int i = 0; i < int(m_plot->size()); i++){
			const PlotLineInfo & pli = m_plot->m_lines[i];
			int adr_x                = m_plot->m_adr[m_plot->key(i,"X")];
			int adr_y                = m_plot->m_adr[m_plot->key(i,"Y")];
			dat.data +=" '" + base + ".dat' u " + String(adr_x + 1) + ":" + String(adr_y + 1)
					+ " w l lw 4 lt " + String(m_ltCounter) + " "
					+ "ti '" + pli.name + "'";
			ltCount();
			if(m_plot->m_fitRequests.found(i)){
				const GnuplotFitInfo & fr = m_plot->m_fitRequests[i];
				dat.data += ", " + fr.functionWithArguments()
						+ " lt " + String(m_ltCounter)
						+ " lw 4 title " + fr.legendName();
				ltCount();
			}
			if(i < int(m_plot->size()) - 1) dat.data += ",";
		}

		out[0] = dat;
		return out;
	}

	return PlotWriter::getOData(type);
}

}
