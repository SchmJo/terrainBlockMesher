#include <fstream>
using namespace std;

#include "OManager.h"
#include "JString.h"

namespace jlib{

OManager::OManager(const string & filename_out, const string & fileType) :
	FileLinked("", filename_out) {
	setType(fileType);
	clear();
}

OManager::~OManager() {
}

void OManager::write(){

	cout << "Writing file " << getFileNameOut() << "..." << endl;

	ofstream file;

	file.open (getFileNameOut().data());
	file << content;
	file.close();

	cout << "done." << endl;

}

void OManager::readFunc_ffc(Outputable* obj,
			Outputable::FctPtr_ffc f, float xmin, float xmax, int points){

	// simple checks:
	if(!obj || !f || xmin > xmax || points < 2){
		cerr << "OManager: Error writing function." << endl;
		return;
	}

	// prepare:
	float deltax = (xmax - xmin) / (points - 1);

	// write function:
	for(float x = xmin; x <= xmax; x += deltax){
		if(x > xmin) content += "\n";
		content += String(x) + " " + String((obj->*f)(x));
	}

}

void OManager::collectAll(const string & nonstdcaseheader){

	// simple checks:
	if(oLinks.empty()) return;

	collectData();
	collectHeader();
	collectClosure();
	addHeader(nonstdcaseheader);

}

void OManager::collectData(){

	// simple checks:
	if(oLinks.empty()) return;

	for(unsigned int i = 0;i<oLinks.size();i++){

		vector<IO::OFILE::Data> hod = oLinks[i]->getOData(type);
		if(hod.empty()) return;
		vector<IO::OFILE::Data>::iterator it;

		for(it = hod.begin(); it < hod.end(); it++){

			addToContent(it->pre_data);
			addToContent(it->data);
			addToContent(it->post_data);
		}
	}
}


void OManager::collectHeader(){

	// simple checks:
	if(oLinks.empty()) return;

	for(unsigned int i = 0;i<oLinks.size();i++){

		vector<IO::OFILE::Data> hod = oLinks[i]->getOData(type);
		if(hod.empty()) return;
		vector<IO::OFILE::Data>::iterator it;
		for(it = hod.end() - 1; it >= hod.begin(); it--)
			addContentTo(it->header);

	}
}

void OManager::collectClosure(){

	// simple checks:
	if(oLinks.empty()) return;

	for(unsigned int i = 0;i<oLinks.size();i++){

		vector<IO::OFILE::Data> hod = oLinks[i]->getOData(type);
		if(hod.empty()) return;
		vector<IO::OFILE::Data>::iterator it;
		for(it = hod.begin(); it < hod.end(); it++)
			addToContent(it->closure);

	}

}

void OManager::addHeader(const string & nonstdcaseheader){

	string pre = "";

	if(nonstdcaseheader.compare("")){

		content = nonstdcaseheader + content;

	}else if(type.compare(IO::OFILE::TYPE::EMPTY)){

		if(type.compare(IO::OFILE::TYPE::VTK_ASCII) == 0)
			pre = IO::OFILE::PREAMBLE::VTK_ASCII;

		content = pre + content;
	}

}

}

