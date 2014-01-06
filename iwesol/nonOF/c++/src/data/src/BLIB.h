/**
 * @file BLIB.h
 * @brief A basic function library.
 *
 */

#ifndef BLIB_H_
#define BLIB_H_

#include <sstream>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

#include "JString.h"
#include "Point2D.h"

namespace jlib{

namespace BLIB {

/// resets the seed
void reseed();

/// The value of 2 PI
const double TWOPI = 2.0 * M_PI;

/// chop limit
const double CHOP = 0.00000001;

/// Define a set of integer values
typedef std::vector<int> SET_I;

/// Define a set of a set of integer values
typedef std::vector<SET_I> SETSET_I;

/// Gives all permutations
void getPermutations(const SETSET_I & in, SETSET_I & out);

/** Gives all permutations. Here the set 'in' contains
 * integers, understood as lists {0, ..., (in[i] - 1)}.
 */
void getIndexPermutations(const SET_I & in, SETSET_I & out);

/// the total number of boxes in a simple box pyramid
int boxPyramid_boxes(const int baseboxes);

/// the number of boxes in a column of a box pyramid
int boxPyramid_height(const int baseboxes, const int column);

/// Converts an angle from deg to rad
double deg2rad(double deg);

/// Converts an angle from rad to deg
double rad2deg(double rad);

/// Defines Cartesian coordinates (x,y,z,..)
typedef std::vector<double> COORD_CART;

/// Defines Cylindrical coordinates (r,phi,z).
typedef std::vector<double> COORD_CYL;

/** Defines coefficients of a std::vector in
 * Cartesian coordinates (Ax,Ay,Az,..)
 */
typedef std::vector<double> VECC_CART;

/** Defines coefficients of a std::vector in
 * cylindrical coordinates (Ar,Aphi,Az)
 */
typedef std::vector<double> VECC_CYL;

/// add two std::vectors
std::vector<double> vAplusB(const std::vector<double> & A, const std::vector<double> & B);

/// subtract two std::vectors
std::vector<double> vAminusB(const std::vector<double> & A, const std::vector<double> & B);

/// scalar multiplication
std::vector<double> vAtimesX(const std::vector<double> & A, double X);

/// scalar multiplication
std::vector<double> vAtimesX(double X,const std::vector<double> & A);

/// scalar division
std::vector<double> vAbyX(const std::vector<double> & A, double X);

/// The Weibull distribution
double weibull(double x, double lambda, double k);

/// Calculates the square of the Euclidean distance between two Cartesian points.
double eucDistCartSqr(const COORD_CART & x,
		const COORD_CART & y);

/// Calculates the Euclidean distance between two Cartesian points.
double eucDistCart(const COORD_CART & x,
		const COORD_CART & y);

/// chops a double at CHOP
double chop(double a);

/// double modulo
inline double mod(double a, double b);

/// converts imax symbols of d to a String
String d2S(double d, int imax = 8,const std::string & nullFill = "X");

/** Calculates the square of the Euclidean distance
 * between a Cartesian points and the origin.
 *
 */
double eucDistCartSqr(const COORD_CART & x);

/** Calculates the Euclidean distance between a
 * Cartesian points and the origin.
 */
double eucDistCart(const COORD_CART & x);

/// std::vector norm
double norm(const std::vector<double> & U);

/// std::vector norm square
double normsqr(const std::vector<double> & U);

/// normalization of a std::vector
void normalize(std::vector<double> & v);

/// rotates a std::vector by omega, using cross product.
void rotateVector(std::vector<double> & v, const std::vector<double> & omega, double resolution = 0.0001);

/// The distance between two points
double dist(const Point2D & p, const Point2D & q);

/// cosine between two std::vectors
double getCos(const std::vector<double> & A, const std::vector<double> & B);

/// angle between two std::vectors
double getAngleRad(const std::vector<double> & A, const std::vector<double> & B);

/// the cross product
std::vector<double> cross(const std::vector<double> & A, const std::vector<double> & B);

/// the dot product
double dot(const std::vector<double> & A, const std::vector<double> & B);

/** Converts Cartesian coordinates (x,y,z) into
 * cylindrical coordinates (r,phi,z)
 */
COORD_CYL coord_xyz2rpz(const COORD_CART & p);

/** Converts Cartesian coordinates (x,y,z) into
 * cylindrical coordinates (r,phi,z)
 */
COORD_CART coord_rpz2xyz(const COORD_CYL & p);

/** Converts Cartesian std::vector coefficients (Ax,Ay,Az) into
 * cylindrical coefficients (Ar,Aphi,Az) at the point p
 */
VECC_CYL vecc_xyz2rpz(const VECC_CART & v, const COORD_CYL & p);

/** Converts cylindrical std::vector coefficients (Ar,Aphi,Az) into
 * Cartesian coefficients (Ax,Ay,Az) at the point p
 */
VECC_CART vecc_rpz2xyz(const VECC_CYL & v, const COORD_CYL & p);

/// a key for two integers
std::string key(int a, int b);

/// Kills duplicates in a std::vector.
template <class T>
void reduceDup_v(std::vector<T> & vi){

	// simple check:
	if(vi.size() < 2) return;

	// the first is always in:
	std::vector<T> out = std::vector<T>(1,vi[0]);

	// check the others:
	typename std::vector<T>::iterator it;
	for(it = vi.begin() + 1; it < vi.end(); it++)
		if(find(out.begin(),out.end(),*it) == out.end())
			out.push_back(*it);

	// return:
	vi = out;
}

/// Reduces in a std::vector by an element, if existent
template <class T>
void reduce_v(std::vector<T> & vi, const T & element){

	// find the element:
	typename std::vector<T>::iterator elpos =
			find(vi.begin(),vi.end(),element);

	// erase element, if found:
	if(elpos != vi.end())
		vi.erase(elpos);

}

/** Checks if a std::vector contains an element. (avoids algorithm::find
 * since this does not qualify as const)
 */
template <class T>
bool vContains(const std::vector<T> & vi, const T & element){

	// simple check:
	if(vi.empty()) return false;

	// find the element:
	for(unsigned int i = 0; i<vi.size(); i++)
		if(vi[i] == element) return true;

	return false;
}

} /* end namespace BLIB */

inline double BLIB::chop(double a){
	return a * a < CHOP * CHOP ? 0. : a;
}


inline std::vector<double> BLIB::vAplusB(const std::vector<double> & A, const std::vector<double> & B){
	std::vector<double> out(3);
	for(unsigned int i = 0; i < A.size(); i++)
		out[i] = A[i] + B[i];
	return out;
}

inline std::vector<double> BLIB::vAminusB(const std::vector<double> & A, const std::vector<double> & B){
	std::vector<double> out(3);
	for(unsigned int i = 0; i < A.size(); i++)
		out[i] = A[i] - B[i];
	return out;
}

inline double BLIB::mod(double a, double b){
	//label temp = a / b;
	double out = chop(a);
	while(chop(out - b) >= 0.) out = chop(out - b);
	while(chop(out) < 0.) out = chop(out + b);
	return out;
}

inline std::vector<double> BLIB::vAtimesX(const std::vector<double> & A, double X){
	std::vector<double> out(3);
	for(unsigned int i = 0; i < A.size(); i++)
		out[i] = X * A[i];
	return out;
}

inline std::vector<double> BLIB::vAtimesX(double X,const std::vector<double> & A){
	return vAtimesX(A,X);
}

inline std::vector<double> BLIB::vAbyX(const std::vector<double> & A, double X){
	std::vector<double> out(3);
	for(unsigned int i = 0; i < A.size(); i++)
		out[i] = A[i] / X;
	return out;
}

inline double BLIB::weibull(double x, double lambda, double k){
	return x < 0. ? 0. :
			k/lambda * std::pow(x/lambda,k - 1.) * std::exp(-std::pow(x/lambda,k));
}

inline double BLIB::deg2rad(double deg) {

	double rad = deg * M_PI / 180.0;
	while (rad > TWOPI)
		rad -= TWOPI;
	return rad;
}

inline double BLIB::rad2deg(double rad) {

	double deg = rad * 180.0 / M_PI;
	while (deg > 360.0)
		deg -= 360.0;
	return deg;
}

inline double BLIB::eucDistCartSqr(const COORD_CART & x,
		const COORD_CART & y){

	if(x.empty() || x.size() != y.size()) return 0;
	double out = 0;
	for(unsigned int i = 0; i < x.size(); i++)
		out += (x[i] - y[i])*(x[i] - y[i]);
	return out;

}

inline double BLIB::eucDistCart(const COORD_CART & x,
		const COORD_CART & y){
	return std::sqrt(eucDistCartSqr(x,y));
}

inline double BLIB::eucDistCartSqr(const COORD_CART & x){

	if(x.empty()) return 0;
	double out = 0;
	for(unsigned int i = 0; i < x.size(); i++)
		out += x[i] * x[i];
	return out;

}

inline double BLIB::eucDistCart(const COORD_CART & x){
	return std::sqrt(eucDistCartSqr(x));
}

inline std::vector<double> BLIB::cross(const std::vector<double> & A, const std::vector<double> & B){
	std::vector<double> out(3);
	out[0] = A[1]*B[2]-A[2]*B[1];
	out[1] = A[2]*B[0]-A[0]*B[2];
	out[2] = A[0]*B[1]-A[1]*B[0];
	return out;
}

inline double BLIB::dot(const std::vector<double> & A, const std::vector<double> & B){
	double out = 0.;
	for(unsigned int i = 0; i < A.size(); i++)
		out += A[i] * B[i];
	return out;
}

inline double BLIB::normsqr(const std::vector<double> & U){
	return dot(U,U);
}

inline double BLIB::norm(const std::vector<double> & U){
	return std::sqrt(normsqr(U));
}

inline void BLIB::normalize(std::vector<double> & v){
	v = vAtimesX(v,norm(v));
}

inline double BLIB::getCos(const std::vector<double> & A, const std::vector<double> & B){
	return dot(A,B)/(norm(A)*norm(B));
}

inline double BLIB::getAngleRad(const std::vector<double> & A, const std::vector<double> & B){
	return std::acos(getCos(A,B));
}

inline double BLIB::dist(const Point2D & p, const Point2D & q){
	Point2D diff = p - q;
	return std::sqrt(diff.x * diff.x + diff.y * diff.y);
}

inline std::string BLIB::key(int a, int b){
	return String(a) + "." + String(b);
}

}

#endif /* BLIB_H_ */
