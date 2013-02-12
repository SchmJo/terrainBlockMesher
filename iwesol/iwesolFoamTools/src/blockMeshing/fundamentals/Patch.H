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
    Foam::iwesol::Patch

Description
    See below.

SourceFiles
    Patch.C

References
	[1] J. Schmidt, C. Peralta, B. Stoevesandt, "Automated Generation of
	    Structured Meshes for Wind Energy Applications", Proceedings of the
	    Open Source CFD International Conference, 2012, London, UK

\*---------------------------------------------------------------------------*/

#ifndef PATCH_H_
#define PATCH_H_

#include "BasicBlock.H"

namespace Foam{
namespace iwesol{

/**
 * @class Foam::iwesol::Patch
 * @brief A patch is a list of faces.
 *
 */
class Patch {

public:

	/// For each patch face, remember the block and its side
	struct PatchFace{
		BasicBlock const * block;
		label blockFaceLabel;
		word blockKey; // typically key(patchNr,i,j)
	};

	/// Constructor.
	Patch(const word & name = "PATCH", const word & type = "patch");

	/// Destructor.
	virtual ~Patch();

	/// Returns the name
	inline const word & getName() const { return name; }

	/// Returns the type
	inline const word & getType() const { return type; }

	/// Returns the cyclic partner, if applicable
	inline const word & getCyclicPartner() const { return cyclicPartner; }

	/// Returns true if patch is cyclic
	inline bool isCyclic() const { return type.compare("cyclic") == 0; }

	/// Sets patch cyclic
	inline void setCyclic(const word & partnerPatch) { type = "cyclic"; cyclicPartner = partnerPatch; }

	/// Returns the number of faces
	inline label size() const { return pfaces.size(); }

	/// Adds a face to the patch
	void addPatch(BasicBlock const * block, label blockFaceLabel,const word & blockKey = "");

	/// Returns the dict entry for a face
	std::string dictEntry() const;

	/// Returns the list of face data
	inline const Foam::List<PatchFace> & getFacesData() const { return pfaces; }

	/// Returns face data, given the key
	inline const PatchFace & getFace(const word & key) const { return pfaces[faceIndices[key]]; }

	/// Checks if key exists
	inline bool found(const word key) const { return faceIndices.found(key); }


private:

	/// The name
	word name;

	/// The type
	word type;

	/// The cyclic partner, if applicable
	word cyclicPartner;

	/// List of faces
	Foam::List<PatchFace> pfaces;

	/// Index memory. key as in PatchFace
	HashTable<label> faceIndices;

};

} /* iwesol */
} /* Foam */

#endif /* Patch_H_ */
