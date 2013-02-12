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
    blib::GenericLinePath

Description
    See below

SourceFiles

References
	[1] J. Schmidt, C. Peralta, B. Stoevesandt, "Automated Generation of
	    Structured Meshes for Wind Energy Applications", Proceedings of the
	    Open Source CFD International Conference, 2012, London, UK

\*---------------------------------------------------------------------------*/

#ifndef GENERICLINEPATH_H_
#define GENERICLINEPATH_H_

#include "Linear1D.h"
#include "GenericRFunction.h"
#include "EuclideanMetric.h"
#include "BLIB.h"

namespace blib {


/**
 * @class blib::GenericLinePath
 * @brief A path of discrete points, parametrized by the path length.
 *
 * Points are also accessible via the parameter s, 0 <= s <= 1.
 *
 * Note: A global function 'double getDist(const T& A,const T& B)' must be provided for T
 */
template <class T>
class GenericLinePath:
	public GenericRFunction<T>{

public:

	/// Constructor
	GenericLinePath(const std::string & name = "");

	/// Constructor
	GenericLinePath(
			const T & p0,
			const T & p1,
			const std::string & name = ""
			);

	/// Constructor
	GenericLinePath(
			const T & p,
			const std::string & name = ""
			);

	/// Constructor
	GenericLinePath(
			const std::vector<T> & points,
			const std::string & name = ""
			);

	/// Constructor
	GenericLinePath(
			const GenericRFunction<T> & f
			);

	/// Destructor
	virtual ~GenericLinePath();

	/** GridDifferentiable1D: Returns the m-th derivative at s
	 * o = -1 corresponds to maximal precision, depending on
	 * number of grid points.
	 */
	T derivative(double s, int m, int o = -1) const;

	/// UnstructuredGrid1D: shifts values
	inline void shift(double deltas) { GenericRFunction<T>::shift(l(deltas)); }

	/// GenericRFunction: Exports the m-th derivative, calculated to order o
	void exportDerivative(GenericRFunction<T> & out, int m, int o) const;

	/// GenericLinePath: The distance of two objects. Please specialize for desired T!
	inline double getDist(const T & A, const T & B) const { return mag(A - B); }

	/// adds a data point
	void addPoint(const T & point);

	/// adds a data point at the end, same as addPoint
	inline void append(const T & point) { addPoint(point); }

	/// adds data points at the end
	void append(const GenericLinePath<T> & path);

	/// adds a data point at the beginning
	void prepend(const T & point);

	/// Merge with another line path
	//virtual void merge(const GenericLinePath & lp, double sMergePoint = 1.);

	/// inserts sub-points
	//virtual void subDivide(int subPoints, double s0 = 0., double s1 = 1.);

	/// Return length
	inline double length() const { return GenericRFunction<T>::size() > 1 ? GenericRFunction<T>::getXRange() : 0.; }

	/// Return dist between two points p(s0), p(s1), 0 <= si <= 1
	inline double dist(double s0, double s1) const { return l(s1) - l(s0); }

	/// Returns the tangent
	inline T getTangent(double s, int order = 2) const { return derivative(mods(s),1,order); }

	/// Returns the tangent at a line point
	inline T getPointTangent(int i, int order = 2) const { return getTangent(getPointS(i),order); }

	/// Return s of a point
	inline double getPointS(unsigned int i) const { return s(GenericRFunction<T>::getPoint1D(i)); }

	/// Returns s of point with maximal curvature, or -1 if c_max/c_average < threshold.
	double getMaxCurvatureS(
			double resolution = 0.01,
			double s0 = 0.,
			double s1 = 1.,
			int o = -1,
			double peakThresholdRel = 3.
			) const;

	/// Returns the curvature
	double getCurvature(double s, int o = -1) const;

	/// The number of points
	inline unsigned int points() const { return GenericRFunction<T>::size(); }

	/// Return the start point
	inline const T & getStart() const { return GenericRFunction<T>::getFirstData(); }

	/// Return the end point
	inline const T & getEnd() const { return GenericRFunction<T>::getLastData(); }

	/// Return a point
	inline const T & getPoint(int i) const { return GenericRFunction<T>::getData(i); }

	/// Returns value at given length
	inline T valueAtDist(double dist) { return GenericRFunction<T>::operator[](dist); }

	/// Returns coordinate list
	inline const std::vector<T> getPointList() const { return GenericRFunction<T>::getListY(); }

	/// Returns the s value that corresponds to a distance
	inline double s(double dist) const { return mods(dist / length()); }

	/// Returns the distance that corresponds to a given s
	double l(double s) const { return s * length(); }

	/// GenericLinePath: Returns a line
	inline Linear1D<T> getLine(int i) const { return Linear1D<T>(GenericRFunction<T>::getData(i),GenericRFunction<T>::getData(i + 1)); }

	/// The number of lines
	inline unsigned int lines() const { return points() > 1 ? points() - 1 : 0; }

	/// Returns the last line
	inline Linear1D<T> backLine() const { return getLine(lines() - 1); }

	/// Returns the first line
	inline Linear1D<T> frontLine() const { return getLine(0); }

	/// Returns the number of the line at s, 0 <= s <= 1
	int getLineNr(double s) const;

	/// gives number i and s value of the sub line that corresponds to initial s
	int toSubLine(double & s) const;

	/// set periodic
	inline void setPeriodic(){ UnstructuredGrid1D::setPeriodic(EuclideanMetric::distance(getStart(),getEnd())); }

	/// mod s
	inline double mods(double s) const { return GenericRFunction<T>::isPeriodic() ? mod(s,1.) : s; }

	/// Returns the point at position s, 0 <= s <= 1
	inline T operator[](double s) const { return GenericRFunction<T>::operator[](l(mods(s))); }


protected:

	/// Replaces point i by p
	inline void replacePoint(int i, const T & p) { /*pointList[i] = p;*/ }

	/// Returns the GenericRFunction
	inline GenericRFunction<T> & getGenericRFunction() { return *this; }
};

template <class T>
GenericLinePath<T>::GenericLinePath(const std::string & name):
		GenericRFunction<T>(4,name){
}

template <class T>
GenericLinePath<T>::GenericLinePath(
		const T & p0,
		const T & p1,
		const std::string & name
		):
		GenericRFunction<T>(4,name){
	addPoint(p0);
	addPoint(p1);
}

template <class T>
GenericLinePath<T>::GenericLinePath(
		const T & p0,
		const std::string & name
		):
		GenericRFunction<T>(4,name){
	addPoint(p0);
}

template <class T>
GenericLinePath<T>::GenericLinePath(
		const std::vector<T> & points,
		const std::string & name
		):
		GenericRFunction<T>(4,name){
	for(unsigned int i = 0; i < points.size(); i++){
		addPoint(points[i]);
	}
}

template <class T>
GenericLinePath<T>::GenericLinePath(
			const GenericRFunction<T> & f
			):
			GenericRFunction<T>(f){
}

template <class T>
GenericLinePath<T>::~GenericLinePath() {
}

template <class T>
inline T GenericLinePath<T>::derivative(double s, int m, int o) const {
	if(GenericRFunction<T>::size() == 2){
		if(m == 1){
			return ( GenericRFunction<T>::getLastData() - GenericRFunction<T>::getFirstData() ) / length();
		} else {
			std::cout << "\nGenericLinePath: Error: Derivative m = " << m
					<< " called for function with no more than 2 support points." << std::endl;
			throw;
		}
	}
	return std::pow(length(),m) * GenericRFunction<T>::derivative(l(mods(s)),m,o);
}

template <class T>
inline void GenericLinePath<T>::addPoint(const T & p){
	double newl = GenericRFunction<T>::size() > 0 ? (length() + getDist(p,getEnd())) : 0.;
	GenericRFunction<T>::add(newl,p);
}

template <class T>
inline void GenericLinePath<T>::prepend(const T & point){
	GenericLinePath<T> newlp;
	newlp.addPoint(point);
	for(unsigned int i = 0; i < GenericRFunction<T>::size(); i++){
		newlp.addPoint(getPoint(i));
	}
	*this = newlp;
}

template <class T>
inline void GenericLinePath<T>::append(const GenericLinePath<T> & path){
	for(unsigned int i = 0; i < path.size(); i++){
		addPoint(path.getPoint(i));
	}
}

/*
void GenericLinePath<T>::subDivide(int subPoints, double s0, double s1){

	// prepare:
	double deltas = (s1 - s0) / (subPoints - 1);
	double s      = s0 + deltas;

	// create new line path between s0 and s1:
	GenericLinePath nlp(
		(*this)[s0], (*this)[s]
	);
	for(int i = 2; i < subPoints; i++){
		s += deltas;
		nlp.addPoint( (*this)[s] );
	}

	// merge:
	GenericLinePath newlp      = getSubLinePath(0.,s0);
	GenericLinePath pieceAfter = getSubLinePath(s1,1.);
	if(newlp.empty()){
		newlp = nlp;
	} else {
		newlp.merge(nlp);
	}
	if(!pieceAfter.empty()){
		newlp.merge(pieceAfter);
	}

	*this = newlp;
	updateGrid();
}

void GenericLinePath<T>::merge(const GenericLinePath & lp, double sMergePoint){

	// prepare:
	T pMerge   = (*this)[sMergePoint];
	GenericLinePath newlp      = getSubLinePath(0.,sMergePoint);
	GenericLinePath pieceAfter = getSubLinePath(sMergePoint,1.);

	// insert:
	if(newlp.empty()){
		newlp = lp;
	} else {
		if(lp.GenericRFunction<T>::getStart() != newlp.GenericRFunction<T>::getEnd()) {
			newlp.addPoint(lp.GenericRFunction<T>::getStart());
		}
		for(unsigned int i = 1; i < lp.points(); i ++){
			newlp.addPoint(lp.getPoint(i));
		}
	}

	// remainder:
	if(!pieceAfter.empty()){
		if(lp.GenericRFunction<T>::getEnd() != pieceAfter.getPoint(1))
			newlp.addPoint(pieceAfter.getPoint(1));
		for(unsigned int i = 2; i < pieceAfter.points(); i ++){
			newlp.addPoint(pieceAfter.getPoint(i));
		}
	}

	*this = newlp;
	updateGrid();
}
*/

template <class T>
double GenericLinePath<T>::getMaxCurvatureS(
		double resolution,
		double s0,
		double s1,
		int o,
		double peakThresholdRel
		) const{

	// prepare:
	if( s0 == s1 ) return s0;
	if( s0 > s1 ) return getMaxCurvatureS(resolution, s0, s1 + 1,o);
	double smax    = -1;
	double curvmax2 = -1;

	// find maximal curvature on points i:

	// prepare:
	int i0 = getIndex_aftereq(l(mod(s0,1.)));
	int i1 = getIndex_before(l(mod(s1,1.)));
	if(s1 >= 1) i1 += GenericRFunction<T>::size();
	double average = 0;

	// find maximum and average:
	for(int i = i0; i <= i1; i++){
		double s  = getPointS(i);
		double c  = getCurvature(s,o);
		average  += c;
		double c2 = c * c;
		if(c2 > curvmax2){
			smax    = s;
			curvmax2 = c2;
		}
	}
	average /= (i1 - i0 + 1);

	// check threshold:
	if(curvmax2 / (average * average) <= peakThresholdRel * peakThresholdRel) return -1;

	return mod(smax,1.);
}

template <class T>
void GenericLinePath<T>::exportDerivative(GenericRFunction<T> & out, int m, int o) const{

	GenericRFunction<T>::exportDerivative(out,m,o);

	for(unsigned int i = 0; i < out.size(); i++){
		out.points[i]  = s(out.points[i]);
		out.data[i]   *= pow(length(),m);
	}

}

template <class T>
double GenericLinePath<T>::getCurvature(double s,int o) const{

	// check order:
	if(o < 0) return getCurvature(s,GenericRFunction<T>::size() - 2);

	T der  = derivative(s,1,o);
	T der2 = derivative(s,2,o);
	return (der.x * der2.y - der.y * der2.x) / std::pow(der.x * der.x + der.y * der.y, 3./2.);

}

template <class T>
int GenericLinePath<T>::toSubLine(double & s) const{

	// simple checks:
	if(s <= 0) return 0;
	if(s >= 1) return lines() - 1;

	// find line:
	double l     = s * GenericLinePath<T>::length();
	int i        = 0;
	double totl  = getLine(0).length();
	double lastl = 0;
	while( totl < l ) {
		i++;
		lastl = totl;
		totl += getLine(i).length();
	}

	s = (l - lastl) / getLine(i).length();
	return i;

}

template <class T>
int GenericLinePath<T>::getLineNr(double s) const{
	double hs = s;
	return toSubLine(hs);
}


}

#endif /* GENERICLINEPATH_H_ */
