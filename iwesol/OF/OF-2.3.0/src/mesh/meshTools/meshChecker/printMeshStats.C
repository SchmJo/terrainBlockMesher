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

#include "printMeshStats.H"
#include "polyMesh.H"
#include "globalMeshData.H"

#include "hexMatcher.H"
#include "wedgeMatcher.H"
#include "prismMatcher.H"
#include "pyrMatcher.H"
#include "tetWedgeMatcher.H"
#include "tetMatcher.H"

namespace Foam
{

namespace oldev
{

void printMeshStats(const polyMesh& mesh, const bool allTopology)
{
    Info<< "Mesh stats" << nl
        << "    points:           "
        << returnReduce(mesh.points().size(), sumOp<label>()) << nl;

    label nInternalPoints = returnReduce
    (
        mesh.nInternalPoints(),
        sumOp<label>()
    );

    if (nInternalPoints != -Pstream::nProcs())
    {
        Info<< "    internal points:  " << nInternalPoints << nl;

        if (returnReduce(mesh.nInternalPoints(), minOp<label>()) == -1)
        {
            WarningIn("printMeshStats(const polyMesh&, const bool)")
                << "Some processors have their points sorted into internal"
                << " and external and some do not." << endl
                << "This can cause problems later on." << endl;
        }
    }

    if (allTopology && nInternalPoints != -Pstream::nProcs())
    {
        label nEdges = returnReduce(mesh.nEdges(), sumOp<label>());
        label nInternalEdges = returnReduce
        (
            mesh.nInternalEdges(),
            sumOp<label>()
        );
        label nInternal1Edges = returnReduce
        (
            mesh.nInternal1Edges(),
            sumOp<label>()
        );
        label nInternal0Edges = returnReduce
        (
            mesh.nInternal0Edges(),
            sumOp<label>()
        );

        Info<< "    edges:            " << nEdges << nl
            << "    internal edges:   " << nInternalEdges << nl
            << "    internal edges using one boundary point:   "
            << nInternal1Edges-nInternal0Edges << nl
            << "    internal edges using two boundary points:  "
            << nInternalEdges-nInternal1Edges << nl;
    }

    Info<< "    faces:            "
        << returnReduce(mesh.faces().size(), sumOp<label>()) << nl
        << "    internal faces:   "
        << returnReduce(mesh.faceNeighbour().size(), sumOp<label>()) << nl
        << "    cells:            "
        << returnReduce(mesh.cells().size(), sumOp<label>()) << nl
        << "    boundary patches: "
        << mesh.boundaryMesh().size() << nl
        << "    point zones:      "
        << mesh.pointZones().size() << nl
        << "    face zones:       "
        << mesh.faceZones().size() << nl
        << "    cell zones:       "
        << mesh.cellZones().size() << nl
        << endl;


    // Construct shape recognizers
    hexMatcher hex;
    prismMatcher prism;
    wedgeMatcher wedge;
    pyrMatcher pyr;
    tetWedgeMatcher tetWedge;
    tetMatcher tet;

    // Counters for different cell types
    label nHex = 0;
    label nWedge = 0;
    label nPrism = 0;
    label nPyr = 0;
    label nTet = 0;
    label nTetWedge = 0;
    label nUnknown = 0;

    for (label cellI = 0; cellI < mesh.nCells(); cellI++)
    {
        if (hex.isA(mesh, cellI))
        {
            nHex++;
        }
        else if (tet.isA(mesh, cellI))
        {
            nTet++;
        }
        else if (pyr.isA(mesh, cellI))
        {
            nPyr++;
        }
        else if (prism.isA(mesh, cellI))
        {
            nPrism++;
        }
        else if (wedge.isA(mesh, cellI))
        {
            nWedge++;
        }
        else if (tetWedge.isA(mesh, cellI))
        {
            nTetWedge++;
        }
        else
        {
            nUnknown++;
        }
    }

    reduce(nHex,sumOp<label>());
    reduce(nPrism,sumOp<label>());
    reduce(nWedge,sumOp<label>());
    reduce(nPyr,sumOp<label>());
    reduce(nTetWedge,sumOp<label>());
    reduce(nTet,sumOp<label>());
    reduce(nUnknown,sumOp<label>());

    Info<< "Overall number of cells of each type:" << nl
        << "    hexahedra:     " << nHex << nl
        << "    prisms:        " << nPrism << nl
        << "    wedges:        " << nWedge << nl
        << "    pyramids:      " << nPyr << nl
        << "    tet wedges:    " << nTetWedge << nl
        << "    tetrahedra:    " << nTet << nl
        << "    polyhedra:     " << nUnknown
        << nl << endl;
}

} // End namespace oldev

} // End namespace Foam
