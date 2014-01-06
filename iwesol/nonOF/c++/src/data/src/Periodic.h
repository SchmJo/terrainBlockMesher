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
License
    This file is part of IWESOL.

    IWESOL is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    IWESOL is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with IWESOL.  If not, see <http://www.gnu.org/licenses/>.

Class

Description

SourceFiles

References

\*---------------------------------------------------------------------------*/

#ifndef PERIODIC_H_
#define PERIODIC_H_

#include "BLIB.h"

namespace jlib{

/**
 * @class Periodic
 * @brief Interface for classes with periodic data (switchable flag)
 */
class Periodic{

public:

	/// Constructor
	Periodic();

	/// Destructor
	virtual ~Periodic(){}

	/// Periodic: returns first point
	virtual const double & firstPoint() const = 0;

	/// Periodic: returns last point
	virtual const double & lastPoint() const = 0;

	/// set periodicity. less 0 is not periodic.
	inline void setPeriodic(const double & periodicStep = 0);

	/// unsets periodic flag
	inline void setNotPeriodic();

	/// returns periodic step
	inline const double & periodicStep() const { return m_periodicStep; }

	/// checks if periodic
	inline bool periodic() const { return m_periodicStep >= 0; }

	/// returns the period
	inline const double & period() const { return m_period; }

	/// applies modulo operation, if periodic
	inline double modx(const double & x) const;

	/// updates the period
	inline void update();


private:

	/// periodic distance,less than 0 if not periodic
	double m_periodicStep;

	/// the total period
	double m_period;

};

inline Periodic::Periodic():
		m_periodicStep(-1){
}

inline void Periodic::setPeriodic(const double & periodicStep) {
	m_periodicStep = periodicStep;
	update();
}

inline void Periodic::setNotPeriodic() {
	m_periodicStep = -1;
	update();
}

inline void Periodic::update(){
	m_period = lastPoint() - firstPoint();
	if(periodic()) m_period += m_periodicStep;
}

inline double Periodic::modx(const double & x) const{
	if(!periodic()) return x;
	return firstPoint() + BLIB::mod(x - firstPoint(),m_period);
}

}

#endif /* PERIODIC_H_ */
