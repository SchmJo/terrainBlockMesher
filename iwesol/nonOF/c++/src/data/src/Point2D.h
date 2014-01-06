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

#ifndef POINT2D_H_
#define POINT2D_H_

#include <iomanip>
#include <vector>

//#include "Parseable.h"

namespace jlib{

class Point2D{//: public Parseable {

public:

	/// x value.
	double x;

	/// y value.
	double y;

	///Constructor.
	Point2D();

	///Constructor.
	Point2D(const double & x, const double & y);

	/// Constructor.
	Point2D(const std::vector<double> & xy);

	/// Destructor.
	virtual ~Point2D(){}

	/// Specification of which data to store where in the class.
	//virtual void initParseDict(ParseDict & dictionary);

	/// Set ParseBlockName for x.
	//void setParseBlockNameX(const std::string & blockNameX);

	/// Set ParseBlockName for y.
	//void setParseBlockNameY(const std::string & blockNameY);

	/// Set parse skip values.
	//void setParseSkipXY(const int skipX, const int skipY);

	/// Returns the parse skip value for x.
	//int getParseSkipX() const;

	/// Returns the parse skip value for y.
	//int getParseSkipY() const;

	/// Returns data as a std::vector
	virtual std::vector<double> v() const;

	/// Returns data size
	virtual unsigned int size() const { return 2;}

	/// Returns the parse block name of x.
	//const std::string & getParseBlockNameX() const;

	/// Returns the parse block name of y.
	//const std::string & getParseBlockNameY() const;

	/// Return a component
	virtual double & operator[](int const & index);

	/// Return a component
	virtual const double & operator[](int const & index) const;

	/// Operator ==
	bool operator==(const Point2D & rhs) const;

	/// Operator !=
	bool operator!=(const Point2D & rhs) const;

	/// Changing sign
	Point2D operator-() const;

	/// Operator +=
	Point2D & operator+=(Point2D const & rhs);

	/// Operator -=
	Point2D & operator-=(Point2D const & rhs);

	/// Operator *=
	Point2D & operator*=(double const & rhs);

	/// Operator /=
	Point2D & operator/=(double const & rhs);
/*
	/// Stream operator
	friend std::ostream &operator<<(std::ostream &stream, const Point2D & ob);

	/// Stream operator
	friend std::istream &operator>>(std::istream &stream, Point2D &ob);
*/

protected:

	/// The parse skip number for x
	//int parseSkipX;

	/// The parse skip number for y
	//int parseSkipY;

	/// The parsing block name for x
	//std::string parseBlockNameX;

	/// The parsing block name for y
	//std::string parseBlockNameY;

};

inline Point2D::Point2D():
	//Parseable("Point2D"),
	x(0),
	y(0)
	//parseSkipX(0),
	//parseSkipY(0),
	//parseBlockNameX("posx"),
	//parseBlockNameY("posy")
	{
}

inline Point2D::Point2D(const double & x, const double & y):
		//Parseable("Point2D"),
		x(x),
		y(y)
		//parseSkipX(0),
		//parseSkipY(0),
		//parseBlockNameX("posx"),
		//parseBlockNameY("posy")
		{
}

/*
inline void Point2D::setParseBlockNameX(const std::string & blockNameX){
	parseBlockNameX = blockNameX;
}

inline void Point2D::setParseBlockNameY(const std::string & blockNameY){
	parseBlockNameY = blockNameY;
}

inline const std::string & Point2D::getParseBlockNameX() const{
	return parseBlockNameX;
}

inline const std::string & Point2D::getParseBlockNameY() const{
	return parseBlockNameY;
}

inline void Point2D::setParseSkipXY(const int skipX, const int skipY){
	parseSkipX = skipX;
	parseSkipY = skipY;
}
*/

inline std::vector<double> Point2D::v() const{
	std::vector<double> out(2);
	out[0] = x;
	out[1] = y;
	return out;
}

/*
inline int Point2D::getParseSkipX() const{
	return parseSkipX;
}

inline int Point2D::getParseSkipY() const{
	return parseSkipY;
}
*/

inline double & Point2D::operator[](int const & index){
	return index == 0 ? x : y;
}

inline const double & Point2D::operator[](int const & index) const{
	return index == 0 ? x : y;
}

inline Point2D Point2D::operator-() const{
	return Point2D(-x,-y);
}

inline Point2D & Point2D::operator+=(Point2D const & rhs){
	x += rhs.x;
	y += rhs.y;
	return *this;
}

inline Point2D & Point2D::operator-=(Point2D const & rhs){
	x -= rhs.x;
	y -= rhs.y;
	return *this;
}

inline Point2D & Point2D::operator*=(double const & rhs){
	x *= rhs;
	y *= rhs;
	return *this;
}

inline Point2D & Point2D::operator/=(double const & rhs){
	x /= rhs;
	y /= rhs;
	return *this;
}

inline const Point2D operator+(Point2D const& lhs, Point2D const& rhs)
{
  Point2D tmp(lhs);
  tmp += rhs;
  return tmp;
}

inline const Point2D operator-(Point2D const& lhs, Point2D const& rhs)
{
  Point2D tmp(lhs);
  tmp -= rhs;
  return tmp;
}

inline const Point2D operator*(Point2D const& lhs, double const& rhs)
{
  Point2D tmp(lhs);
  tmp *= rhs;
  return tmp;
}

inline const Point2D operator*(double const& rhs,Point2D const& lhs)
{
  return lhs * rhs;
}

inline const Point2D operator/(Point2D const& lhs, double const& rhs)
{
  Point2D tmp(lhs);
  tmp /= rhs;
  return tmp;
}

/*
inline std::ostream &operator<<(std::ostream &stream, const Point2D & ob)
{
  stream << std::setprecision(4) << std::setw(10) << ob.x << ' ' << std::setw(10) << ob.y;

  return stream;
}

inline std::istream &operator>>(std::istream &stream, Point2D & ob)
{
  stream >> ob.x >> ob.y;

  return stream;
}
*/

}

#endif /* POINT2D_H_ */
