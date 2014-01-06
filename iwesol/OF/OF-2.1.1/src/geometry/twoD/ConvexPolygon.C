/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "ConvexPolygon.H"
#include "addToRunTimeSelectionTable.H"

namespace Foam
{

namespace oldev
{

defineTypeNameAndDebug(ConvexPolygon, 0);
addToRunTimeSelectionTable(GroundObject,ConvexPolygon,dict);
addToRunTimeSelectionTable(OptimizerGroundObject,ConvexPolygon,dict);

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void ConvexPolygon::init(){

	// delete z coordinate:
	p_inside_[2] = 0;

	// calc max dist:
	forAll(points_,pI){

		// delete z coordinates:
		point & q = points_[pI];
		q[2]      = 0;
		scalar d  = dist(q,p_inside_);

		// update max dist:
		if(pI == 0 || d > maxDist_) maxDist_ = d;
	}

	// calculate lines:
	linesf_.resize(size());
	forAll(linesf_,lineI){
		const point & p0 = points_[lineI];
		const point & p1 = ( lineI == size() - 1 ) ? points_.first() : points_[lineI + 1];
		linesf_[lineI]     = p1 - p0;
	}

	// set box:
	{
		// collect data:
		point pmin(0,0,0);
		point pmax(0,0,0);
		forAll(points_,pI){
			const point & p = points_[pI];
			if(pI == 0){
				pmin = p;
				pmax = p;
			} else {
				if(p[0] < pmin[0]) pmin[0] = p[0];
				if(p[1] < pmin[1]) pmin[1] = p[1];
				if(p[0] > pmax[0]) pmax[0] = p[0];
				if(p[1] > pmax[1]) pmax[1] = p[1];
			}
		}
		scalarList lengths(3);
		lengths[0] = pmax[0] - pmin[0];
		lengths[1] = pmax[1] - pmin[1];
		lengths[2] = 1;

		// init box:
		initRangeBox
		(
				Box
				(
						CoordinateSystem
						(
								pmin,
								vector(1,0,0),
								vector(0,1,0),
								vector(0,0,1)
						),
						lengths,
						res_
				)
		);
	}

	// calculate inside signs:
	{
		insideSigns_ = scalarField(size(),1);
		scalarField his(size());
		calcPointConstraints(p_inside_,his);
		forAll(insideSigns_,sI) insideSigns_[sI] = -Foam::sign(his[sI]);
	}
}



// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

ConvexPolygon::ConvexPolygon
(
		const pointField & points_,
		const point & insidePoint,
		const scalar & res_olution,
		const scalar & height_,
		const word & addType
):
		OptimizerGroundObject(addType),
		points_(points_),
		p_inside_(insidePoint),
		res_(res_olution),
		height_(height_){

	init();
}

ConvexPolygon::ConvexPolygon
(
		const dictionary & dict
):
		OptimizerGroundObject(dict),
		points_(dict.lookup("points_")),
		p_inside_(dict.lookup("p_inside_")),
		res_(0.001),
		height_(0){

	if(dict.found("res_olution")){
		res_ = readScalar(dict.lookup("res_olution"));
	} else {
		Info << "ConvexPolygon: keyword 'res_olution' not found in dictionary, choosing " << res_ << endl;
	}

	if(dict.found("height_")){
		height_ = readScalar(dict.lookup("height_"));
	} else {
		Info << "ConvexPolygon: keyword 'height_' not found in dictionary, choosing " << height_ << " m." << endl;
	}

	init();
}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

scalarList ConvexPolygon::calcConstraintScales() const{

	// prepare:
	scalarList cscale(size());

	// loop over lines:
	forAll(lines(),lI){

		// find max delta:
		scalar maxDelta = 0;
		forAll(rangeBox().points(),pI){
			scalar hdelta = mag(rangeBox().points()[pI] - points_[lI]);
			if(pI == 0 || hdelta > maxDelta) maxDelta = hdelta;
		}

		// set:
		cscale[lI] = maxDelta * maxDelta;
	}

	return cscale;
}

std::vector<double> ConvexPolygon::getPlotColumn(int i) const{

	std::vector<double> out(size() + 1);
	forAll(*this,pI){
		out[pI] = (*this)[pI][i];
	}
	out[out.size() - 1] = (*this)[0][i];
	return out;
}

jlib::Plot ConvexPolygon::boundaryPlot(
		const word & plotName,
		const fileName & folder
		) const{

	// prepare:
	point pmin = rangeBox().centre()
			- 1.1 * (rangeBox().centre() - rangeBox().pMin()) ;
	point pmax = rangeBox().centre()
			+ 1.1 * (rangeBox().pMax() - rangeBox().centre());

	// create plot:
	jlib::Plot out
	(
			plotName,
			folder,
			"x [m]",
			"y [m]",
			pmin[0],
			pmax[0],
			pmin[1],
			pmax[1]
	);

	// add line:
	out.addLine(this,0,1,"boundary");

	return out;
}

dictionary ConvexPolygon::exportDict() const{

	dictionary out = OptimizerGroundObject::exportDict();

	out.add("res_olution",res_);
	out.add("height_",height_);
	out.add("points_",points_);
	out.add("p_inside_",p_inside_);

	return out;
}


// ************************************************************************* //

} // End namespace oldev

} // End namespace Foam
