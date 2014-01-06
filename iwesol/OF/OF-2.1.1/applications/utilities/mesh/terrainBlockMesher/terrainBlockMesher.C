#include "TerrainManager.H"
#include "argList.H"
#include "Time.H"
#include "fvMesh.H"
#include "searchableSurfaces.H"
using namespace Foam;
using namespace oldev;


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


int main(int argc, char *argv[])
{

#   include "setRootCase.H"
#   include "createTime.H"

    // Read dictionary
    IOdictionary dict
    (
       IOobject
       (
            "terrainBlockMesherDict",
            runTime.system(),
            runTime,
            IOobject::MUST_READ_IF_MODIFIED,
            IOobject::NO_WRITE
       )
    );
    bool writeDict = readBool(dict.lookup("writeBlockMeshDict"));
    bool writeMesh = readBool(dict.lookup("writePolyMesh"));

    // Read geometry:
    autoPtr< searchableSurfaces > stlSurfaces;
    if(dict.found("stl")){
        Info << "Reading stl surface..." << endl;
        const dictionary& geometryDict = dict.subDict("stl");
        stlSurfaces.set
        (
        		new searchableSurfaces
        		(
        				IOobject
        				(
        						"abc",                      // dummy name
        						runTime.time().constant(),     // instance
        						//mesh.time().findInstance("triSurface", word::null),// instance
        						"triSurface",               // local
        						runTime.time(),                // registry
        						IOobject::MUST_READ,
        						IOobject::NO_WRITE
        				),
        				geometryDict
        		)
        );
        Info << "...done, after " << runTime.cpuTimeIncrement() << " s."<< endl;
    } else {
    	Info << "No entry 'stl' found in dictionary. Choosing empty landscape." << endl;
    }

    // Create TerrainManager:
    Info << "\nRunning TerrainManager" << endl;
    autoPtr< TerrainManager > bm;
    if(stlSurfaces.valid()){
        bm.set
        (
        		new TerrainManager
        		(
        				runTime,
        				dict.subDict("blockManager"),
        				&(stlSurfaces()[0])
        		)
        );
    } else {
        bm.set
        (
        		new TerrainManager
        		(
        				runTime,
        				dict.subDict("blockManager")
        		)
        );
    }

    if(!bm().calc()){
    	Info << "\nError during terrain setup." << endl;
      	throw;
    }
    //if(bmDict.found("check")) bm().check();
    Info << "TerrainManager finished, after " << runTime.cpuTimeIncrement() << " s.\n"<< endl;

	// create mesh:
    if(writeMesh){
        autoPtr< polyMesh > mesh = bm().createPolyMesh(writeDict);
        mesh().removeFiles();
        Info << "\nwriting polyMesh" << endl;
        mesh().write();
    } else {
    	if(writeDict){
        	bm().createBlockMeshDict(writeDict);
    	}
    }

    Info<< "\nFinished in = "
        << runTime.elapsedCpuTime() << " s." << endl;
    Info<< "\nEnd\n" << endl;

    return 0;
}


// ************************************************************************* //
