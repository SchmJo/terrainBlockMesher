#include "BLIB.h"

namespace blib{

String key(double d, int imax, const std::string & zeroFill){
	String key("");
	double x = chop(d);
	String st(x);
	for(int i = 0; i < imax; i++){
		if( i < (int)st.size()){
			key += String(st[i]);
		} else {
			key += zeroFill;
		}
	}
	return key;
}

} /* blib */
