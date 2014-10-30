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

#include "TerrainManagerModuleCylinder.H"
#include "TerrainManager.H"
#include "mathematicalConstants.H"
#include "Globals.H"
#include "PointLinePath.H"

namespace Foam
{

namespace oldev
{

bool TerrainManagerModuleCylinder::setCylinderPatches(){

	// init:
	word groundName           = moduleBase().patches()[Block::GROUND].name();
	word groundType           = moduleBase().patches()[Block::GROUND].type();
	word skyName              = moduleBase().patches()[Block::SKY].name();
	word skyType              = moduleBase().patches()[Block::SKY].type();
	moduleBase().patchesRef().clear();
	for(label sectionI = 0; sectionI < cylinderSectionNr_; sectionI++){
		moduleBase().addPatch
		(
				"section_" + jlib::String(sectionI),
				"patch"
		);
	}
	moduleBase().addPatch
	(
			skyName,
			skyType
	);
	moduleBase().addPatch
	(
			groundName,
			groundType
	);

	// south:
	labelList v(8);
	for(label i = 0; i < moduleBase().blockNrs_[TerrainManager::BASE1]; i++){

		// get point labels:
		v[Block::NWL] = moduleBase().pointAdr_ijv_[key(i,0,Block::SWL)];
		v[Block::NEL] = moduleBase().pointAdr_ijv_[key(i,0,Block::SEL)];
		v[Block::NWH] = moduleBase().pointAdr_ijv_[key(i,0,Block::SWH)];
		v[Block::NEH] = moduleBase().pointAdr_ijv_[key(i,0,Block::SEH)];

		// get section info:
		labelList secionInfo_NWL = cylinderConnections_pI_secSubsec_[v[Block::NWL]];
		labelList secionInfo_NEL = cylinderConnections_pI_secSubsec_[v[Block::NEL]];
		label sectionI           = secionInfo_NEL[0];

		// grab block:
		word key_block = key(Block::SOUTH,i,cylinderRadialBlocks_ - 1);
		label blockI   = cylinderBlockAdr_[key_block];

		// add to patches:
		moduleBase().contributeToPatch(sectionI,blockI,Block::SOUTH);
		moduleBase().contributeToPatch(cylinderSectionNr_,blockI,Block::SKY);
		moduleBase().contributeToPatch(cylinderSectionNr_ + 1,blockI,Block::GROUND);
	}

	// east:
	for(label j = 0; j < moduleBase().blockNrs_[TerrainManager::BASE2]; j++){

		// get point labels:
		v[Block::SWL] = moduleBase().pointAdr_ijv_[key(moduleBase().blockNrs_[TerrainManager::BASE1] - 1,j,Block::SEL)];
		v[Block::NWL] = moduleBase().pointAdr_ijv_[key(moduleBase().blockNrs_[TerrainManager::BASE1] - 1,j,Block::NEL)];
		v[Block::SWH] = moduleBase().pointAdr_ijv_[key(moduleBase().blockNrs_[TerrainManager::BASE1] - 1,j,Block::SEH)];
		v[Block::NWH] = moduleBase().pointAdr_ijv_[key(moduleBase().blockNrs_[TerrainManager::BASE1] - 1,j,Block::NEH)];

		// get section info:
		labelList secionInfo_SWL = cylinderConnections_pI_secSubsec_[v[Block::SWL]];
		labelList secionInfo_NWL = cylinderConnections_pI_secSubsec_[v[Block::NWL]];
		label sectionI           = secionInfo_NWL[0];

		// grab block:
		word key_block = key(Block::EAST,j,cylinderRadialBlocks_ - 1);
		label blockI   = cylinderBlockAdr_[key_block];

		// add to patches:
		moduleBase().contributeToPatch(sectionI,blockI,Block::EAST);
		moduleBase().contributeToPatch(cylinderSectionNr_,blockI,Block::SKY);
		moduleBase().contributeToPatch(cylinderSectionNr_ + 1,blockI,Block::GROUND);
	}

	// north:
	for(label i = 0; i < moduleBase().blockNrs_[TerrainManager::BASE1]; i++){

		// get point labels:
		v[Block::SWL] = moduleBase().pointAdr_ijv_[key(i,moduleBase().blockNrs_[TerrainManager::BASE2] - 1,Block::NWL)];
		v[Block::SEL] = moduleBase().pointAdr_ijv_[key(i,moduleBase().blockNrs_[TerrainManager::BASE2] - 1,Block::NEL)];
		v[Block::SWH] = moduleBase().pointAdr_ijv_[key(i,moduleBase().blockNrs_[TerrainManager::BASE2] - 1,Block::NWH)];
		v[Block::SEH] = moduleBase().pointAdr_ijv_[key(i,moduleBase().blockNrs_[TerrainManager::BASE2] - 1,Block::NEH)];

		// get section info:
		labelList secionInfo_SWL = cylinderConnections_pI_secSubsec_[v[Block::SWL]];
		labelList secionInfo_SEL = cylinderConnections_pI_secSubsec_[v[Block::SEL]];
		label sectionI           = secionInfo_SWL[0];

		// grab block:
		word key_block = key(Block::NORTH,i,cylinderRadialBlocks_ - 1);
		label blockI   = cylinderBlockAdr_[key_block];

		// add to patches:
		moduleBase().contributeToPatch(sectionI,blockI,Block::NORTH);
		moduleBase().contributeToPatch(cylinderSectionNr_,blockI,Block::SKY);
		moduleBase().contributeToPatch(cylinderSectionNr_ + 1,blockI,Block::GROUND);
	}

	// west:
	for(label j = 0; j < moduleBase().blockNrs_[TerrainManager::BASE2]; j++){

		// get point labels:
		v[Block::SEL] = moduleBase().pointAdr_ijv_[key(0,j,Block::SWL)];
		v[Block::NEL] = moduleBase().pointAdr_ijv_[key(0,j,Block::NWL)];
		v[Block::SEH] = moduleBase().pointAdr_ijv_[key(0,j,Block::SWH)];
		v[Block::NEH] = moduleBase().pointAdr_ijv_[key(0,j,Block::NWH)];

		// get section info:
		labelList secionInfo_SEL = cylinderConnections_pI_secSubsec_[v[Block::SEL]];
		labelList secionInfo_NEL = cylinderConnections_pI_secSubsec_[v[Block::NEL]];
		label sectionI           = secionInfo_SEL[0];

		// grab block:
		word key_block = key(Block::WEST,j,cylinderRadialBlocks_ - 1);
		label blockI   = cylinderBlockAdr_[key_block];

		// add to patches:
		moduleBase().contributeToPatch(sectionI,blockI,Block::WEST);
		moduleBase().contributeToPatch(cylinderSectionNr_,blockI,Block::SKY);
		moduleBase().contributeToPatch(cylinderSectionNr_ + 1,blockI,Block::GROUND);
	}

	// center:
	for(label i = 0; i < moduleBase().blockNrs_[TerrainManager::BASE1];i++){
		for(label j = 0; j < moduleBase().blockNrs_[TerrainManager::BASE2];j++){

			// grab block:
			word key_block = key(i,j);
			label blockI   = moduleBase().blockAdr_ij_[key_block];

			// add to patches:
			moduleBase().contributeToPatch(cylinderSectionNr_,blockI,Block::SKY);
			moduleBase().contributeToPatch(cylinderSectionNr_ + 1,blockI,Block::GROUND);
		}
	}


	Info << "      created " << moduleBase().patches().size() << " patches." << endl;


	return true;
}

bool TerrainManagerModuleCylinder::setCylinderSplines(){

	Info << "      setting cylinder splines" << endl;

	// prepare:
	const pointField & points = moduleBase().points();
	const vector & nup        = moduleBase().coordinateSystem().e(TerrainManager::UP);
	label oldEdgeCounter      = moduleBase().edgeCounter();

	// west:
	labelList v(8);
	for(label j = 0; j < moduleBase().blockNrs_[TerrainManager::BASE2]; j++){

		// get point labels:
		v[Block::SEL] = moduleBase().pointAdr_ijv_[key(0,j,Block::SWL)];
		v[Block::SEH] = moduleBase().pointAdr_ijv_[key(0,j,Block::SWH)];

		// grab blocks:
		word key_cylBlock      = key(Block::WEST,j,0);
		label cylBlockI        = cylinderBlockAdr_[key_cylBlock];
		label terBlockI        = moduleBase().blockAdr_ij_[key(0,j)];
		const Block & cylBlock = moduleBase().blocks()[cylBlockI];

		// get points:
		point p0_L         = points[moduleBase().blockVertex(terBlockI,Block::SEL)];
		point p0_H         = points[moduleBase().blockVertex(terBlockI,Block::SEH)];
		const point & p1_L = points[v[Block::SEL]];
		const point & p1_H = points[v[Block::SEH]];
		const point & p2_L = points[moduleBase().blockVertex(cylBlockI,Block::SWL)];
		const point & p2_H = points[moduleBase().blockVertex(cylBlockI,Block::SWH)];


		// calc deltas L:
		Foam::vector deltaA = p1_L - p0_L;
		if(j == 0){
			const point & p0B_L  = points[moduleBase().blockVertex(terBlockI,Block::NWL)];
			Foam::vector deltaA2 = p1_L - p0B_L;
			deltaA = 0.5 * (mag(deltaA) + mag(deltaA2)) * 0.5 * (deltaA/mag(deltaA) + deltaA2/mag(deltaA2));
			p0_L   = p1_L - deltaA;
		}
		Foam::vector deltaB = (p1_L - p2_L) * mag(deltaA) / mag(p1_L - p2_L);

		// st spline L:
		pointField pts(cylinderRadialCells_ - 1);
		forAll(pts,pI){
			scalar s = scalar(1 + pI) / scalar(pts.size() + 2);
			pts[pI]  = p1_L + s * ( p2_L - p1_L );
			scalar z = blending_().blend
			(
					p1_L,
					p2_L,
					p1_L & nup,
					p2_L & nup,
					s
			);
			pts[pI] += ( z - ( pts[pI] & nup ) ) * nup;
		}
		moduleBase().setEdge(cylBlockI,Block::SEL_SWL,pts);

		// calc deltas H:
		deltaA = p1_H - p0_H;
		if(j == 0){
			const point & p0B_H = points[moduleBase().blockVertex(terBlockI,Block::NWH)];
			Foam::vector deltaA2 = p1_H - p0B_H;
			deltaA = 0.5 * (mag(deltaA) + mag(deltaA2)) * 0.5 * (deltaA/mag(deltaA) + deltaA2/mag(deltaA2));
			p0_H   = p1_H - deltaA;
		}
		deltaB = (p1_H - p2_H) * mag(deltaA) / mag(p1_H - p2_H);

		// st spline H:
		forAll(pts,pI){
			scalar s = scalar(1 + pI) / scalar(pts.size() + 2);
			//scalar z = p2_H & nup;
			pts[pI]  = p1_H + s * ( p2_H - p1_H );
			//pts[pI] += ( z - ( pts[pI] & nup ) ) * nup;
		}
		moduleBase().setEdge(cylBlockI,Block::SEH_SWH,pts);

		// outer arc spline L:
		Foam::vector n = (p2_L - cylinderCentre_)/ mag(p2_L - cylinderCentre_);
		pointField ptsArc(cylBlock.blockCells()[TerrainManager::BASE2] - 1);
		scalar deltaAngle = getAngleRad
							(
									n,
									points[moduleBase().blockVertex(cylBlockI,Block::NWL)] - cylinderCentre_
							) / (ptsArc.size() + 2);
		forAll(ptsArc,pI){
			moduleBase().coordinateSystem().rotate
			(
					n,
					deltaAngle,
					-moduleBase().coordinateSystem().e(TerrainManager::UP)
			);
			ptsArc[pI] = cylinderCentre_ + cylinderRadius_ * n;
		}
		moduleBase().setEdge(cylBlockI,Block::SWL_NWL,ptsArc);

		// outer arc spline H:
		n = (p2_L - cylinderCentre_)/ mag(p2_L - cylinderCentre_);
		forAll(ptsArc,pI){
			moduleBase().coordinateSystem().rotate(n,deltaAngle,-moduleBase().coordinateSystem().e(TerrainManager::UP));
			ptsArc[pI]  = cylinderCentre_ + cylinderRadius_ * n;
			ptsArc[pI] += ( (p2_H - ptsArc[pI]) & moduleBase().coordinateSystem().e(TerrainManager::UP) ) * moduleBase().coordinateSystem().e(TerrainManager::UP);
		}
		moduleBase().setEdge(cylBlockI,Block::SWH_NWH,ptsArc);
	}

	// north:
	for(label i = 0; i <= moduleBase().blockNrs_[TerrainManager::BASE1]; i++){

		// get point labels:
		label i2 = (i == moduleBase().blockNrs_[TerrainManager::BASE1]) ? i - 1 : i;
		v[Block::SWL] = moduleBase().pointAdr_ijv_[key(i2,moduleBase().blockNrs_[TerrainManager::BASE2] - 1,Block::NWL)];
		v[Block::SWH] = moduleBase().pointAdr_ijv_[key(i2,moduleBase().blockNrs_[TerrainManager::BASE2] - 1,Block::NWH)];
		if(i != i2){
			v[Block::SWL] = moduleBase().pointAdr_ijv_[key(i2,moduleBase().blockNrs_[TerrainManager::BASE2] - 1,Block::NEL)];
			v[Block::SWH] = moduleBase().pointAdr_ijv_[key(i2,moduleBase().blockNrs_[TerrainManager::BASE2] - 1,Block::NEH)];
		}

		// grab blocks:
		word key_cylBlock      = key(Block::NORTH,i2,0);
		label cylBlockI        = cylinderBlockAdr_[key_cylBlock];
		label terBlockI        = moduleBase().blockAdr_ij_[key(i2,moduleBase().blockNrs_[TerrainManager::BASE2] - 1)];
		const Block & cylBlock = moduleBase().blocks()[cylBlockI];

		// get points:
		point p0_L         = points[moduleBase().blockVertex(terBlockI,i == i2 ? Block::SWL : Block::SEL)];
		point p0_H         = points[moduleBase().blockVertex(terBlockI,i == i2 ? Block::SWH : Block::SEH)];
		const point & p1_L = points[v[Block::SWL]];
		const point & p1_H = points[v[Block::SWH]];
		const point & p2_L = points[moduleBase().blockVertex(cylBlockI,i == i2 ? Block::NWL : Block::NEL)];
		const point & p2_H = points[moduleBase().blockVertex(cylBlockI,i == i2 ? Block::NWH : Block::NEH)];

		// calc deltas L:
		Foam::vector deltaA = p1_L - p0_L;
		if(i == 0){
			const point & p0B_L  = points[moduleBase().blockVertex(terBlockI,Block::NEL)];
			Foam::vector deltaA2 = p1_L - p0B_L;
			deltaA               = 0.5 * (mag(deltaA) + mag(deltaA2)) * 0.5 * (deltaA/mag(deltaA) + deltaA2/mag(deltaA2));
			p0_L                 = p1_L - deltaA;
		} else if(i != i2){
			const point & p0B_L  = points[moduleBase().blockVertex(terBlockI,Block::NWL)];
			Foam::vector deltaA2 = p1_L - p0B_L;
			deltaA               = 0.5 * (mag(deltaA) + mag(deltaA2)) * 0.5 * (deltaA/mag(deltaA) + deltaA2/mag(deltaA2));
			p0_L                 = p1_L - deltaA;
		}
		Foam::vector deltaB = (p1_L - p2_L) * mag(deltaA) / mag(p1_L - p2_L);

		// st spline L:
		pointField pts(cylinderRadialCells_ - 1);
		forAll(pts,pI){
			scalar s = scalar(1 + pI) / scalar(pts.size() + 2);
			pts[pI]  = p1_L + s * ( p2_L - p1_L );
			scalar z = blending_().blend
			(
					p1_L,
					p2_L,
					p1_L & nup,
					p2_L & nup,
					s
			);
			pts[pI] += ( z - ( pts[pI] & nup ) ) * nup;
		}
		moduleBase().setEdge(cylBlockI,i == i2 ? Block::SWL_NWL : Block::SEL_NEL,pts);

		// calc deltas H:
		deltaA = p1_H - p0_H;
		if(i == 0){
			const point & p0B_H  = points[moduleBase().blockVertex(terBlockI,Block::NEH)];
			Foam::vector deltaA2 = p1_H - p0B_H;
			deltaA               = 0.5 * (mag(deltaA) + mag(deltaA2)) * 0.5 * (deltaA/mag(deltaA) + deltaA2/mag(deltaA2));
			p0_H                 = p1_H - deltaA;
		} else if(i != i2){
			const point & p0B_H  = points[moduleBase().blockVertex(terBlockI,Block::NWH)];
			Foam::vector deltaA2 = p1_H - p0B_H;
			deltaA               = 0.5 * (mag(deltaA) + mag(deltaA2)) * 0.5 * (deltaA/mag(deltaA) + deltaA2/mag(deltaA2));
			p0_H                 = p1_H - deltaA;
		}
		deltaB = (p1_H - p2_H) * mag(deltaA) / mag(p1_H - p2_H);

		// st spline H:
		forAll(pts,pI){
			scalar s = scalar(1 + pI) / scalar(pts.size() + 2);
			//scalar z = p2_H & nup;
			pts[pI]  = p1_H + s * ( p2_H - p1_H );
			//pts[pI] += ( z - ( pts[pI] & nup ) ) * nup;
		}
		moduleBase().setEdge(cylBlockI,i == i2 ? Block::SWH_NWH : Block::SEH_NEH,pts);

		// outer arc spline L:
		if(i == i2){
			Foam::vector n = (p2_L - cylinderCentre_)/ mag(p2_L - cylinderCentre_);
			pointField ptsArc(cylBlock.blockCells()[TerrainManager::BASE1] - 1);
			scalar deltaAngle = getAngleRad
								(
										n,
										points[moduleBase().blockVertex(cylBlockI,Block::NEL)] - cylinderCentre_
								) / (ptsArc.size() + 2);
			forAll(ptsArc,pI){
				moduleBase().coordinateSystem().rotate
				(
						n,
						deltaAngle,
						-moduleBase().coordinateSystem().e(TerrainManager::UP)
				);
				ptsArc[pI] = cylinderCentre_ + cylinderRadius_ * n;
			}
			moduleBase().setEdge(cylBlockI,Block::NWL_NEL,ptsArc);
		}

		// outer arc spline H:
		if(i == i2){
			Foam::vector n = (p2_L - cylinderCentre_)/ mag(p2_L - cylinderCentre_);
			pointField ptsArc(cylBlock.blockCells()[TerrainManager::BASE1] - 1);
			scalar deltaAngle = getAngleRad
								(
										n,
										points[moduleBase().blockVertex(cylBlockI,Block::NEL)] - cylinderCentre_
								) / (ptsArc.size() + 2);
			forAll(ptsArc,pI){
				moduleBase().coordinateSystem().rotate
				(
						n,
						deltaAngle,
						-moduleBase().coordinateSystem().e(TerrainManager::UP)
				);
				ptsArc[pI]  = cylinderCentre_ + cylinderRadius_ * n;
				ptsArc[pI] += ( (p2_H - ptsArc[pI]) & moduleBase().coordinateSystem().e(TerrainManager::UP) ) * moduleBase().coordinateSystem().e(TerrainManager::UP);
			}
			moduleBase().setEdge(cylBlockI,Block::NWH_NEH,ptsArc);
		}
	}

	// east:
	for(label j = 0; j < moduleBase().blockNrs_[TerrainManager::BASE2]; j++){

		// get point labels:
		v[Block::SWL] = moduleBase().pointAdr_ijv_[key(moduleBase().blockNrs_[TerrainManager::BASE1] - 1,j,Block::SEL)];
		v[Block::SWH] = moduleBase().pointAdr_ijv_[key(moduleBase().blockNrs_[TerrainManager::BASE1] - 1,j,Block::SEH)];

		// grab blocks:
		word key_cylBlock      = key(Block::EAST,j,0);
		label cylBlockI        = cylinderBlockAdr_[key_cylBlock];
		label terBlockI        = moduleBase().blockAdr_ij_[key(moduleBase().blockNrs_[TerrainManager::BASE1] - 1,j)];
		const Block & cylBlock = moduleBase().blocks()[cylBlockI];

		// get points:
		point p0_L         = points[moduleBase().blockVertex(terBlockI,Block::SWL)];
		point p0_H         = points[moduleBase().blockVertex(terBlockI,Block::SWH)];
		const point & p1_L = points[v[Block::SWL]];
		const point & p1_H = points[v[Block::SWH]];
		const point & p2_L = points[moduleBase().blockVertex(cylBlockI,Block::SEL)];
		const point & p2_H = points[moduleBase().blockVertex(cylBlockI,Block::SEH)];

		if(j > 0){

			// calc deltas L:
			Foam::vector deltaA = p1_L - p0_L;
			Foam::vector deltaB = (p1_L - p2_L) * mag(deltaA) / mag(p1_L - p2_L);

			// st spline L:
			pointField pts(cylinderRadialCells_ - 1);
			forAll(pts,pI){
				scalar s = scalar(1 + pI) / scalar(pts.size() + 2);
				pts[pI]  = p1_L + s * ( p2_L - p1_L );
				scalar z = blending_().blend
				(
						p1_L,
						p2_L,
						p1_L & nup,
						p2_L & nup,
						s
				);
				pts[pI] += ( z - ( pts[pI] & nup ) ) * nup;
			}
			moduleBase().setEdge(cylBlockI,Block::SWL_SEL,pts);

			// calc deltas H:
			deltaA = p1_H - p0_H;
			deltaB = (p1_H - p2_H) * mag(deltaA) / mag(p1_H - p2_H);

			// st spline H:
			forAll(pts,pI){
				scalar s = scalar(1 + pI) / scalar(pts.size() + 2);
				//scalar z = p2_H & nup;
				pts[pI]  = p1_H + s * ( p2_H - p1_H );
				//pts[pI] += ( z - ( pts[pI] & nup ) ) * nup;
			}
			moduleBase().setEdge(cylBlockI,Block::SWH_SEH,pts);
		}

		// outer arc spline L:
		Foam::vector n = (p2_L - cylinderCentre_)/ mag(p2_L - cylinderCentre_);
		pointField ptsArc(cylBlock.blockCells()[TerrainManager::BASE2] - 1);
		scalar deltaAngle = getAngleRad
							(
									n,
									points[moduleBase().blockVertex(cylBlockI,Block::NEL)] - cylinderCentre_
							) / (ptsArc.size() + 2);
		forAll(ptsArc,pI){
			moduleBase().coordinateSystem().rotate
			(
					n,
					deltaAngle,
					moduleBase().coordinateSystem().e(TerrainManager::UP)
			);
			ptsArc[pI] = cylinderCentre_ + cylinderRadius_ * n;
		}
		moduleBase().setEdge(cylBlockI,Block::SEL_NEL,ptsArc);

		// outer arc spline H:
		n = (p2_L - cylinderCentre_)/ mag(p2_L - cylinderCentre_);
		forAll(ptsArc,pI){
			moduleBase().coordinateSystem().rotate(n,deltaAngle,moduleBase().coordinateSystem().e(TerrainManager::UP));
			ptsArc[pI]  = cylinderCentre_ + cylinderRadius_ * n;
			ptsArc[pI] += ( (p2_H - ptsArc[pI]) & moduleBase().coordinateSystem().e(TerrainManager::UP) ) * moduleBase().coordinateSystem().e(TerrainManager::UP);
		}
		moduleBase().setEdge(cylBlockI,Block::SEH_NEH,ptsArc);
	}

	// south:
	for(label i = 0; i <= moduleBase().blockNrs_[TerrainManager::BASE1]; i++){

		// get point labels:
		label i2 = (i == moduleBase().blockNrs_[TerrainManager::BASE1]) ? i - 1 : i;
		v[Block::NWL] = moduleBase().pointAdr_ijv_[key(i2,0,Block::SWL)];
		v[Block::NWH] = moduleBase().pointAdr_ijv_[key(i2,0,Block::SWH)];
		if(i != i2){
			v[Block::NWL] = moduleBase().pointAdr_ijv_[key(i2,0,Block::SEL)];
			v[Block::NWH] = moduleBase().pointAdr_ijv_[key(i2,0,Block::SEH)];
		}

		// grab blocks:
		word key_cylBlock      = key(Block::SOUTH,i2,0);
		label cylBlockI        = cylinderBlockAdr_[key_cylBlock];
		label terBlockI        = moduleBase().blockAdr_ij_[key(i2,0)];
		const Block & cylBlock = moduleBase().blocks()[cylBlockI];

		// get points:
		point p0_L         = points[moduleBase().blockVertex(terBlockI,i == i2 ? Block::NWL : Block::NEL)];
		point p0_H         = points[moduleBase().blockVertex(terBlockI,i == i2 ? Block::NWH : Block::NEH)];
		const point & p1_L = points[v[Block::NWL]];
		const point & p1_H = points[v[Block::NWH]];
		const point & p2_L = points[moduleBase().blockVertex(cylBlockI,i == i2 ? Block::SWL : Block::SEL)];
		const point & p2_H = points[moduleBase().blockVertex(cylBlockI,i == i2 ? Block::SWH : Block::SEH)];

		if(i > 0){
			// calc deltas L:
			Foam::vector deltaA = p1_L - p0_L;
			if(i != i2){
				const point & p0B_L = points[moduleBase().blockVertex(terBlockI,Block::SWL)];
				Foam::vector deltaA2 = p1_L - p0B_L;
				deltaA = 0.5 * (mag(deltaA) + mag(deltaA2)) * 0.5 * (deltaA/mag(deltaA) + deltaA2/mag(deltaA2));
				p0_L   = p1_L - deltaA;
			}
			Foam::vector deltaB = (p1_L - p2_L) * mag(deltaA) / mag(p1_L - p2_L);

			// st spline L:
			pointField pts(cylinderRadialCells_ - 1);
			forAll(pts,pI){
				scalar s = scalar(1 + pI) / scalar(pts.size() + 2);
				pts[pI]  = p1_L + s * ( p2_L - p1_L );
				scalar z = blending_().blend
				(
						p1_L,
						p2_L,
						p1_L & nup,
						p2_L & nup,
						s
				);
				pts[pI] += ( z - ( pts[pI] & nup ) ) * nup;
			}
			moduleBase().setEdge(cylBlockI,i == i2 ? Block::NWL_SWL : Block::NEL_SEL,pts);

			// calc deltas H:
			deltaA = p1_H - p0_H;
			if(i != i2){
				const point & p0B_H = points[moduleBase().blockVertex(terBlockI,Block::SWH)];
				Foam::vector deltaA2 = p1_H - p0B_H;
				deltaA = 0.5 * (mag(deltaA) + mag(deltaA2)) * 0.5 * (deltaA/mag(deltaA) + deltaA2/mag(deltaA2));
				p0_H   = p1_H - deltaA;
			}
			deltaB = (p1_H - p2_H) * mag(deltaA) / mag(p1_H - p2_H);

			// st spline H:
			forAll(pts,pI){
				scalar s = scalar(1 + pI) / scalar(pts.size() + 2);
				//scalar z = p2_H & nup;
				pts[pI]  = p1_H + s * ( p2_H - p1_H );
				//pts[pI] += ( z - ( pts[pI] & nup ) ) * nup;
			}
			moduleBase().setEdge(cylBlockI,i == i2 ? Block::NWH_SWH : Block::NEH_SEH,pts);
		}

		// outer arc spline L:
		if(i == i2){
			Foam::vector n = (p2_L - cylinderCentre_)/ mag(p2_L - cylinderCentre_);
			pointField ptsArc(cylBlock.blockCells()[TerrainManager::BASE2] - 1);
			scalar deltaAngle = getAngleRad
								(
										n,
										points[moduleBase().blockVertex(cylBlockI,Block::SEL)] - cylinderCentre_
								) / (ptsArc.size() + 2);
			forAll(ptsArc,pI){
				moduleBase().coordinateSystem().rotate(n,deltaAngle,moduleBase().coordinateSystem().e(TerrainManager::UP));
				ptsArc[pI] = cylinderCentre_ + cylinderRadius_ * n;
			}
			moduleBase().setEdge(cylBlockI,Block::SWL_SEL,ptsArc);
		}

		// outer arc spline H:
		if(i == i2){
			Foam::vector n = (p2_L - cylinderCentre_)/ mag(p2_L - cylinderCentre_);
			pointField ptsArc(cylBlock.blockCells()[TerrainManager::BASE2] - 1);
			scalar deltaAngle = getAngleRad
								(
										n,
										points[moduleBase().blockVertex(cylBlockI,Block::SEL)] - cylinderCentre_
								) / (ptsArc.size() + 2);
			forAll(ptsArc,pI){
				moduleBase().coordinateSystem().rotate(n,deltaAngle,moduleBase().coordinateSystem().e(TerrainManager::UP));
				ptsArc[pI]  = cylinderCentre_ + cylinderRadius_ * n;
				ptsArc[pI] += ( (p2_H - ptsArc[pI]) & moduleBase().coordinateSystem().e(TerrainManager::UP) ) * moduleBase().coordinateSystem().e(TerrainManager::UP);
			}
			moduleBase().setEdge(cylBlockI,Block::SWH_SEH,ptsArc);
		}
	}

	Info << "      added " << moduleBase().edgeCounter() - oldEdgeCounter
			<< " edges. Total edges = " << moduleBase().edgeCounter() << endl;

	return true;
}

} // End namespace oldev

} // End namespace Foam

