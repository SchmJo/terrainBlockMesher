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
    blib::EuclideanMetric

Description
    See below

SourceFiles
    EuclideanMetric.C

References
	[1] J. Schmidt, C. Peralta, B. Stoevesandt, "Automated Generation of
	    Structured Meshes for Wind Energy Applications", Proceedings of the
	    Open Source CFD International Conference, 2012, London, UK

\*---------------------------------------------------------------------------*/


#ifndef EUCLIDEANMETRIC_H_
#define EUCLIDEANMETRIC_H_

#include <cmath>

#include "Metric.h"

namespace blib{

/**
 * @class blib::EuclideanMetric
 * @brief The Euclidean metric.
 *
 */
class EuclideanMetric:
	public Metric {

public:

	/// the distance
	inline static double distance(const DoublePoint & x, const DoublePoint & y);

	/// Constructor.
	EuclideanMetric();

	/// Metric: Metric compute distances between two points.
	inline double dist(const Metric::DoublePoint & x, const Metric::DoublePoint & y) const{
		return distance(x.v(),y.v());
	}

};

inline double EuclideanMetric::distance(
		const DoublePoint & x,
		const DoublePoint & y
		){
	return std::sqrt(
			(x.x - y.x) * (x.x - y.x)
			+ (x.y - y.y) * (x.y - y.y)
			+ (x.z - y.z) * (x.z - y.z)
			);
}

}


#endif /* EUCLIDEANMETRIC_H_ */
