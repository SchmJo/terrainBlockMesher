/**
 * @file Periodic.h
 * @brief Definition/Implementation of class Periodic.
 *
 */

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
