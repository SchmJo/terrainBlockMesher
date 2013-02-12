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
    along with IWESOL. If not, see <http://www.gnu.org/licenses/>.

Class
    blib::String

Description
    See below

SourceFiles
    String.cpp

References
	[1] J. Schmidt, C. Peralta, B. Stoevesandt, "Automated Generation of
	    Structured Meshes for Wind Energy Applications", Proceedings of the
	    Open Source CFD International Conference, 2012, London, UK

\*---------------------------------------------------------------------------*/

#ifndef STRING_H_
#define STRING_H_

#include <string>
#include <sstream>
#include <iostream>
#include <vector>

namespace blib{

/**
 * @class String
 * @brief A basic string that also knows about standard casting.
 *
 */
class String:
	public std::string {

public:

	/************************************
	 *
	 *     Static member functions:
	 *
	 ************************************/

	/// Returns position of the string s in vs or -1
	static int scontains(const std::vector<std::string> & vs, const std::string & s);


	/************************************
	 *
	 *     Non-static member functions:
	 *
	 ************************************/

	/// Constructor
	String(){}

	/// Constructor
	String(const std::string & s):std::string(s){}

	/// Constructor
	String(const String & s):std::string(s.s()){}

	/// Constructor
	String(const char* s):std::string(s){}

	/// Constructor
	String(int ac,char* av[]);

	/// Constructor
	String(const double & d);

	/// Constructor
	String(const float & f);

	/// Constructor
	String(const int & i);

	/// Constructor
	String(const char & c);

	/// Constructor
	String(const std::vector<std::string> & vs);

	/// Constructor
	String(const std::vector<String> & vS);

	/// Constructor
	String(const std::vector<int> & vi);

	/// Constructor
	String(const std::vector<double> & vd);

	/// Constructor
	String(const std::vector<float> & vf);

	/// Constructor
	template <class T>
	String(const T& t);

	/// Constructor
	template <class T>
	String(const std::vector<T>& vt);

	/// Destructor
	virtual ~String(){}

	/// Generic cast to data T
	template <class T>
	bool read(T& t, /**< Storage of the result */
			std::ios_base& (*f)(std::ios_base&) = std::dec  /**< Either dec or oct or hex */
			) const;

	/// Casts to string
	inline std::string s() const { return *this; }

	/// Casts to double
	inline double d() const;

	/// Casts to float
	inline float f() const;

	/// Casts to int
	inline int i() const;

	/// Casts to char
	inline char c() const;

	/// Casts to std::vector<double>
	std::vector<double> vd(int maxout = 0) const;

	/// Casts to std::vector<int>
	std::vector<int> vi(int maxout = 0) const;

	/// Casts to std::vector<string>
	std::vector<std::string> vs(int maxwords = 0) const;

	/// Casts to std::vector<String>
	std::vector<String> vS(int maxwords = 0) const;

	/// Filters out comment, following the marker
	void filterComments(const std::string & marker = "//");

	/// Erases seperators
	void eraseSeperators(
			const std::string & seperators /*! The seperator, seperated by blanks*/
			);

	/// Erases pre and post blanks
	void erasePrePostBlanks();

	//! Converts string-character to uppercase, is limited to 600 characters
	void upcase();

	/// Counts the words, ie., (multiple) blanks + 1.
	int countWords() const;

};

template <class T>
inline bool String::read(T& t,
		std::ios_base& (*f)(std::ios_base&)
		) const{
	  std::istringstream iss(*this);
	  return !(iss >> f >> t).fail();
}

template <class T>
inline String::String(const T& t){
	std::stringstream ss;
	ss << t;
	*this = std::string(ss.str());
}

template <class T>
inline String::String(const std::vector<T>& vt){
	std::stringstream ss;
	for(unsigned int i = 0; i < vt.size(); i++){
		ss << vt[i];
		if(i < vt.size() - 1) ss << '\n';
	}
	*this = std::string(ss.str());
}

inline int String::scontains(const std::vector<std::string> & vs, const std::string & s) {
	if (vs.empty() || s.empty())
		return -1;
	for (unsigned int i = 0; i < vs.size(); i++) {

		if (vs.at(i).compare(s) == 0)
			return i;
	}
	return -1;
}

inline double String::d() const{
	double d;
	std::ios_base& (*f)(std::ios_base&) = std::dec;
	std::istringstream iss(*this);
	if ((iss >> f >> d).fail())
		return 0;
	return d;
}

inline float String::f() const{
	float ff;
	std::ios_base& (*f)(std::ios_base&) = std::dec;
	std::istringstream iss(*this);
	if ((iss >> f >> ff).fail())
		return 0;
	return ff;
}

inline int String::i() const{
	int i;
	std::ios_base& (*f)(std::ios_base&) = std::dec;
	std::istringstream iss(*this);
	if ((iss >> f >> i).fail())
		return 0;
	return i;
}

inline char String::c() const{
	char c;

	std::ios_base& (*f)(std::ios_base&) = std::dec;
	std::istringstream iss(*this);
	if ((iss >> f >> c).fail())
		return 0;
	return c;
}

inline String::String(const std::vector<std::string> & vs) {

	std::string out = "";

	for (unsigned int i = 0; i < vs.size(); i++) {
		if (i > 0)
			out += " ";
		out += vs[i];
	}

	*this = out;
}

inline String::String(const std::vector<int> & vi) {

	String out = "";

	for (unsigned int i = 0; i < vi.size(); i++) {
		if (i > 0)
			out += " ";
		out += String(vi[i]);
	}

	*this = out;
}

inline String::String(const std::vector<double> & vd) {

	String out = "";

	for (unsigned int i = 0; i < vd.size(); i++) {
		if (i > 0)
			out += " ";
		out += String(vd[i]);
	}

	*this = out;
}

inline String::String(const std::vector<float> & vf) {

	String out = "";

	for (unsigned int i = 0; i < vf.size(); i++) {
		if (i > 0)
			out += " ";
		out += String(vf[i]);
	}

	*this = out;
}



inline String::String(const double & value) {

	std::stringstream ss;
	ss << value;

	*this =  ss.str();
}

inline String::String(const float & value) {

	std::stringstream ss;
	ss << value;

	*this = ss.str();
}

inline String::String(const int & value) {

	std::stringstream ss;
	ss << value;

	*this = ss.str();
}

inline String::String(const char & c) {

	std::stringstream ss;
	ss << c;

	*this = ss.str();
}

inline String::String(int ac, char* av[]) {

	std::string out = "";

	for (int i = 0; i < ac; i++) {

		std::string s(av[i]);
		out += " " + s;

	}

	*this = out;
}

} /* blib */

#endif /* STRING_H_ */
