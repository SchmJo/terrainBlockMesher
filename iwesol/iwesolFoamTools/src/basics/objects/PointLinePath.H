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
    Foam::iwesol::PointLinePath

Description
    See below.

SourceFiles
    PointLinePath.C

References
	[1] J. Schmidt, C. Peralta, B. Stoevesandt, "Automated Generation of
	    Structured Meshes for Wind Energy Applications", Proceedings of the
	    Open Source CFD International Conference, 2012, London, UK

\*---------------------------------------------------------------------------*/

#ifndef POINTLINEPATH_H_
#define POINTLINEPATH_H_

#include "pointField.H"

#include "ObjectLinePath.H"
//#include "Globals.H"
//#include "jlibAdaptor.H"

namespace Foam{
namespace iwesol{


/**
 * @class Foam::iwesol::PointLinePath
 * @brief A wrapper class for blib::ObjectLinePath<Foam::point>.
 *
 */
class PointLinePath:
	public ObjectLinePath<point>{

public:

	/// Constructor
	PointLinePath(const std::string & name = "PointLinePath");

	/// Constructor
	PointLinePath(
			const point & p0,
			const point & p1,
			const std::string & name = "PointLinePath"
			);

	/// Constructor
	PointLinePath(
			const point & p,
			const std::string & name = "PointLinePath"
			);

	/// Constructor
	PointLinePath(
			const pointField & points,
			const std::string & name = "PointLinePath"
			);

	// Constructor
	// removed, thanks to Philippe Gomme, fails for centos machines
	/*
	PointLinePath(
			const blib::GenericRFunction<point> & f
			);
	 */

	/// Constructor
	PointLinePath(
			const dictionary & dict,
			const word & keyWord = "points"
			);

	/// Destructor
	virtual ~PointLinePath();

	/// Outputable: Provides the data that will be written to the output file.
	virtual blib::Outputable::OData getOData(
			const std::string & type = blib::IO::OFILE::TYPE::EMPTY) const;

	/// Writes vtk files
	void writeVTK(
			const std::string & folderName = ""
			) const;

	/// Returns all points
	inline pointField getPoints() const { return pointField(getList()); }


	/// Returns all points
	//const List<point*>& getPointsList() const;

};

/// vector line path type:
typedef PointLinePath VectorLinePath;

} /* iwesol */
} /* Foam */




#endif /* POINTLINEPATH_H_ */
