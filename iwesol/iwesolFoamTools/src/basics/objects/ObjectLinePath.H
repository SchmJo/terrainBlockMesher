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
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright  held by original author
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of IWESOL and it is based on OpenFOAM.

    IWESOL and OpenFOAM are free software: you can redistribute them and/or modify
    them under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    IWESOL and OpenFOAM are distributed in the hope that they will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
    or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Class
    Foam::iwesol::ObjectLinePath

Description
    See below.

SourceFiles

References
	[1] J. Schmidt, C. Peralta, B. Stoevesandt, "Automated Generation of
	    Structured Meshes for Wind Energy Applications", Proceedings of the
	    Open Source CFD International Conference, 2012, London, UK

\*---------------------------------------------------------------------------*/

#ifndef OBJECTLINEPATH_H_
#define OBJECTLINEPATH_H_

#include "GenericLinePath.h"

#include "Globals.H"
#include "dictionary.H"

namespace Foam{
namespace iwesol{

/**
 * @class ObjectLinePath
 * @brief A wrapper class for blib::GenericLinePath.
 *
 */
template <class T>
class ObjectLinePath:
	public blib::GenericLinePath< T >{

public:

	/// Constructor
	ObjectLinePath(const std::string & name = "");

	/// Constructor
	ObjectLinePath(
			const T & p0,
			const T & p1,
			const std::string & name = ""
			);

	/// Constructor
	ObjectLinePath(
			const T & p,
			const std::string & name = ""
			);

	/// Constructor
	ObjectLinePath(
			const List<T> & points,
			const std::string & name = ""
			);

	/// Constructor
	ObjectLinePath(
			const blib::GenericRFunction< T > & f
			);

	/// Constructor
	ObjectLinePath(
			const dictionary & dict,
			const word & keyWord = "points"
			);

	/// Destructor
	virtual ~ObjectLinePath(){}

	/// Returns all points
	List<T> getList() const;

};

template <class T>
ObjectLinePath<T>::ObjectLinePath(
		const std::string & name
		):
		blib::GenericLinePath< T >(name){
}

template <class T>
ObjectLinePath<T>::ObjectLinePath(
		const T & p0,
		const T & p1,
		const std::string & name
		):
		blib::GenericLinePath< T >(p0,p1,name){
}

template <class T>
ObjectLinePath<T>::ObjectLinePath(
		const T & p,
		const std::string & name
		):
		blib::GenericLinePath< T >(p,name){
}

template <class T>
ObjectLinePath<T>::ObjectLinePath(
		const List<T> & points,
		const std::string & name
		):
		blib::GenericLinePath< T >(name){
	forAll(points,pI){
		addPoint(points[pI]);
	}
}

template <class T>
ObjectLinePath<T>::ObjectLinePath(
		const blib::GenericRFunction< T > & f
		):
		blib::GenericLinePath< T >(f){
}

template <class T>
ObjectLinePath<T>::ObjectLinePath(
		const dictionary & dict,
		const word & keyWord
		):
		blib::GenericLinePath< T >(word(dict.lookup("name"))){

	// read vertices:
	List<T> l = readList<T>(dict.lookup(keyWord));

	// case no resolution:
	if(!dict.found("storePoints")){
		forAll(l,I){
			addPoint(l[I]);
		}
	}

	// else fill up:
	else {
		scalar storep  = readScalar(dict.lookup("storePoints"));
		scalar totdist = 0;
		for(label i = 0; i < l.size() - 1; i++){
			totdist += getDist(l[i],l[i + 1]);
		}
		for(label i = 0; i < l.size() - 1; i++){
			label inners = storep * ( getDist(l[i],l[i + 1]) / totdist ) - 2;
			List<T> l2 = getLinearList<T>(
					l[i],
					l[i + 1],
					inners,
					true,
					false
					);
			forAll(l2,lI){
				addPoint(l2[lI]);
			}
		}
		addPoint(l.last());
	}

	if(dict.found("displayPoints")){
		blib::GenericLinePath< T >::displayPoints = readLabel(dict.lookup("displayPoints"));
	}
}

template <class T>
inline List<T> ObjectLinePath<T>::getList() const{
	List<T> out(blib::GenericLinePath< T >::size());
	forAll(out,pI) out[pI] = blib::GenericLinePath< T >::getPoint(pI);
	return out;
}

} /* iwesol */
} /* Foam */


#endif /* OBJECTLINEPATH_H_ */
