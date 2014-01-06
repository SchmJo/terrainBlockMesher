/**
 * @file GeometryGlobals.C
 * @brief Definition/Implementation of class GeometryGlobals.
 *
 */

#include "GeometryGlobals.H"
#include "STLsurfaceFormat.H"

namespace Foam
{
namespace oldev
{

void writeSTL
(
		const Foam::fileName & file,
		const pointField & points,
		const faceList & faces,
		const List< surfZone > & zones,
		bool binary
){

	Info << "Writing file " << file << endl;
	if(binary){
		Foam::fileFormats::STLsurfaceFormat< face >::writeBinary(
				file,
				MeshedSurfaceProxy< face >
				(
						points,
						faces,
						zones
				)
			);
	} else {
		Foam::fileFormats::STLsurfaceFormat< face >::writeAscii(
				file,
				MeshedSurfaceProxy< face >
				(
						points,
						faces,
						zones
				)
			);
	}
}

}
}
