#include "Coordinate2D.h"
using namespace std;

namespace jlib{

Coordinate2D::~Coordinate2D() {
}


////////////
double Coordinate2D::length(void) const {
	return sqrt(x * x + y * y);
}

////////////
double Coordinate2D::length(const Coordinate2D& r) const {
	return sqrt((x - r.x) * (x - r.x) + (y - r.y) * (y - r.y));
}


}

