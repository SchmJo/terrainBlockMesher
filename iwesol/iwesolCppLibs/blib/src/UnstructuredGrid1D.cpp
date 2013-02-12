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
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright  held by original author
     \\/     M anipulation  |
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

\*---------------------------------------------------------------------------*/

#include "UnstructuredGrid1D.h"
using namespace std;

namespace blib {

UnstructuredGrid1D::UnstructuredGrid1D(
		bool periodic,
		const string & gridName
		):
	Grid(0,gridName),
	flag_periodic(periodic),
	lastStepX(0){
}

UnstructuredGrid1D::UnstructuredGrid1D(
		const vector<double> & points,
		bool periodic,
		const string & gridName
		):
		Grid(points.size(),gridName),
		points(points),
		flag_periodic(periodic),
		lastStepX(0){
	orderInit();
}

UnstructuredGrid1D::~UnstructuredGrid1D() {
}

int UnstructuredGrid1D::getIndexCellNr(const COORD_GRID & p) const{
	if(!insideGrid(p) || points.size() < 2) return -1;
	int out = 0;
	while(p[0] <= getPoint1D(out + 1)) out++;
	return out;
}

MultiIndexed::IndexCell UnstructuredGrid1D::getIndexCell(const int cellnr) const{
	MultiIndexed::IndexSet is(1,cellnr);
	return MultiIndexed::IndexCell(1,is);
}

Grid::ConnectorSet UnstructuredGrid1D::getConnectors(const MultiIndexed::IndexSet & x) const{
	Grid::Connector conn1(1);
	Grid::Connector conn2(1);
	Grid::ConnectorSet cs(1);
	int i = x[0];
	if(i == 0) {
		conn1[0] = 1;
		cs[0]    = conn1;
	}
	else if(i == (int)points.size()){
		conn1[0] = points.size() - 1;
		cs[0]    = conn1;
	}
	else {
		conn1[0] = i - 1;
		conn2[0] = i + 1;
		cs.resize(2);
		cs[0]    = conn1;
		cs[1]    = conn2;
	}
	return cs;
}

double UnstructuredGrid1D::_findXPeriodic(int oin) const{

	// non-periodic case:
	if(!flag_periodic) return getPoint1D(oin);

	// periodic case:
	int perdiff = oin - modi(oin);
	double xn   = 0;

	// right shift:
	if(perdiff > 0) {
		xn = getLastPoint1D() + lastStepX + getPoint1D(oin);
	}

	// left shift:
	else if(perdiff < 0){

		// prepare:
		xn      = getFirstPoint1D();
		int act = size() - 1;

		// loop:
		for(int oim = oin; oim < 0; oim++){

			// first subtract last step:
			if(oim == oin){
				xn -= lastStepX;
			}

			// then subtract step by step:
			else {
				xn -= getPoint1D(act) - getPoint1D(act - 1);
				act--;
			}
		}
	}

	// else no shift:
	else {
		xn = getPoint1D(oin);
	}

	return xn;
}

HashStringTable<double> UnstructuredGrid1D::calcDerivCoeffs(double x0,int mm,int o) const {

	// check order:
	if(o < 0) return calcDerivCoeffs(x0,mm,size() - mm);

	// calculate domain:
	const vector<int> & oi = getDerivDomain(x0,mm,o);

	// prepare:
	HashStringTable<double> derCoeffs(oi.size());
	unsigned int M    = mm;
	unsigned int nmax = oi.size() - 1;
	double x          = modx(x0);

	// initial:
	derCoeffs.set(1.,key(x,0,0,0,o));
	double c1 = 1;
	for(unsigned int n = 1; n <= nmax; n++){

		double c2 = 1.;
		unsigned int mmax  = n <= M ? n : M;

		for(unsigned int nu = 0; nu < n; nu++){

			double c3 = _findXPeriodic(oi[n]) - _findXPeriodic(oi[nu]);
			c2       *= c3;

			for(unsigned int m = 0; m <= mmax; m++){
				double xn = _findXPeriodic(oi[n]);

				double dc1 = 0.;
				double dc2 = 0.;
				derCoeffs.recall(dc1,key(x,m,n - 1,nu,o));
				derCoeffs.recall(dc2,key(x,m - 1,n - 1,nu,o));
				double coeff = (
						(xn - x0) * dc1 - m * dc2
						) / c3;
				derCoeffs.set(coeff,key(x,m,n,nu,o));
			}

		}

		for(unsigned int m = 0; m <= mmax; m++){

			double xn = _findXPeriodic(oi[n - 1]);

			double dc1 = 0.;
			double dc2 = 0.;
			derCoeffs.recall(dc1,key(x,m - 1,n - 1,n - 1,o));
			derCoeffs.recall(dc2,key(x,m,n - 1,n - 1,o));

			double coeff = c1 / c2 * (
					m * dc1 - (xn - x0) * dc2
					);
			derCoeffs.set(coeff,key(x,m,n,n,o));
		}

		c1 = c2;

	}

	return derCoeffs;
}


void UnstructuredGrid1D::_getPointIntervall(vector<int> & intervall, int iL, int iR) const{

	// prepare:
	vector<int> & out = intervall;
	int outSize       = int(out.size());
	int il            = 0;
	int ir            = 0;

	// right left right left:
	int actSign = 1;
	for(int i = 0; i < outSize; i++){

		// next to the right:
		if(actSign > 0){

			// case right side is ok:
			if(flag_periodic || iR + ir < int(size())){
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
			if(flag_periodic || iL - il >= 0){
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


	/*
	// shortest distance:
	for(int i = 1; i < outSize; i++){

		// case right side is ok:
		if(x0 + ir < (int)size()){

			// case no boundary is hit:
			if(x0 - il >= 0){
				double distr = fabs(p0 - getPoint1D(x0 + ir));
				double distl = fabs(p0 - getPoint1D(x0 - il));
				if(distr <= distl){
					out[i] = x0 + ir;
					ir++;
				} else {
					out[i] = x0 - il;
					il++;
				}
			}

			// else left not ok, add at right:
			else {
				out[i] = x0 + ir;
				ir++;
			}

			continue;
		}

		// else right not ok, check if left is ok:
		else if(x0 - il >= 0){
			out[i] = x0 - il;
			il++;
		}

		// else the domain must end:
		else {
			out.resize(i);
			break;
		}

	}
	*/


}

vector<int> UnstructuredGrid1D::getNearbyPointIntervall_int(int x0I, int m, int o) const{

	// check order:
	if(o < 0) return getNearbyPointIntervall_int(x0I,m,size() - m);

	// prepare:
	vector<int> out;

	// case non-periodic:
	if(!flag_periodic){

		// check center point:
		if(x0I < 0 || x0I >= int(size())) return vector<int>();

		// prepare:
		int outSize = m + o;
		if(outSize % 2 == 0){
			int osq = outSize / 2 - 1;
			if(x0I - osq >= 0 && x0I + osq < int(size())){
				outSize--;
			}
		}
		out.resize(outSize);
	}

	// periodic case:
	else {

		// find size:
		int outSize = m + o;
		if(outSize % 2 == 0){
			outSize--;
		}
		out.resize(outSize);

	}

	// fill intervall:
	_getPointIntervall(out,x0I,x0I);

	return out;
}

vector<int> UnstructuredGrid1D::getNearbyPointIntervall(double x0, int m, int o) const{

	// check order:
	if(o < 0) return getNearbyPointIntervall(x0,m,size() - m);

	// prepare:
	double x = modx(x0);

	// check if point is listed:
	int iL = getIndex_beforeeq(x);
	if(iL >= 0 && getPoint1D(iL) == x){
		return getNearbyPointIntervall_int(iL,m,o);
	}

	// prepare:
	vector<int> out;

	// non-periodic case:
	if(!flag_periodic){

		// find size:
		int outSize = m + o;
		if(outSize % 2 == 1){
			int osq = (outSize - 1) / 2 - 1;
			if(iL - osq >= 0 && iL + 1 + osq < int(size())){
				outSize--;
			}
		}
		out.resize(outSize);

	}

	// periodic case:
	else {

		// find size:
		int outSize = m + o;
		if(outSize % 2 == 1){
			outSize--;
		}
		out.resize(outSize);

	}

	// fill intervall:
	_getPointIntervall(out,iL,iL + 1);

	return out;
}

void UnstructuredGrid1D::print() const{

	for(unsigned int i = 0; i < size(); i++){
		cout << "i = " << i << ": x[i] = " << getPoint1D(i) << endl;
	}

}

void UnstructuredGrid1D::printDerivDomain(double x0, int m, int o) const{

	cout << "x0 = " << x0 << endl;
	cout << "m  = " << m << endl;
	cout << "o  = " << o << endl;
	cout << "x  = " << modx(x0) << endl;

	// prepare:
	vector<int> dom = getDerivDomain(modx(x0),m,o);

	for(unsigned int n = 0; n < dom.size(); n++){
		cout << n << ": i = " << dom[n] << ", x[i] = " << getPoint1D(dom[n]) << endl;
	}
}

void UnstructuredGrid1D::printDerivCoeffs(double x0, int m, int o) const{

	// prepare:
	double x                       = modx(x0);
	vector<int>  dom               = getDerivDomain(x,m,o);
	HashStringTable<double> coeffs = getDerivCoeffs(x,m,o);

	for(unsigned int i = 0; i < dom.size(); i++){

		cout << i << ": i = " << dom[i] << ", x[i] = " << getPoint1D(dom[i])
				<< ": coeff = " << coeffs[key(x,m, dom.size() - 1, i, o)] << endl;

	}
}

void UnstructuredGrid1D::shift(double deltax){

	// find current boundaries:
	double lfirst = getFirstPoint1D();
	double llast  = getFirstPoint1D() + getXRange();
	double range  = getXRange();

	// shift points:
	int countRot = 0;
	for(unsigned int i = 0; i < size(); i++){

		// new point:
		double newx = points[elementIndex(i)] + deltax;

		// rotate point, if periodic:
		if(isPeriodic()){
			if(newx < lfirst){
				countRot--;
				newx += range;
			} else if(newx >= llast){
				countRot++;
				newx -= range;
			}
		}

		// shift:
		points[elementIndex(i)] = newx;
	}

	// rotate order:
	if(isPeriodic()){
		orderRotate(countRot);
	}

}

double UnstructuredGrid1D::modx(double x) const {
	if(!flag_periodic || size() < 2
			|| (
					chop(x) >= chop(getFirstPoint1D())
					&& chop(x) < chop(getFirstPoint1D() + getXRange())
				)
		) return chop(x);
	if(chop(x) < chop(getFirstPoint1D())) return modx(x + getXRange());
	return modx(x - getXRange());
}

}
