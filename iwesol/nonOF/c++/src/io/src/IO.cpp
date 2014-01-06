/**
 * @file IO.cpp
 * @brief Simple Input/Output routines.
 */

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

#include "IO.h"
#include "JString.h"

namespace jlib{

namespace IO {

bool readWord(ifstream* file, string & word){

	// simple checks:
	if(!file || !file->is_open() || !file->good() )
		return false;

	// don't want to start with a blank.
	// prepare:
	char c;
	file->get(c);

	// trivial case: Single letter word
	if(file->eof()){
		word = (c == ' ') ? "" : String(c);
		return !word.empty();
	}

	// shift behind blanks:
	while(file->good() && (c == ' ' || c == '\t' || c == '\n'))
		file->get(c);
	file->seekg((long)file->tellg() - 1);

	// read word:
	getline(*file,word,' ');

	// erase blanks:
	String sw(word);
	sw.erasePrePostBlanks();
	word = sw.s();

	// shift file position to the blank
	// (otherwise no backward reading possible later):
	file->seekg((long)file->tellg() - 1);

	return true;
}

bool readWordB(ifstream* file, string & word){

	// simple checks:
	if(!file || !file->is_open() || !file->good() )
		return false;

	// prepare:
	char c;
	file->get(c);
	long pos     = file->tellg();
	string hword = "";

	// trivial case: Single letter word
	if(file->eof()){
		word = (c == ' ') ? "" : String(c);
		return !word.empty();
	}

	// first get rid of blanks:
	while(pos > 0 && (c == ' ' || c == '\t' || c == '\n')){
		pos--;
		file->seekg(pos);
		file->get(c);
		if(!file->good()) return false;
	}

	// empty case:
	if(pos == 0){
		word = "";
		return false;
	}

	// last letter of the word must have been found:
	hword = String(c);

	// else find previous blank:
	while(pos > 0 && !(c == ' ' || c == '\t' || c == '\n')){
		pos--;
		file->seekg(pos);
		file->get(c);
		if(!file->good()) return false;
		if(!(c == ' ' || c == '\t' || c == '\n'))
			hword = String(c) + hword;
	}

	// correct file position:
	pos = (long)file->tellg() - 1;
	if(pos >= 0) file->seekg(pos);

	// all done:
	word = hword;
	return true;
}

bool readWords(ifstream* file, string & words,
		vector<long> & postWordPos, const int nr){

	// trivial case:
	if(nr == 0)
		return true;

	// prepare:
	string word;

	// read first word:
	if(!readWord(file,word))
		return false;

	// if only empty word found repeat:
	if(word.empty())
		return readWords(file,words,postWordPos,nr);

	// else add it:
	words += (words.empty() ? "" : " ") + word;

	// also add file position:
	long pos = file->tellg();
	postWordPos.push_back(pos);

	// now add the next word:
	return readWords(file,words,postWordPos,nr - 1);

}

bool readWordsB(ifstream* file, string & words,
		vector<long> & preWordPos, const int nr){

	// trivial case:
	if(nr == 0)
		return true;

	// prepare:
	string word;

	// read last word:
	if(!readWordB(file,word))
		return false;

	// if only empty word found repeat:
	if(word.empty())
		return readWordsB(file,words,preWordPos,nr);

	// else add it:
	words = word + (words.empty() ? "" : " ") + words;

	// also add file position:
	long pos = file->tellg();
	//preWordPos.insert(preWordPos.begin(),pos);
	preWordPos.push_back(pos);

	return readWordsB(file,words,preWordPos,nr - 1);

}

long getFileSize(ifstream* file) {

  long begin,end,cur;
  cur = file->tellg();
  file->seekg (0, ios::beg);
  begin = file->tellg();
  file->seekg (0, ios::end);
  end = file->tellg();
  file->seekg(cur);

  return end-begin;
}



bool fileExists(const string & filename) {

	bool flag = false;

	fstream fin;
	fin.open(filename.c_str(), ios::in);

	if (fin.is_open())
		flag = true;
	fin.close();

	return flag;
}

string find(istream& file, const string & term, const string & sep,
		bool restonly, int returnhowmany, bool beginning, int skip,
		bool autoUpcase) {
	String line;

	// deal with multiple terms, seperated by blanks:
	vector<String> t = String(term).vS();
	if (t.empty())
		return "";

	ios_base::seekdir position = (beginning) ? ios::beg : ios::cur;
	long pos = file.tellg();
	ios_base::fmtflags flag = file.flags();
	file.clear();
	file.seekg(skip, position);

	while (file.good()) {

		getline(file, line);
		line.eraseSeperators(sep);

		vector<String> l = line.vS();
		if (l.empty())
			continue;

		// auto upcase:
		vector<String> lhigh(l);
		unsigned int lpos = 0;
		if (autoUpcase)
			for (lpos = 0; lpos < l.size(); lpos++)
				lhigh[lpos].upcase();

		// find all entries of t:
		lpos = 0;
		unsigned int tpos = 0;
		for (tpos = 0; tpos < t.size(); tpos++) {

			// auto upcase:
			if (autoUpcase)
				t[tpos].upcase();

			while (lpos < l.size() && lhigh[lpos].compare(t[tpos]) != 0)
				lpos++;

			// no, no agreement:
			if (lpos == l.size())
				break;

		}
		// everything found, so return:
		if (tpos == t.size()) {
			int imin = 0;
			if (restonly) {
				if (l.size() > 1)
					imin = lpos + 1;
				else
					return "";
			}

			string out = l[imin];

			for (unsigned int i = imin + 1; i < l.size(); i++) {

				if (returnhowmany > 0 && (int)(i - imin) >= returnhowmany)
					break;

				out += " " + l[i];
			}

			return out;
		}

	}

	// set back file position
	file.clear();
	file.seekg(pos);
	file.flags(flag);

	return "";
}

int countChars(std::istream & is,const char & c){

	// check:
	if(!is.good()) return -1;

	// prepare:
	int out = 0;
	int pos = is.tellg() ;

	// loop:
	while(is.good()){
		char hc;
		is.read(&hc,1);
		if( hc == c ) out++;
	}

	// post care:
	is.clear(ios_base::goodbit);
	is.seekg(pos,is.beg);

	return out;
}


vector<string> readAll(const string &filename) {

	vector<string> out;

	string line;

	ifstream file(filename.c_str());

	if (file.is_open()) {

		while (file.good()) {

			getline(file, line);
			out.push_back(line);

		}

		file.close();
	}

	return out;
}

double readReplaceGermanComma(istream & is){

	double out;

	string s;
	is >> s;
	for(unsigned int i = 0; i < s.size(); i++){
		if(s[i] == ',') s[i] = '.';
	}

	stringstream ss;
	ss << s;
	ss >> out;

	return out;
}

bool forward(std::istream & is, const char & c, int howMany){
	char hc     = '\0';
	int counter = 0;
	while(counter < howMany){
		while(hc != c){
			if(!is.good()) return false;
			is.read(&hc,1);
		}
		counter++;
	}
	return true;
}

bool forwardWord(std::istream & is, int howMany){
	int counter    = 0;
	while(counter < howMany){
		std::string hs = "";
		if(!is.good()) return false;
		is >> hs;
		counter++;
	}
	return true;
}

} // end namespace IO


}


