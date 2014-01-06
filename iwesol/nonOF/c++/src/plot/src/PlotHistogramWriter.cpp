/**
 * @file PlotHistogramWriter.cpp
 * @brief This file defines/implements the class PlotHistogramWriter.
 */

#include "PlotHistogramWriter.h"
#include "Plot.h"
#include "IO.h"
#include <sstream>
using namespace std;

namespace jlib {

PlotHistogramWriter::PlotHistogramWriter(
		Plot const * plot,
		const double & binWidth
		):
		PlotWriter(plot),
		m_binWidth(binWidth){
}

Outputable::OData PlotHistogramWriter::getOData(
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
		dat.pre_data += "\nset xlabel '" + m_plot->m_label_x + "'";
		dat.pre_data += "\nset ylabel '" + m_plot->m_label_y + "'";
		if(m_plot->m_xRange[0] != m_plot->m_xRange[1]){
			dat.pre_data += "\nset xrange [" + String(m_plot->m_xRange[0])
					+ ":" + String(m_plot->m_xRange[1]) + "]";
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
		dat.pre_data += "\nset border 3";
		dat.pre_data += "\nset boxwidth " + String(3./4. * m_binWidth) + " absolute";
		dat.pre_data += "\nset style fill solid 1.0 noborder";
		dat.pre_data += "\nset key under";
		if(m_plot->m_fitRequests.size() > 0){
			for(int i = 0; i < int(m_plot->m_fitRequests.size()); i++){
				int toc                   = m_plot->m_fitRequests.toc()[i];
				const GnuplotFitInfo & fr = m_plot->m_fitRequests[toc];
				int adr_x                 = m_plot->m_adr[m_plot->key(toc,"X")];
				int adr_y                 = m_plot->m_adr[m_plot->key(toc,"Y")];
				const std::vector< double > & range = m_plot->m_fitRanges[toc];
				if(range[0] != range[1]){
					dat.pre_data += "\nset xrange [" + String(range[0]) + ":" + String(range[1]) + "]";
				}
				dat.pre_data += "\n" + fr.gnuplot_defineFunction();
				dat.pre_data += "\n" + fr.gnuplot_setStartingValues();
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

		// data:
		dat.data = "\nplot";
		for(int i = 0; i < int(m_plot->size()); i++){
			const PlotLineInfo & pli = m_plot->m_lines[i];
			int adr_x                = m_plot->m_adr[m_plot->key(i,"X")];
			int adr_y                = m_plot->m_adr[m_plot->key(i,"Y")];
			dat.data +=" '" + base + ".dat' u " + String(adr_x + 1)
					+ ":" + String(adr_y + 1) + " smooth frequency with boxes"
					+ " lt " + String(m_ltCounter) + " "
					+ " ti '" + pli.name + "'";
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

void PlotHistogramWriter::writePostCare() const{

	// read fit data:
	if(m_plot->m_fitRequests.size() > 0){

		// loop over fit requests:
		for(int i = 0; i < int(m_plot->m_fitRequests.size()); i++){

			// prepare:
			int toc = m_plot->m_fitRequests.toc()[i];

			// read file:
			string fname = m_folder + "/log";
			vector< string > fs = jlib::IO::readAll(fname);

			// skip lines:
			vector< string >::const_iterator it = fs.begin();
			size_t sit = string::npos;
			for(it = fs.begin(); it != fs.end(); it++){
				sit = it->find("Final set of parameters");
				if (sit != string::npos) break;
			}

			// check:
			if(it == fs.end()){
				cout << "\nPlotHistogramWriter: Error: Cannot find fitting results in file " << fname << endl;
				throw;
			}

			// find parameters:
			int curpar = 0;
			vector < double > fitresults(int(m_plot->m_fitRequests[toc].fitParameters().size()));
			while(curpar != int(m_plot->m_fitRequests[toc].fitParameters().size()) && it != fs.end()){

				// search parameter in line:
				string pname = m_plot->m_fitRequests[toc].fitParameters()[curpar];
				sit = it->find(pname);

				// parameter found:
				if (sit != string::npos){

					// search equal sign:
					size_t nsit = it->find("=", sit + 1);

					// check:
					if (nsit == string::npos){
						cout << "\nPlotHistogramWriter: Error: Cannot find fitting results in file " << fname << endl;
						throw;
					}

					// consider substring after '=':
					string rs = it->substr(nsit + 1);

					// cast value:
					stringstream ss;
					ss << rs;
					ss >> fitresults[curpar];
					cout << "PlotHistogramWriter: fitted " << pname << " = " << fitresults[curpar] << endl;

					curpar++;
				}

				it++;
			}

			// check:
			if(curpar != int(m_plot->m_fitRequests[i].fitParameters().size())){
				cout << "\nPlotHistogramWriter: Error: Cannot find fitting results in file " << fname << endl;
				throw;
			}

			// set values:
			m_plot->m_fitResults.set(toc,fitresults);
		}
	}
}

} /* namespace jlib */
