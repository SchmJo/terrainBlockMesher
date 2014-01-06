/**
 * @file LinePath2D.cpp
 * @brief Definition/Implementation of class LinePath2D.
 *
 */

#include "LinePath2D.h"
#include "JString.h"
#include "BLIB.h"

using namespace std;

namespace jlib {

using namespace BLIB;

LinePath2D::LinePath2D(const string & name):
		GenericLinePath<Coordinate2D>(name){
}

LinePath2D::LinePath2D(
		const double & x0,
		const double & y0,
		const double & x1,
		const double & y1,
		const string & name
		):
		GenericLinePath<Coordinate2D>(name){
	addPoint(Coordinate2D(x0,y0));
	addPoint(Coordinate2D(x1,y1));
}

LinePath2D::LinePath2D(
		const double & x,
		const double & y,
		const string & name
		):
		GenericLinePath<Coordinate2D>(name){
	addPoint(Coordinate2D(x,y));
}

LinePath2D::LinePath2D(
		const Coordinate2D & p0,
		const Coordinate2D & p1,
		const string & name
		):
		GenericLinePath<Coordinate2D>(p0,p1,name){
}

LinePath2D::LinePath2D(
		const Coordinate2D & p0,
		const string & name
		):
		GenericLinePath<Coordinate2D>(p0,name){
}

LinePath2D::LinePath2D(
		const vector<Coordinate2D> & points,
		const string & name
		):
		GenericLinePath<Coordinate2D>(points,name){
}

LinePath2D::LinePath2D(
		const vector<Point2D> & points,
		const string & name
		):
		GenericLinePath<Coordinate2D>(name){
	for(unsigned int i = 0; i < points.size(); i++){
		addPoint(points[i]);
	}
}

LinePath2D::~LinePath2D() {
}

/*
void LinePath2D::print(
		const double & s0,
		const double & s1
		) const{

	// print first point:
	Coordinate2D p = (*this)[s0];
	cout << s0 << " " << p.x << " " << p.y << endl;

	// print points in between:
	LinePath2D sl = getSubLinePath(s0,s1);
	for(unsigned int i = 0; i < sl.size(); i++){
		if(sl.getPointL(i) > l(s0) && sl.getPointL(i) < l(s1)){
			p = sl.getPoint(i);
			cout << s(sl.getPointL(i)) << " " << p.x << " " << p.y << endl;
		}
	}

	// print last point:
	p = (*this)[s1];
	cout << s1 << " " << p.x << " " << p.y << endl;
}
*/

void LinePath2D::print() const{

	// print points in between:
	const LinePath2D & sl = *this;
	for(unsigned int i = 0; i < sl.size(); i++){
		Coordinate2D p = sl.getPoint(i);
		cout << i << " " << s(sl.getPointL(i)) << " " << p.x << " " << p.y << endl;
	}
}


int LinePath2D::coordSysRegister(CoordinateSystem2D* cs){

	int out = 0;
/*
	for(unsigned int i = 1; i < points(); i ++){
		out += cs->registerObj(&pointList[i]);
	}
*/
	return out;

}

bool LinePath2D::getHitPoint(const Line2D & lineB,Coordinate2D & hitPoint) const{
	for(unsigned int i = 0; i < lines(); i++){
		if(Line2D(getLine(i)).getHitPoint(lineB,hitPoint)) return true;
	}
	return false;
}

double LinePath2D::getNearestS(const Coordinate2D & p) const{
	double dist = -1;
	double lmin = 0.;
	double l    = 0.;
	for(unsigned int i = 0; i < lines(); i++){
		Line2D li = getLine(i);
		double hd = li.dist(p);
		if(hd >= 0){
			if(dist < 0 || hd < dist){
				dist = hd;
				lmin = l + li.dist(0.,li.getNearestS(p));
			}
		}
		l += li.length();
	}
	return lmin / length();
}

double LinePath2D::dist(const Coordinate2D & p) const{
	double out = -1;
	for(unsigned int i = 0; i < lines(); i++){
		double hd = Line2D(getLine(i)).dist(p);
		if(hd >= 0){
			if(out < 0 || hd < out) out = hd;
		}
	}
	return out;
}


Outputable::OData LinePath2D::getOData(const string & type) const{


	vector<IO::OFILE::Data> out;
	IO::OFILE::Data dat;


	////////////////////////////////////////////////////////////
	//
	// 		VTK ascii file output:
	//
	if (type.compare(IO::OFILE::TYPE::VTK_ASCII) == 0) {

		// set pre and post fields:
		dat.name = "Line2D";
		dat.type = type;
		dat.origin = "Line2D";
		dat.pre_data = "DATASET POLYDATA\n\nPOINTS " + String(lines() + 1) + " float\n";

		dat.data = "";
		for(unsigned int i = 0; i < points(); i++){
			dat.data += String(getPoint(i).x) + " " + String(getPoint(i).y) + " 0\n";
		}

		dat.post_data = "\nLINES " + String(3) + " " + String(3 * lines()) + " \n";
		for(unsigned int i = 0; i < lines(); i++){
			dat.post_data += "2 " + String(i) + " " + String(i+1) + "\n";
		}

	}

	out.push_back(dat);

	return out;


}

/*
LinePath2D LinePath2D::getSubLinePath(
		const double & s0,
		const double & s1
		) const{

	// simple check:
	if(s0 >= s1) return LinePath2D();
	if(s0 == 0. && s1 == 1.) return *this;

	return LinePath2D(getSubFunction(l(s0),l(s1)));
}
*/

LinePath2D & LinePath2D::flip(){

	LinePath2D newlp(name());

	newlp.addPoint(getPoint(0));
	for(int i = 0; i < int(size()) - 1; i++){
		newlp.addPoint(getPoint(size() - 1 - i));
	}
	if(periodic()) newlp.setPeriodic();

	*this = newlp;
	return *this;
}

vector<double> LinePath2D::getPlotColumn(int w) const{
	vector<double> out(size());
	for(int i = 0; i < int(size()); i++) {
		double dat = 0;
		const Coordinate2D & coo = getPoint(i);
		switch(w){
		case 0: dat = getPointS(i); break;
		case 1: dat = coo.x; break;
		case 2: dat = coo.y; break;
		cout << "\nLinePath2D: Error: getPlotColumn argument " << w << " exceeds data range." << endl; throw;
		}
		out[i] = dat;
	}
	return out;
}


}
