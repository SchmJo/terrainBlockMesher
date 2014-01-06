/**
 * @file GnuplotFitInfo.H
 * @brief This file defines/implements the class GnuplotFitInfo.
 */

#ifndef GNUPLOTFITINFO_H_
#define GNUPLOTFITINFO_H_

#include <string>
#include <vector>

namespace jlib {

/**
 * @class jlib::GnuplotFitInfo
 * @brief A collection of what is needed for gnuplot function fits.
 *
 * functionName(arguments) = functionDefinition(arguments,parameters)
 */
class GnuplotFitInfo {

public:

	/// Constructor
	GnuplotFitInfo(
			const std::string & name = "",
			const std::vector< std::string > & arguments = std::vector< std::string >(),
			const std::vector< std::string > & parameters = std::vector< std::string >(),
			const std::vector< double > & startingValues = std::vector< double >(),
			const std::string & functionDefinition = "",
			const std::vector< std::string > & legendsAllLanguages = std::vector< std::string >()
			);

	/// return the function name
	inline const std::string & functionName() const { return m_name; }

	/// return the function arguments
	inline const std::vector< std::string > & functionArguments() const { return m_arguments; }

	/// return the function parameter names
	inline const std::vector< std::string > & fitParameters() const { return m_parameters; }

	/// return the parameter starting values
	inline const std::vector< double > & startingValues() const { return m_startingValues; }

	/// return the function name
	inline const std::string & function() const { return m_functionDefinition; }

	/// checks if empty
	inline bool empty() const { return m_parameters.empty(); }

	/// returns legend name
	std::string legendName() const;

	/// returns function with arguments
	std::string functionWithArguments() const;

	/// returns a lilst of parameters A, B, ...
	std::string parameterList() const;

	/// returns function definition line
	std::string gnuplot_defineFunction() const;

	/// returns lines for setting staring values
	std::string gnuplot_setStartingValues() const;

	/// returns fit legend defining line
	std::string gnuplot_defineLegend(int languageI) const;

	/// returns the gnuplot fit command. dataInput = 'file' u i:j
	std::string gnuplot_fit(const std::string & dataInput) const;

	/** returns the gnuplot plot command.
	 *
	 * dataInput = 'file' u i:j.
	 * line styles = wl lw 4 ...
	 */
	std::string gnuplot_plot(
			const std::string & dataInput,
			const std::string & dataLineStyle,
			const std::string & fitLineStyle,
			int languageI
			) const;

	/** returns gnuplot script
	 *
	 * dataInput = 'file' u i:j.
	 * line styles = wl lw 4 ...
	 */
	std::string gnuplotScript(
			const std::string & dataInput,
			const std::string & dataLineStyle,
			const std::string & fitLineStyle,
			int languageI
			) const;


private:

	/// the function name
	std::string  m_name;

	/// the function arguments
	std::vector< std::string >  m_arguments;

	/// the parameters to be fitted
	std::vector< std::string >  m_parameters;

	/// the starting values
	std::vector< double >  m_startingValues;

	/// the right hand side of the f(params) = RHS
	std::string m_functionDefinition;

	/// the function legend in all languages, see PlotWriter
	std::vector< std::string >  m_legendsAllLanguages;
};


} /* namespace jFoam */
#endif /* GNUPLOTFITINFO_H_ */
