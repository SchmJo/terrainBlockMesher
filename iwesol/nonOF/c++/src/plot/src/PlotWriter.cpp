/**
 * @file PlotWriter.cpp
 * @brief This file defines/implements the class PlotWriter.
 */

#include <string>
#include <cstdlib>
#include <iomanip>
#include <sstream>
using namespace std;

#include "PlotWriter.h"
#include "Plot.h"
#include "OManager.h"

namespace jlib {

const int PlotWriter::NONE = -1;
const int PlotWriter::ENGLISH = 0;
const int PlotWriter::GERMAN  = 1;

int PlotWriter::language(const std::string & l){
	for(int i = 0; i < 2; i++){
		if(l.compare(language(i)) == 0) return i;
	}
	return NONE;
}

std::string PlotWriter::language(int l){
	if(l == ENGLISH) return "English";
	if(l == GERMAN) return "German";
	return "None";
}

bool PlotWriter::gnuLinetypeVeto(int lt){
	if(lt == 2) return true;
	if(lt == 4) return true;
	if(lt == 5) return true;
	if(lt == 6) return true;
	if(lt == 10) return true;
	if(lt == 12) return true;
	return false;
}

PlotWriter::PlotWriter(
		Plot const * plot
		):
		m_plot(plot),
		m_folder(m_plot->m_folder + "/" + m_plot->m_name),
		m_ltCounter(1){

	// setup folders:
	string base  = m_plot->m_folder + "/" + m_plot->m_name;
	string log   = base + ".log";
	string tolog = " > " + log + " 2>&1";
	int temp     = 0;
	int c        = temp;
	c = std::system(String("mkdir " + m_plot->m_folder + tolog).c_str());
	c = std::system(String("mkdir " + m_plot->m_folder + "/" + m_plot->m_name + tolog).c_str());
	c = std::system(String("rm -f " + log).c_str());
	temp = c;

}

PlotWriter::~PlotWriter() {
}

Outputable::OData PlotWriter::getOData(
			const std::string & type
			) const{

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

		// set header:
		dat.header = IO::OFILE::PREAMBLE::GNUPLOT_DATA;

		// set pre- and post-data:
		dat.pre_data  = "# Data for plot '" + m_plot->getName() + "'";
		int bc = -1;
		m_bandCols.resize(m_plot->m_source_bandMin.size(),std::vector<int>(3,-1));
		for(int i = 0; i < int(m_plot->size()); i++){

			const PlotLineInfo & pli = m_plot->m_lines[i];
			int adr_x                = m_plot->m_adr[m_plot->key(i,"X")];
			int adr_y                = m_plot->m_adr[m_plot->key(i,"Y")];
			dat.pre_data += "\n#   plot line " + String(i) + " '" + pli.name
					+ "': X-data '" + pli.source_x->getPlotColumnLabel(pli.column_x) + "' in col " + String(adr_x + 1)
					+  ", Y-data '" + pli.source_y->getPlotColumnLabel(pli.column_y) + "' in col " + String(adr_y + 1);
			if(adr_x > bc) bc = adr_x;
			if(adr_y > bc) bc = adr_y;
		}

		for(int i = 0; i < int(m_plot->m_source_bandMin.size()); i++){
			bc++;
			const PlotLineInfo & pli = m_plot->m_source_bandMin[i];
			int adr_x                = m_plot->m_xadr_band[i];
			int adr_y                = bc;
			if(adr_x < 0){
				adr_x = adr_y;
				adr_y++;
				bc++;
			}
			dat.pre_data += "\n#   band " + String(i) + " min"
					+ ": X-data '" + pli.source_x->getPlotColumnLabel(pli.column_x) + "' in col " + String(adr_x + 1)
					+  ", Y-data '" + pli.source_y->getPlotColumnLabel(pli.column_y) + "' in col " + String(adr_y + 1);
			m_bandCols[i][0] = adr_x;
			m_bandCols[i][1] = adr_y;
			bc++;
			adr_y = bc;
			dat.pre_data += "\n#   band " + String(i) + " max"
					+ ": X-data '" + pli.source_x->getPlotColumnLabel(pli.column_x) + "' in col " + String(adr_x + 1)
					+  ", Y-data '" + pli.source_y->getPlotColumnLabel(pli.column_y) + "' in col " + String(adr_y + 1);
			m_bandCols[i][2] = adr_y;
		}
		dat.post_data = "";
		dat.data      = "";

		// data:
		int row        = 0;
		bool dataFound = true;
		std::vector< std::vector< double > > bandx(m_plot->m_source_bandMin.size());
		std::vector< std::vector< double > > bandMin(m_plot->m_source_bandMin.size());
		std::vector< std::vector< double > > bandMax(m_plot->m_source_bandMin.size());
		for(int bI = 0; bI < int(m_plot->m_source_bandMin.size()); bI++){
			const PlotLineInfo & pli   = m_plot->m_source_bandMin[bI];
			int adr_x                  = m_plot->m_xadr_band[bI];
			if(adr_x >= 0){
				bandx[bI] = pli.source_x->getPlotColumn(pli.column_x);
			}
			bandMin[bI] = pli.source_y->getPlotColumn(pli.column_y);
			const PlotLineInfo & pli2  = m_plot->m_source_bandMax[bI];
			bandMax[bI] = pli2.source_y->getPlotColumn(pli.column_y);
		}
		while(dataFound){
			dataFound = false;
			std::string newline = "\n";
			for(int i = 0; i < int(m_plot->m_data.size()); i++){
				const Plot::column & col = m_plot->m_data[i];
				if(row < int(col.size())){
					dataFound = true;
					newline += String(col[row]) + " ";
				} else {
					newline += "NaN ";
				}
			}
			for(int bI = 0; bI < int(m_plot->m_source_bandMin.size()); bI++){
				int adr_x = m_plot->m_xadr_band[bI];
				if(adr_x < 0){
					if(int(bandx[bI].size()) > row){
						newline  += String(bandx[bI][row]) + " ";
						dataFound = true;
					}
				}
				if(int(bandx[bI].size()) > row){
					newline  += String(bandMin[bI][row]) + " ";
					newline  += String(bandMax[bI][row]) + " ";
					dataFound = true;
				}
			}
			if(dataFound){
				dat.data += newline;
				row++;
			}
		}
		dat.pre_data += "\n# Data: " + String(row) + " rows, " + String(m_plot->m_data.size()) + " Plot::columns";
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
		dat.pre_data += "\nset key under";
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
		dat.data += "\nplot";
		addBandsToGnuScript(dat.data,base);
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
	}

