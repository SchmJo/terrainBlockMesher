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

#include "SplineBlock.H"

namespace Foam{
namespace iwesol{

SplineBlock::SplineBlock() {
}

SplineBlock::SplineBlock(
		pointField* globalPoints,
		const labelList & verticesI,
		const labelList & cells,
		HashTable<Spline>* globalSplines,
		const std::string & gradingCommand,
		const scalarList & gradingFactors
		):
		BasicBlock(
				globalPoints,
				verticesI,
				cells,
				gradingCommand,
				gradingFactors
				),
		splineIndexList(24),
		globalSplines(globalSplines){
}

SplineBlock::SplineBlock(
		pointField* globalPoints,
		const label & p_SWL,const label & p_SWH,
		const label & p_NWL,const label & p_NWH,
		const label & p_SEL,const label & p_SEH,
		const label & p_NEL,const label & p_NEH,
		const label cells_x,
		const label cells_y,
		const label cells_z,
		HashTable<Spline>* globalSplines,
		const std::string & gradingCommand,
		const scalarList & gradingFactors
		):
		BasicBlock(
				globalPoints,
				p_SWL, p_SWH,
				p_NWL, p_NWH,
				p_SEL, p_SEH,
				p_NEL, p_NEH,
				cells_x,
				cells_y,
				cells_z,
				gradingCommand,
				gradingFactors
				),
		splineIndexList(24),
		globalSplines(globalSplines){
}

SplineBlock::~SplineBlock() {
}

bool SplineBlock::hasSpline(label i) const{

	// find vertex labels:
	labelList hv = getSplineVerticesI(i);
	label pvA    = getVertexI(hv[0]);
	label pvB    = getVertexI(hv[1]);

	return globalSplines->found(key(pvA,pvB));

}

void SplineBlock::eraseSpline(label iSpline){

	if(hasSpline(iSpline)){

		labelList hv = getSplineVerticesI(iSpline);
		label pvA    = getVertexI(hv[0]);
		label pvB    = getVertexI(hv[1]);
		globalSplines->erase(key(pvA,pvB));

	} else {
		label jSpline = switchedOrientationLabel(iSpline);
		if(hasSpline(jSpline)) eraseSpline(jSpline);
	}

}

const Spline & SplineBlock::getSpline(label i) const{

	// find vertex labels:
	labelList hv = getSplineVerticesI(i);
	label pvA    = getVertexI(hv[0]);
	label pvB    = getVertexI(hv[1]);

	// error if spline not found:
	if(!globalSplines->found(key(pvA,pvB))){
		Info << "\nSplineBlock: Error: Spline '" << key(pvA,pvB) << "' undefined." << endl;
		throw;
	}

	return (*globalSplines)[key(pvA,pvB)];
}

Spline & SplineBlock::getSpline(label i){

	// find vertex labels:
	labelList hv = getSplineVerticesI(i);
	label pvA    = getVertexI(hv[0]);
	label pvB    = getVertexI(hv[1]);

	// error if spline not found:
	if(!globalSplines->found(key(pvA,pvB))){
		Info << "\nSplineBlock: Error: Spline '" << key(pvA,pvB) << "' undefined." << endl;
		throw;
	}

	return (*globalSplines)[key(pvA,pvB)];

}

Spline SplineBlock::getSplineCopy(label i) const{

	// spline exists:
	if(hasSpline(i)) return getSpline(i);

	// find vertex labels:
	labelList hv = getSplineVerticesI(i);
	label pvA    = getVertexI(hv[0]);
	label pvB    = getVertexI(hv[1]);

	// search other direction:
	if(globalSplines->found(key(pvB,pvA))){
		const Spline & temp = (*globalSplines)[key(pvB,pvA)];
		pointField sPoints(temp.size() - 2);
		forAll(sPoints,pI){
			sPoints[pI] = temp.getPoint(temp.size() - 2 -pI);
		}
		return Spline(globalPoints,pvA,pvB,sPoints);
	}

	return Spline(globalPoints,getVertexI(hv[0]),getVertexI(hv[1]));
}

bool SplineBlock::orderSpline(label splineLabel, const Foam::vector & direction){

	bool out = false;
	if(hasSpline(splineLabel)){
		Spline s = getSpline(splineLabel);
		out      = s.order(direction);
		if(out){
			label newsl = switchedOrientationLabel(splineLabel);
			setSpline(newsl,s);
		}
	}
	return out;

}

std::string SplineBlock::dictEntry_splines() const{

	// simple check:
	if(!ok()) return word("");

	std::string out("");

	// loop over splines:
	for(label i = 0; i < 24; i++){

		Spline spline = getSpline(i);
		if(spline.empty()) continue;
		out += spline.dictEntry() + "\n";
	}

	return out;
}

bool SplineBlock::check(scalar tolerance) const{

	// check if there are doubly defined splines:
	for(label s = 0; s < 24 ; s++){

		labelList vl = getSplineVerticesI(s);
		label sA     = getSplineLabel(vl[0],vl[1]);
		label sB     = getSplineLabel(vl[1],vl[0]);
		if(hasSpline(sA) && hasSpline(sB)){

			const Spline & splA = getSpline(sA);
			const Spline & splB = getSpline(sB);

			Info << "\nSplineBlock: Error: Double spline definition between vertices " << vl[0] << " and " << vl[1] << "." << endl;
			Info << "\n     spline " << sA << ": " <<splA.getPoints() << endl;
			Info << "\n     spline " << sB << ": " <<splB.getPoints() << endl;
			return false;
		}

	}


	// check if spline vertices match block vertices:
	for(label s = 0; s < 24 ; s++){

		if(!hasSpline(s)) continue;

		labelList vl = getSplineVerticesI(s);
		for(label i = 0; i < 2; i++){

			const Spline & spl = getSpline(s);

			const point & p = (i == 0) ?
					spl[0]:
					spl[spl.size() - 1];

			scalar d = mag(getVertex(vl[i]) - p);

			if(d > tolerance){
				Info << "\nSplineBlock: Error in spline " << s << ": Vertex " << vl[i]
				     << " " << getVertex(vl[i]) << " does not match boundary spline point " << p << endl;
				Info << "\n     spline: " << spl.getPoints() << endl;
				Info << "     dist = " << d << ", tolerance = " << tolerance << endl;
				return false;
			}
		}
	}

	return true;
}

void SplineBlock::setDummySplines(){
	for(label s = 0; s < 24 ; s++){
		labelList vl = getSplineVerticesI(s);
		label sA     = getSplineLabel(vl[0],vl[1]);
		label sB     = getSplineLabel(vl[1],vl[0]);
		if(!hasSpline(sA) && !hasSpline(sB)){
			pointField sp(1);
			sp[0] = 0.5 * (getVertex(vl[0]) + getVertex(vl[1]));
			setSpline(s,sp);
		}
	}
}

void SplineBlock::setSpline(label i, const pointField & splinePoints){

	// find vertex labels:
	labelList hv = getSplineVerticesI(i);
	label pvA    = getVertexI(hv[0]);
	label pvB    = getVertexI(hv[1]);

	// delete other direction:
	if(globalSplines->found(key(pvB,pvA))){
		globalSplines->erase(key(pvB,pvA));
	}

	// create new spline:
	Spline s(globalPoints,pvA,pvB,splinePoints);

	// storage:
	globalSplines->set(key(pvA,pvB),s);
}


void SplineBlock::setSpline(label i, const Spline & s){

	// find vertex labels:
	labelList hv = getSplineVerticesI(i);
	label pvA    = getVertexI(hv[0]);
	label pvB    = getVertexI(hv[1]);

	// delete other direction:
	if(globalSplines->found(key(pvB,pvA))){
		globalSplines->erase(key(pvB,pvA));
	}

	// storage:
	globalSplines->set(key(pvA,pvB),s);

}

void SplineBlock::setSpline(label i, label splinePoints){
	labelList vl = getSplineVerticesI(i);
	pointField spoints(getLinearList<point>(getVertex(vl[0]),getVertex(vl[1]),splinePoints,false,false));
	setSpline(i,spoints);
}

void SplineBlock::setSpline(label splineI, const Foam::vector & delta0, label splinePointNr){

	// prepare:
	labelList vl         = getSplineVerticesI(splineI);
	label vA             = vl[0];
	label vB             = vl[1];
	const point & p0     = getVertex(vA);
	const point & p_goal = getVertex(vB);

	// calc spline points:
	pointField spoints = getSplinePoints(
			p0,
			p_goal,
			delta0,
			splinePointNr
			);

	// add to spline list:
	setSpline(splineI,spoints);

}


void SplineBlock::setSpline(
		label splineI,
		const Foam::vector & deltaA,
		const Foam::vector & deltaB,
		label splinePointNr
		){

	// prepare:
	labelList vl         = getSplineVerticesI(splineI);
	label vA             = vl[0];
	label vB             = vl[1];
	const point & p0     = getVertex(vA);
	const point & p_goal = getVertex(vB);

	// calc spline points:
	pointField spoints = getSplinePoints(
			p0,
			p_goal,
			deltaA,
			deltaB,
			splinePointNr
			);

	// setSpline:
	setSpline(splineI,spoints);

}

pointField SplineBlock::getSplinePoints(
			const point & p0,
			const point & p_goal,
			const Foam::vector & delta0,
			label splinePointNr
			){

	// set up line path:
	PointLinePath path;
	path.addPoint(p0);
	path.addPoint(p0 + delta0);
	path.addPoint(p_goal);

	// fill point field:
	pointField out(splinePointNr);
	forAll(out,pI){
		scalar s = (pI + 1.) / (out.size() + 1.);
		out[pI]  = path[s];
	}
	return out;
}

pointField SplineBlock::getSplinePoints(
			const point & p0,
			const point & p_goal,
			const Foam::vector & deltaA,
			const Foam::vector & deltaB,
			label splinePointNr
			){

	// set up line path:
	PointLinePath path;
	path.addPoint(p0);
	path.addPoint(p0 + deltaA);
	path.addPoint(p_goal + deltaB);
	path.addPoint(p_goal);

	// fill point field:
	pointField out(splinePointNr);
	forAll(out,pI){
		scalar s = (pI + 1.) / (out.size() + 1.);
		out[pI]  = path[s];
	}

	return out;
}

label SplineBlock::getSplineBefore(
			label iSpline,
			Spline & splineBefore
			){

	// prepare:
	point origin(0,0,0);
	List<Foam::vector> nList(3,Foam::vector(0,0,0));
	label faceFrom  = SplineBlock::splineStartFace(iSpline);

	// case no neighbor:
	if(!hasNeighbor(faceFrom)) return 0;

	// grab neighbor:
	SplineBlock * nblock = dynamic_cast<SplineBlock *>(getNeighbor(faceFrom));

	// trivial spline:
	if(!nblock->hasSpline(iSpline)){
		labelList hv = SplineBlock::getSplineVerticesI(iSpline);
		splineBefore = Spline(globalPoints,nblock->getVertexI(hv[0]),nblock->getVertexI(hv[1]));
		return 1;
	}

	// set spline:
	splineBefore = nblock->getSpline(iSpline);

	return 2;
}

Spline & SplineBlock::flipSpline(label iSpline){

	Spline & spline = getSpline(iSpline);

	pointField spoints(spline.size() - 2);
	forAll(spoints,pI){
		spoints[pI] = spline.getPoint(spline.size() - 1 - (pI + 1));
	}

	label jSpline = switchedOrientationLabel(iSpline);
	setSpline(jSpline,spoints);

	return getSpline(jSpline);
}

Foam::vector SplineBlock::getSplineTangent(label iSpline, bool atStartPoint, bool ignoreEmptyNeighborSplines) const{

	// prepare:
	Foam::vector out(0,0,0);
	label face                = atStartPoint ? splineStartFace(iSpline) : splineEndFace(iSpline);
	const Spline * splineHere = 0;
	const Spline * splineNeig = 0;
	Spline tempHere;
	Spline tempNeig;

	// check spline:
	if(hasSpline(iSpline)){
		splineHere = &getSpline(iSpline);
	} else {
		tempHere   = getSplineCopy(iSpline);
		splineHere = &tempHere;
	}

	// case no neighbor:
	if(!hasNeighbor(face)){
		out = atStartPoint ?
			(splineHere->getPoint(1) - splineHere->getPoint(0)) :
			(splineHere->getLastPoint() - splineHere->getPoint(splineHere->size() - 2));
		normalize(out);
		return out;
	}

	// grab neighbor:
	SplineBlock * nblock = dynamic_cast<SplineBlock *>(getNeighbor(face));

	// check for neighbor spline:
	if(nblock->hasSpline(iSpline)){
		splineNeig = &(nblock->getSpline(iSpline));
	} else {
		if(!ignoreEmptyNeighborSplines){
			tempNeig   = nblock->getSplineCopy(iSpline);
			splineNeig = &tempNeig;
		} else {
			out = atStartPoint ?
				(splineHere->getPoint(1) - splineHere->getPoint(0)) :
				(splineHere->getLastPoint() - splineHere->getPoint(splineHere->size() - 2));
			normalize(out);
			return out;
		}
	}

	// set up line path:
	blib::GenericLinePath<point> path;
	if(atStartPoint) {
		path.addPoint(splineNeig->getPoint(splineNeig->size() - 2));
		path.addPoint(splineHere->getPoint(0));
		path.addPoint(splineHere->getPoint(1));
	} else {
		path.addPoint(splineHere->getPoint(splineHere->size() - 2));
		path.addPoint(splineNeig->getPoint(0));
		path.addPoint(splineNeig->getPoint(1));
	}

	// return tangent:
	scalar s = path.getPointS(1);
	return path.getTangent(s,2);
}

label SplineBlock::getSplineAfter(
			label iSpline,
			Spline & splineAfter
			){

	// prepare:
	point origin(0,0,0);
	List<Foam::vector> nList(3,Foam::vector(0,0,0));
	label faceTo        = SplineBlock::splineEndFace(iSpline);

	// case no neighbor:
	if(!hasNeighbor(faceTo)) return 0;

	// grab neighbor:
	SplineBlock * nblock = dynamic_cast<SplineBlock *>(getNeighbor(faceTo));

	// trivial spline:
	if(!nblock->hasSpline(iSpline)){
		labelList hv = SplineBlock::getSplineVerticesI(iSpline);
		splineAfter = Spline(globalPoints,nblock->getVertexI(hv[0]),nblock->getVertexI(hv[1]));
		return 1;
	}

	// set spline:
	splineAfter = nblock->getSpline(iSpline);

	return 2;
}

CoordinateSystem SplineBlock::getLocalSplineCoordinateSystem(
		label iSpline,
		scalar s
		){

	// prepare:
	point origin(0,0,0);
	List<Foam::vector> nList(3,Foam::vector(0,0,0));

	// set up splines:
	Spline splineBefore;
	Spline splineAfter;
	Spline splineHere = getSplineCopy(iSpline);
	label yesBefore   = getSplineBefore(iSpline,splineBefore);
	label yesAfter    = getSplineAfter(iSpline,splineBefore);

	// set up line path:
	blib::GenericLinePath<point> path;
	if(yesBefore) path.addPoint(splineBefore.getPoint(splineBefore.size() - 2));
	for(label i = 0; i < label(splineHere.size()); i++){
		path.addPoint(splineHere.getPoint(i));
	}
	if(yesAfter) path.addPoint(splineAfter.getPoint(1));
	double s0 = yesBefore ? path.getPointS(1) : 0;
	//double s1 = yesAfter  ? path.getPointS(path.size() - 2) : 1;

	// get coordinate system at first original spline point (s = 0):
	point origin_0(splineHere.getFirstPoint());
	List<Foam::vector> nList_0(3,Foam::vector(0,0,0));
	nList_0 = path.getTangent(s0,2);

	return CoordinateSystem(origin,nList);
}

Spline & SplineBlock::ensureSpline(label iSpline){

	// check:
	if(hasSpline(iSpline)) return getSpline(iSpline);

	// prepare:
	labelList vl = getSplineVerticesI(iSpline);
	label dir    = getDirectionEdge(iSpline);
	label vA     = vl[0];
	label vB     = vl[1];

	// check other direction:
	label jSpline = switchedOrientationLabel(iSpline);
	if(hasSpline(jSpline)) return flipSpline(jSpline);

	// create linear spline:
	pointField spoints(
				getLinearList<point>(
					getVertex(vA),
					getVertex(vB),
					getCells(dir) - 1,
					false,
					false
				)
			);

	// set spline:
	setSpline(iSpline,spoints);
	return getSpline(iSpline);
}

} /* iwesol */
} /* Foam */
