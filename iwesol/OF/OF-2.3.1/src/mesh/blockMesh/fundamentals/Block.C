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

#include "Block.H"
#include "Ostream.H"
#include "cellModeller.H"

namespace Foam
{

namespace oldev
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Block::Block
(
		const pointField & globalPoints,
		const labelList & pointsI,
		const curvedEdgeList & globalEdges,
		const Vector<label> & blockCells,
		const scalarList & gradingFactors,
        const word& zoneName
):
		block
		(
				blockDescriptor
				(
			            cellShape
			            (
			            	*(cellModeller::lookup("hex")),
			            	mapToHexModel(pointsI),
			            	true
			            ),
			            globalPoints,
			            globalEdges,
			            blockCells,
			            gradingFactors2ExpandRatios(gradingFactors),
			            zoneName
				)
		),
		gradingFactors_(gradingFactors),
		faces_(6,face(4)){

	faces_ = blockShape().faces();
}

Block::Block
(
		const pointField & globalPoints,
		const label & p_SWL,const label & p_SWH,
		const label & p_NWL,const label & p_NWH,
		const label & p_SEL,const label & p_SEH,
		const label & p_NEL,const label & p_NEH,
		const curvedEdgeList & globalEdges,
		const Vector<label> & blockCells,
		const scalarList & gradingFactors,
        const word& zoneName
):
		block
		(
				blockDescriptor
				(
			            cellShape
			            (
			            	*(cellModeller::lookup("hex")),
			            	mapToHexModel
			            	(
			            			vertices2vertexList
			            			(
			            					p_SWL,p_SWH,
			            					p_NWL,p_NWH,
			            					p_SEL,p_SEH,
			            					p_NEL,p_NEH
			            			)
			            	),
			            	true
			            ),
			            globalPoints,
			            globalEdges,
			            blockCells,
			            gradingFactors2ExpandRatios(gradingFactors),
			            zoneName
				)
		),
		gradingFactors_(gradingFactors),
		faces_(6,face(4)){

	faces_ = blockShape().faces();
}

// * * * * * * * * * * * * * * Friend Operators * * * * * * * * * * * * * * //

Ostream & operator<<(Ostream & os, const Block & b){

	const blockDescriptor & bd = b;
	os << bd << endl;

	return os;
}

// ************************************************************************* //

} // End namespace oldev

} // End namespace Foam
