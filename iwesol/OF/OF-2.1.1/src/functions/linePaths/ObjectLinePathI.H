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

\*---------------------------------------------------------------------------*/

namespace Foam
{

namespace oldev
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template < class Data, class DataDist >
ObjectLinePath< Data, DataDist >::ObjectLinePath
(
		const std::string & name,
		int interpolOrder
):
		jlib::GenericLinePath< Data, DataDist >(name,interpolOrder){
}

template < class Data, class DataDist >
ObjectLinePath< Data, DataDist >::ObjectLinePath
(
		const Data & p0,
		const Data & p1,
		const std::string & name,
		int interpolOrder
):
		jlib::GenericLinePath< Data, DataDist >(p0,p1,name,interpolOrder){
}

template < class Data, class DataDist >
ObjectLinePath< Data, DataDist >::ObjectLinePath
(
		const Data & p,
		const std::string & name,
		int interpolOrder
):
		jlib::GenericLinePath< Data, DataDist >(p,name,interpolOrder){
}

template < class Data, class DataDist >
ObjectLinePath< Data, DataDist >::ObjectLinePath
(
		const List< Data > & points,
		const std::string & name,
		int interpolOrder
):
		jlib::GenericLinePath< Data, DataDist >(name,interpolOrder){
	forAll(points,pI){
		addPoint(points[pI]);
	}
}

template < class Data, class DataDist >
ObjectLinePath< Data, DataDist >::ObjectLinePath
(
		const jlib::GenericRFunction< Data > & f
):
		jlib::GenericLinePath< Data, DataDist >(f){
}

template < class Data, class DataDist >
ObjectLinePath< Data, DataDist >::ObjectLinePath
(
		const dictionary & dict,
		const word & keyWord
):
		jlib::GenericLinePath< Data, DataDist >
		(
				word(dict.lookup("name")),
				readLabel(dict.lookup("interpolOrder"))
		){

	// read vertices:
	List< Data > l = readList< Data >(dict.lookup(keyWord));

	// case no resolution:
	if(!dict.found("storePoints")){
		forAll(l,I){
			jlib::GenericLinePath< Data, DataDist >::addPoint(l[I]);
		}
	}

	// else fill up:
	else {
		scalar storep  = readScalar(dict.lookup("storePoints"));
		scalar totdist = 0;
		for(label i = 0; i < l.size() - 1; i++){
			totdist += DataDist()(l[i],l[i + 1]);
		}
		for(label i = 0; i < l.size() - 1; i++){
			label inners = storep * ( DataDist()(l[i],l[i + 1]) / totdist ) - 2;
			List< Data > l2 = getLinearList< Data >(
					l[i],
					l[i + 1],
					inners,
					true,
					false
					);
			forAll(l2,lI){
				jlib::GenericLinePath< Data, DataDist >::addPoint(l2[lI]);
			}
		}
		jlib::GenericLinePath< Data, DataDist >::addPoint(l.last());
	}
}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template < class Data, class DataDist >
inline List< Data > ObjectLinePath< Data, DataDist >::getList() const{
	List< Data > out(jlib::GenericLinePath< Data, DataDist >::size());
	forAll(out,pI) out[pI] = jlib::GenericLinePath< Data, DataDist >::getPoint(pI);
	return out;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


// ************************************************************************* //

} // End namespace oldev

} // End namespace Foam
