/**
 * @file RRFunction.h
 * @brief Definition/Implementation of class RRFunction.
 *
 */

#ifndef RRFUNCTION_H_
#define RRFUNCTION_H_

#include "GenericRFunction.h"
#include "Plotable.h"

/**
 * @class jlib::RRFunction
 * @brief A function f: R -> R
 */
namespace jlib {

class RRFunction:
	public GenericRFunction< double >,
	public Plotable{

public:

		/// Constructor
		RRFunction(
				int interpolOrder = 4,
				const std::string & name = "RRFunction"
				);

		/// Constructor
		RRFunction(
				const std::vector< double > xvals,
				const std::vector< double > yvals,
				int interpolOrder = 4,
				const std::string & name = "RRFunction"
				);

		/// Plotable: Returns the number of columns
		inline int getPlotColumns() const { return 2 + GenericRFunction< double >::highestDerivative(); }

		/// Plotable: Returns columns no. i as vector of doubles
		std::vector< double > getPlotColumn(int i) const;

		/// Plotable: Returns label of columns no. i
		inline std::string getPlotColumnLabel(int i) const;

		/// set number of points in plot. zero = as many as size (default)
		inline void setPlotPoints(int plotPoints) const { m_plotPoints = plotPoints; }


private:

		/// the number of points in plot
		mutable int m_plotPoints;
};

inline std::string RRFunction::getPlotColumnLabel(int i) const{
	assert(i >= 0 && i < getPlotColumns());
	if(i == 0) return "x";
	if(i == 1) return "f(x)";
	return "fd" + String(i - 1) + "(x)";
}

}

#endif /* RRFUNCTION_H_ */
