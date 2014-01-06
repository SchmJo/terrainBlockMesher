/**
 * @file RRFunction.cpp
 * @brief Definition/Implementation of class RRFunction.
 *
 */

#include "RRFunction.h"

namespace jlib {

RRFunction::RRFunction(
		int interpolOrder,
		const std::string & name
		):
		GenericRFunction< double >
		(
				interpolOrder,
				name
		),
		m_plotPoints(0){
}

RRFunction::RRFunction(
		const std::vector< double > xvals,
		const std::vector< double > yvals,
		int interpolOrder,
		const std::string & name
		):
		GenericRFunction< double >
		(
				xvals,
				yvals,
				interpolOrder,
				name
		),
		m_plotPoints(0){
}

std::vector< double > RRFunction::getPlotColumn(int i) const{

	assert(i >= 0 && i < getPlotColumns());
	if(m_plotPoints <= 0){
		if(i == 0) return pointList();
		if(i == 1) return dataList();
		std::vector< double > out(size());
		for(unsigned int j = 0; j < size(); j++){
			out[j] = derivative
					(
							point(j),
							i - 1
					);
		}
		return out;
	}
	std::vector< double > out(m_plotPoints);
	double delta = ( lastPoint() - firstPoint() ) / double(m_plotPoints - 1);
	for(int j = 0; j < m_plotPoints; j++){
		if(i == 0){
			out[j] = firstPoint() + j * delta;
		} else {
			out[j] = derivative
					(
							firstPoint() + j * delta,
							i - 1
					);
		}
	}

	return out;
}

}
