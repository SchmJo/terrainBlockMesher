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
    Foam::iwesol::BasicBlock

Description
    See below.

SourceFiles
    BasicBlock.C
    vertexLabelData.C

References
	[1] J. Schmidt, C. Peralta, B. Stoevesandt, "Automated Generation of
	    Structured Meshes for Wind Energy Applications", Proceedings of the
	    Open Source CFD International Conference, 2012, London, UK

\*---------------------------------------------------------------------------*/

#ifndef BASICBLOCK_H_
#define BASICBLOCK_H_

#include "Outputable.h"
#include "pointField.H"

namespace Foam{
namespace iwesol{

/**
 * @class BasicBlock
 * @brief Basic block data.
 *
 */
class BasicBlock:
	public blib::Outputable{

public:

	/// define box corner labels: eg south-west-low = SWL
	static const label SWL, SWH, NWL, NWH, SEL, SEH, NEL, NEH;

	/// define x, y, z labels
	static const label X, Y, Z;

	/// define face labels
	static const label NONE, WEST, EAST, NORTH, SOUTH, GROUND, SKY;

	/// returns the constant direction of a face
	static label getConstantDirectionFace(label faceLabel);

	/// returns the opposite face
	static label getOppositeFace(label faceLabel);

	/// Constructor.
	BasicBlock();

	/// Constructor.
	BasicBlock(
			pointField* globalPoints,
			const labelList & verticesI,
			const labelList & cells,
			const std::string & gradingCommand = "simpleGrading",
			const scalarList & gradingFactors = scalarList(3,1.)
			);

	/// Constructor.
	BasicBlock(
			pointField* globalPoints,
			const label & p_SWL,const label & p_SWH,
			const label & p_NWL,const label & p_NWH,
			const label & p_SEL,const label & p_SEH,
			const label & p_NEL,const label & p_NEH,
			const label cells_x,
			const label cells_y,
			const label cells_z,
			const std::string & gradingCommand = "simpleGrading",
			const scalarList & gradingFactors = scalarList(3,1.)
			);

	/// Destructor.
	virtual ~BasicBlock();

	/// Simple checks
	virtual bool ok() const;

	/// BasicBlock: prints the points
	virtual void print() const;

	/// Outputable: Provides the data that will be written to the output file.
	virtual blib::Outputable::OData getOData(
			const std::string & type = blib::IO::OFILE::TYPE::EMPTY) const;

	/// Return boundary points.
	inline const labelList & getVertices() const { return verticesI; }

	/// Return boundary point
	inline label getVertexI(label i) const { return verticesI[i]; }

	/// Return global point reference
	inline point & getVertex(label i) const { return (*globalPoints)[verticesI[i]]; }

	/// Return vertex points
	pointField getVertexPoints() const;

	/// Global points pointer
	inline pointField* getGlobalPoints() const { return globalPoints; }

	/// Return the lowest coordinates
	inline const point & getLowest() const  { return lowCoo; }

	/// Return the highest coordinates
	inline const point & getHighest() const  { return highCoo; }

	/// Return the number of cells
	inline const labelList & getCells() const { return cells;}

	/// Return cell number in x or y or z
	inline label getCells(label i) const { return cells[i]; }

	/// Returns the dict entry hex for blockMeshDict/blocks
	std::string dictEntry_hex() const;

	/// Returns the dict entry spline for blockMeshDict/edges
	virtual std::string dictEntry_splines() const { return std::string(""); }

	/// Returns the dict entry boundary for blockMeshDict
	virtual std::string dictEntry_boundary() const;

	/// Returns the dict entry spline for blockMeshDict/edges
	virtual std::string dictEntry_mergePatchPairs() const;

	/// Returns the dict entry for a face
	std::string dictEntry_face(label i) const;

	/// Returns vertex indices of a face
	labelList getFaceI(label i) const;

	/// Get full grading command
	std::string getGradingCommand() const;

	/// returns neighbors
	inline const Foam::List<BasicBlock*> & getNeighborBlockI() const { return neighbors; }

	/// returns neighbor
	inline BasicBlock* getNeighbor(label i) const { return neighbors[i]; }

	/// returns neighbors
	inline const List<BasicBlock*> & getNeighbors() const { return neighbors; }

	/// returns if neighbor exists
	inline bool hasNeighbor(label face) const { return neighbors[face] == 0 ? false : true; }

	/// checks and sets neighbor. returns face index.
	label checkSetNeighbor(BasicBlock & block);

	/// checks if block is neighbor
	inline bool isNeighbor(const BasicBlock & block) const { return commonFace(block) == NONE ? false : true; }

	/// returns common face index
	label commonFace(const BasicBlock & block) const;

	/// returns line of neighbors, starting from this block, nmax < 0 means all
	template<class T>
	List<T*> getConnectedBlockLine(label face, label nmax = -1) const;

	/// returns line of neighbors, starting from this block, nmax < 0 means all
	template<class T>
	List<T*> getConnectedBlockLine(label face, label nmax = -1);

	/// returns Square of neighbors, starting from this block
	template<class T>
	List< List<T*> > getConnectedBlockSquare(label faceA,label faceB, label nmaxA = -1, label nmaxB = -1);


protected:

	/// The lowest coordinates
	point lowCoo;

	/// The highes coordinates
	point highCoo;

	/// The global point list
	pointField* globalPoints;

	/// the boundary points.
	labelList verticesI;

	/// the number of cells in x,y,z
	labelList cells;

	/// The grading command
	std::string gradingCommand;

	/// the grading factors
	scalarList gradingFactors;

	/// the neighbor list. i = WEST, EAST, NORTH, SOUTH, GROUND, SKY
	Foam::List<BasicBlock*> neighbors;


private:

	/// Called by constructors
	void __init();
};

inline bool BasicBlock::ok() const{
	if(verticesI.size() != 8
			|| cells.size() != 3) return false;
	return true;
}

template<class T>
List<T*> BasicBlock::getConnectedBlockLine(label face, label nmax){

	// prepare:
	List<T*> out(1);
	out[0] = dynamic_cast<T*>(this);
	if(nmax == 1) return out;

	if(hasNeighbor(face)){
		out.append(neighbors[face]->getConnectedBlockLine<T>(face, nmax - 1));
	}

	return out;
}

template<class T>
List<T*> BasicBlock::getConnectedBlockLine(label face, label nmax) const{

	// prepare:
	List<T*> out(1);
	out[0] = dynamic_cast<T*>(this);
	if(nmax == 1) return out;

	if(hasNeighbor(face)){
		out.append(neighbors[face]->getConnectedBlockLine<T>(face, nmax - 1));
	}

	return out;
}

template<class T>
List< List<T*> > BasicBlock::getConnectedBlockSquare(
		label faceA,label faceB, label nmaxA, label nmaxB){

	// prepare:
	List< List<T*> > out;
	List<T*> line0 = getConnectedBlockLine<T>(faceB,nmaxB);

	// case 0D:
	if(line0.size() == 1 && !hasNeighbor(faceA)){
		out.append(line0);
	}

	// case 1D:
	else if(line0.size() == 1){
		out.append(getConnectedBlockLine<T>(faceA));
	} else if(!hasNeighbor(faceA)){
		out.append(line0);
	}

	// else 2D:
	else {

		// prepare:
		out.append(line0);
		label nmax = line0.size();
		label h    = 1;
		if(nmaxB > 0 && nmax > nmaxB) nmax = nmaxB;

		// first fill pyramid:
		while(nmax > 0){

			List<T*> line;

			// find nmax of next line:
			for(int i = 0; i < nmax; i++){
				if(out[h - 1][i]->hasNeighbor(faceA)){
					line.append(
							dynamic_cast<T*>(
									out[h - 1][i]->getNeighbor(faceA)
								)
							);
				} else {
					break;
				}
			}
			nmax = line.size();

			// store:
			if(nmax > 0){

				out.append(line);
				h++;
			}
		}

		// cut:
		forAll(out,oI){
			out[oI].resize(out.last().size());
		}

	}

	return out;
}

} /* iwesol */
} /* Foam */

#endif /* BASICBLOCK_H_ */
