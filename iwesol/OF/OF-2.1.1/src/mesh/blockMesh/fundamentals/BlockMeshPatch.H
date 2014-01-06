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

Class
    Foam::oldev::BlockMeshPatch

Description

SourceFiles
    BlockMeshPatch.C

\*---------------------------------------------------------------------------*/

#ifndef BlockMeshPatch_H
#define BlockMeshPatch_H

#include "word.H"
#include "HashTable.H"
#include "labelList.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward declaration of classes
class Ostream;

namespace oldev
{

// Forward declaration of friend functions and operators
class BlockMeshPatch;
class BlockMeshCreator;
Ostream& operator<<(Ostream&, const BlockMeshPatch&);


/*---------------------------------------------------------------------------*\
                         Class BlockMeshPatch Declaration
\*---------------------------------------------------------------------------*/

class BlockMeshPatch
{
    // Private data

    	/// the underlying block mesh creator
    	const BlockMeshCreator & bmc_;

    	/// the name
    	word name_;

    	/// the type
    	word type_;

    	/// the block face memory
    	HashTable< labelList, label > blockFaceMem_;


    // Private Member Functions

        /// Disallow default bitwise copy construct
        BlockMeshPatch(const BlockMeshPatch&);

        /// Disallow default bitwise assignment
        void operator=(const BlockMeshPatch&);


public:


    // Constructors

        /// Construct from components
        BlockMeshPatch
        (
    			const BlockMeshCreator & bmc,
    			const word & name,
    			const word & type
        );


    // Member Functions

        // Access

    	/// returns the name
    	inline const word & name() const { return name_; }

    	/// returns the type
    	inline const word & type() const { return type_; }

    	/// return block face ID labels
    	inline const HashTable< labelList, label > & blockFaceID() const { return blockFaceMem_; }


        // Check

    	/// checks if block contributes
    	inline bool blockContributes(label blockI) const { return blockFaceMem_.found(blockI); }


        // Edit

    	/// add a face
    	void addFace(label blockI, label faceID);


    // IOstream Operators

        friend Ostream& operator<<(Ostream&, const BlockMeshPatch&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace oldev

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
