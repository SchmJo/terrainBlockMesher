/**
 * @file GeometryGlobals.H
 * @brief Definition/Implementation of class GeometryGlobals.
 *
 */

#ifndef GEOMETRYGLOBALS_H_
#define GEOMETRYGLOBALS_H_

#include "fvMesh.H"
#include "surfZone.H"

namespace Foam
{
namespace oldev
{

/// write an stl file
void writeSTL
(
		const Foam::fileName & file,
		const pointField & points,
		const faceList & faces,
		const List< surfZone > & zones = List< surfZone >(),
		bool binary = false
);


}
}

#endif /* GEOMETRYGLOBALS_H_ */
