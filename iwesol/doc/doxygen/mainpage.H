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

\mainpage IWESOL: IWES Open Library, developed by Fraunhofer IWES at Oldenburg, Germany

\section about About IWESOL

IWESOL contains libraries and tools for C++ and OpenFOAM.
In the version available here it contains:

1) The application 'terrainBlockMesher'

    This tool for OpenFOAM generates structured meshes
    over complex terrain. The latter has to be provided
    in form of a STL data file. It features

      - the basic mesh generation (ie, the generation 
        of an extensive blockMeshDict file),

      - the creation of an outer box around the STL,

      MISSING: - the creation of a cylinder around the STL, with
        an arbitrary number of sector patches,

      - the possibility to add symmetric and oval hills
        to the orography,

      MISSING: - the option to make the mesh cyclic,

      - the addition of grading stripes in (x,y) directions.

    The workflow is as follows:

      - First set up the dictionary file in your case folder
        'system/terrainBlockMesherDict'. For doing so, copy
        and paste and modify the tutorials' examples, located at
        'iwesol/OF/OF-2.1.1/tutorials/utilities/mesh/terrainBlockMesher'.

      - run 'terrainBlockMesher'. This creates and writes the mesh.

For installation instructions, consult the file
'iwesol/README_INSTALL'.

Thank you for using IWESOL. Please consider this a beta version and
send comments, bugs, complaints to jonas.schmidt@iwes.fraunhofer.de .

Enjoy.

\*---------------------------------------------------------------------------*/
