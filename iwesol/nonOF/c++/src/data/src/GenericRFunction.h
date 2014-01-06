/**
 * @file GenericRFunction.h
 * @brief This file defines/implements the class GenericRFunction.
 *
 */

#ifndef GENERICRFUNCTION_H_
#define GENERICRFUNCTION_H_

#include "HashTable.h"
#include "Differentiating1D.h"

namespace jlib {

/**
 * @class jlib::GenericRFunction
 * @brief A differentiable function f: R -> Data
 *
 */

template < class Data >
class GenericRFunction:
	public Differentiating1D< Data >{

public:

	/// Constructor
	GenericRFunction(
			int interpolOrder = 4,
			const std::string & name = "GenericRFunction"
			);

	/// Constructor
	GenericRFunction(
			const std::vector< double > xvals,
			const std::vector< Data > yvals,
			int interpolOrder = 4,
			const std::string & name = "GenericRFunction"
			);

	/// Periodic: returns first point
	inline const double & firstPoint() const { return m_data.firstKey(); }

	/// Periodic: returns last point
	inline const double & lastPoint() const { return m_data.lastKey(); }

	/// Differentiating: returns the HashTable
	inline const HashTable< Data, double > & hashTable() const { return m_data; }

	/// returns the HashTable
	inline HashTable< Data, double > hashTable() { return m_data; }

	/// returns first data
	inline const Data & firstData() const { return m_data.firstData(); }

	/// returns last data
	inline const Data & lastData() const { return m_data.lastData(); }

	/// returns a point
	inline const double & point(int i) const { return m_data.key(Differentiating1D< Data >::modi(i)); }

	/// returns data
	inline const Data & data(int i) const { return m_data.data(Differentiating1D< Data >::modi(i)); }

	/// Returns point list
	inline const std::vector< double > & pointList() const { return m_data.toc(); }

	/// Returns data list
	inline std::vector< Data > dataList() const;

	/// the size
	inline unsigned int size() const { return m_data.size(); }

	/// adds a data point
	inline void add(const double & xval, const Data & yval);

	/// erase a point
	inline void erase(const double & xval) { m_data.erase(xval); Differentiating1D< Data >::update(); }

	/// check if empty
	inline bool empty() const { return m_data.empty(); }

	/// shift entries
	void shift(const double & delta);

	/// returns the name
	inline const std::string & name() const { return m_name; }

	/// resets the name
	inline void setName(const std::string & name) { m_name = name; }

	/// returns the interpolated value at x
	inline Data operator[](const double & x) const;

	/// assignment operator
	GenericRFunction< Data > & operator=(const GenericRFunction< Data > & rhs);


private:

	/// The data
	HashTable< Data, double > m_data;

	/// the name
	std::string m_name;
};

template < class Data >
GenericRFunction< Data >::GenericRFunction(
		int interpolOrder,
		const std::string & name
		):
		Differentiating1D< Data >
		(
				interpolOrder
		),
		m_name(name){
}

template < class Data >
GenericRFunction< Data >::GenericRFunction(
		const std::vector< double > xvals,
		const std::vector< Data > yvals,
		int interpolOrder,
		const std::string & name
		):
		Differentiating1D< Data >
		(
				interpolOrder
		),
		m_data(xvals,yvals),
		m_name(name){
}

template < class Data >
inline void GenericRFunction< Data >::add(
		const double & xval,
		const Data & yval
		) {
	m_data.set(xval,yval);
	Differentiating1D< Data >::update();
}

template < class Data >
inline std::vector< Data > GenericRFunction< Data >::dataList() const{
	std::vector< Data > out(m_data.size());
	for(unsigned int i = 0; i < m_data.size(); i++){
		out[i] = m_data[m_data.toc()[i]];
	}
	return out;
}

template < class Data >
inline Data GenericRFunction< Data >::operator[](const double & x) const{
	assert(size() > 0);
	if(size() == 1) return data(0);
	return Differentiating1D< Data >::derivative(x,0);
}

template < class Data >
GenericRFunction< Data > & GenericRFunction< Data >::operator=(
		const GenericRFunction< Data > & rhs
		){
	if(this != &rhs){
		m_data = rhs.m_data;
		m_name = rhs.m_name;
		if(!empty()) Differentiating1D< Data >::update();
	}
	return *this;
}

template < class Data >
void GenericRFunction< Data >::shift(const double & delta){
	std::vector< Data > pts = dataList();
	std::vector< double > xs = pointList();
	m_data.clear();
	for(unsigned int i = 0; i < xs.size(); i++){
		add
		(
				Differentiating1D< Data >::modx(xs[i] + delta),
				pts[i]
		);
	}
}

}

#endif /* GENERICRFUNCTION_H_ */
