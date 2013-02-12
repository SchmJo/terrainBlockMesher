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
    This file is part of IWESOL and it is based on OpenFOAM.

    IWESOL and OpenFOAM are free software: you can redistribute them and/or modify
    them under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    IWESOL and OpenFOAM are distributed in the hope that they will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
    or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "Globals.H"

namespace Foam{
namespace iwesol{

bool isInsideBox(
		const point & p,
		const point & p_SWL,
		const CoordinateSystem & cooSys,
		const scalarList & dimensions
		){

	const point c_delta = cooSys.point2coord(p - p_SWL);
	for(label i = 0; i < 3; i++){
		if(c_delta[i] < 0 || c_delta[i] > dimensions[i]) return false;
	}
	return true;
}

word key(scalar height,label imax, scalar resolution, const word & fill){
	blib::String key("");
	blib::String st(height);
	if(height == label(height)){
		key = blib::String(height) + blib::String(".0");
		while(label(key.size()) < imax) key += fill;
		return word(key);
	}
	for(int i = 0; i < imax; i++){
		if(mag(height) > resolution / 100 && i < int(st.size())){
			key += blib::String(st[i]);
		} else {
			key += fill;
		}
	}
	return word(key);
}

labelList unkey_l(const word & k,const std::string & seperator){
	blib::String ks(k);
	ks.eraseSeperators(seperator);
	std::vector<int> kv = ks.vi();
	labelList out(kv.size());
	for(unsigned int i = 0; i < kv.size(); i++){
		out[i] = kv[i];
	}
	return out;
}

List<dictionary> getDictTasks(const dictionary & dict, const word & keyWord){

	// prepare:
	List<word> keywords = dict.toc();
	List<dictionary> out(keywords.size(),dict);
	label counter = 0;

	forAll(keywords,wI){

		// extract keyword before '_':
		blib::String s(keywords[wI]);
		s.eraseSeperators("_");
		s = (s.vs())[0];

		// check:
		if(!keyWord.empty() && keyWord.compare(s) != 0) continue;

		// write to list:
		out[counter].clear();
		out[counter].add(dict.lookupEntry(keywords[wI],false,true));
		out[counter].changeKeyword(keywords[wI],word(s));
		counter++;

	}
	out.resize(counter);

	// check:
	if(counter == 0){
		Info << "\nGlobals: Error: keyword '" << keyWord << "' not found in dictionary." << endl;
		throw;
	}

	return out;
}

} /* iwesol */
} /* Foam */