	////////////////////////////////////////////////////////////
	//
	// 		shell script file output:
	//
	if(type.compare(IO::OFILE::TYPE::SHELL_SCRIPT) == 0){

		// set header:
		dat.header = IO::OFILE::PREAMBLE::SHELL_SCRIPT;

		// prepare:
		string base = "./" + m_plot->m_name;

		// set pre- and post-data:
		dat.pre_data  = "# This script creates the plot '" + m_plot->getName() + ".pdf'";

		// set data:
		dat.data  = "\ngnuplot " + base + ".dem";
		dat.data += "\nlatex " + base + ".tex";
		dat.data += "\ndvipdf " + base + ".dvi";
		dat.data += "\nrm -f " + base + ".dvi";
		dat.data += "\nrm -f " + base + ".aux";
		dat.data += "\nrm -f " + base + ".log";

	}

	out[0] = dat;
	return out;
}

void PlotWriter::addBandsToGnuScript(
		std::string & gnuScript,
		const std::string & base
		) const{

	for(unsigned int bI = 0; bI < m_plot->m_source_bandMin.size(); bI++){
		gnuScript += " '" + base + ".dat' u "
				  +  String(m_bandCols[bI][0] + 1) + ":"
				  +  String(m_bandCols[bI][1] + 1) + ":"
				  +  String(m_bandCols[bI][2] + 1) + " lc rgb 'light-gray' w filledcu notitle, ";
	}
}


void PlotWriter::writeData() const{

	// prepare:
	string filename = m_folder + "/" + m_plot->m_name + ".dat";

	// set up OManager:
	OManager om(filename,IO::OFILE::TYPE::GNUPLOT_DATA);
	om.addOLink(this);
	om.collectAll();
	om.write();
}

void PlotWriter::writeGnuplot() const{

	// prepare:
	string filename = m_folder + "/" + m_plot->m_name + ".dem";

	// set up OManager:
	OManager om(filename,IO::OFILE::TYPE::GNUPLOT_SCRIPT);
	om.addOLink(this);
	om.collectAll();
	om.write();
}

void PlotWriter::writeScript() const{

	// prepare:
	string filename = m_folder + "/" + m_plot->m_name + ".sh";

	// set up OManager:
	OManager om(filename,IO::OFILE::TYPE::SHELL_SCRIPT);
	om.addOLink(this);
	om.collectAll();
	om.write();
}

void PlotWriter::runScript() const{

	// prepare:
	string base  = m_folder + "/" + m_plot->m_name;
	string log   = "./log";
	string sh    = "./" + m_plot->m_name + ".sh";
	string tolog = " > " + log + " 2>&1";
	int temp     = 0;
	int c        = temp;

	// execute the shell script:
	cout << "Creating '" << base << ".pdf' ... " << flush;
	c = std::system(String("rm -f " + log).c_str());
	c = std::system(String("cd " + m_folder + "; bash " + sh + tolog).c_str());
	temp = c;
	cout << " done." << endl;
}


void PlotWriter::write() const{

	// write the data file:
	writeData();

	// write the gnuplot file:
	writeGnuplot();

	// write the shell script:
	writeScript();

	// run the shell script:
	runScript();

	// post-care:
	writePostCare();
}

} /* namespace jlib */
