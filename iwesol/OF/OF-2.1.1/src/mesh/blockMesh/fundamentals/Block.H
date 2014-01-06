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
    Foam::oldev::Block

Description
	test test test

SourceFiles
    Block.C

\*---------------------------------------------------------------------------*/

#ifndef Block_H
#define Block_H

#include "BlockConventions.H"
#include "block.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward declaration of classes
class Ostream;

namespace oldev
{

// Forward declaration of friend functions and operators
class Block;
Ostream& operator<<(Ostream&, const Block&);


/*---------------------------------------------------------------------------*\
                         Class Block Declaration
\*---------------------------------------------------------------------------*/

class Block
:
    public BlockConventions,
    public block
{
    // Private data

    	/// the grading factors
    	scalarList gradingFactors_;

    	/// the block faces
    	faceList faces_;


    // Private Member Functions

        /// Disallow default bitwise copy construct
        Block(const Block&);

        /// Disallow default bitwise assignment
        void operator=(const Block&);


public:

    // Constructors

    	/// Constructor
    	Block
    	(
    			const pointField & globalPoints,
    			const labelList & pointsI,
    			const curvedEdgeList & globalEdges,
    			const Vector<label> & blockCells,
    			const scalarList & gradingFactors = scalarList(3,scalar(1)),
                const word& zoneName = ""
    	);

    	/// Constructor
    	Block
    	(
    			const pointField & globalPoints,
    			const label & p_SWL,const label & p_SWH,
    			const label & p_NWL,const label & p_NWH,
    			const label & p_SEL,const label & p_SEH,
    			const label & p_NEL,const label & p_NEH,
    			const curvedEdgeList & globalEdges,
    			const Vector<label> & blockCells,
    			const scalarList & gradingFactors = scalarList(3,scalar(1)),
                const word& zoneName = ""
    	);


    // Member Functions

        // Access

    	/// returns grading factors
    	inline const scalarList & gradingFactors() const { return gradingFactors_; }

    	/// returns vertex labels
    	inline const labelList & vertexLabels() const { return blockShape(); }

    	/// returns cells
    	inline const Vector< label > & blockCells() const { return meshDensity(); }

    	/// Returns faces
    	inline const faceList & faces() const { return faces_; }


    // IOstream Operators

        friend Ostream& operator<<(Ostream&, const Block&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace oldev

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
