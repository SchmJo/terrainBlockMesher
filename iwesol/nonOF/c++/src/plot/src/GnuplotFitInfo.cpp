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

#include "GnuplotFitInfo.h"
#include "JString.h"

namespace jlib {

GnuplotFitInfo::GnuplotFitInfo(
		const std::string & name,
		const std::vector< std::string > & arguments,
		const std::vector< std::string > & parameters,
		const std::vector< double > & startingValues,
		const std::string & functionDefinition,
		const std::vector< std::string > & legendsAllLanguages
		):
		m_name(name),
		m_arguments(arguments),
		m_parameters(parameters),
		m_startingValues(startingValues),
		m_functionDefinition(functionDefinition),
		m_legendsAllLanguages(legendsAllLanguages){
}

std::string GnuplotFitInfo::functionWithArguments() const{
	std::string out = m_name + "(";
	for(int i = 0; i < int(m_arguments.size()); i++){
		if(i > 0) out += ", ";
		out += m_arguments[i];
	}
	out += ")";
	return out;
}

std::string GnuplotFitInfo::parameterList() const{
	std::string out = "";
	for(int i = 0; i < int(m_parameters.size()); i++){
		if(i > 0) out += ", ";
		out += m_parameters[i];
	}
	return out;
}

std::string GnuplotFitInfo::gnuplot_defineFunction() const{
	return functionWithArguments() +  " = " + m_functionDefinition;
}

std::string GnuplotFitInfo::legendName() const{
	return "ti" + m_name;
}

std::string GnuplotFitInfo::gnuplot_setStartingValues() const{
	std::string out = "";
	for(int pI = 0; pI < int(m_parameters.size());pI++){
		if(pI > 0) out += "\n";
		out += m_parameters[pI] + " = " + jlib::String(m_startingValues[pI]);
	}
	return out;
}

std::string GnuplotFitInfo::gnuplot_defineLegend(int languageI) const{
	std::string createTitle = legendName() + " = sprintf(" + String('"') + "%s (";
	for(int pI = 0; pI < int(m_parameters.size());pI++){
		if(pI > 0) createTitle += ", ";
		createTitle += "%s = %.2f";
	}
	createTitle += ")" + String('"') + ", '" + m_legendsAllLanguages[languageI] + "', ";
	for(int pI = 0; pI < int(m_parameters.size());pI++){
		if(pI > 0)createTitle += ", ";
		createTitle += "'" + m_parameters[pI] + "', ";
		createTitle += m_parameters[pI];
	}
	createTitle += ")";
	return createTitle;
}

std::string GnuplotFitInfo::gnuplot_fit(
		const std::string & dataInput
		) const{
	return "fit " + functionWithArguments()
			+ " " + dataInput
			+ " via " + parameterList();
}

std::string GnuplotFitInfo::gnuplot_plot(
		const std::string & dataInput,
		const std::string & dataLineStyle,
		const std::string & fitLineStyle,
		int languageI
		) const{
	return gnuplot_defineLegend(languageI) + "\nplot " + dataInput + " " + dataLineStyle
			+ ", " + functionWithArguments() + " " + fitLineStyle
			+ " title " + legendName();
}

std::string GnuplotFitInfo::gnuplotScript(
			const std::string & dataInput,
			const std::string & dataLineStyle,
			const std::string & fitLineStyle,
			int languageI
			) const{
	return gnuplot_defineFunction() + "\n"
			+ gnuplot_setStartingValues() + "\n"
			+ gnuplot_fit(dataInput) + "\n"
			+ gnuplot_plot(dataInput,dataLineStyle,fitLineStyle,languageI);
}

} /* namespace jFoam */
