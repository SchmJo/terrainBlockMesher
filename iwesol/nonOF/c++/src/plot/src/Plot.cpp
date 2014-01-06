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

#include "Plot.h"
#include "OManager.h"
using namespace std;

namespace jlib {

Plot::Plot(
		const std::string & name,
		const std::string & folder,
		const std::string & label_x,
		const std::string & label_y,
		const double & xRangeMin,
		const double & xRangeMax,
		const double & yRangeMin,
		const double & yRangeMax
		):
		m_name(name),
		m_language(PlotWriter::ENGLISH),
		m_folder(folder),
		m_label_x(label_x),
		m_label_y(label_y),
		m_xRange(2),
		m_yRange(2){
	m_xRange[0] = xRangeMin;
	m_xRange[1] = xRangeMax;
	m_yRange[0] = yRangeMin;
	m_yRange[1] = yRangeMax;
}

Plot::~Plot() {
}

int Plot::m_colExists(const column & c){

	// trivial check:
	if(m_data.size() == 0) return -1;

	// check:
	for(int i = 0; i < int(m_data.size()); i++){
		if(m_data[i].size() == c.size()){
			bool found = true;
			for(int j = 0; j < int(m_data[i].size()); j++){
				if(m_data[i][j] != c[j]) {
					found = false;
					break;
				}
			}
			if(found) return i;
		}
	}

	return -1;
}

int Plot::m_addCol(const column & c, const std::string & cname){

	// check if column exists:
	int pos = m_colExists(c);

	// case new:
	if(pos < 0){
		m_data.push_back(c);
		m_dataNames.push_back(cname);
		pos = m_data.size() - 1;
	}

	return pos;
}

void Plot::addLine(const PlotLineInfo & line) {

	// add line info:
	m_lines.push_back(line);

	// add data:
	int pos_x = m_addCol(
			line.source_x->getPlotColumn(line.column_x),
			line.source_x->getPlotColumnLabel(line.column_x)
			);

	int pos_y = m_addCol(
			line.source_y->getPlotColumn(line.column_y),
			line.source_y->getPlotColumnLabel(line.column_y)
			);

	// write addresses:
	m_adr.set(key(size() - 1,"X"),pos_x);
	m_adr.set(key(size() - 1,"Y"),pos_y);
}

void Plot::addLine(
			Plotable const * source,
			int column_x,
			int column_y,
			std::string name
			){
	addLine(
			source,
			source,
			column_x,
			column_y,
			name
			);
}

void Plot::addLine(
			Plotable const * source_x,
			Plotable const * source_y,
			int column_x,
			int column_y,
			std::string name
			) {
	PlotLineInfo pli;
	pli.source_x = source_x;
	pli.source_y = source_y;
	pli.column_x = column_x;
	pli.column_y = column_y;
	pli.name     = name;
	addLine(pli);
}

void Plot::requestBand(
			Plotable const * source_x,
			Plotable const * source_bandMin,
			Plotable const * source_bandMax,
			int column_x,
			int column_bandMin,
			int column_bandMax
			){
	PlotLineInfo pli;
	pli.source_x = source_x;
	pli.column_x = column_x;
	pli.source_y = source_bandMin;
	pli.column_y = column_bandMin;
	pli.name     = "bandMin";
	m_source_bandMin.push_back(pli);
	pli.source_y = source_bandMax;
	pli.column_y = column_bandMax;
	pli.name     = "bandMax";
	m_source_bandMax.push_back(pli);
	int xadr = -1;
	for(unsigned int i = 0; i < m_lines.size(); i++){
		if(m_lines[i].source_x == source_x && m_lines[i].column_x == column_x){
			xadr = i;
			break;
		}
	}
	m_xadr_band.push_back(xadr);
}

} /* namespace jFoam */
