/**
 * @file Plotable.h
 * @brief Definition/Implementation of class Plotable.
 *
 */

#ifndef PLOTABLE_H_
#define PLOTABLE_H_

#include <vector>
#include <string>

namespace jlib {

/**
 * @class Plotable
 * @brief Interface for classes that can be plotted using the class Plot.
 *
 */
class Plotable {

public:

	/// Constructor
	Plotable();

	/// Destructor
	virtual ~Plotable();

	/// Plotable: Returns the number of columns
	virtual int getPlotColumns() const = 0;

	/// Plotable: Returns columns no. i as vector of doubles
	virtual std::vector<double> getPlotColumn(int i) const = 0;

	/// Plotable: Returns label of columns no. i
	virtual std::string getPlotColumnLabel(int i) const = 0;

	/// checks if plot is empty
	inline bool emptyPlot() const { return getPlotColumns() == 0; }

};

} /* namespace jlib */
#endif /* PLOTABLE_H_ */
