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
    blib::GenericRFunction

Description
    See below

SourceFiles

References
	[1] J. Schmidt, C. Peralta, B. Stoevesandt, "Automated Generation of
	    Structured Meshes for Wind Energy Applications", Proceedings of the
	    Open Source CFD International Conference, 2012, London, UK

\*---------------------------------------------------------------------------*/

#ifndef GENERICRFUNCTION_H_
#define GENERICRFUNCTION_H_

#include "String.h"
#include "GridDifferentiable1D.h"
#include "UnstructuredGrid1D.h"

namespace blib {

/**
 * @class blib::GenericRFunction
 * @brief A differentiable function f: R -> T
 *
 * Note: For non-trivial output the following functions are required to be specialized for T:
 * 			std::string GenericRFunction<T>::dataOutputDescription() const;
 * 			std::string GenericRFunction<T>::dataOutput(const T & t) const;
 */

template <class T>
class GenericLinePath;

template <class T>
class GenericRFunction:
	public GridDifferentiable1D<T>,
	public UnstructuredGrid1D{

public:

	friend class GenericLinePath<T>;

	/// struct for function data
	struct FunctionPoint{
		double x;
		T y;
	};

	/// Constructor
	GenericRFunction(int interpolOrder = 4, const std::string & name = "GenericRFunction");

	/// Constructor
	GenericRFunction(
			const std::vector<double> xvals,
			const std::vector<T> yvals,
			int interpolOrder = 4,
			const std::string & name = "GenericRFunction"
			);

	/// Copy constructor
	GenericRFunction(
			const GenericRFunction<T> & grf
			);

	/// Destructor
	virtual ~GenericRFunction(){}

	/// GenericRFunction: Description of data output. Please specialize elsewhere!
	inline std::string dataOutputDescription() const { return ""; }

	/// GenericRFunction: Data output. Please specialize elsewhere!
	inline std::string dataOutput(const T & t) const { return ""; }

	/// Outputable: Provides the data that will be written to the output file.
	Outputable::OData getOData(
			const std::string & type = IO::OFILE::TYPE::EMPTY) const;

	/// adds a data point
	void add(double xval, const T & yval);

	/// GenericRFunction: Prints data
	virtual void print() const;

	/// GenericRFunction: Exports the m-th derivative, calculated to order o
	virtual void exportDerivative(GenericRFunction<T> & out, int m, int o) const;

	/// UnstructuredGrid1D: erases element i
	virtual void erase(int i);

	/// Provides data at a grid point
	inline T & getData(int i) { return data[elementIndex(modi(i))]; }

	/// GridDifferentiable1D: Provides data at a grid point
	inline const T & getData(int i) const { return data[elementIndex(modi(i))]; }

	/// Provides first data
	inline const T & getFirstData() const { return data[firstElementIndex()]; }

	/// Provides last data
	inline const T & getLastData() const { return data[lastElementIndex()]; }

	/// The operator =
	GenericRFunction<T> & operator=(const GenericRFunction<T> & rhs);

	/// Reference to the Grid
	inline const UnstructuredGrid1D & getGrid() const { return *this; }

	/// returns a point
	FunctionPoint getFunctionPoint(int i) const;

	/// returns list of x vals, in order
	std::vector<double> getListX() const;

	/// returns list of y vals, in order
	std::vector<T> getListY() const;

	/// sets the interpolOrder
	inline void setInterpolOrder(int order) { interpolOrder = order; }

	/// Returns the interpolOrder
	inline int getInterpolOrder() const { return int(size()) < interpolOrder ? ( size() - size() % 2 ) : interpolOrder; }

	/// Returns the number of display points. Negative or zero = as many as size.
	inline int getDisplayPoints() const { return displayPoints; }

	/// Sets the number of display points. Negative or zero = as many as size.
	inline void setDisplayPoints(int n) { displayPoints = n; }

	/// Returns the sub-function with xlow <= x <= xhigh.
	GenericRFunction<T> getSubFunction(double xlow, double xhigh, bool addEnds = true) const;

	/// interpolate the function value, to interpolOrder
	virtual T operator[](double x) const { return GridDifferentiable1D<T>::derivative(x,0,getInterpolOrder()); }


protected:

	/// the number of points to be displayed
	int displayPoints;

	/// Reference to the Grid
	inline UnstructuredGrid1D & _getGrid() { return *this; }


private:

	/// the interpolation order
	int interpolOrder;

	/// The data
	std::vector<T> data;
};

template <class T>
GenericRFunction<T>::GenericRFunction(
		int interpolOrder,
		const std::string & name
		):
	GridDifferentiable1D<T>(this),
	UnstructuredGrid1D(false,name),
	displayPoints(-1),
	interpolOrder(interpolOrder),
	data(){
}

template <class T>
GenericRFunction<T>::GenericRFunction(
		const std::vector<double> xvals,
		const std::vector<T> yvals,
		int interpolOrder,
		const std::string & name
		):
		GridDifferentiable1D<T>(this),
		UnstructuredGrid1D(xvals,false,name),
		displayPoints(-1),
		interpolOrder(interpolOrder),
		data(yvals){
	orderInit();
	order();
}

template <class T>
GenericRFunction<T>::GenericRFunction(
		const GenericRFunction<T> & grf
		):
		GridDifferentiable1D<T>(this),
		UnstructuredGrid1D(grf),
		displayPoints(grf.displayPoints),
		interpolOrder(grf.interpolOrder),
		data(grf.data){
}

template <class T>
GenericRFunction<T> & GenericRFunction<T>::operator=(const GenericRFunction<T> & rhs){
	if(this != &rhs){
		_getGrid()       = rhs;
		Ordered::dim     = rhs.Ordered::dim;
		Ordered::name    = rhs.Ordered::name;
		Ordered::n_order = rhs.Ordered::n_order;
		Ordered::map_n2i = rhs.Ordered::map_n2i;
		interpolOrder    = rhs.interpolOrder;
		data             = rhs.data;
		displayPoints    = rhs.displayPoints;
	}
	return *this;
}

template <class T>
void GenericRFunction<T>::add(double xval, const T & yval){
	UnstructuredGrid1D::add(xval);
	data.push_back(yval);
}

template <class T>
typename GenericRFunction<T>::FunctionPoint GenericRFunction<T>::getFunctionPoint(int i) const{

	FunctionPoint out;
	int hi = modi(i);
	out.x = getPoint1D(hi);
	out.y = getData(hi);

	return out;

}

template <class T>
std::vector<double> GenericRFunction<T>::getListX() const{

	std::vector<double> out(size());
	for(unsigned int i = 0; i < size(); i++){
		out[i] = getPoint1D(i);
	}
	return out;
}

template <class T>
std::vector<T> GenericRFunction<T>::getListY() const{

	std::vector<T> out(size());
	for(unsigned int i = 0; i < size(); i++){
		out[i] = getData(i);
	}
	return out;
}

template <class T>
void GenericRFunction<T>::exportDerivative(GenericRFunction<T> & out, int m, int o) const{

	// prepare:
	std::string outName      = name + "_deriv_" + blib::String(m) + "_order_" + blib::String(o);
	std::vector<double> xout = getListX();
	std::vector<T> yout(size());

	for(unsigned int i = 0; i < size(); i++){
		yout[i] = GridDifferentiable1D<T>::derivative(xout[i],m,o);
	}

	out = GenericRFunction<T>(xout,yout,interpolOrder,outName);
}

template <class T>
GenericRFunction<T> GenericRFunction<T>::getSubFunction(double xlow, double xhigh, bool addEnds) const{

	// prepare:
	std::string subName = name + "_sub_" + blib::String(xlow) + "_to_" + blib::String(xhigh);
	GenericRFunction<T> out(interpolOrder,subName);

	// check:
	if(xlow > xhigh) return out;
	if(xlow > getLastPoint1D() || xhigh < getFirstPoint1D()) return out;

	// get first and last indices:
	int ifirst = getIndex_aftereq(xlow);
	if(ifirst < 0) return out;
	int ilast = getIndex_beforeeq(xhigh);
	if(ilast < 0) return out;

	//fill:
	for(int i = ifirst; i <= ilast; i++){
		out.add(getPoint1D(i),getData(i));
	}

	// add ends:
	if(addEnds){

		// add first point:
		if(out.size() == 0 || out.getFirstPoint1D() > xlow){
			out.add(xlow,(*this)[xlow]);
		}

		// add last point:
		if(out.getLastPoint1D() < xhigh){
			out.add(xhigh,(*this)[xhigh]);
		}
	}

	return out;
}

template <class T>
void GenericRFunction<T>::print() const{
	/*
	for(unsigned int i = 0; i < size(); i++){
		cout << i << " " << getPoint1D(i) << " " << getData(i) << endl;
	}
	*/
}

template <class T>
inline void GenericRFunction<T>::erase(int i){
	data.erase(data.begin() + i);
	UnstructuredGrid1D::erase(i);
}

template <class T>
Outputable::OData GenericRFunction<T>::getOData(
			const std::string & type) const{

	Outputable::OData out(1);
	IO::OFILE::Data   dat;

	// set basic info:
	dat.name   = UnstructuredGrid1D::getName();
	dat.type   = type;
	dat.origin = "GenericRFunction";

	////////////////////////////////////////////////////////////
	//
	// 		gnuplot data file output:
	//
	if(type.compare(IO::OFILE::TYPE::GNUPLOT_DATA) == 0){

		// set header:
		dat.header = IO::OFILE::PREAMBLE::GNUPLOT_DATA;

		// set pre- and post-data:
		dat.pre_data  = "# Function " + dat.name + ": function_x " + dataOutputDescription() + "\n";
		dat.post_data = "";
		dat.data      = "";

		// case as many as available:
		if(displayPoints <= 0 || displayPoints == int(size())){
			for(int i = 0; i < int(size()); i++){
				dat.data += String(getPoint1D(i)) + " " + dataOutput(getData(i)) + "\n";
			}
		}

		// case interpolation:
		else {
			double delta = (getLastPoint1D() - getFirstPoint1D()) / (displayPoints - 1);
			for(int i = 0; i < displayPoints; i++){
				double x  = getFirstPoint1D() + i * delta;
				dat.data += String(x) + " " + dataOutput(operator[](x)) + "\n";
			}
		}
	}

	out[0] = dat;
	return out;

}

}

#endif /* GENERICRFUNCTION_H_ */
