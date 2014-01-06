#include "Point2D.h"
#include "BLIB.h"
using namespace std;

namespace jlib{

Point2D::Point2D(const vector<double> & xy):
	//Parseable("Point2D"),
	x(0),
	y(0)
	//parseSkipX(0),
	//parseSkipY(0),
	//parseBlockNameX("posx"),
	//parseBlockNameY("posy")
	{

	if(xy.size() > 1){
		x = xy[0];
		y = xy[1];
	}

}

/*
void Point2D::initParseDict(ParseDict & dictionary){

	// start with an empty dictionary:
	newParseDict(dictionary);

	// set up the dictionary entries:
	ParseDictEntry entryX("X",parseBlockNameX,1);
	ParseDictEntry entryY("Y",parseBlockNameY,1);

	// create the parse commands, and register them for killing by destructor:
	ParseComm<double>* parseX = new ParseComm<double>("x",&x,parseSkipX);
	ParseComm<double>* parseY = new ParseComm<double>("y",&y,parseSkipY);
	regForKilling(parseX);
	regForKilling(parseY);

	// Place the commands in the entries:
	entryX.addCommand(parseX);
	entryY.addCommand(parseY);

	// add the entries to the dictionary:
	dictionary.addParseDictEntry(entryX);
	dictionary.addParseDictEntry(entryY);

}
*/

bool Point2D::operator==(const Point2D & rhs) const {
    return BLIB::chop(x - rhs.x) == 0. && BLIB::chop(y - rhs.y) == 0.;
}

bool Point2D::operator!=(const Point2D & rhs) const {
    return !(*this == rhs);
}

}
