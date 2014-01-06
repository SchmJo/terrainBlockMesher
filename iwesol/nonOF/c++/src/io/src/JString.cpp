/**
 * @file String.cpp
 * @brief This file defines/implements the class String.
 *
 */

#include <fstream>
using namespace std;

#include "JString.h"

namespace jlib {

String::String(const vector<String> & vS){

	vector<string> vs(vS.size());
	for(unsigned int i = 0; i < vS.size();i++){
		vs[i] = vS[i].s();
	}
	*this = String(vs);

}

String & String::filterComments(const string & marker) {

	if (!marker.empty()) {

		if (this->substr(0, marker.length()).compare(marker) == 0) {
			*this = "";
		} else {

			size_t pos = this->find(marker);

			if (pos != string::npos) {

				*this = substr(0, (int) pos);

			}

		}
	}
	return *this;
}

String & String::eraseSeperators(const string & seperators) {

	string out = s();
	size_t pos;

	if (empty() || seperators.empty())
		return *this;

	vector<string> sep = String(seperators).vs();

	for (unsigned int i = 0; i < sep.size(); i++) {

		pos = 0;
		while (true) {
			pos = out.find(sep[i]);
			if (pos == string::npos)
				break;
			out = out.substr(0, pos) + " " + out.substr(pos + 1);
		}

	}

	*this = out;
	return *this;
}

String & String::strip(
			char sep,
			int howMany,
			bool fromLeft
			){

	string out = s();
	size_t pos;

	if (empty())return *this;

	// from left:
	if(fromLeft){

		int counter = 0;
		pos         = 0;
		for(pos = 0; pos < out.size(); pos++){
			if(out[pos] == sep) counter++;
			if(counter == howMany) break;
		}
		if(counter == howMany){
			*this = out.substr(pos + 1);
		}
	}

	// from right:
	else {

		int counter = 0;
		int pos     = 0;
		for(pos = out.size() - 1; pos >= 0; pos--){
			if(out[pos] == sep) counter++;
			if(counter == howMany) break;
		}
		if(counter == howMany){
			*this = out.substr(0,pos);
		}
	}
	return *this;
}

String & String::erasePrePostBlanks(){

	string line = s();

	// trivial case:
	if(line.empty()) return *this;

	// preceding blanks:
	while(!line.empty() && (
			line.at(0) == ' ' ||
			line.at(0) == '\t' ||
			line.at(0) == '\n')
			)
		line = line.substr(1);

	// subsequent blanks"
	while(!line.empty() && (
			line.at(line.length() -1) == ' ' ||
			line.at(line.length() -1) == '\t' ||
			line.at(line.length() -1) == '\n'
			))
		line = line.substr(0,line.length() -1);

	*this = line;
	return *this;
}

String & String::upcase() {
	String & rep = *this;
	const char * down = rep.c_str();
	int length = rep.length();
	char up[600] = "";
	for (int i = 0; i < length; i++) {
		if (islower(down[i]))
			up[i] = toupper(down[i]);
		else
			up[i] = down[i];
	};
	rep.assign(up);
	return *this;
}

int String::countWords() const{

	// prepare:
	String t = *this;

	// clear pre and post blanks:
	t.erasePrePostBlanks();

	// simple checks:
	if(t.empty()) return 0;

	// find next seperator:
	string::size_type pos   = t.find(' ');
	string::size_type pos_t = t.find('\t');
	string::size_type pos_n = t.find('\n');
	if(pos_t < pos) pos = pos_t;
	if(pos_n < pos) pos = pos_n;

	// no seperator found:
	if(pos == string::npos) return 1;

	// check the rest, return:
	return 1 + String(t.erase(0,pos + 1)).countWords();

}

vector<string> String::vs(int maxwords) const{
	string sentence = s();
	vector<string> words;

	if (!sentence.empty()) {

		string line(sentence);
		string::size_type pos;
		string::size_type pos_t;

		if (line.at(line.size() - 1) != ' ')
			line += " ";

		while (!line.empty() && line.at(0) == ' ') {
			line = line.substr(1);
		}
		if (line.empty())
			return words;

		while (true) {
			pos = line.find(' ');
			pos_t = line.find('\t');
			if (pos_t < pos)
				pos = pos_t;
			if (pos != string::npos) {
				string ns = line.substr(0, pos);
				if (ns.length() > 0)
					words.push_back(ns);
				if (maxwords > 0 && (int) words.size() == maxwords)
					break;
				line.erase(0, pos + 1);//notice that this will modify your starting string
			} else {
				//words.push_back(sentence);
				break;
			}
		}
	}

	return words;
}

vector<double> String::vd(int maxout) const{

	vector<string> words = vs();

	vector<double> out;

	for (unsigned int i = 0; i < words.size(); i++) {

		double x;

		ios_base& (*f)(ios_base&) = dec;
		istringstream iss(words[i]);
		if ((iss >> f >> x).fail())
			continue;

		out.push_back(x);
		if (maxout > 0 && (int) out.size() == maxout)
			break;

	}

	return out;

}

vector<char> String::vc(int maxout) const{

	vector<char> out(size());

	for (unsigned int i = 0; i < size(); i++) {
		out[i] = (*this)[i];
	}

	return out;

}

vector<int> String::vi(int maxout) const{

	vector<string> words = vs();

	vector<int> out;

	for (unsigned int i = 0; i < words.size(); i++) {

		int x;

		ios_base& (*f)(ios_base&) = dec;
		istringstream iss(words[i]);
		if ((iss >> f >> x).fail())
			continue;

		out.push_back(x);
		if (maxout > 0 && (int) out.size() == maxout)
			break;

	}

	return out;

}

vector<String> String::vS(int maxwords) const{
	vector<string> vs(maxwords);
	vector<String> vS(vs.size());
	for(unsigned int i = 0;i<vs.size();i++)
		vS[i] = String(vs[i]);
	return vS;
}

}
