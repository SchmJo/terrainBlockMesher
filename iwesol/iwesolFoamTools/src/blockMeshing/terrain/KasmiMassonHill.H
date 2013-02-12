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
    Foam::iwesol::KasmiMassonHill

Description
    See below.

SourceFiles
    KasmiMassonHill.C

References
	[1] J. Schmidt, C. Peralta, B. Stoevesandt, "Automated Generation of
	    Structured Meshes for Wind Energy Applications", Proceedings of the
	    Open Source CFD International Conference, 2012, London, UK

\*---------------------------------------------------------------------------*/

#ifndef KASMIMASSONHILL_H_
#define KASMIMASSONHILL_H_

#include "GroundObject.H"
#include "Globals.H"

namespace Foam{
namespace iwesol{


/**
 * @class Foam::iwesol::KasmiMassonHill
 * @brief A hill based on the article DOI:10.1002/we.390, Wind Energ. 2010; 13:689-704 by A. El Kasmi and C. Masson
 *
 */
class KasmiMassonHill:
	public virtual GroundObject{

public:

	/// Constructor
	KasmiMassonHill(const point & p_center,
			const scalar & maxHeight,
			const scalar & length,
			const word & addType,
			const scalar & resolution = 0.001);

	/// Constructor
	KasmiMassonHill(const dictionary & dict);

	/// Constructor
	KasmiMassonHill(){}

	/// Destructor
	virtual ~KasmiMassonHill();

	/// GroundObject: Returns height at point (here this must me in the same plane, no projection)
	virtual scalar getHeight(const point & p) { return getHeight(mag(p - p_center)); }

	/// Returns the center coordinate
	const point & getCenter() const { return p_center; }

	/// Returns height at a distance from the center
	scalar getHeight(const scalar & dist);

	/// Calculates the complete hill in the given resolution
	virtual void calcAll();

	/// Returns the maximal height
	scalar getMaxHeight() const { return maxHeight; }

	/// Returns the aspect ratio
	scalar getAspectRatio() const { return aspectRatio; }

	/// Returns the length
	scalar getRadius() const { return a; }

	/// Returns the parameter m
	scalar getm() const { return m; }

	/// Returns x of xi
	scalar xofxi(const scalar & xi) const;

	/// Returns z of xi
	scalar zofxi(const scalar & xi) const;


protected:

	/// The center coordinate
	point p_center;

	/// The maximal height
	scalar maxHeight;

	/// The aspect ratio
	scalar aspectRatio;

	/// The parameter m
	scalar m;

	/// The length a
	scalar a;

	/// calculates the height list
	scalar calcHeight(
			const scalar & dist,
			const scalar & xi0,
			const scalar & xi1
			);

	/// The key
	word hkey(const scalar & d) const;


private:

	/// The height at a given xi value. key = hkey(dist)
	HashTable<scalar> heightTable;

	/// called by contructors
	void init();

};

inline scalar KasmiMassonHill::xofxi(const scalar & xi) const{

	return 0.5 * xi * (
			1. + a * a / ( xi * xi + m * m * ( a * a - xi * xi ) )
		);

}

inline scalar KasmiMassonHill::zofxi(const scalar & xi) const{

	return 0.5 * m * Foam::sqrt(a * a - xi * xi) * (
			1. - a * a / ( xi * xi + m * m * ( a * a - xi * xi ) )
		);

}

inline word KasmiMassonHill::hkey(const scalar & d) const{
	return blib::key(d,8,"X");
}

} /* iwesol */
} /* Foam */

#endif /* KASMIMASSONHILL_H_ */
