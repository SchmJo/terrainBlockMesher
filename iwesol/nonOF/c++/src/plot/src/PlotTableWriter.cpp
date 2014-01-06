/**
 * @file PlotTableWriter.cpp
 * @brief This file defines/implements the class PlotTableWriter.
 */

#include <string>
#include <iomanip>
using namespace std;

#include "PlotTableWriter.h"
#include "OManager.h"
#include "Plot.h"

namespace jlib {

PlotTableWriter::PlotTableWriter(
		Plot const * plot,
		int m_colWidth_x,
		int m_colWidth_y,
		int m_precision_x,
		int m_precision_y,
		bool m_scientific_x,
		bool m_scientific_y
		):
		PlotWriter(plot),
		m_colWidth_x(m_colWidth_x),
		m_colWidth_y(m_colWidth_y),
		m_precision_x(m_precision_x),
		m_precision_y(m_precision_y),
		m_scientific_x(m_scientific_x),
		m_scientific_y(m_scientific_y){
}

Outputable::OData PlotTableWriter::getOData(
			const std::string & type) const{
	Outputable::OData out(1);
	IO::OFILE::Data   dat;

	// set basic info:
	dat.name   = m_plot->getName();
	dat.type   = type;
	dat.origin = "Plot";

	////////////////////////////////////////////////////////////
	//
	// 		text file output (a table):
	//
	if(type.compare(IO::OFILE::TYPE::TXT) == 0){

		// set header:
		dat.header = "Table: " + m_plot->m_name;

		// set pre- and post-data:
		{
			dat.post_data = "";
			std::stringstream ss;
			ss	<< "\n"
				<< setfill('-')
				<< setw(m_colWidth_x + m_colWidth_y * m_plot->size() + 1 )
				<< "\n"
				<< setfill(' ');
			for(int i = 0; i < int(m_plot->size()); i++){
				const PlotLineInfo & pli  = m_plot->m_lines[i];
				if(i == 0){
					ss 	<< setw(m_colWidth_x)
						<< right
						<< pli.source_x->getPlotColumnLabel(pli.column_x);
				}
				ss 	<< setw(m_colWidth_y)
					<< right
					<< pli.source_y->getPlotColumnLabel(pli.column_y);
			}
			ss << "\n";
			for(int i = 0; i < int(m_plot->size()); i++){
				if(i == 0){
					ss 	<< setw(m_colWidth_x)
						<< right
						<< m_plot->m_label_x;
				}
				ss 	<< setw(m_colWidth_y)
					<< right
					<< m_plot->m_label_y;
			}
			ss	<< "\n"
				<< setfill('-')
				<< setw(m_colWidth_x + m_colWidth_y * m_plot->size() )
				<< "";
			dat.pre_data = ss.str();
		}


		// set data:
		{
			dat.data       = "";
			int row        = 0;
			bool dataFound = true;
			while(dataFound){
				dataFound = false;
				std::stringstream ss;
				ss << "\n";
				for(int i = 0; i < int(m_plot->size()); i++){
					if(i == 0){
						int adr_x          = m_plot->m_adr[m_plot->key(i,"X")];
						const Plot::column & col = m_plot->m_data[adr_x];
						if(row < int(col.size())){
							dataFound = true;
							if(m_scientific_x) ss << setiosflags(ios::scientific);
							else ss << setiosflags(ios::fixed);
							ss 	<< setprecision(m_precision_x)
								<< setw(m_colWidth_x)
								<< right
								<< m_plot->m_data[adr_x][row];
						}
					}
					int adr_y          = m_plot->m_adr[m_plot->key(i,"Y")];
					const Plot::column & col = m_plot->m_data[adr_y];
					if(row < int(col.size())){
						dataFound = true;
						if(m_scientific_y) ss << setiosflags(ios::scientific);
						else ss << setiosflags(ios::fixed);
						ss 	<< setprecision(m_precision_y)
							<< setw(m_colWidth_y)
							<< right
							<< m_plot->m_data[adr_y][row];
					} else if( dataFound == true ){
						ss 	<< setw(m_colWidth_y)
							<< right
							<< "NaN";
					}
				}
				if(dataFound){
					row++;
					dat.data += ss.str();
				}
			}
		}
	}

	out[0] = dat;
	return out;
}

void PlotTableWriter::write() const{

	string filename = m_folder + "/" + m_plot->m_name + ".txt";

	// set up OManager:
	OManager om(filename,IO::OFILE::TYPE::TXT);
	om.addOLink(this);
	om.collectAll();
	om.write();
}

} /* namespace jlib */
