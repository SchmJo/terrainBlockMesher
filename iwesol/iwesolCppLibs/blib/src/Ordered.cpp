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

#include <iostream>
using namespace std;

#include "Ordered.h"
#include "IO.h"

namespace blib{

Ordered::Ordered(const string & name) :
	name(name)
	{
}

Ordered::~Ordered() {
}

void Ordered::orderInit() {

	unsigned int dim = size();

	if (dim > 0) {
		n_order.clear();
		for(unsigned i = 0;i<dim;i++)
			n_order.push_back(i);

	}
	__calcMap_n2i();

}

void Ordered::order() {

	// pre-check that n_order is not empty:
	if (n_order.empty()) {
		orderInit();
		if (n_order.empty())
			return;
	}

	__mergesort();

	__calcMap_n2i();

}

void Ordered::orderPrint(){

	vector<int>::iterator it = n_order.begin();

	cout<<"[ ";
	while(it < n_order.end()){
		cout << *it << " ";
		it++;
	}
	cout << "]"<<endl;

}

bool Ordered::orderEraseEl(int deli){

	// simple checks:
	if(size() != orderSize() - 1) return false;
	if(deli < 0 || deli > (int)orderSize() - 1) return false;

	// prepare:
	vector<int> neworder(size());
	vector<int> newmap_n2i(size());
	int deln = n_order[deli];

	// lower elements:
	for(int n = 0; n < (int)orderSize(); n++){

		if(n != deln){

			int hn = n < deln ? n : n - 1;
			int hi = map_n2i[n] < deli ? map_n2i[n] : map_n2i[n] - 1;

			neworder[hn]   = hi;
			newmap_n2i[hi] = hn;

		}
	}

	// set:
	n_order = neworder;
	map_n2i = newmap_n2i;

	return true;
}

bool Ordered::orderRegNewEl(){


	// simple checks:
	if(size() != orderSize() + 1) return false;
	if(size() == 0) return false;
	if(size() == 1){
		n_order = vector<int>(1,0);
		map_n2i = vector<int>(1,0);
		return true;
	}

	// find position where to enter the element i = size() - 1:
	int newi = size() - 1;
	int n = enterWhere(newi,0,orderSize() - 1);

	// insert:
	n_order.push_back(n);
	for(int i = 0; i < newi; i++){
		if(n_order[i] >= n) n_order[i]++;
	}

	// recalc inverse:
	__calcMap_n2i();

	return true;
}

void Ordered::__calcMap_n2i(){

	// init:
	map_n2i.resize(n_order.size());

	// invert n_order:
	for(unsigned int i = 0; i < size(); i++){
		int n = n_order[i];
		map_n2i[n] = i;
	}

}

/* Code based on
 * http://www.cprogramming.com/tutorial/computersciencetheory/merge.html,
 * adopted to vector<int> and 'i_before_j'
 */
void Ordered::__merge_helper(int left, int right, vector<int> & scratch) {

	vector<int> & input = n_order;

	/* base case: one element */
	if (right == left + 1) {
		return;
	} else {
		int i = 0;
		int length = right - left;
		int midpoint_distance = length / 2;
		/* l and r are to the positions in the left and right subarrays */
		int l = left, r = left + midpoint_distance;

		/* sort each subarray */
		__merge_helper(left, left + midpoint_distance, scratch);
		__merge_helper(left + midpoint_distance, right, scratch);

		/* merge the arrays together using scratch for temporary storage */
		for (i = 0; i < length; i++) {
			/* Check to see if any elements remain in the left array; if so,
			 * we check if there are any elements left in the right array; if
			 * so, we compare them.  Otherwise, we know that the merge must
			 * use take the element from the left array */
			if (l < left + midpoint_distance && (r == right ||
			//max(input[l],input[r]) == input[l]
					i_before_j(input[l], input[r]))) {
				scratch[i] = input[l];
				l++;
			} else {
				scratch[i] = input[r];
				r++;
			}
		}
		/* Copy the sorted subarray back to the input */
		for (i = left; i < right; i++) {
			input[i] = scratch[i - left];
		}
	}
}

/* Code based on
 * http://www.cprogramming.com/tutorial/computersciencetheory/merge.html,
 * adopted to vector<int>
 */
int Ordered::__mergesort() {

	if (n_order.empty())
		return 0;

	vector<int> & input = n_order;

	unsigned int size = input.size();
	vector<int> scratch(size);

	__merge_helper(0, size, scratch);

	return 1;

}

int Ordered::enterWhere(int i, int nmin, int nmax) const {

	// simple checks:
	if(nmin > nmax) return enterWhere(i,nmax,nmax);
	if(i_beforeeq_j(i,map_n2i[nmin])) return nmin;
	if(i_after_j(i,map_n2i[nmax])) return nmax + 1;
	if(i_equal_j(i,map_n2i[nmax])) return nmax;
	if(nmax == nmin + 1) return nmax;

	// divide in roughly half:
	int nmid = (nmin + nmax) / 2;

	// pick a half:
	if(i_beforeeq_j(i,map_n2i[nmid])) {
		return enterWhere(i,nmin,nmid);
	}

	return enterWhere(i,nmid + 1,nmax);
}

void Ordered::orderRotate(int d){

	// check:
	if(d == 0) return;
	if(d >= int(size())) return orderRotate(d - size());
	if(d < 0) return orderRotate(d + size());

	// prepare:
	int minusd = size() - d;

	vector<int> temp = map_n2i;
	for(unsigned int n = 0; n < size(); n++){
		int newn            = (n + minusd) % size();
		map_n2i[n]          = temp[newn];
		n_order[temp[newn]] = n;
	}

}

}

