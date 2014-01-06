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

#ifndef LINEAR1D_H_
#define LINEAR1D_H_

#include <string>
#include <vector>
#include <iostream>
#include "Distance.h"

namespace jlib {

/**
 * @class jlib::Linear1D
 * @brief Linear1D interpolation class, with one parameter 0 <= s <= 1
 *
 * Note: A function 'double operator()(const Data & A,const Data & B) const' must
 * be provided by the class DataDist
 */
template< class Data, class DataDist = Distance< Data > >
class Linear1D{

public:

	/// Constructor
	Linear1D(
			const std::string & name = ""
			): name(name){}

	/// Constructor
	Linear1D(
			const Data & start,
			const Data & end,
			const std::string & name = ""
			);

	/// Destructor
	virtual ~Linear1D(){}

	/// Return the start point
	inline const Data & getStart() const { return start; }

	/// Return the end point
	inline const Data & getEnd() const { return end; }

	/// Returns the tangent (ie normalized derivative)
	inline Data getTangent(const double & s = 0) const;

	/// Returns the derivative
	inline Data getDerivative(const double & s = 0) const;

	/// Return length
	inline double length() const { return DataDist()(end,start); }

	/// Return dist between two points p(s0), p(s1), 0 <= si <= 1
	inline double dist(const double & s0, const double & s1) const;

	/// Returns the point at position s, 0 <= s <= 1
	inline Data operator[](const double & s) const;

	/// Returns the name
	inline const std::string & getName() const { return name; }

	/// Returns a discretized version of the line
	std::vector<Data> discretize(int points) const;


protected:

	/// The name of the line
	std::string name;

	/// The start point
	Data start;

	/// The end point
	Data end;
};

template < class Data, class DataDist >
Linear1D< Data, DataDist >::Linear1D(
		const Data & start,
		const Data & end,
		const std::string & name
		):
		name(name),
		start(start),
		end(end){
}

template < class Data, class DataDist >
std::vector<Data> Linear1D< Data, DataDist >::discretize(
		int points
		) const{

	// prepare:
	std::vector<Data> out(points);

	// simple checks:
	if(points <=0) return out;

	// start and end:
	out[0] = getStart();
	if(points == 1) return out;
	out.back() = getEnd();
	if(points == 2) return out;

	// prepare:
	double delta = 1./(points - 1);

	// fill:
	for(int i = 1; i < points - 1; i++)
		out[i] = (*this)[i * delta];

	return out;
}

template < class Data, class DataDist >
inline Data Linear1D< Data, DataDist >::operator[](
		const double & s
		) const{
	return s <= 0. ? start :
			(s >= 1. ? end :
		start + s * (end - start)
	);
}

template < class Data, class DataDist >
inline double Linear1D< Data, DataDist >::dist(
		const double & s0,
		const double & s1
		) const{
	return DataDist()((*this)[s0],(*this)[s1]);
}

template < class Data, class DataDist >
inline Data Linear1D< Data, DataDist >::getDerivative(
		const double & s
		) const{
	return end - start;
}

template < class Data, class DataDist >
inline Data Linear1D< Data, DataDist >::getTangent(
		const double & s
		) const{
	return getDerivative() / length();
}


}


#endif /* LINEAR1D_H_ */
