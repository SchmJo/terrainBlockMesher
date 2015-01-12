/**
 * @file Globals.C
 * @brief This file defines/implements the class Globals.
 *
 */
#include "JString.h"
#include "BLIB.h"

#include "Globals.H"
#include "mathematicalConstants.H"
#include "volPointInterpolation.H"
#include "interpolatePointToCell.H"
#include "searchableSurface.H"

#include <ctime>
#include <cstdlib>
using namespace std;

namespace Foam{

namespace oldev{

bool isInsideBox(
		const point & p,
		const point & p_SWL,
		const CoordinateSystem & cooSys,
		const scalarList & dimensions
		){

	const point c_delta = cooSys.point2coord(p - p_SWL);
	for(label i = 0; i < 3; i++){
		if(c_delta[i] < 0 || c_delta[i] > dimensions[i]) return false;
	}
	return true;
}

void reseed(){
	srand ( time(NULL) );
}

point randomDirection(){
	point out(-500. + rand() % 1001, -500. + rand() % 1001, -500. + rand() % 1001);
	normalize(out);
	return out;
}

labelListList permutations(const labelList & list){

	// prepare for jlib:
	std::vector< std::vector< int> > in(list.size());
	std::vector< int > l(list.size());
	forAll(list,lI){
		l[lI] = list[lI];
	}
	forAll(list,lI){
		in[lI] = l;
	}

	// call jlib:
	std::vector< std::vector< int> > out;
	jlib::BLIB::getPermutations(in,out);

	// translate to OF format:
	labelListList lout(out.size());
	forAll(lout,lI){
		labelList ll(out[lI].size());
		forAll(ll,llI){
			ll[llI] = out[lI][llI];
		}
		lout[lI] = ll;
	}

	// throw away multiple entry solutions:
	labelListList lout2(lout.size());
	label counter = 0;
	forAll(lout,lI){
		if(HashSet< label >(lout[lI]).toc().size() == list.size()){
			lout2[counter] = lout[lI];
			counter++;
		}
	}
	lout2.resize(counter);

	return lout2;
}

labelListList mappings(const labelListList & listlist){

	// prepare for jlib:
	std::vector< std::vector< int> > in(listlist.size());
	forAll(listlist,lI){
		std::vector< int > l(listlist[lI].size());
		forAll(listlist[lI],llI){
			l[llI] = listlist[lI][llI];
		}
		in[lI] = l;
	}

	// call jlib:
	std::vector< std::vector< int> > out;
	jlib::BLIB::getPermutations(in,out);

	// translate to OF format:
	labelListList lout(out.size());
	forAll(lout,lI){
		labelList ll(out[lI].size());
		forAll(ll,llI){
			ll[llI] = out[lI][llI];
		}
		lout[lI] = ll;
	}
	return lout;

	return labelListList();
}

labelListList indexMappings(const labelListList & listlist){
	labelListList in(listlist.size());
	forAll(listlist,llI){
		labelList l(listlist[llI].size());
		forAll(l,lI){
			l[lI] = lI;
		}
		in[llI] = l;
	}
	Info << in << endl;
	return mappings(in);
}

CoordinateSystem  calcOrthonormalSystem(
		const Foam::vector & U,
		const point & origin
		){
	Foam::vector nlong(0,0,0);
	Foam::vector nside(0,0,0);
	Foam::vector nup(0,0,0);
	calcOrthonormalSystem(
			U,
			nlong,
			nside,
			nup
			);
	return CoordinateSystem(origin,nlong,nside,nup);
}

void calcOrthonormalSystem(
		const Foam::vector & U,
		Foam::vector & nlong,
		Foam::vector & nside,
		Foam::vector & nup){

	// check:
	scalar nu = norm(U);
	if(chop(nu) == 0.) return;

	// longitudinal component:
	nlong = U / nu;

    // side component:
	nside = Foam::vector(-nlong[1],nlong[0],0);
	nside = nside / norm(nside);

	// up component:
	nup   = cross(nlong,nside);

	// check:
	if(nup[2] < 0) {
		nup = -nup;
		nside = cross(nup,nlong);
	}

}

void rotateVector(Foam::vector & v, const Foam::vector & omega, scalar resolution){

	// prepare:
	label rotSteps      = 1;
	scalar vnorm        = norm(v);
	scalar hda          = norm(omega);
	Foam::vector homega = omega;
	scalar hresolution  = resolution < 0.01 ? resolution : 0.01;
	if(vnorm * hda > hresolution){
		homega /= hda;
		rotSteps    = hda / (hresolution / vnorm);
		// Info << "   Invoking rotSteps = " << rotSteps << endl;
		hda /= rotSteps;
		homega *= hda;
	}

	// rotate:
	for(int rs = 0; rs < rotSteps; rs++){
		v += cross(homega,v);
		v *= vnorm / norm(v);
	}

}

bool getSTLHit(searchableSurface const * stl,const point & p_start, const point & p_end, point & surfacePoint){

	// hit a line through the surface:
	List< pointIndexHit > hitList;
	pointField start(1,p_start);
	pointField end(1,p_end);
	stl->findLine(start,end,hitList);

	// check hit:
	if(!hitList[0].hit()){
		return false;
	}

	surfacePoint = hitList[0].hitPoint();
	return true;
}

List<dictionary> getDictTasks(const dictionary & dict, const word & keyWord){

	// prepare:
	List<word> keywords = dict.toc();
	List<dictionary> out(keywords.size(),dict);
	label counter = 0;

	forAll(keywords,wI){

		// extract keyword before '_':
		jlib::String s(keywords[wI]);
		s.eraseSeperators("_");
		s = (s.vs())[0];

		// check:
		if(!keyWord.empty() && keyWord.compare(s) != 0) continue;

		// write to list:
		out[counter].clear();
		out[counter].add(dict.lookupEntry(keywords[wI],false,true));
		out[counter].changeKeyword(keywords[wI],word(s));
		counter++;

	}
	out.resize(counter);

	return out;
}

word key(scalar height,label imax, scalar resolution, const word & fill){
	jlib::String key("");
	jlib::String st(height);
	if(height == label(height)){
		key = jlib::String(height) + jlib::String(".0");
		while(label(key.size()) < imax) key += fill;
		return word(key);
	}
	for(int i = 0; i < imax; i++){
		if(mag(height) > resolution / 100 && i < int(st.size())){
			key += jlib::String(st[i]);
		} else {
			key += fill;
		}
	}
	return word(key);
}

labelList unkey_l(const word & k,const std::string & seperator){
	jlib::String ks(k);
	ks.eraseSeperators(seperator);
	std::vector<int> kv = ks.vi();
	labelList out(kv.size());
	for(unsigned int i = 0; i < kv.size(); i++){
		out[i] = kv[i];
	}
	return out;
}

scalarList unkey_s(const word & k,const std::string & seperator){
	jlib::String ks(k);
	ks.eraseSeperators(seperator);
	std::vector<double> kv = ks.vd();
	scalarList out(kv.size());
	for(unsigned int i = 0; i < kv.size(); i++){
		out[i] = kv[i];
	}
	return out;
}

Foam::vector dot(const SquareMatrix<scalar> & M, const Foam::vector & U){

	Foam::vector out;

	for(label row = 0; row < 3; row++){
		out[row] = 0;
		for(label col = 0; col < 3; col++){
			out[row] += M[row][col] * U[col];
		}
	}

	return out;
}

word key(const labelList & labels){
	word out = "";
	forAll(labels,lI){
		if(lI > 0) out += ".";
		out += jlib::String(labels[lI]);
	}
	return out;
}

label patchEdgeToGlobalEdge(const polyPatch & patch, label patchEdgeI){

	// prepare:
	const polyMesh & mesh   = patch.boundaryMesh().mesh();
	const edge & pEdge      = patch.edges()[patchEdgeI];
	label pA                = patch.meshPoints()[pEdge.start()];
	label pB                = patch.meshPoints()[pEdge.end()];
	const labelList & elist = mesh.pointEdges()[pA];

	// loop over global edges:
	forAll(elist,leI){
		label eI       = elist[leI];
		const edge & e = mesh.edges()[eI];
		if( (e.start() == pA && e.end() == pB) || (e.start() == pB && e.end() == pA)){
			return eI;
		}
	}

	return -1;
}

volScalarField interpolatePointToVolScalar(
		const fvMesh & fvMesh_,
		const pointScalarField & p,
		const IOobject & io
		){

	// prepare:
	const polyBoundaryMesh& bMesh = fvMesh_.boundaryMesh();

	// create a zero value field:
	volScalarField out(
				io,
				fvMesh_,
				dimensionedScalar
				(
					io.name(),
					p.dimensions(),
					0
				),
				p.boundaryField().types()
			);

	// fill values for internal cells:
	forAll(out,cI){
		out[cI] = interpolatePointToCell< Foam::scalar >(p,cI);
	}

	// fill boundary values:
	forAll(bMesh,patchI){

		// grab patch:
		const polyPatch & patch = bMesh[patchI];

		// loop over faces:
		forAll(patch,faceI){

			// grab face:
			const face & f = patch[faceI];

			// grab boundary field value:
			Foam::scalar & outValue = out.boundaryField()[patchI][faceI];

			// set to mean of point values:
			outValue = 0;
			forAll(f,pointI){
				outValue += p[f[pointI]];
			}
			outValue /= f.size();
		}
	}

	return out;
}

volVectorField interpolatePointToVolVector(
		const fvMesh & fvMesh_,
		const pointVectorField & p,
		const IOobject & io
		){

	// prepare:
	const polyBoundaryMesh& bMesh = fvMesh_.boundaryMesh();

	// create a zero value field:
	volVectorField out(
				io,
				fvMesh_,
				dimensionedVector
				(
					io.name(),
					p.dimensions(),
					Foam::vector::zero
				),
				p.boundaryField().types()
			);

	// fill values for internal cells:
	forAll(out,cI){
		out[cI] = interpolatePointToCell< Foam::vector >(p,cI);
	}

	// fill boundary values:
	forAll(bMesh,patchI){

		// grab patch:
		const polyPatch & patch = bMesh[patchI];

		// loop over faces:
		forAll(patch,faceI){

			// grab face:
			const face & f = patch[faceI];

			// grab boundary field value:
			Foam::vector & outValue = out.boundaryField()[patchI][faceI];

			// set to mean of point values:
			outValue = Foam::vector::zero;
			forAll(f,pointI){
				outValue += p[f[pointI]];
			}
			outValue /= f.size();
		}
	}

	return out;
}

bool setClosestTime(Time & runTime, const scalar & goalTime){

	const instant & inst      = runTime.findClosestTime(goalTime);
	const instantList & times = runTime.times();
	forAll(times,tI){
		if(times[tI].name().compare(inst.name()) == 0){
			runTime.setTime(inst,tI);
			return true;
		}
	}
	return false;
}

}
}

