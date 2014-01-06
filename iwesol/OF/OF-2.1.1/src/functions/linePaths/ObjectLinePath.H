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
    Foam::oldev::ObjectLinePath

Description

SourceFiles
    ObjectLinePathI.H

\*---------------------------------------------------------------------------*/

#ifndef ObjectLinePath_H
#define ObjectLinePath_H

#include "List.H"
#include "dictionary.H"
#include "Globals.H"
#include "GenericLinePath.h"
#include "Distance.h"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

namespace oldev
{


/*---------------------------------------------------------------------------*\
                         Class ObjectLinePath Declaration
\*---------------------------------------------------------------------------*/

template<class Data, class DataDist = jlib::Distance< Data > >
class ObjectLinePath
:
    public jlib::GenericLinePath< Data, DataDist >
{

public:


    // Constructors

    	/// Constructor
    	ObjectLinePath
    	(
    			const std::string & name = "",
    			int interpolOrder = 4
    	);

    	/// Constructor
    	ObjectLinePath
    	(
    			const Data & p0,
    			const Data & p1,
    			const std::string & name = "",
    			int interpolOrder = 4
    	);

    	/// Constructor
    	ObjectLinePath
    	(
    			const Data & p,
    			const std::string & name = "",
    			int interpolOrder = 4
    	);

    	/// Constructor
    	ObjectLinePath
    	(
    			const List< Data > & points,
    			const std::string & name = "",
    			int interpolOrder = 4
    	);

    	/// Constructor
    	ObjectLinePath
    	(
    			const jlib::GenericRFunction< Data > & f
    	);

    	/// Constructor
    	ObjectLinePath
    	(
    			const dictionary & dict,
    			const word & keyWord = "points"
    	);


    // Member Functions

    	/// Returns all points
    	inline List< Data > getList() const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace oldev

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "ObjectLinePathI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
