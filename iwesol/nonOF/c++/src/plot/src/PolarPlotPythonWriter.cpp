/**
 * @file PolarPlotPythonWriter.cpp
 * @brief This file defines/implements the class PolarPlotPythonWriter.
 */

#include <string>
#include <cstdlib>
#include <iomanip>
#include <sstream>
using namespace std;

#include "PolarPlotPythonWriter.h"
#include "Plot.h"
#include "OManager.h"

namespace jlib {

PolarPlotPythonWriter::PolarPlotPythonWriter(
		Plot const * plot,
		int sectors,
		bool flag_12axes
		):
		PlotWriter(plot),
		m_sectors(sectors),
		m_flag_12axes(flag_12axes){

	// load data files:
	load();
}

Outputable::OData PolarPlotPythonWriter::getOData(
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
	// 		python script file output:
	//
	if(type.compare(IO::OFILE::TYPE::PYTHON_SCRIPT) == 0){

		// set header:
		dat.header = IO::OFILE::PREAMBLE::PYTHON_SCRIPT;

		// prepare:
		string base = "./" + m_plot->m_name;

		// set pre- and post-data:
		dat.pre_data  = "# This script creates the plot '" + m_plot->getName() + ".pdf'";

		// set data:
		dat.data = "\nfrom windrose import WindroseAxes";
		dat.data += "\nfrom matplotlib import pyplot as plt";
		dat.data += "\nimport matplotlib.cm as cm";
		dat.data += "\nfrom numpy.random import random";
		dat.data += "\nfrom numpy import arange";
		dat.data += "\nfrom pylab import savefig";
		dat.data += "\nimport scitools.filetable as ft\n";
		dat.data += "\ns = open('" + base + ".dat','r')";
		dat.data += "\n";
		int adr_x = m_plot->m_adr[m_plot->key(0,"X")];
		int adr_y = m_plot->m_adr[m_plot->key(0,"Y")];
		for(int i = 0; i < int(m_plot->m_data.size()); i++){
			if(i > 0) dat.data += ", ";
			if(i == adr_x){
				dat.data += "wd";
			} else if(i == adr_y){
				dat.data += "ws";
			} else {
				dat.data += "col" + String(i);
			}
		}
		dat.data += " = ft.read_columns(s)";
		dat.data += "\ns.close()\n";

		dat.data += "\ndef new_axes():";
		dat.data += "\n    fig = plt.figure(figsize=(8, 8), dpi=80, facecolor='w', edgecolor='w')";
		dat.data += "\n    rect = [0.1, 0.1, 0.8, 0.8]";
		dat.data += "\n    ax = WindroseAxes(fig, rect, axisbg='w', naxes12="
				+ jlib::String(m_flag_12axes ? "True" : "False") + ")";
		dat.data += "\n    fig.add_axes(ax)";
		dat.data += "\n    return ax\n";
		dat.data += "\ndef set_legend(ax):";
		dat.data += "\n    l = ax.legend(axespad=-0.10)";
		dat.data += "\n    plt.setp(l.get_texts(), fontsize=8)\n";
		dat.data += "\nax = new_axes()";
		dat.data += "\nax.bar(wd, ws, normed=True, opening=0.8, edgecolor='white', nsector="
				+ jlib::String(m_sectors) + ")";
		dat.data += "\nset_legend(ax)";
		dat.data += "\nsavefig('" + base + ".pdf')";
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
		dat.data += "\npython " + base + ".py";
	}

	out[0] = dat;
	return out;
}

void PolarPlotPythonWriter::writePython() const{

	// prepare:
	string filename = m_folder + "/" + m_plot->m_name + ".py";

	// set up OManager:
	OManager om(filename,IO::OFILE::TYPE::PYTHON_SCRIPT);
	om.addOLink(this);
	om.collectAll();
	om.write();
}

void PolarPlotPythonWriter::write() const{

	// write the data file:
	writeData();

	// write the python file:
	writePython();

	// write the shell script:
	writeScript();

	// run the shell script:
	runScript();
}

void PolarPlotPythonWriter::load(){

	// prepare:
	int temp = 0;
	int c    = temp;

	// copy data files:
	if(m_plot->m_language == ENGLISH){
		c = std::system
				(
						String
						(
								"cp $JLIB/jlib/data/windRose/windrose_en.py "
								+ m_folder + "/windrose.py"
						).c_str()
				);
	} else if(m_plot->m_language == GERMAN){
		c = std::system
				(
						String
						(
								"cp $JLIB/jlib/data/windRose/windrose_de.py "
								+ m_folder + "/windrose.py"
						).c_str()
				);
	} else {
		cout << "\nPolarPlotPythinWriter: Error: Language " << m_plot->m_language << " not supported." << endl;
		throw;
	}
	c = std::system(String("cp $JLIB/jlib/data/windRose/LICENCE " + m_folder).c_str());

	// clean up:
	temp = c;
}

} /* namespace jlib */
