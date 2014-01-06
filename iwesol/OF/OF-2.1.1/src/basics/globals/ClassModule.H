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
    Foam::oldev::ClassModule

Description

SourceFiles
    ClassModuleI.H
    ClassModule.C
    ClassModuleIO.C

\*---------------------------------------------------------------------------*/

#ifndef ClassModule_H
#define ClassModule_H

#include "dictionary.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

namespace oldev
{


/*---------------------------------------------------------------------------*\
                         Class ClassModule Declaration
\*---------------------------------------------------------------------------*/

template<class Class>
class ClassModule
{
    // Private data

    	/// the underlying base
    	Class * moduleBase_;

    	/// flag is loading is complete
    	bool flag_ready_;


protected:

    // Member Functions

    	/// the underlying base
    	inline Class & moduleBase() { return *moduleBase_; }


public:


    // Constructors

        /// Construct from components
        ClassModule(Class * moduleBase);


    /// Destructor
    virtual ~ClassModule(){}


    // Member Functions

		/// ClassModule: load the module
		virtual bool load
		(
			const dictionary & dict
		) { flag_ready_ = true; return ready(); }

		/// checks if module is loaded
		inline bool ready() const { return moduleBase_ != 0 && flag_ready_; }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace oldev

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "ClassModuleI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
