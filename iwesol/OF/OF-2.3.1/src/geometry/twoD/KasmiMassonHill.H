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
    Foam::oldev::KasmiMassonHill

Description

SourceFiles
    KasmiMassonHillI.H
    KasmiMassonHill.C
    KasmiMassonHillIO.C

\*---------------------------------------------------------------------------*/

#ifndef KasmiMassonHill_H
#define KasmiMassonHill_H

#include "BLIB.h"
#include "GroundObject.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

namespace oldev
{

/*---------------------------------------------------------------------------*\
                         Class KasmiMassonHill Declaration
\*---------------------------------------------------------------------------*/

class KasmiMassonHill
:
    public GroundObject
{
    // Private data

    	/// The height at a given xi value. key = hkey(dist)
    	mutable HashTable<scalar> heightTable_;


    // Private Member Functions

    	/// called by contructors
    	void init();


protected:

    // Protected data

    	/// The center coordinate
    	point p_center_;

    	/// The maximal height
    	scalar maxHeight_;

    	/// The aspect ratio
    	scalar aspectRatio_;

    	/// The parameter m
    	scalar m_;

    	/// The length a
    	scalar a_;


    // Protected Member Functions

    	/// calculates the height list
    	scalar calcHeight(scalar dist, scalar xi0, scalar xi1) const;

    	/// The key
    	word hkey(scalar d) const;


public:

    // The OF run time selection type name
    TypeName("KasmiMassonHill");


    // Constructors

    	/// Constructor
    	KasmiMassonHill
    	(
    			const point & p_center,
    			scalar maxHeight,
    			scalar length,
    			const word & addType,
    			scalar resolution = 0.001
    	);

    	/// Constructor
    	KasmiMassonHill(const dictionary & dict);

    	/// Constructor
    	KasmiMassonHill(){}


    /// Destructor
    virtual ~KasmiMassonHill();


    // Member Functions

        // Access

    	/// GroundObject: Returns height at point (here this must me in the same plane, no projection)
    	virtual scalar height(const point & p) const { return height(mag(p - p_center_)); }

    	/// Returns the center coordinate
    	inline const point & center() const { return p_center_; }

    	/// Returns height at a distance from the center
    	scalar height(scalar dist) const;

    	/// Calculates the complete hill in the given resolution
    	virtual void calcAll();

    	/// Returns the maximal height
    	inline scalar maxHeight() const { return maxHeight_; }

    	/// Returns the aspect ratio
    	inline scalar aspectRatio() const { return aspectRatio_; }

    	/// Returns the length
    	inline scalar radius() const { return a_; }

    	/// Returns the parameter m
    	inline scalar m() const { return m_; }

    	/// Returns x of xi
    	scalar xofxi(scalar xi) const;

    	/// Returns z of xi
    	scalar zofxi(scalar xi) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace oldev

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "KasmiMassonHillI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
