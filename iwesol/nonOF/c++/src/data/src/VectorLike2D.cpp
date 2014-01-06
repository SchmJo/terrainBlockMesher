#include <cmath>
#include <iostream>
using namespace std;

#include "VectorLike2D.h"

namespace jlib{

inline void VectorLike2D::rotate(double cosval, double sinval){

	double x = getRefX();
	double y = getRefY();

	getRefX() =  cosval*x + sinval*y;
	getRefY() = -sinval*x + cosval*y;

}


inline void VectorLike2D::translate(double delta_x, double delta_y){

	getRefX() += delta_x;
	getRefY() += delta_y;

}


inline void VectorLike2D::rescale(double factor){

	getRefX() *= factor;
	getRefY() *= factor;

}

}
