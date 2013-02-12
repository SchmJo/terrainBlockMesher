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
    along with IWESOL. If not, see <http://www.gnu.org/licenses/>.

Class
    blib::Point3D

Description
    See below

SourceFiles
    Point3D.cpp

References
	[1] J. Schmidt, C. Peralta, B. Stoevesandt, "Automated Generation of
	    Structured Meshes for Wind Energy Applications", Proceedings of the
	    Open Source CFD International Conference, 2012, London, UK

\*---------------------------------------------------------------------------*/

#ifndef POINT3D_H_
#define POINT3D_H_

#include "Point2D.h"

namespace blib{

/**
 * @class blib::Point3D
 * @brief A simple point in 3D.
 *
 */
class Point3D:
	public Point2D {

public:


	/// z value.
	double z;

	///Constructor.
	Point3D();

	///Constructor.
	Point3D(const double & x, const double & y, const double & z);

	/// Constructor.
	Point3D(const std::vector<double> & xyz);

	/// Destructor.
	virtual ~Point3D();

	/// Return a component
	double & operator[](int const & index);

	/// Return a component
	const double & operator[](int const & index) const;

	/// Returns data as a std::vector
	std::vector<double> v() const;

	/// Returns data size
	virtual unsigned int size() const { return 3;}

	/// Changing sign
	Point3D operator-() const;

	/// Operator +=
	Point3D & operator+=(Point3D const & rhs);

	/// Operator -=
	Point3D & operator-=(Point3D const & rhs);

	/// Operator *=
	Point3D & operator*=(double const & rhs);

	/// Operator /=
	Point3D & operator/=(double const & rhs);

	/// Stream operator
	friend std::ostream &operator<<(std::ostream &stream, Point3D ob);

	/// Stream operator
	friend std::istream &operator>>(std::istream &stream, Point3D &ob);

	/* !! PARSE FUNCTIONALITY NOT SUPPORTED !!
	 *
	 	/// Specification of which data to store where in the class
	virtual void initParseDict(ParseDict & dictionary);

	/// Set ParseBlockName for z:
	void setParseBlockNameZ(const std::string & blockNameX);

	/// Set parse skip values.
	void setParseSkipXYZ(const int skipX, const int skipY, const int skipZ);

	/// Returns the parse skip value for x.
	int getParseSkipZ() const;

	/// Returns the parse block name of z
	const std::string & getParseBlockNameZ() const;
	 */

protected:

	/* !! PARSE FUNCTIONALITY NOT SUPPORTED !!
	 *
	/// The parse skip value for z.
	int parseSkipZ;

	/// The parsing block name for Z
	std::string parseBlockNameZ;
	*/

};


inline Point3D::Point3D():
	Point2D(),
	z(0)
	//parseSkipZ(0),
	//parseBlockNameZ("posz")
	{
	//dictName = "Point3D";
}

inline Point3D::Point3D(const double & x, const double & y, const double & z):
		Point2D(x,y),
		z(z)
		//parseSkipZ(0),
		//parseBlockNameZ("posz")
		{
	//dictName = "Point3D";
}

/*
inline void Point3D::setParseBlockNameZ(const std::string & blockNameZ){
	parseBlockNameZ = blockNameZ;
}

inline void Point3D::setParseSkipXYZ(const int skipX,
		const int skipY, const int skipZ){
	setParseSkipXY(skipX,skipY);
	parseSkipZ = skipZ;
}

inline int Point3D::getParseSkipZ() const{
	return parseSkipZ;
}

inline const std::string & Point3D::getParseBlockNameZ() const{
	return parseBlockNameZ;
}
*/

inline double & Point3D::operator[](int const & index){
	return index == 2 ? z : Point2D::operator[](index);
}

inline const double & Point3D::operator[](int const & index) const{
	return index == 2 ? z : Point2D::operator[](index);
}

inline std::vector<double> Point3D::v() const{
	std::vector<double> out(3);
	out[0] = x;
	out[1] = y;
	out[2] = z;
	return out;
}

inline Point3D Point3D::operator-() const{
	return Point3D(-x,-y,-z);
}

inline Point3D & Point3D::operator+=(Point3D const & rhs){
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	return *this;
}

inline Point3D & Point3D::operator-=(Point3D const & rhs){
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	return *this;
}

inline Point3D & Point3D::operator*=(double const & rhs){
	x *= rhs;
	y *= rhs;
	z *= rhs;
	return *this;
}

inline Point3D & Point3D::operator/=(double const & rhs){
	x /= rhs;
	y /= rhs;
	z /= rhs;
	return *this;
}

inline const Point3D operator+(Point3D const& lhs, Point3D const& rhs)
{
  Point3D tmp(lhs);
  tmp += rhs;
  return tmp;
}

inline const Point3D operator-(Point3D const& lhs, Point3D const& rhs)
{
  Point3D tmp(lhs);
  tmp -= rhs;
  return tmp;
}

inline const Point3D operator*(Point3D const& lhs, double const& rhs)
{
  Point3D tmp(lhs);
  tmp *= rhs;
  return tmp;
}

inline const Point3D operator*(double const& rhs,Point3D const& lhs)
{
  return lhs * rhs;
}

inline const Point3D operator/(Point3D const& lhs, double const& rhs)
{
  Point3D tmp(lhs);
  tmp /= rhs;
  return tmp;
}

inline bool operator==(Point3D const& lhs, Point3D const& rhs){
	return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

inline std::ostream &operator<<(std::ostream &stream, Point3D ob)
{
  stream << std::setprecision(4) << std::setw(10) << ob.x << ' ' << std::setw(10) << ob.y << ' ' << std::setw(10) << ob.z;

  return stream;
}

inline std::istream &operator>>(std::istream &stream, Point3D &ob)
{
  stream >> ob.x >> ob.y >> ob.z;

  return stream;
}

}

#endif /* POINT3D_H_ */
