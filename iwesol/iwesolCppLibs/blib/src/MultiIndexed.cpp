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

#include "MultiIndexed.h"
#include "IO.h"

namespace blib{

MultiIndexed::MultiIndexed(const int n1, const int n2, const int n3) {
	vector<int> hn;

	hn.push_back(n1);
	hn.push_back(n2);
	hn.push_back(n3);

	init(hn);
}

MultiIndexed::MultiIndexed(const vector<int> & n) {
	init(n);
}

MultiIndexed::~MultiIndexed() {
}

void MultiIndexed::init(const vector<int> & hn) {

	dim = 0;
	trueDim = 0;
	size = 0;

	if (!hn.empty()) {

		vector<int> hhn(hn);
		vector<int>::iterator it_n = hhn.begin();

		// fill n, including zeros:
		while (it_n < hhn.end()) {

				dim++;
				n.push_back(*it_n);

			if (*it_n > 0) {
				trueDim++;
				if (size == 0)
					size = *it_n;
				else
					size *= *it_n;
			}

			it_n++;
		}

		// all entries zero:
		if(trueDim == 0){
			//cerr << "MultiIndexed: Error: all dimensions are zero." << endl;
			n.clear(); dim = 0;
			return;
		}

		// else delete final zeros:
		it_n = n.end() - 1;
		while(it_n >n.begin() && *it_n == 0){
			n.erase(it_n);
			it_n--;
			dim--;
		}


	}

}

void MultiIndexed::print_labels() const {

	bool ok = check();

	if (!ok)
		cout << "# MultiIndexed: Error in the indexing:\n" << endl;

	for (unsigned int i = 0; i < size; i++) {

		vector<int> x = MultiIndexed::i2x(i);

		cout << "i = " << i << " --> ";
		for (unsigned int j = 0; j < x.size(); j++) {
			cout << "x(" << j << ") = " << x[j] << "  ";
		}
		if (!ok)
			cout << "---> i = " << MultiIndexed::x2i(x);
		cout << endl;

	}

}

bool MultiIndexed::check() const {

	for (unsigned int i = 0; i < size; i++) {
		vector<int> x = i2x(i);
		if (x2i(x) != (int) i)
			return false;

	}

	return true;
}

}
