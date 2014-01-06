#include "BLIB.h"

#include <ctime>
#include <cstdlib>
#include <iostream>
using namespace std;

namespace jlib{

namespace BLIB {

void reseed(){
	srand ( time(0)/2 );
}


void getPermutations(const SETSET_I & in, SETSET_I & out) {

	// trivial case:
	if (in.empty())
		return;

	// extract the first set:
	SET_I set = in[0];
	SETSET_I smallerin(in);
	smallerin.erase(smallerin.begin());

	// append the values of the first set to the output:
	SETSET_I newout;
	if (out.empty()) {
		for	(unsigned int s = 0; s < set.size(); s++){
			SET_I ns;
			ns.push_back(set[s]);
			newout.push_back(ns);
		}
	} else {
		for	(unsigned int i = 0; i < out.size(); i++){
			for	(unsigned int s = 0; s < set.size(); s++){
				SET_I ns = out[i];
				ns.push_back(set[s]);
				newout.push_back(ns);
			}
		}
	}
	out = newout;

	// call again for smaller problem:
	getPermutations(smallerin,out);

}

void rotateVector(vector<double> & v, const vector<double> & omega, double resolution){

	// prepare:
	int rotSteps          = 1;
	double vnorm          = norm(v);
	double hda            = norm(omega);
	vector<double> homega = omega;
	if(vnorm * hda > resolution){
		homega = vAbyX(homega,hda);
		rotSteps    = hda / (resolution / vnorm);
		//Info << "   Invoking rotSteps = " << rotSteps << endl;
		hda /= rotSteps;
		homega = vAtimesX(homega,hda);
	}

	// rotate:
	for(int rs = 0; rs < rotSteps; rs++){
		v = vAplusB(v,cross(homega,v));
		v = vAtimesX(v,vnorm/norm(v));
	}

}

void getIndexPermutations(const SET_I & in, SETSET_I & out){

	SETSET_I newin;
	for(unsigned int i = 0; i<in.size(); i++){
		SET_I ns;
		for(int j = 0; j<in[i]; j++){
			ns.push_back(j);
		}
		newin.push_back(ns);
	}
	getPermutations(newin, out);

}

COORD_CYL coord_xyz2rpz(const COORD_CART & p){

	// reserve storage:
	COORD_CYL out(p);

	// calc r:
	out.resize(2,0);
	out[0] = eucDistCart(out,COORD_CART(2,0));

	// calc phi:
	if(out[0] == 0)
		out[1] = TWOPI/4.;
	else
		out[1] = acos(p[1]/out[0]);

	// append further coordinates:
	while(p.size() > out.size())
		out.push_back(p[out.size()]);

	return out;
}

COORD_CART coord_rpz2xyz(const COORD_CYL & p){

	// trivial case:
	if(p.empty())
		return COORD_CART();

	// reserve storage:
	COORD_CART out(p);

	// calc x and y:
	out[0] = p[0]*cos(p[1]);
	out[1] = p[0]*sin(p[1]);

	return out;
}

VECC_CYL vecc_xyz2rpz(const VECC_CART & v, const COORD_CYL & p){

	// simple checks:
	if(v.size() < 2 || p.size() < 2)
		return VECC_CYL();

	// prepare:
	VECC_CYL out(v);
	double c = cos(p[1]);
	double s = sin(p[1]);

	// calc:
	out[0] = c * v[0] + s * v[1];
	out[1] = -s * v[0] + c * v[1];

	return out;
}

VECC_CART vecc_rpz2xyz(const VECC_CYL & v, const COORD_CYL & p){

	// simple checks:
	if(v.size() < 2 || p.size() < 2)
		return VECC_CART();

	// prepare:
	VECC_CART out(v);
	double c = cos(p[1]);
	double s = sin(p[1]);

	// calc:
	out[0] = c * v[0] - s * v[1];
	out[1] = s * v[0] + c * v[1];

	return out;
}

int boxPyramid_boxes(const int baseboxes){
	if(baseboxes < 3) return baseboxes;
	return baseboxes + boxPyramid_boxes(baseboxes - 2);
}

int boxPyramid_height(const int baseboxes, const int column){
	if(column < 0 || column > baseboxes) return 0;
	if(column + 1 > baseboxes / 2 + baseboxes % 2)
		return boxPyramid_height(baseboxes, baseboxes - column - 1);
	return column + 1;
}

String d2S(double d, int imax, const std::string & zeroFill){
	String key("");
	double x = chop(d);
	String st(x);
	for(int i = 0; i < imax; i++){
		if( i < (int)st.size()){
			key += jlib::String(st[i]);
		} else {
			key += zeroFill;
		}
	}
	return key;
}

} /* end namespace BLIB */

}
