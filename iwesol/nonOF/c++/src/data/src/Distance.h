/**
 * @file Distance.h
 * @brief Definition/Implementation of distance classes
 *
 */

#ifndef DISTANCE_H_
#define DISTANCE_H_

#include <cmath>
#include <vector>
#include "Coordinate2D.h"

namespace jlib {

template< class Data >
struct Distance{

	double operator()(const Data & a, const Data & b) const{
		return std::sqrt( (b - a) * (b - a)  );
	}

};

template<>
double Distance< int >::operator()(
		const int & a,
		const int & b
		) const;

template<>
double Distance< std::vector< double > >::operator()(
		const std::vector< double > & a,
		const std::vector< double > & b
		) const;

template<>
double Distance< std::vector< int > >::operator()(
		const std::vector< int > & a,
		const std::vector< int > & b
		) const;

template<>
double Distance< Coordinate2D >::operator()(
		const Coordinate2D & a,
		const Coordinate2D & b
		) const;

}

#endif /* DISTANCE_H_ */
