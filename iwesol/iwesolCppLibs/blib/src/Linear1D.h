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
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Class
    blib::Linear1D

Description
    See below

SourceFiles

References
	[1] J. Schmidt, C. Peralta, B. Stoevesandt, "Automated Generation of
	    Structured Meshes for Wind Energy Applications", Proceedings of the
	    Open Source CFD International Conference, 2012, London, UK

\*---------------------------------------------------------------------------*/

#ifndef LINEAR1D_H_
#define LINEAR1D_H_

#include <string>
#include <vector>
#include <iostream>

namespace blib {

/**
 * @class blib::Linear1D
 * @brief Linear1D interpolation class, with one parameter 0 <= s <= 1
 *
 * Note: A global function 'double getDist(const T& A,const T& B)' must be provided for T
 */
template<class T>
class Linear1D{

public:

	/// Constructor
	Linear1D(const std::string & name = ""): name(name){}

	/// Constructor
	Linear1D(const T & start, const T & end,
			const std::string & name = "");

	/// Destructor
	virtual ~Linear1D(){}

	/// Linear1D: The distance of two objects. Please specialize for desired T!
	double getDist(const T & A, const T & B) const { return abs(A - B); }

	/// Return the start point
	const T & getStart() const { return start; }

	/// Return the end point
	const T & getEnd() const { return end; }

	/// Returns the tangent (ie normalized derivative)
	T getTangent(double s = 0) const;

	/// Returns the derivative
	T getDerivative(double s = 0) const;

	/// Return length
	double length() const { return getDist(end,start); }

	/// Return dist between two points p(s0), p(s1), 0 <= si <= 1
	double dist(double s0, double s1) const;

	/// Returns the point at position s, 0 <= s <= 1
	T operator[](double s) const;

	/// Returns the name
	const std::string & getName() const { return name; }

	/// Returns a discretized version of the line
	std::vector<T> discretize(int points) const;


protected:

	/// The name of the line
	std::string name;

	/// The start point
	T start;

	/// The end point
	T end;



};

template <class T>
Linear1D<T>::Linear1D(const T & start, const T & end,
		const std::string & name):
		name(name),start(start), end(end){
}

template <class T>
std::vector<T> Linear1D<T>::discretize(int points) const{

	// simple checks:
	if(points <=0) return std::vector<T>();

	std::vector<T> out(points);

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

template <class T>
inline T Linear1D<T>::operator[](double s) const{

	return s <= 0. ? start :
			(s >= 1. ? end :
		start + s * (end - start)
	);

}

template <class T>
inline double Linear1D<T>::dist(double s0, double s1) const{
	return getDist((*this)[s0],(*this)[s1]);
}

template <class T>
inline T Linear1D<T>::getDerivative(double s) const{
	return end - start;
}

template <class T>
inline T Linear1D<T>::getTangent(double s) const{
	return getDerivative() / length();
}


}


#endif /* LINEAR1D_H_ */
