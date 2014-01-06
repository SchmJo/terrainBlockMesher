/**
 * @file GenericLinePath.h
 * @brief Definition/Implementation of class GenericLinePath.
 *
 */

#ifndef GENERICLINEPATH_H_
#define GENERICLINEPATH_H_

#include "Linear1D.h"
#include "BLIB.h"
#include "GenericRFunction.h"
#include "Distance.h"

namespace jlib {


/**
 * @class jlib::GenericLinePath
 * @brief A path of discrete points, parametrized by the path length.
 *
 * Points are also accessible via the parameter s, 0 <= s <= 1.
 *
 * Note: A function 'double operator()(const Data & A,const Data & B) const' must
 * be provided by the class DataDist
 */
template < class Data, class DataDist = Distance< Data > >
class GenericLinePath:
	public GenericRFunction< Data >{

public:

	/// Constructor
	GenericLinePath(
			const std::string & name = "",
			int interpolOrder = 4
			);

	/// Constructor
	GenericLinePath(
			const Data & p0,
			const Data & p1,
			const std::string & name = "",
			int interpolOrder = 4
			);

	/// Constructor
	GenericLinePath(
			const Data & p,
			const std::string & name = "",
			int interpolOrder = 4
			);

	/// Constructor
	GenericLinePath(
			const std::vector< Data > & points,
			const std::string & name = "",
			int interpolOrder = 4
			);

	/// Destructor
	virtual ~GenericLinePath();

	/** GridDifferentiable1D: Returns the m-th derivative at s
	 * number of grid points.
	 */
	inline Data derivative(const double & s, int m) const;

	/// UnstructuredGrid1D: shifts values
	inline void shift(const double & deltas) { GenericRFunction< Data >::shift(l(deltas)); }

	/// adds a data point
	void addPoint(const Data & point);

	/// adds a data point at the end, same as addPoint
	inline void append(const Data & point) { addPoint(point); }

	/// make path cyclic
	inline void setCyclic() { GenericRFunction< Data >::setPeriodic(mag(getLastPoint() - getFirstPoint())); }

	/// make path un-cyclic
	inline void setUnCyclic() { GenericRFunction< Data >::setNotperiodic(); }

	/// adds data points at the end
	void append(const GenericLinePath< Data, DataDist > & path);

	/// adds a data point at the beginning
	void prepend(const Data & point);

	/// erase a point
	void erase(int i);

	/// returns a point
	inline const Data & getPoint(int i) const { return GenericRFunction< Data >::data(i); }

	/// returns first point
	inline const Data & getFirstPoint() const { return GenericRFunction< Data >::firstData(); }

	/// returns last point
	inline const Data & getLastPoint() const { return GenericRFunction< Data >::lastData(); }

	/// Merge with another line path
	//virtual void merge(const GenericLinePath & lp, double sMergePoint = 1.);

	/// inserts sub-points
	//virtual void subDivide(int subPoints, double s0 = 0., double s1 = 1.);

	/// Return length
	inline double length() const {
		return GenericRFunction< Data >::size() > 1 ? GenericRFunction< Data >::pointRange() : 0.; }

	/// Return dist between two points p(s0), p(s1), 0 <= si <= 1
	inline double dist(
			const double & s0,
			const double & s1
			) const { return l(s1) - l(s0); }

	/// Returns the tangent
	inline Data getTangent(const double & s) const { return derivative(mods(s),1); }

	/// Returns the tangent at a line point
	inline Data getPointTangent(int i) const { return getTangent(getPointS(i)); }

	/// Return l of a point
	inline double getPointL(unsigned int i) const { return GenericRFunction< Data >::point(i); }

	/// Return s of a point
	inline double getPointS(unsigned int i) const { return s(getPointL(i)); }

	/// Returns s of point with maximal curvature, or -1 if c_max/c_average < threshold.
	/*
	double getMaxCurvatureS(
			const double & resolution = 0.01,
			const double & s0 = 0.,
			const double & s1 = 1.,
			const double & peakThresholdRel = 3.
			) const;
	*/

	/// Returns the curvature
	//double getCurvature(const double & s) const;

	/// The number of points
	inline unsigned int points() const { return GenericRFunction< Data >::size(); }

	/// Returns value at given length
	inline Data valueAtDist(const double & dist) { return GenericRFunction< Data >::operator[](dist); }

	/// Returns coordinate list
	inline std::vector< Data > getPointList() const { return GenericRFunction< Data >::getDataList(); }

	/// Returns the s value that corresponds to a distance
	inline double s(const double & dist) const { return mods(dist / length()); }

	/// Returns the distance that corresponds to a given s
	inline double l(const double & s) const { return s * length(); }

	/// GenericLinePath: Returns a line
	inline Linear1D< Data, DataDist > getLine(int i) const {
		return Linear1D< Data, DataDist >(GenericRFunction< Data >::data(i),GenericRFunction< Data >::data(i + 1)); }

	/// The number of lines
	inline unsigned int lines() const { return points() > 1 ? points() - 1 : 0; }

	/// Returns the last line
	inline Linear1D< Data, DataDist > backLine() const { return getLine(lines() - 1); }

	/// Returns the first line
	inline Linear1D< Data, DataDist > frontLine() const { return getLine(0); }

	/// Returns the number of the line at s, 0 <= s <= 1
	int getLineNr(const double & s) const;

	/// gives number i and s value of the sub line that corresponds to initial s
	int toSubLine(double & s) const;

	/// mod s
	inline double mods(
			const double & s
			) const { return GenericRFunction< Data >::periodic() ? BLIB::mod(s,1.) : s; }

	/// Returns the point at position s, 0 <= s <= 1
	inline Data operator[](const double & s) const { return GenericRFunction< Data >::operator[](l(mods(s))); }


protected:

	/// Returns the GenericRFunction
	inline GenericRFunction< Data > & getGenericRFunction() { return *this; }


private:

	/// disable function
	const double & point(int i) const;

	/// disable function
	inline const double & firstPoint() const { return GenericRFunction< Data >::firstPoint(); }

	/// rdisable function
	inline const double & lastPoint() const { return GenericRFunction< Data >::lastPoint(); }

	/// disable function
	const Data & data(int i) const;

	/// disable function
	const Data & firstData() const;

	/// disable function
	const Data & lastData() const;
};

template < class Data, class DataDist >
GenericLinePath< Data, DataDist >::GenericLinePath(
		const std::string & name,
		int interpolOrder
		):
		GenericRFunction< Data >(interpolOrder,name){
}

template < class Data, class DataDist >
GenericLinePath< Data, DataDist >::GenericLinePath(
		const Data & p0,
		const Data & p1,
		const std::string & name,
		int interpolOrder
		):
		GenericRFunction< Data >(interpolOrder,name){
	addPoint(p0);
	addPoint(p1);
}

template < class Data, class DataDist >
GenericLinePath< Data, DataDist >::GenericLinePath(
		const Data & p0,
		const std::string & name,
		int interpolOrder
		):
		GenericRFunction< Data >(interpolOrder,name){
	addPoint(p0);
}

template < class Data, class DataDist >
GenericLinePath< Data, DataDist >::GenericLinePath(
		const std::vector< Data > & points,
		const std::string & name,
		int interpolOrder
		):
		GenericRFunction< Data >(interpolOrder,name){
	for(unsigned int i = 0; i < points.size(); i++){
		addPoint(points[i]);
	}
}

template < class Data, class DataDist >
GenericLinePath< Data, DataDist >::~GenericLinePath() {
}

template < class Data, class DataDist >
inline Data GenericLinePath< Data, DataDist >::derivative(
		const double & s,
		int m
		) const {
	if(GenericRFunction< Data >::size() == 2){
		if(m == 1){
			return ( GenericRFunction< Data >::lastData() - GenericRFunction< Data >::firstData() ) / length();
		} else {
			std::cout << "\nGenericLinePath: Error: Derivative m = " << m
					<< " called for function with no more than 2 support points." << std::endl;
			throw;
		}
	}
	return std::pow(length(),m) * GenericRFunction< Data >::derivative(l(mods(s)),m);
}

template < class Data, class DataDist >
inline void GenericLinePath< Data, DataDist >::addPoint(const Data & p){
	double newl = GenericRFunction< Data >::size() > 0 ?
			(length() + DataDist()(p,GenericRFunction< Data >::lastData()))
			: 0.;
	GenericRFunction< Data >::add(newl,p);
}

template < class Data, class DataDist >
inline void GenericLinePath< Data, DataDist >::prepend(const Data & point){
	GenericLinePath< Data, DataDist > newlp;
	newlp.addPoint(point);
	for(unsigned int i = 0; i < GenericRFunction< Data >::size(); i++){
		newlp.addPoint(getPoint(i));
	}
	*this = newlp;
}

template < class Data, class DataDist >
inline void GenericLinePath< Data, DataDist >::append(const GenericLinePath< Data, DataDist > & path){
	for(unsigned int i = 0; i < path.size(); i++){
		addPoint(path.getPoint(i));
	}
}

/*
void GenericLinePath< Data, DataDist >::subDivide(int subPoints, double s0, double s1){

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

void GenericLinePath< Data, DataDist >::merge(const GenericLinePath & lp, double sMergePoint){

	// prepare:
	Data pMerge   = (*this)[sMergePoint];
	GenericLinePath newlp      = getSubLinePath(0.,sMergePoint);
	GenericLinePath pieceAfter = getSubLinePath(sMergePoint,1.);

	// insert:
	if(newlp.empty()){
		newlp = lp;
	} else {
		if(lp.GenericRFunction< Data >::getStart() != newlp.GenericRFunction< Data >::getEnd()) {
			newlp.addPoint(lp.GenericRFunction< Data >::getStart());
		}
		for(unsigned int i = 1; i < lp.points(); i ++){
			newlp.addPoint(lp.getPoint(i));
		}
	}

	// remainder:
	if(!pieceAfter.empty()){
		if(lp.GenericRFunction< Data >::getEnd() != pieceAfter.getPoint(1))
			newlp.addPoint(pieceAfter.getPoint(1));
		for(unsigned int i = 2; i < pieceAfter.points(); i ++){
			newlp.addPoint(pieceAfter.getPoint(i));
		}
	}

	*this = newlp;
	updateGrid();
}
*/

/*
template < class Data, class DataDist >
double GenericLinePath< Data, DataDist >::getMaxCurvatureS(
		const double & resolution,
		const double & s0,
		const double & s1,
		const double & peakThresholdRel
		) const{

	// prepare:
	if( s0 == s1 ) return s0;
	if( s0 > s1 ) return getMaxCurvatureS(resolution, s0, s1 + 1);
	double smax    = -1;
	double curvmax2 = -1;

	// find maximal curvature on points i:

	// prepare:
	int i0 = getIndex_aftereq(l(BLIB::mod(s0,1.)));
	int i1 = getIndex_before(l(BLIB::mod(s1,1.)));
	if(s1 >= 1) i1 += GenericRFunction< Data >::size();
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

	return BLIB::mod(smax,1.);
}

template < class Data, class DataDist >
void GenericLinePath< Data, DataDist >::exportDerivative(GenericRFunction< Data > & out, int m, int o) const{

	GenericRFunction< Data >::exportDerivative(out,m,o);

	for(unsigned int i = 0; i < out.size(); i++){
		out.points[i]  = s(out.points[i]);
		out.data[i]   *= pow(length(),m);
	}

}

template < class Data, class DataDist >
double GenericLinePath< Data, DataDist >::getCurvature(double s,int o) const{

	// check order:
	if(o < 0) return getCurvature(s,GenericRFunction< Data >::size() - 2);

	Data der  = derivative(s,1,o);
	Data der2 = derivative(s,2,o);
	return (der.x * der2.y - der.y * der2.x) / std::pow(der.x * der.x + der.y * der.y, 3./2.);

}
*/

template < class Data, class DataDist >
int GenericLinePath< Data, DataDist >::toSubLine(double & s) const{

	// simple checks:
	if(s <= 0) return 0;
	if(s >= 1) return lines() - 1;

	// find line:
	double l     = s * GenericLinePath< Data, DataDist >::length();
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

template < class Data, class DataDist >
int GenericLinePath< Data, DataDist >::getLineNr(const double & s) const{
	double hs = s;
	return toSubLine(hs);
}

template < class Data, class DataDist >
void GenericLinePath< Data, DataDist >::erase(int i){

	// check:
	assert( i >= 0 && i < int(GenericRFunction< Data >::size()) );

	// collect all higher points:
	std::vector< Data > pts(GenericRFunction< Data >::size() - i - 1);
	for(unsigned int j = 0; j < pts.size(); j++){
		pts[j] = getPoint(i + 1 + j);
	}

	// erase points:
	for(unsigned int j = 0; j <= pts.size(); j++){
		GenericRFunction< Data >::erase(getPointL(i + j));
	}

	// re-insert higher points:
	for(unsigned int j = 0; j < pts.size(); j++){
		addPoint(pts[j]);
	}
}


}

#endif /* GENERICLINEPATH_H_ */
