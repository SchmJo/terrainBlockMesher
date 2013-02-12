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
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Class
    blib::Metric

Description
    See below

SourceFiles
    Metric.cpp

References
	[1] J. Schmidt, C. Peralta, B. Stoevesandt, "Automated Generation of
	    Structured Meshes for Wind Energy Applications", Proceedings of the
	    Open Source CFD International Conference, 2012, London, UK

\*---------------------------------------------------------------------------*/


#ifndef METRIC_H_
#define METRIC_H_

#include <vector>
#include <string>

#include "Point3D.h"

namespace blib{

/**
 * @class blib::Metric
 * @brief Abstract base class for metrics.
 *
 */
class Metric {

public:

	/// Definition of a point:
	typedef Point3D DoublePoint;

	// Definition of a cell:
	typedef std::vector<DoublePoint> DoubleCell;

	/// Constructor.
	Metric(const std::string & name = "");

	/// Destructor.
	virtual ~Metric();

	/// Metric: Metric compute distances between two points.
	virtual double dist(const DoublePoint & x, const DoublePoint & y) const = 0;

	/// Returns the name.
	inline std::string getName() const { return metricName; }


protected:

	/// Name of the metric.
	std::string metricName;
};

}

#endif /* METRIC_H_ */
