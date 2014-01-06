/**
 * @file Distance.cpp
 * @brief Definition/Implementation of class Distance.
 *
 */

#include "Distance.h"
#include <stdlib.h>

namespace jlib{

template<>
double Distance< int >::operator()(
		const int & a,
		const int & b
		) const{
	return std::abs(b - a);
}

template<>
double Distance< std::vector< double > >::operator()(
		const std::vector< double > & a,
		const std::vector< double > & b
		) const{
	double out = 0;
	for(unsigned int i = 0; i< a.size(); i++){
		out     += (b[i] - a[i]) * (b[i] - a[i]);
	}
	return std::sqrt(out);
}

template<>
double Distance< std::vector< int > >::operator()(
		const std::vector< int > & a,
		const std::vector< int > & b
		) const{
	double out = 0;
	for(unsigned int i = 0; i< a.size(); i++){
		double d = b[i] - a[i];
		out     += d * d;
	}
	return std::sqrt(out);
}

template<>
double Distance< Coordinate2D >::operator()(
		const Coordinate2D & a,
		const Coordinate2D & b
		) const{
	return std::sqrt( (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) );
}

}
