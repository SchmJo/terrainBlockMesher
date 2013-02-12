/**
 * @file BlockManager.C
 * @brief This file defines/implements the class BlockManager.
 *
 */

#include "BlockManager.H"

namespace Foam{
namespace iwesol{

scalar BlockManager::getLinearGradingFactor(label i, label gradingBlocks, scalar grading){
	if(i < 0 || i >= gradingBlocks) return 0;
	return ( (gradingBlocks - 1 - i) * grading + i ) / ( gradingBlocks - 1 );
}

scalar BlockManager::getLinearGradingSum(label gradingBlocks, scalar grading){
	scalar tot = 0;
	for(label j = 0; j < gradingBlocks; j++){
		tot += getLinearGradingFactor(j,gradingBlocks,grading);
	}
	return tot;
}

scalar BlockManager::getLinearGradingFraction(label i, label gradingBlocks, scalar grading){
	return getLinearGradingFactor(i,gradingBlocks,grading) / getLinearGradingSum(gradingBlocks,grading);
}

scalar BlockManager::getGradingFraction(
		label i,
		label blocks_total,
		label blocks_grading,
		scalar gradingFactor,
		bool gradingAtHighI
		){
	if(i < 0 || i >= blocks_total) return 0;
	label nogradingblocks = blocks_total- blocks_grading;
	scalar g              = getLinearGradingFactor(0,blocks_grading,gradingFactor);
	scalar gsum           = getLinearGradingSum(blocks_grading,gradingFactor)
							+ g * nogradingblocks;
	label j               = gradingAtHighI ? i - nogradingblocks : blocks_grading - 1 - i;
	if(j >= 0) g = getLinearGradingFactor(j,blocks_grading,gradingFactor);
	return g / gsum;
}

BlockManager::BlockManager(const dictionary & dict,
		CoordinateSystem * cooSys) :
	HasCoordinateSystem(cooSys),
	resolution(0.001),
	splinePointNrs(3),
	gradingCommand("simpleGrading"),
	gradingFactors(3,1.),
	flag_topologyCalculated(false){

	init(dict);

}

BlockManager::BlockManager(
		CoordinateSystem * cooSys,
		scalar resolution,
		const labelList & blockNrs,
		const labelList & cellNrs
		) :
	HasCoordinateSystem(cooSys),
	resolution(resolution),
	blockNrs(blockNrs),
	cellNrs(cellNrs),
	splinePointNrs(3),
	flag_topologyCalculated(false){

	forAll(cellNrs,cI){
		splinePointNrs[cI] = cellNrs[cI] + 1;
	}

}

BlockManager::~BlockManager() {
}

void BlockManager::init(const dictionary & dict){

	// read dictionary:
	if(dict.found("blocks")){
		blockNrs = labelList(dict.lookup("blocks"));
	}
	if(dict.found("cells")){
	 cellNrs  = labelList(dict.lookup("cells"));
	}

	if(dict.found("resolution")){
		resolution   = readScalar(dict.lookup("resolution"));
	}

	forAll(cellNrs,cI){
		splinePointNrs[cI] = cellNrs[cI] - 1;
	}
	if(dict.found("splinePointNrs")){
		splinePointNrs = labelList(dict.lookup("splinePointNrs"));
	}

	if(dict.found("grading")){
		gradingCommand = word(dict.lookup("grading"));
		gradingFactors = scalarList(dict.lookup("gradingFactors"));
		Info << "   Grading command '" << getGradingCommand() << "'" << endl;
	}

}

std::string BlockManager::getGradingCommand() const {

	std::string out = gradingCommand + " (";
		for(label i = 0; i < 3; ++i){
			out += word(blib::String(gradingFactors[i]));
			if(i < 2) out += " ";
		}
	out += ")";
	return out;
}

blib::Outputable::OData BlockManager::getOData(const std::string & type) const {

	Outputable::OData out(1);
	blib::IO::OFILE::Data outdat;

	// set basic info:
	outdat.name = "BlockDictWriter";
	outdat.type = type;
	outdat.origin = "BlockDictWriter";


	// open foam output:
	if (type.compare(blib::IO::OFILE::TYPE::OPEN_FOAM) == 0) {

		// set header:
		outdat.header = blib::IO::OFILE::PREAMBLE::OPEN_FOAM_HALF1 + "blockMeshDict"
				+ blib::IO::OFILE::PREAMBLE::OPEN_FOAM_HALF2;

		// set pre- and post-data:
		outdat.pre_data = "";
		outdat.post_data = "";

		// write vertices:
		outdat.data = "vertices\n(\n\n";
		forAll(points, pI) {
			point p = points[pI];
			chop(p);
			outdat.data += "    (" + blib::String(p.x()) + " " + blib::String(p.y())
					+ " " + blib::String(p.z()) + ") // " + blib::String(pI) + "\n";
		}
		outdat.data += "\n);\n";

		//write blocks:
		outdat.data += "\nblocks\n(\n\n    ";
		for(label tbI = 0; tbI < size(); tbI++) {
			outdat.data += getBasicBlock(tbI).dictEntry_hex() + "\n    ";
		}
		outdat.data += "\n);\n";

		// write edges:
		outdat.data += "\nedges\n(\n\n";
		for(HashTable<Spline>::const_iterator cit = splines.cbegin();
				cit != splines.cend(); cit++){
			outdat.data += cit().dictEntry() + "\n";
		}
		outdat.data += "\n);\n";

		// write boundary:
		outdat.data += "\nboundary\n(\n";
		forAll(patches, pI){
			outdat.data += patches[pI].dictEntry() + "\n";
		}

		outdat.data += "\n);";

		outdat.data += "\n\n";
		outdat.data += "mergePatchPairs\n(\n);";
		outdat.data += "\n";


	}

	out[0] = outdat;
	return out;

}

void BlockManager::findAllNeighbors(){
	if(flag_topologyCalculated) return;
	for(label i = 0; i < size(); ++i){
		for(label j = i + 1; j < size(); j++){
			BasicBlock & bA = getBasicBlock(i);
			BasicBlock & bB = getBasicBlock(j);
			bA.checkSetNeighbor(bB);
		}
	}
}

void BlockManager::findAllNeighbors(label i){
	if(flag_topologyCalculated) return;
	BasicBlock & bA = getBasicBlock(i);
	for(label j = 0; j < size(); j++){
		if(j != i){
			BasicBlock & bB = getBasicBlock(j);
			bA.checkSetNeighbor(bB);
		}
	}
}

void BlockManager::setInterBlockSpline(
			const pointField & splinep,
			label iStartBlock,
			label faceToNextBlock,
			label splineI,
			label nBlockMax
		){

	// get block line:
	List<SplineBlock*> blockLine = getBasicBlock(iStartBlock).getConnectedBlockLine<SplineBlock>(faceToNextBlock,nBlockMax);

	// prepare:
	label pImax           = splinep.size() + 1;
	label lmax            = blockLine.size() - 1;
	scalar temp           = (splinep.size() + 1.) / (blockLine.size() - 1.);
	label subSplineLength = temp;
	label pI              = 0;

	// check:
	if(temp != subSplineLength){
		Info << "\n   BlockManager: Error: setInterBlockSpline found " << lmax + 1 << " aligned blocks, but "
				<< splinep.size() << " total spline points. SubSplineLength = " << temp << endl;
		throw;
	}

	for(label l = 0; l <= lmax; l++){

		// prepare:
		pointField supp(subSplineLength);

		// grab block and spline:
		SplineBlock & block = *(blockLine[l]);

		// extract sub-spline:
		for(label spI = 0; spI < subSplineLength; spI++){

			// next point:
			pI++;

			// set spline points:
			supp[spI] = splinep[pI - 1];

		}

		// count vertex:
		pI++;

		// set new spline:
		block.setSpline(splineI,supp);
		if(pI < pImax) block.getSpline(splineI).last() = splinep[pI - 1];
	}
}

void BlockManager::setInterBlockSpline(
			const point & pA,
			const point & pB,
			const Foam::vector & deltaA,
			const Foam::vector & deltaB,
			label iStartBlock,
			label faceToNextBlock,
			label iSpline,
			label nBlockMax,
			const scalarList & sList
		){

	// get block line:
	const List<SplineBlock*> blockLine = getBasicBlock(iStartBlock).getConnectedBlockLine<SplineBlock>(faceToNextBlock,nBlockMax);

	// find existing points along the line:
	// TODO  please delete unused variable: oldPath
	const PointLinePath oldPath = getInterBlockPath(
			iStartBlock,
			iSpline,
			faceToNextBlock,
			nBlockMax,
			true,
			true
			);

	// find s values:
	const scalarList sval = getBlockLineSList(
			iStartBlock,
			iSpline,
			faceToNextBlock,
			nBlockMax,
			true,
			true
			);

	// find new path:
	PointLinePath newPath;
	newPath.addPoint(pA);
	newPath.addPoint(pA + deltaA);
	newPath.addPoint(pB + deltaB);
	newPath.addPoint(pB);

	// set new splines:
	label counter = 0;
	for(label l = 0; l < blockLine.size(); ++l){

		// prepare:
		SplineBlock & block = *(blockLine[l]);
		Spline & spline     = block.ensureSpline(iSpline);

		// set very first point:
		if(l == 0){
			spline.getFirstPoint() = pA;
			counter++;
		}

		// move spline points:
		for(label i = 1; i < label(spline.size()); ++i){

			// move point:
			spline.getPoint(i) = newPath[sval[counter]];

			// count:
			counter++;
		}
	}
}


void BlockManager::setInterBlockSpline(
			const Foam::vector & deltaA,
			const Foam::vector & deltaB,
			label iStartBlock,
			label faceToNextBlock,
			label iSpline,
			label nBlockMax,
			const scalarList & sList
		){

	// get block line:
	const List<SplineBlock*> blockLine = getBasicBlock(iStartBlock).getConnectedBlockLine<SplineBlock>(faceToNextBlock,nBlockMax);

	// prepare:
	const labelList vI = SplineBlock::getSplineVerticesI(iSpline);
	const point pA     = blockLine.first()->getVertex(vI[0]);
	const point pB     = blockLine.last()->getVertex(vI[1]);

	return setInterBlockSpline(pA,pB,deltaA,deltaB,iStartBlock,faceToNextBlock,iSpline,nBlockMax,sList);
}


void BlockManager::setInterBlockSpline(
			const point & pA,
			const Foam::vector & deltaA,
			const Foam::vector & deltaB,
			label iStartBlock,
			label faceToNextBlock,
			label iSpline,
			label nBlockMax,
			const scalarList & sList
		){

	// get block line:
	const List<SplineBlock*> blockLine = getBasicBlock(iStartBlock).getConnectedBlockLine<SplineBlock>(faceToNextBlock,nBlockMax);

	// prepare:
	const labelList vI = SplineBlock::getSplineVerticesI(iSpline);
	const point pB     = blockLine.last()->getVertex(vI[1]);

	return setInterBlockSpline(pA,pB,deltaA,deltaB,iStartBlock,faceToNextBlock,iSpline,nBlockMax,sList);
}
void BlockManager::interpolateParallelSplines(
		label iStartBlock,
		label faceToNextBlock,
		label iSplineStart,
		label iSplineEnd,
		label nBlockMax,
		bool trueDistwise_falseIwise,
		const scalarList & sList,
		bool withFirst,
		bool withLast
		){

	// prepare:
	const labelList vIStart = SplineBlock::getSplineVerticesI(iSplineStart);
	const labelList vIEnd   = SplineBlock::getSplineVerticesI(iSplineEnd);

	// get block line:
	List<SplineBlock*> blockLine = getBasicBlock(iStartBlock).getConnectedBlockLine<SplineBlock>(faceToNextBlock,nBlockMax);

	// check:
	if(!blockLine[0]->hasSpline(iSplineStart)){
		Info << "   \nBlockManager: Error: interpolateParallelSplines called for trivial original spline." << endl;
		throw;
	}

	// grab original spline and make it a line path:
	const Spline spline0 = blockLine[0]->getSpline(iSplineStart);
	PointLinePath splinePath0;
	for(label i = 0; i < label(spline0.size()); ++i){
		splinePath0.addPoint(spline0.getPoint(i));
	}

	// goal spline does not exist, so create it:
	if(!blockLine.last()->hasSpline(iSplineEnd)){

		 // prepare:
		pointField spoints(spline0.size() - 2);
		SplineBlock & hblock = *(blockLine.last());

		// linear points:
		point p            = hblock.getVertex(vIEnd[0]);
		Foam::vector delta = (hblock.getVertex(vIEnd[1]) - p) / (spoints.size() + 1.);
		forAll(spoints, sI){
			spoints[sI] = p + (sI + 1.) * delta;
		}

		// set spline:
		blockLine.last()->setSpline(iSplineEnd,spoints);
	}

	// grab goal spline and make it a line path:
	Spline spline1 = blockLine.last()->getSpline(iSplineEnd);
	PointLinePath splinePath1;
	for(label i = 0; i < label(spline1.size()); ++i){
		splinePath1.addPoint(spline1.getPoint(i));
	}

	// remember s locations of spline points:
	scalarList sloc(sList);
	if(sList.size() == 0){
		sloc = scalarList(splinePath0.size());
		forAll(sloc, sI){
			sloc[sI] = splinePath0.getPointS(sI);
		}
	}

	// loop over block list:
	label imax = blockLine.size() - 1;
	for(label i = 1; i <= imax; ++i){

		// prepare:
		pointField spoints(splinePath0.size() - 2);
		SplineBlock & hblock = *(blockLine[i]);
		labelList vI         = vIStart;
		label iSpline        = iSplineStart;

		// get distance:
		Foam::vector n = blockLine.last()->getVertex(vIEnd[0]) - blockLine.first()->getVertex(vIStart[0]);
		scalar dist    = mag(n);
		scalar x       = mag(dot(hblock.getVertex(vI[0]) - blockLine.first()->getVertex(vIStart[0]),n)) / dist;

		// interpolate points:
		for(label sI = 0; sI < label(splinePath0.size()); sI++){

			// prepare:
			const scalar s = sloc[sI];
			const point q0 = splinePath0[s];
			const point q1 = splinePath1[s];

			// calc point:
			point p(0,0,0);
			if(trueDistwise_falseIwise){
				p = ( (dist - x) * q0 + x * q1 ) / dist;
			} else {
				p = ( (imax + 1. - i) * q0 + i * q1 ) / (imax + 1.);
			}

			// set first point:
			if(sI == 0){
				if(i > 1 || withFirst) hblock.getVertex(vI[0]) = p;
			}

			// set last point:
			else if(sI == label(splinePath0.size()) - 1){
				if(i < imax || withLast) hblock.getVertex(vI[1]) = p;
			}

			// set spline point:
			else {
				spoints[sI - 1] = p;
			}

		}

		// set spline:
		hblock.setSpline(iSpline,spoints);
	}

}

void BlockManager::interpolateInterBlockSpline(
			label iStartBlock,
			label faceToNextBlock,
			label iSpline,
			label order,
			label nBlockMax
		){

	// prepare:
	const labelList vI  = SplineBlock::getSplineVerticesI(iSpline);
	const label edgeDir = SplineBlock::getDirectionEdge(iSpline);

	// get block line:
	const List<SplineBlock*> blockLine = getBasicBlock(iStartBlock).getConnectedBlockLine<SplineBlock>(faceToNextBlock,nBlockMax);

	// set up line path:
	blib::GenericLinePath<point> path;
	forAll(blockLine, blI){
		path.addPoint(blockLine[blI]->getVertex(vI[0]));
	}
	path.addPoint(blockLine.last()->getVertex(vI[1]));

	// interpolate spline:
	label pathPointI  = 0;
	scalar pathPointS = 0;
	forAll(blockLine, blI){

		// prepare:
		SplineBlock & block = *(blockLine[blI]);
		const label cells         = block.getCells(edgeDir);
		const label spts          = cells - 1;

		// rotate:
		scalar lastPathPointS = pathPointS;
		pathPointI++;
		pathPointS = path.getPointS(pathPointI);

		// fill spline points:
		pointField spoints(spts);
		forAll(spoints,spI){
			scalar s     = lastPathPointS + (spI + 1) * (pathPointS - lastPathPointS) / (spts + 1);
			spoints[spI] = path.derivative(s,0,order);
		}

		// set spline:
		block.setSpline(iSpline,spoints);
	}

}

scalarList BlockManager::getBlockLineVertexSList(
		label iBlock,
		label iSpline,
		label face,
		label nBlockMax,
		bool withFirst,
		bool withLast
		){

	// set up path:
	const PointLinePath path = getInterBlockVertexPath(iBlock,iSpline,face,nBlockMax,withFirst,withLast);

	// calc s:
	scalarList out(path.size());
	forAll(out,sI){
		out[sI] = path.getPointS(sI);
	}

	return out;
}

scalarList BlockManager::getBlockLineSList(
		label iBlock,
		label iSpline,
		label face,
		label nBlockMax,
		bool withFirst,
		bool withLast
		) const {

	// set up path:
	const PointLinePath path = getInterBlockPath(iBlock,iSpline,face,nBlockMax,withFirst,withLast);

	// calc s:
	scalarList out(path.size());
	forAll(out,sI){
		out[sI] = path.getPointS(sI);
	}

	return out;
}


void BlockManager::setAllDummySplines(){
	for(label i = 0; i < size(); ++i){
		getSplineBlock(i).setDummySplines();
	}
}

void BlockManager::setAverageInterBlockSpline(
		label iBlock,
		label iNeighborBlock,
		label faceAlongLine,
		label iOppositeSpline_thisBlock,
		label iOppositeSpline_neighborBlock,
		label iOppositeSpline_thisBlock_preSpline,
		label iOppositeSpline_neighborBlock_preSpline,
		label nBlockMax,
		bool withFirst,
		bool withLast
		){

	// get block lines:
	List<SplineBlock*> blockLine = getBasicBlock(iBlock).getConnectedBlockLine<SplineBlock>(faceAlongLine,nBlockMax);

	// prepare:
	const label dir             = SplineBlock::getDirectionEdge(iOppositeSpline_neighborBlock);
	labelList vI_opp_this = SplineBlock::getSplineVerticesI(iOppositeSpline_thisBlock); // TODO unused variable <-- to delete 
	labelList vI_opp_neig = SplineBlock::getSplineVerticesI(iOppositeSpline_neighborBlock);

	// get this line path:
	PointLinePath path_this;
	if(iOppositeSpline_thisBlock_preSpline != SplineBlock::NONE){
		path_this = blockLine.first()->ensureSpline(iOppositeSpline_thisBlock_preSpline).getPointLinePath();
	}
	path_this.append(
				getInterBlockPath(
						iBlock,
						iOppositeSpline_thisBlock,
						faceAlongLine,
						-1,
						path_this.size() == 0,
						true
				)
			);

	// get neig line path:
	PointLinePath path_neig;
	if(iOppositeSpline_neighborBlock_preSpline != SplineBlock::NONE){
		path_neig = getSplineBlock(iNeighborBlock).ensureSpline(iOppositeSpline_neighborBlock_preSpline).getPointLinePath();
	}
	path_neig.append(
				getInterBlockPath(
						iNeighborBlock,
						iOppositeSpline_neighborBlock,
						faceAlongLine,
						-1,
						path_neig.size() == 0,
						true
				)
			);

	// find goal spline s values:
	scalarList sloc = getBlockLineSList(
				iBlock,
				iOppositeSpline_neighborBlock,
				faceAlongLine,
				-1,
				true,
				true
				);

	// calc average path:
	PointLinePath path_av;
	forAll(sloc,sI){
		scalar s = sloc[sI];
		path_av.addPoint(
				0.5 * (path_this[s] + path_neig[s])
				);
	}

	// loop over line:
	label counter = 0;
	for(label i = 0; i < blockLine.size(); ++i){

		// grab block:
		SplineBlock & block = *(blockLine[i]);

		// add first point:
		if(i > 0 || withFirst){
			const scalar s = sloc[counter];
			block.getVertex(vI_opp_neig[0]) = path_av[s];
		}

		// count first point:
		counter++;

		// middle points:
		pointField spoints(block.getCells(dir) - 1);
		forAll(spoints,sI){
			const scalar s    = sloc[counter];
			spoints[sI] = path_av[s];
			counter++;
		}

		// last point:
		if(i == blockLine.size() - 1 && withLast){
			const scalar s = sloc[counter];
			block.getVertex(vI_opp_neig[1]) = path_av[s];
			counter++;
		}

		// set spline:
		block.setSpline(iOppositeSpline_neighborBlock,spoints);
	}
}

PointLinePath BlockManager::getInterBlockPath(
		label iBlock,
		label iSpline,
		label face,
		label nBlockMax,
		bool withFirst,
		bool withLast
		) const{

	// get block line:
	List<SplineBlock const *> blockLine = getBasicBlock(iBlock).getConnectedBlockLine<SplineBlock const>(face,nBlockMax);

	// prepare:
	const labelList vI  = SplineBlock::getSplineVerticesI(iSpline);
	const label jSpline = SplineBlock::switchedOrientationLabel(iSpline);

	// set up path:
	PointLinePath path;

	// first point:
	if(withFirst) {
		path.addPoint(blockLine.first()->getVertex(vI[0]));
	}

	for(label i = 0; i < blockLine.size(); ++i){

		// grab block:
		const SplineBlock & block = *(blockLine[i]);

		// case spline exists:
		if(block.hasSpline(iSpline)){

			// get spline:
			const Spline & spline = block.getSpline(iSpline);

			// add spline points:
			for(label j = 1; j < label(spline.size()); j++){
				path.addPoint(spline.getPoint(j));
			}
		}

		// case opposite spline exists:
		else if(block.hasSpline(jSpline)){

			// get spline:
			const Spline & spline = block.getSpline(jSpline);

			// add spline points:
			for(label j = 1; j < label(spline.size()); j++){
				path.addPoint(spline.getPoint(spline.size() - 1 - j));
			}
		}

		// else no spline exists:
		else {

			// find points:
			Spline spline = block.getSplineCopy(iSpline);
			pointField pts(
					getLinearList(
							spline.getFirstPoint(),
							spline.getLastPoint(),
							block.getCells(SplineBlock::getDirectionEdge(iSpline)) - 1,
							false,
							true
							)
					);

			// add to path:
			forAll(pts,pI){
				path.addPoint(pts[pI]);
			}
		}
	}

	if(!withLast){
		path.erase(path.size() - 1);
	}

	return path;
}

PointLinePath BlockManager::getInterBlockVertexPath(
		label iBlock,
		label iSpline,
		label face,
		label nBlockMax,
		bool withFirst,
		bool withLast
		) const{

	// get block line:
	List<SplineBlock const *> blockLine = getBasicBlock(iBlock).getConnectedBlockLine<SplineBlock const>(face,nBlockMax);

	// prepare:
	const labelList vI = SplineBlock::getSplineVerticesI(iSpline);

	// set up path:
	PointLinePath path;

	// first point:
	if(withFirst) {
		path.addPoint(blockLine.first()->getVertex(vI[0]));
	}

	label bImax = withLast ? blockLine.size() - 1 : blockLine.size() - 2;
	for(label bI = 0; bI <= bImax; bI++){
		path.addPoint(blockLine[bI]->getVertex(vI[1]));
	}

	return path;
}

labelList BlockManager::walkBox(label n,const word & HL, bool counterClockWise) const{

	// preapre:
	labelList out(3,0);
	label counter = 0;
	label face    = counterClockWise ? BasicBlock::SOUTH : BasicBlock::WEST;
	bool isHigh   = HL.compare("H") == 0 ? true : false;

	// walk counter clock wise:
	if(counterClockWise){
		while(counter < n){

			// west:
			if(face == BasicBlock::WEST){
				out[2] = isHigh ? BasicBlock::NWH : BasicBlock::NWL;
				out[BasicBlock::Y]--;
				if(out[BasicBlock::Y] < 0){
					Info << "\nBLockManager: Error: walkBox overshoot." << endl;
					Info << "   n    = " << n << endl;
					Info << "   nmax = " << walkBoxMaximum() << endl;
					throw;
				}
			}

			// north:
			if(face == BasicBlock::NORTH){
				out[2] = isHigh ? BasicBlock::NEH : BasicBlock::NEL;
				out[BasicBlock::X]--;
				if(out[BasicBlock::X] < 0){
					out[BasicBlock::X]++;
					out[2] = isHigh ? BasicBlock::NWH : BasicBlock::NWL;
					face   = BasicBlock::WEST;
				}
			}

			// east:
			if(face == BasicBlock::EAST){
				out[2] = isHigh ? BasicBlock::SEH : BasicBlock::SEL;
				out[BasicBlock::Y]++;
				if(out[BasicBlock::Y] == blockNrs[BasicBlock::Y] ){
					out[BasicBlock::Y]--;
					out[2] = isHigh ? BasicBlock::NEH : BasicBlock::NEL;
					face   = BasicBlock::NORTH;
				}
			}

			// south:
			if(face == BasicBlock::SOUTH){
				out[2] = isHigh ? BasicBlock::SWH : BasicBlock::SWL;
				out[BasicBlock::X]++;
				if(out[BasicBlock::X] == blockNrs[BasicBlock::X] ){
					out[BasicBlock::X]--;
					out[2] = isHigh ? BasicBlock::SEH : BasicBlock::SEL;
					face   = BasicBlock::EAST;
				}
			}

			counter++;
		}
	}

	// else walk clockwise:
	else {
		while(counter < n){

			// south:
			if(face == BasicBlock::SOUTH){
				out[2] = isHigh ? BasicBlock::SEH : BasicBlock::SEL;
				out[BasicBlock::X]--;
				if(out[BasicBlock::X] < 0 ){
					Info << "\nBLockManager: Error: walkBox overshoot." << endl;
					Info << "   n    = " << n << endl;
					Info << "   nmax = " << walkBoxMaximum() << endl;
					throw;
				}
			}

			// east:
			if(face == BasicBlock::EAST){
				out[2] = isHigh ? BasicBlock::NEH : BasicBlock::NEL;
				out[BasicBlock::Y]--;
				if(out[BasicBlock::Y] < 0 ){
					out[BasicBlock::Y]++;
					out[2] = isHigh ? BasicBlock::SEH : BasicBlock::SEL;
					face   = BasicBlock::SOUTH;
				}
			}

			// north:
			if(face == BasicBlock::NORTH){
				out[2] = isHigh ? BasicBlock::NWH : BasicBlock::NWL;
				out[BasicBlock::X]++;
				if(out[BasicBlock::X] == blockNrs[BasicBlock::X]){
					out[BasicBlock::X]--;
					out[2] = isHigh ? BasicBlock::NEH : BasicBlock::NEL;
					face   = BasicBlock::EAST;
				}
			}

			// west:
			if(face == BasicBlock::WEST){
				out[2] = isHigh ? BasicBlock::SWH : BasicBlock::SWL;
				out[BasicBlock::Y]++;
				if(out[BasicBlock::Y] == blockNrs[BasicBlock::Y]){
					out[BasicBlock::Y]--;
					out[2] = isHigh ? BasicBlock::NWH : BasicBlock::NWL;
					face   = BasicBlock::NORTH;
				}
			}

			counter++;
		}
	}

	return out;
}

}
}
