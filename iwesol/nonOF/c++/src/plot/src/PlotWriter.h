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

#ifndef PLOTWRITER_H_
#define PLOTWRITER_H_

#include "Outputable.h"

namespace jlib {

class Plot;

/**
 * @class jlib::PlotWriter
 * @brief This class writes a plot to pdf, via gnuplot.
 */
class PlotWriter:
	public Outputable{

public:

	/// languages
	static const int NONE,ENGLISH, GERMAN;

	/// returns language
	static int language(const std::string & l);

	/// returns language
	static std::string language(int l);

	/// gnuplot line type exceptions
	static bool gnuLinetypeVeto(int lt);

	/// Constructor
	PlotWriter(
			Plot const * plot
			);

	/// Destuctor
	virtual ~PlotWriter();

	/// Outputable: Provides the data that will be written to the output file.
	virtual Outputable::OData getOData(
			const std::string & type = IO::OFILE::TYPE::EMPTY) const;

	/// PlotWriter: create the plot
	virtual void write() const;

	/// PlotWriter: optional last step in write process
	virtual void writePostCare() const {}


protected:

	/// the plot
	Plot const * m_plot;

	/// the folder name
	std::string m_folder;

	/// the line type counter
	mutable int m_ltCounter;

	/// the band cols: (x,min,max)
	mutable std::vector< std::vector< int > > m_bandCols;

	/// PlotWriter: write the data file
	virtual void writeData() const;

	/// PlotWriter: write the gnuplot file
	virtual void writeGnuplot() const;

	/// PlotWriter: write the shell script
	virtual void writeScript() const;

	/// PlotWriter: run the shell script
	virtual void runScript() const;

	/// PlotWriter: add band script
	void addBandsToGnuScript(
			std::string & gnuScript,
			const std::string & base
			) const;

	/// count line type
	inline void ltCount() const;

	/// disallow copy
	PlotWriter(const PlotWriter&);
};

inline void PlotWriter::ltCount() const{
	m_ltCounter++;
	while(gnuLinetypeVeto(m_ltCounter)){
		m_ltCounter++;
	}
}

} /* namespace jlib */
#endif /* PLOTWRITER_H_ */
