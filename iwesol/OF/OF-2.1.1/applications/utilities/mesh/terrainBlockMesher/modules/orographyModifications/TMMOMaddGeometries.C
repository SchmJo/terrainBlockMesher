#include "TerrainManagerModuleOrographyModifications.H"
#include "TerrainManager.H"
#include "OvalKMHill.H"
#include "ConvexPolygon.H"
#include "PointLinePath.H"

namespace Foam {

namespace oldev {

void TerrainManagerModuleOrographyModifications::addKasmiMassonHill(const dictionary & dict){

	// prepare:
	const Foam::vector & n_up = moduleBase().coordinateSystem().e(2);

	// set up hill:
	KasmiMassonHill kmh(dict);

	// provide memory which points have been shifted:
	HashSet<label> shiftMem;

	// loop over blocks and shift lower block vertices:
	label countShifts = 0;
	forAll(moduleBase().blocks(), bI){

		// shift low vertices:
		for(label v = 0; v < 8; v++){
			if( v == Block::SWL || v == Block::NWL || v == Block::SEL || v == Block::NEL){

				// grab vertex index:
				label vI = moduleBase().blocks()[bI].vertexLabels()[v];

				// check if this vertex has not been touched yet:
				if(!shiftMem.found(vI)){

					// prepare:
					point & p         = moduleBase().getPoint(vI);
					const point q     = p - kmh.center();
					const scalar dist = mag(q - (q & n_up) * n_up);

					// get hill height:
					const scalar h = kmh.height(dist);

					// move point:
					moveGroundPoint(p,kmh.center() & n_up,h,kmh.addType());

					// remember:
					shiftMem.set(vI);
					if( h != 0.) countShifts++;

				}
			}
		}
	}
	Info << "      " << countShifts << " vertex points shifted" << endl;

	// shift edges:
	countShifts = 0;
	forAll(moduleBase().edges(),sI){

		// grab spline points:
		pointField pts = moduleBase().edges().edgePoints(sI,true);

		// loop over spline points:
		forAll(pts,pI){

			// prepare:
			point & p         = pts[pI];
			const point q     = p - kmh.center();
			const scalar dist = mag(q - (q & n_up) * n_up);

			// get hill height:
			const scalar h = kmh.height(dist);

			// move point:
			if( h != 0.){
				moveGroundPoint(p,kmh.center() & n_up,h,kmh.addType());
				countShifts++;
			}
		}

		// set edge:
		moduleBase().resetEdge(sI,pts);
	}
	Info << "      " << countShifts << " spline points shifted" << endl;

}

void TerrainManagerModuleOrographyModifications::addConvexPolygon(const dictionary & dict){

	// set up hill:
	ConvexPolygon kmh(dict);

	// provide memory which points have been shifted:
	HashSet<label> shiftMem;

	// loop over blocks and shift lower block vertices:
	label countShifts = 0;
	forAll(moduleBase().blocks(), bI){

		// shift low vertices:
		for(label v = 0; v < 8; v++){
			if( v == Block::SWL || v == Block::NWL || v == Block::SEL || v == Block::NEL){

				// grab vertex index:
				label vI = moduleBase().blocks()[bI].vertexLabels()[v];

				// check if this vertex has not been touched yet:
				if(!shiftMem.found(vI)){

					// prepare:
					point & p   = moduleBase().getPoint(vI);

					// get hill height:
					scalar h = kmh.height(p);

					// move point:
					moveGroundPoint(p,0,h,kmh.addType());

					// remember:
					shiftMem.set(vI);
					if( h != 0.) countShifts++;

				}
			}
		}
	}
	Info << "      " << countShifts << " vertex points shifted" << endl;


	// shift moduleBase().edges():
	countShifts = 0;
	forAll(moduleBase().edges(),sI){

		// grab spline points:
		pointField pts = moduleBase().edges().edgePoints(sI,true);

		// loop over spline points:
		forAll(pts,pI){

			// prepare:
			point & p = pts[pI];

			// get hill height:
			scalar h = kmh.height(p);

			// move point:
			if( h != 0.){
				moveGroundPoint(p,0,h,kmh.addType());
				countShifts++;
			}
		}

		// set edge:
		moduleBase().resetEdge(sI,pts);
	}
	Info << "      " << countShifts << " spline points shifted" << endl;

}

void TerrainManagerModuleOrographyModifications::addOvalKMHill(const dictionary & dict){

	// prepare:
	const Foam::vector & n_up = moduleBase().coordinateSystem().e(TerrainManager::UP);

	// set up hill:
	OvalKMHill kmh(dict);

	// provide memory which points have been shifted:
	HashSet<label> shiftMem;

	// loop over blocks and shift lower block vertices:
	label countShifts = 0;
	forAll(moduleBase().blocks(), bI){

		// shift low vertices:
		for(label v = 0; v < 8; v++){

			if( v == Block::SWL || v == Block::NWL || v == Block::SEL || v == Block::NEL){

				// grab vertex index:
				label vI = moduleBase().blocks()[bI].vertexLabels()[v];

				// check if this vertex has not been touched yet:
				if(!shiftMem.found(vI)){

					// prepare:
					point & p = moduleBase().getPoint(vI);
					point   q = p - (p & n_up)* n_up;

					// get hill height:
					scalar h = kmh.height(q);

					// move point:
					moveGroundPoint(p,kmh.center() & n_up,h,kmh.addType());

					// remember:
					shiftMem.set(vI);
					if( h != 0.) countShifts++;

				}
			}
		}
	}
	Info << "      " << countShifts << " vertex points shifted" << endl;


	// shift moduleBase().edges():
	countShifts = 0;
	forAll(moduleBase().edges(),sI){

		// grab spline points:
		pointField pts = moduleBase().edges().edgePoints(sI,true);

		// loop over spline points:
		forAll(pts,pI){

			// prepare:
			point & p   = pts[pI];
			point   q   = p - ( p & n_up )* n_up;

			// get hill height:
			scalar h = kmh.height(q);

			// move point:
			if( h != 0.){
				moveGroundPoint(p,kmh.center() & n_up,h,kmh.addType());
				countShifts++;
			}
		}

		// set edge:
		moduleBase().resetEdge(sI,pts);
	}
	Info << "      " << countShifts << " spline points shifted" << endl;

}

}
}
