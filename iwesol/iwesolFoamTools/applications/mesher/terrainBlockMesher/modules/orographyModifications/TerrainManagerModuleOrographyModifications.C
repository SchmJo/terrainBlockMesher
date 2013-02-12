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

#include "TerrainManagerModuleOrographyModifications.H"
#include "TerrainManager.H"
#include "OvalKMHill.H"
#include "ConvexPolygon.H"

#include "HashSet.H"

namespace Foam{
namespace iwesol{

TerrainManagerModuleOrographyModifications::TerrainManagerModuleOrographyModifications(
		TerrainManager * terrainManager
		):
		ClassModule< TerrainManager >(terrainManager){
}

bool TerrainManagerModuleOrographyModifications::load(
			const dictionary & dict
			){

    // get the modify orography tasks:
    modifyTasks = getDictTasks(dict);
    Info << "   found " << modifyTasks.size() << " orography modification requests." << endl;

	return ClassModule< TerrainManager >::load(dict);
}

bool TerrainManagerModuleOrographyModifications::calc(){

	forAll(modifyTasks,mI){

		Info << "   Task " << mI + 1 << ": ";

		// grab task:
		const dictionary & modifyDict = modifyTasks[mI];

	    // add a KasmiMassonHill:
	    if(modifyDict.found("KasmiMassonHill")){

	    	Info << "Adding Kasmi-Masson-Hill" << endl;
	        addKasmiMassonHill(modifyDict.subDict("KasmiMassonHill"));

	    }

	    // add an OvalKMHill:
	    if(modifyDict.found("OvalKMHill")){

	    	Info << "Adding oval Kasmi-Masson-Hill" << endl;
	        addOvalKMHill(modifyDict.subDict("OvalKMHill"));
	    }

	    // add an ConvexPolygon:
	    if(modifyDict.found("ConvexPolygon")){

	    	Info << "Adding convex polygon" << endl;
	        addConvexPolygon(modifyDict.subDict("ConvexPolygon"));
	    }

	    // make mesh cyclic:
	    if(modifyDict.found("makeCyclicNorthSouth")){

	    	const scalar depth = readScalar(modifyDict.subDict("makeCyclicNorthSouth").lookup("adjDepth"));
	    	Info << "Enforcing cyclic boundaries: "
	    			<< moduleBase().patches[BasicBlock::NORTH].getName() << " <--> "
	    			<< moduleBase().patches[BasicBlock::SOUTH].getName() << ", depth = " << depth << endl;
	    	makeCyclic(BasicBlock::NORTH,BasicBlock::SOUTH,depth);

	    }
	    if(modifyDict.found("makeCyclicWestEast")){

	    	const scalar depth = readScalar(modifyDict.subDict("makeCyclicWestEast").lookup("adjDepth"));
	    	Info << "Enforcing cyclic boundaries: "
	    			<< moduleBase().patches[BasicBlock::WEST].getName() << " <--> "
	    			<< moduleBase().patches[BasicBlock::EAST].getName() << ", depth = " << depth << endl;
	    	makeCyclic(BasicBlock::WEST,BasicBlock::EAST,depth);

	    }

	}

	return true;
}


void TerrainManagerModuleOrographyModifications::moveGroundPoint(
		point & p,scalar height0, scalar height, const word & addType){

	// prepare:
	const Foam::vector & n_up = moduleBase().cooSys->e(TerrainBlock::UP);

	// add type 'add':
	if(addType.compare("add") == 0){

		if( height != 0 ) p += height * n_up;

	}

	// add type 'max':
	else if(addType.compare("max") == 0){

		const scalar h0 = dot(p,n_up);
		if( height0 + height > h0 ){
			p += (height0 + height - h0) * n_up;
		}
	}

	// add type 'average':
	else if(addType.compare("average") == 0){

		if( height != 0 ){
			const scalar h0 = dot(p,n_up);
			scalar h  = 0.5 * ( 2 * h0 + height0 + height );
			p        += (h - h0) * n_up;
		}

	}

	// add type 'hill':
	else if(addType.compare("hill") == 0){

		if( height != 0.){
			const scalar h0 = dot(p,n_up);
			p        += (height0 + height - h0) * n_up;
		}
	}

}


void TerrainManagerModuleOrographyModifications::makeCyclic(label patchIA, label patchIB,const scalar & depth){

	// prepare:
	label dir_perp = 0;
	label dir_long = 0;
	labelList vListA(2);
	labelList vListB(2);
	if(patchIA == BasicBlock::WEST && patchIB == BasicBlock::EAST){
		dir_perp   = 0;
		dir_long   = 1;
		vListA[0]  = BasicBlock::SWL;
		vListA[1]  = BasicBlock::NWL;
		vListB[0]  = BasicBlock::SEL;
		vListB[1]  = BasicBlock::NEL;
	} else if(patchIA == BasicBlock::EAST && patchIB == BasicBlock::WEST){
		makeCyclic(patchIB,patchIA,depth);
		return;
	} else if(patchIA == BasicBlock::SOUTH && patchIB == BasicBlock::NORTH){
		dir_perp   = 1;
		dir_long   = 0;
		vListA[0]  = BasicBlock::SWL;
		vListA[1]  = BasicBlock::SEL;
		vListB[0]  = BasicBlock::NWL;
		vListB[1]  = BasicBlock::NEL;
	} else if(patchIA == BasicBlock::NORTH && patchIB == BasicBlock::SOUTH){
		makeCyclic(patchIB,patchIA,depth);
		return;
	}
	labelList splPerp(2);
	const Foam::vector & n_perp  = moduleBase().cooSys->e(dir_perp);
	const Foam::vector & n_long  = moduleBase().cooSys->e(dir_long);
	const Foam::vector & n_up    = moduleBase().cooSys->e(2);
	label splineA                = SplineBlock::getSplineLabel(vListA[0],vListA[1]);
	label splineB                = SplineBlock::getSplineLabel(vListB[0],vListB[1]);
	splPerp[0]                   = SplineBlock::getSplineLabel(vListA[0],vListB[0]);
	splPerp[1]                   = SplineBlock::getSplineLabel(vListA[1],vListB[1]);

	// calculate goal mean heights, set boundary values:
	HashTable<scalar> hMean_ilv;
	List<pointField> hMeanSplines_il(moduleBase().blockNrs[dir_long]);
	for(label il = 0; il < moduleBase().blockNrs[dir_long]; il++){

		// point means:
		for(label v = 0; v < 2; v++){

			// v = 1 is only relevant for last block:
			if(v == 1 && il < moduleBase().blockNrs[dir_long] - 1) continue;

			// calc average height:
			word keyA = dir_long == 0 ?
					key(il,0,vListA[v]):
					key(0,il,vListA[v]);
			word keyB = dir_long == 0 ?
					key(il,moduleBase().blockNrs[dir_perp] - 1,vListB[v]):
					key(moduleBase().blockNrs[dir_perp] - 1,il,vListB[v]);
			point & pA = moduleBase().points[moduleBase().pointAdr_ijv[keyA]];
			point & pB = moduleBase().points[moduleBase().pointAdr_ijv[keyB]];
			scalar h_av = 0.5 * (
					dot(pA,n_up) + dot(pB,n_up)
				);

			// store:
			hMean_ilv.set(key(il,v),h_av);

			// set boundary points to average:
			pA += (h_av - dot(pA,n_up)) * n_up;
			pB += (h_av - dot(pB,n_up)) * n_up;

		}
	}


	// mean parallel moduleBase().splines:
	for(label il = 0; il < moduleBase().blockNrs[dir_long]; il++){

		// grab moduleBase().splines:
		word keyA = dir_long == 0 ?
				key(il,0):
				key(0,il);
		word keyB = dir_long == 0 ?
				key(il,moduleBase().blockNrs[dir_perp] - 1):
				key(moduleBase().blockNrs[dir_perp] - 1,il);
		TerrainBlock & blockA = moduleBase().blocks[moduleBase().blockAdr_ij[keyA]];
		TerrainBlock & blockB = moduleBase().blocks[moduleBase().blockAdr_ij[keyB]];
		Spline & splA         = blockA.getSpline(splineA);
		Spline & splB         = blockB.getSpline(splineB);
		label sPoints         = splA.size();

		// correct for possibly inverse point order in moduleBase().splines:
		label signA = 1;
		label signB = 1;
		scalar oA = dot(splA.last() - splA.first(),n_long);
		scalar oB = dot(splB.last() - splB.first(),n_long);
		if(oA < 0) signA = -1;
		if(oB < 0) signB = -1;

		// find average moduleBase().splines and fix them at boundaries:
		pointField hList_av(sPoints);
		forAll(hList_av,hI){

			// get spline points:
			label hIA  = signA > 0 ? hI : sPoints - 1 - hI;
			label hIB  = signB > 0 ? hI : sPoints - 1 - hI;
			point & pA = splA[hIA];
			point & pB = splB[hIB];

			// calc mean:
			hList_av[hI] = 0.5 * ( pA + pB );

			// adjust boundary moduleBase().splines:
			if(hI == 0){
				pA = blockA.getVertex(vListA[0]);
				pB = blockB.getVertex(vListB[0]);
			} else if(hI == sPoints - 1){
				pA = blockA.getVertex(vListA[1]);
				pB = blockB.getVertex(vListB[1]);
			} else {
				pA += dot(hList_av[hI] - pA,n_up) * n_up;
				pB += dot(hList_av[hI] - pB,n_up) * n_up;
			}

		}
		hMeanSplines_il[il] = hList_av;

	}


	// adjust points within depth:
	word k,k0;
	for(label isB = 0; isB < 2; isB++){

		for(label il = 0; il < moduleBase().blockNrs[dir_long]; il++){
			for(label ip = 1; ip < moduleBase().blockNrs[dir_perp] - 1; ip++){

				// adjust points:
				for(label v = 0; v < 2; v++){

					// v = 1 is only relevant for last block:
					if(v == 1 && il < moduleBase().blockNrs[dir_long] - 1) continue;

					// grab points:
					word k = dir_long == 0 ?
							key(il,ip,vListA[v]):
							key(ip,il,vListA[v]);
					word k0 = dir_long == 0 ?
							key(il,0,vListA[v]):
							key(0,il,vListA[v]);
					if(isB){
						k = dir_long == 0 ?
								key(il,ip,vListB[v]):
								key(ip,il,vListB[v]);
						k0 = dir_long == 0 ?
								key(il,moduleBase().blockNrs[dir_perp] - 1,vListB[v]):
								key(moduleBase().blockNrs[dir_perp] - 1,il,vListB[v]);
					}
					point & p  = moduleBase().points[moduleBase().pointAdr_ijv[k]];
					point & p0 = moduleBase().points[moduleBase().pointAdr_ijv[k0]];

					// calc dist:
					point q  = p - p0;
					q       -= dot(q,n_up) * n_up;
					scalar d = mag(q);

					// change points within depth:
					if( d < depth){
						scalar newHeight = (depth - d) / depth * hMean_ilv[key(il,v)] + d / depth * dot(p,n_up);
						p               += (newHeight - dot(p,n_up)) * n_up;
					}

				}

				// adjust moduleBase().splines parallel:
				k = dir_long == 0 ?
						key(il,ip):
						key(ip,il);
				k0 = dir_long == 0 ?
						key(il,0):
						key(0,il);
				if(isB){
					k0 = dir_long == 0 ?
							key(il,moduleBase().blockNrs[dir_perp] - 1):
							key(moduleBase().blockNrs[dir_perp] - 1,il);
				}
				Spline & spl  = !isB ?
						moduleBase().blocks[moduleBase().blockAdr_ij[k]].getSpline(splineA):
						moduleBase().blocks[moduleBase().blockAdr_ij[k]].getSpline(splineB);
				Spline & spl0 = !isB ?
						moduleBase().blocks[moduleBase().blockAdr_ij[k0]].getSpline(splineA):
						moduleBase().blocks[moduleBase().blockAdr_ij[k0]].getSpline(splineB);

				//
				// check if point order is the same:
				label signA = 1;
				label signB = 1;
				scalar oA = dot(spl.last() - spl.first(),n_long);
				scalar oB = dot(spl0.last() - spl0.first(),n_long);
				if(oA < 0) signA = -1;
				if(oB < 0) signB = -1;
				//
				forAll(spl.getPoints(),pI){

					// get spline points:
					point & p  = signA > 0 ? spl[pI] : spl[spl.size() - 1 - pI];
					point & p0 = signB > 0 ? spl0[pI] : spl0[spl.size() - 1 - pI];

					// calc dist:
					point q  = p - p0;
					q       -= dot(q,n_up) * n_up;
					scalar d = mag(q);

					// change points within depth:
					if( d < depth){

						// calc height:
						scalar h0 = dot(hMeanSplines_il[il][pI],n_up);
						scalar newHeight = (depth - d) / depth * h0 + d / depth * dot(p,n_up);

						// adjust parallel moduleBase().splines points:
						if(pI > 0 && pI < label(spl.size()) - 1){
							p += (newHeight - dot(p,n_up)) * n_up;
						}
					}

				}
			}

			// adjust moduleBase().splines perp:
			for(label ip = 0; ip < moduleBase().blockNrs[dir_perp]; ip++){
				for(label srest = 0; srest < 2; srest++){

					// only for last block there are two perp moduleBase().splines:
					if(srest == 1 && il < moduleBase().blockNrs[dir_long] - 1) continue;

					// grab moduleBase().splines:
					k = dir_long == 0 ?
							key(il,ip):
							key(ip,il);
					k0 = dir_long == 0 ?
							key(il,0):
							key(0,il);
					if(isB){
						k0 = dir_long == 0 ?
								key(il,moduleBase().blockNrs[dir_perp] - 1):
								key(moduleBase().blockNrs[dir_perp] - 1,il);
					}
					TerrainBlock & blockR       = moduleBase().blocks[moduleBase().blockAdr_ij[k]];
					const TerrainBlock & block0 = moduleBase().blocks[moduleBase().blockAdr_ij[k0]];
					Spline & splr               = blockR.getSpline(splPerp[srest]);

					// grab goal point at boundary:
					const point & q = !isB ?
							block0.getVertex(vListA[srest]):
							block0.getVertex(vListB[srest]);

					// get spline orientation:
					label signR = !isB ? 1 : -1;
					scalar oR = dot(splr.last() - splr.first(),n_perp);
					if(oR < 0) signR *= -1;

					// loop over spline points:
					forAll(splr.getPoints(),pI){

						// grap point:
						label pIR = signR > 0 ? pI : splr.size() - 1 - pI;
						point & p = splr[pIR];

						// calc distance to boundary point:
						point q2  = p - q;
						q2       -= dot(q2,n_up) * n_up;
						scalar d  = mag(q2);

						// change points within depth:
						if(pI == 0 && ( (ip == 0 && !isB) || (ip == moduleBase().blockNrs[dir_perp] - 1 && isB) )){

							p = q;

						} else if( d < depth){

							scalar h0 = dot(q,n_up);
							scalar newHeight = (depth - d) / depth * h0 + d / depth * dot(p,n_up);

							// adjust perpendicular moduleBase().splines points:
							if(pI > 0 && pI < label(splr.size()) - 1){
								p += (newHeight - dot(p,n_up)) * n_up;
							}

						}

					}


				}


			}
		}
	}


	// finally set patch info:
	moduleBase().patches[patchIA].setCyclic(moduleBase().patches[patchIB].getName());
	moduleBase().patches[patchIB].setCyclic(moduleBase().patches[patchIA].getName());

}



void TerrainManagerModuleOrographyModifications::addKasmiMassonHill(const dictionary & dict){

	// prepare:
	const Foam::vector & n_up = moduleBase().cooSys->e(2);

	// set up hill:
	KasmiMassonHill kmh(dict);

	// provide memory which points have been shifted:
	HashSet<label> shiftMem;

	// loop over blocks and shift lower block vertices:
	label countShifts = 0;
	forAll(moduleBase().blocks, bI){

		// shift low vertices:
		for(label v = 0; v < 8; v++){
			if( v == BasicBlock::SWL || v == BasicBlock::NWL || v == BasicBlock::SEL || v == BasicBlock::NEL){

				// grab vertex index:
				label vI = moduleBase().blocks[bI].getVertexI(v);

				// check if this vertex has not been touched yet:
				if(!shiftMem.found(vI)){

					// prepare:
					point & p   = moduleBase().points[vI];
					const point   q   = p - kmh.getCenter();
					const scalar dist = mag(q - dot(q,n_up) * n_up);

					// get hill height:
					const scalar h = kmh.getHeight(dist);

					// move point:
					moveGroundPoint(p,dot(kmh.getCenter(),n_up),h,kmh.getAddType());

					// remember:
					shiftMem.set(vI);
					if( h != 0.) countShifts++;

				}
			}
		}
	}
	Info << "      " << countShifts << " vertex points shifted" << endl;


	// shift moduleBase().splines:
	countShifts = 0;
	forAll(moduleBase().splines,sI){

		// grab spline:
		Spline & s      = moduleBase().splines[moduleBase().splines.toc()[sI]];
		List<point*> pf = s.getPointers();

		// loop over spline points:
		for(label pI = 1; pI < label(s.size()) - 1; pI++){

			// prepare:
			point & p   = *pf[pI];
			const point   q   = p - kmh.getCenter();
			const scalar dist = mag(q - dot(q,n_up) * n_up);

			// get hill height:
			const scalar h = kmh.getHeight(dist);

			// move point:
			if( h != 0.){
				moveGroundPoint(p,dot(kmh.getCenter(),n_up),h,kmh.getAddType());
				countShifts++;
			}
		}
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
	forAll(moduleBase().blocks, bI){

		// shift low vertices:
		for(label v = 0; v < 8; v++){
			if( v == BasicBlock::SWL || v == BasicBlock::NWL || v == BasicBlock::SEL || v == BasicBlock::NEL){

				// grab vertex index:
				label vI = moduleBase().blocks[bI].getVertexI(v);

				// check if this vertex has not been touched yet:
				if(!shiftMem.found(vI)){

					// prepare:
					point & p   = moduleBase().points[vI];

					// get hill height:
					scalar h = kmh.getHeight(p);

					// move point:
					moveGroundPoint(p,0,h,kmh.getAddType());

					// remember:
					shiftMem.set(vI);
					if( h != 0.) countShifts++;

				}
			}
		}
	}
	Info << "      " << countShifts << " vertex points shifted" << endl;


	// shift moduleBase().splines:
	countShifts = 0;
	forAll(moduleBase().splines,sI){

		// grab spline:
		Spline & s      = moduleBase().splines[moduleBase().splines.toc()[sI]];
		List<point*> pf = s.getPointers();

		// loop over spline points:
		for(label pI = 1; pI < label(s.size()) - 1; pI++){

			// prepare:
			point & p   = s.getPoint(pI);

			// get hill height:
			scalar h = kmh.getHeight(p);

			// move point:
			if( h != 0.){
				moveGroundPoint(p,0,h,kmh.getAddType());
				countShifts++;
			}
		}
	}
	Info << "      " << countShifts << " spline points shifted" << endl;

}

void TerrainManagerModuleOrographyModifications::addOvalKMHill(const dictionary & dict){

	// prepare:
	const Foam::vector & n_up = moduleBase().cooSys->e(2);

	// set up hill:
	OvalKMHill kmh(dict);

	// provide memory which points have been shifted:
	HashSet<label> shiftMem;

	// loop over blocks and shift lower block vertices:
	label countShifts = 0;
	forAll(moduleBase().blocks, bI){

		// shift low vertices:
		for(label v = 0; v < 8; v++){

			if( v == BasicBlock::SWL || v == BasicBlock::NWL || v == BasicBlock::SEL || v == BasicBlock::NEL){

				// grab vertex index:
				label vI = moduleBase().blocks[bI].getVertexI(v);

				// check if this vertex has not been touched yet:
				if(!shiftMem.found(vI)){

					// prepare:
					point & p   = moduleBase().points[vI];
					point   q   = p - dot(p,n_up)* n_up;

					// get hill height:
					scalar h = kmh.getHeight(q);

					// move point:
					moveGroundPoint(p,dot(kmh.getCenter(),n_up),h,kmh.getAddType());

					// remember:
					shiftMem.set(vI);
					if( h != 0.) countShifts++;

				}
			}
		}
	}
	Info << "      " << countShifts << " vertex points shifted" << endl;


	// shift moduleBase().splines:
	countShifts = 0;
	forAll(moduleBase().splines,sI){

		// grab spline:
		Spline & s      = moduleBase().splines[moduleBase().splines.toc()[sI]];
		List<point*> pf = s.getPointers();

		// loop over spline points:
		for(label pI = 1; pI < label(s.size()) - 1; pI++){

			// prepare:
			point & p   = *pf[pI];
			point   q   = p - dot(p,n_up)* n_up;

			// get hill height:
			scalar h = kmh.getHeight(q);

			// move point:
			if( h != 0.){
				moveGroundPoint(p,dot(kmh.getCenter(),n_up),h,kmh.getAddType());
				countShifts++;
			}
		}
	}
	Info << "      " << countShifts << " spline points shifted" << endl;

}

} /* iwesol */
} /* Foam */
