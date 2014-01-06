/**
 * @file Differentiating1D.h
 * @brief This file defines/implements the class Differentiating1D.
 *
 */

#ifndef Differentiating1D_H_
#define Differentiating1D_H_

#include <vector>
#include <assert.h>

#include "Periodic.h"
#include "HashTable.h"
#include "JString.h"

namespace jlib {

/**
 * @class Differentiating1D
 * @brief An interface for classes that are differentiable on arbitrary spaced 1D points.
 *
 * reference: http://amath.colorado.edu/faculty/fornberg/Docs/MathComp_88_FD_formulas.pdf
 */
template< class Data >
class Differentiating1D:
	public Periodic{

public:

	/// Constructor.
	Differentiating1D(
			int interpolOrder = 4
			);

	/// Destructor
	virtual ~Differentiating1D(){}

	/// Differentiating: returns the HashTable
	virtual const HashTable< Data, double > & hashTable() const = 0;

	/// set interpolation order
	inline void setInterpolOrder(int order) { m_interpolOrder = order; }

	/// returns the interpolation order
	inline int interpolOrder() const { return m_interpolOrder; }

	/// returns the point range
	inline const double & pointRange() const { return period(); }

	/** Differentiating1D: Returns the m-th derivative at x0
	 */
	Data derivative(const double & x0I, int m) const;

	/// prints a deriv domain
	void printDerivDomain(const double & x0, int m) const;

	/// prints the deriv coefficients
	void printDerivCoeffs(const double & x0, int m) const;

	/// returns the highest derivative
	inline int highestDerivative() const { return hashTable().size() - m_interpolOrder; }


protected:

	/// returns i inside range
	int modi(int i) const;


private:

	/// the interpolation order
	int m_interpolOrder;

	/** Returns the derivative coefficients delta(m,n,i) for m-th derivative at x0, a la
	 * http://amath.colorado.edu/faculty/fornberg/Docs/MathComp_88_FD_formulas.pdf
	 * The m-th derivative is
	 *
	 * 			d^m f / dx^m = sum_{nu = 0}^n delta(m,n,i) * f(point_near_i).
	 *
	 * The key is key(x0, m, n, i, o).
	 */
	HashTable< double, std::string > getDerivCoeffs_int(int x0,int m) const;

	/// interpolating at x0, possibly different from grid points
	HashTable< double, std::string > getDerivCoeffs(const double & x0,int m) const;

	/** Returns the domain of the derivative at x0
	 */
	std::vector< int > getDerivDomain_int(int x0, int m) const;

	/** Key for coefficient table, see
	 * http://amath.colorado.edu/faculty/fornberg/Docs/MathComp_88_FD_formulas.pdf
	 */
	std::string key(const double & x0, int m, int n, int i,int o) const;

	/** Calculates the derivative coefficients at x0, a la
	 * http://amath.colorado.edu/faculty/fornberg/Docs/MathComp_88_FD_formulas.pdf
	 */
	HashTable< double, std::string > calcDerivCoeffs(
			const double & x0,
			int m,
			const std::vector< int > & oi
			) const;

	/// Returns intervall of 2N + 1 points around x0
	std::vector< int > getNearbyPointIntervall_int(int x0, int m) const;

	/// interpolating at x0, possibly different from grid points
	std::vector< int > getNearbyPointIntervall(const double & x0, int m) const;

	/// defines the point interval
	void getPointIntervall(std::vector<int> & intervall, int iL, int iR) const;

	/// finds x position of point, including periodicity
	double findXPeriodic(int oin) const;
};

template< class Data >
Differentiating1D< Data >::Differentiating1D(
			int interpolOrder
			):
			Periodic(),
			m_interpolOrder(interpolOrder){
}

template< class Data >
Data Differentiating1D< Data >::derivative(const double & x0, int m) const{

	// check derivative request:
	assert( m >= 0 && m <= highestDerivative() );

	// prepare:
	double x                                = modx(x0);
	std::vector< int > oi                   = getNearbyPointIntervall(x,m);
	HashTable< double, std::string > coeffs = calcDerivCoeffs(x,m,oi);
	unsigned int n                          = oi.size() - 1;

	// first point:
	Data out = coeffs[key(x,m,n,0,m_interpolOrder)] * hashTable().data(oi[0]);

	// remaining points of domain:
	for(unsigned int i = 1; i <= n; i++){
		out += coeffs[key(x,m,n,i,m_interpolOrder)] * hashTable().data(oi[i]);
	}

	return out;
}

template< class Data >
std::vector< int > Differentiating1D< Data >::getNearbyPointIntervall(
		const double & x0,
		int m
		) const{

	// prepare:
	double x = modx(x0);

	// check if point is listed:
	int iL = hashTable().lastLessEqualKeyIndex(x);
	if(iL >= 0 && hashTable().key(iL) == x){
		return getNearbyPointIntervall_int(iL,m);
	}

	// prepare:
	std::vector< int > out;

	// non-periodic case:
	if(!periodic()){

		// find size:
		int outSize = m + m_interpolOrder;
		if(outSize % 2 == 1){
			int osq = (outSize - 1) / 2 - 1;
			if(iL - osq >= 0 && iL + 1 + osq < int(hashTable().size())){
				outSize--;
			}
		}
		out.resize(outSize);

	}

	// periodic case:
	else {

		// find size:
		int outSize = m + m_interpolOrder;
		if(outSize % 2 == 1){
			outSize--;
		}
		out.resize(outSize);

	}

	// fill intervall:
	getPointIntervall(out,iL,iL + 1);

	return out;
}

template< class Data >
std::vector< int > Differentiating1D< Data >::getNearbyPointIntervall_int(
		int x0I,
		int m
		) const{

	// prepare:
	std::vector< int > out;

	// case non-periodic:
	if(!periodic()){

		// check center point:
		if(x0I < 0 || x0I >= int(hashTable().size())) return out;

		// prepare:
		int outSize = m + m_interpolOrder;
		if(outSize % 2 == 0){
			int osq = outSize / 2 - 1;
			if(x0I - osq >= 0 && x0I + osq < int(hashTable().size())){
				outSize--;
			}
		}
		out.resize(outSize);
	}

	// periodic case:
	else {

		// find size:
		int outSize = m + m_interpolOrder;
		if(outSize % 2 == 0){
			outSize--;
		}
		out.resize(outSize);

	}

	// fill intervall:
	getPointIntervall(out,x0I,x0I);

	return out;
}

template< class Data >
void Differentiating1D< Data >::getPointIntervall(
		std::vector<int> & intervall,
		int iL,
		int iR
		) const{

	// prepare:
	std::vector< int > & out = intervall;
	int outSize         = int(out.size());
	int il              = 0;
	int ir              = 0;

	// right left right left:
	int actSign = 1;
	for(int i = 0; i < outSize; i++){

		// next to the right:
		if(actSign > 0){

			// case right side is ok:
			if(periodic() || iR + ir < int(hashTable().size())){
				out[i] = iR + ir;
				ir++;
				actSign = -1;
				if(iL == iR && il == 0) il = 1;
			}

			// else switch direction:
			else {
				actSign = -1;
				i--;
			}
		}

		// next to the left:
		else {

			// case left side is ok:
			if(periodic() || iL - il >= 0){
				out[i] = iL - il;
				il++;
				actSign = 1;
				if(iL == iR && ir == 0) ir = 1;
			}

			// else switch direction:
			else {
				actSign = 1;
				i--;
			}
		}
	}
}

template< class Data >
HashTable< double, std::string > Differentiating1D< Data >::calcDerivCoeffs(
		const double & x0,
		int mm,
		const std::vector< int > & oi
		) const {

	// prepare:
	HashTable< double, std::string > derCoeffs;

	unsigned int M    = mm;
	unsigned int nmax = oi.size() - 1;
	double x          = modx(x0);

	// initial:
	derCoeffs.set(key(x,0,0,0,m_interpolOrder),1);
	double c1 = 1;

	for(unsigned int n = 1; n <= nmax; n++){

		double c2 = 1.;
		unsigned int mmax  = n <= M ? n : M;

		for(unsigned int nu = 0; nu < n; nu++){

			double c3 = findXPeriodic(oi[n]) - findXPeriodic(oi[nu]);
			c2       *= c3;

			for(unsigned int m = 0; m <= mmax; m++){

				double xn = findXPeriodic(oi[n]);

				double dc1 = derCoeffs[key(x,m,n - 1,nu,m_interpolOrder)];
				double dc2 = derCoeffs[key(x,m - 1,n - 1,nu,m_interpolOrder)];
				double coeff = (
						(xn - x0) * dc1 - m * dc2
						) / c3;
				derCoeffs.set(key(x,m,n,nu,m_interpolOrder),coeff);
			}
		}

		for(unsigned int m = 0; m <= mmax; m++){

			double xn  = findXPeriodic(oi[n - 1]);
			double dc1 = derCoeffs[key(x,m - 1,n - 1,n - 1,m_interpolOrder)];
			double dc2 = derCoeffs[key(x,m,n - 1,n - 1,m_interpolOrder)];

			double coeff = c1 / c2 * (
					m * dc1 - (xn - x0) * dc2
					);

			derCoeffs.set(key(x,m,n,n,m_interpolOrder),coeff);
		}

		c1 = c2;

	}

	return derCoeffs;
}

template< class Data >
double Differentiating1D< Data >::findXPeriodic(
		int oin
		) const{

	// non-periodic case:
	if(!periodic()) return hashTable().toc()[oin];

	// periodic case:
	int perdiff = oin - modi(oin);
	double xn   = 0;

	// right shift:
	if(perdiff > 0) {
		xn = hashTable().lastKey() + periodicStep() + hashTable().toc()[oin];
	}

	// left shift:
	else if(perdiff < 0){

		// prepare:
		xn      = hashTable().firstKey();
		int act = hashTable().size() - 1;

		// loop:
		for(int oim = oin; oim < 0; oim++){

			// first subtract last step:
			if(oim == oin){
				xn -= periodicStep();
			}

			// then subtract step by step:
			else {
				xn -= hashTable().toc()[act] - hashTable().toc()[act - 1];
				act--;
			}
		}
	}

	// else no shift:
	else {
		xn = hashTable().toc()[oin];
	}

	return xn;
}

template< class Data >
int Differentiating1D< Data >::modi(int i) const{
	if(!periodic() || hashTable().size() < 2 || ( i >= 0 && i < int(hashTable().size()) ) ) return i;
	if(i < 0) return modi(i + hashTable().size());
	return modi(i - hashTable().size());
}

template< class Data >
inline std::string Differentiating1D< Data >::key(
		const double & x0,
		int m,
		int n,
		int i,
		int o
		) const{
	return String(x0) + "::" + String(m) + ":" + String(n) + ":" + String(i)+ ":" + String(o);
}

}

#endif /* Differentiating1D_H_ */
