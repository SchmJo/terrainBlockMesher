# Default target
OF2.1.1: \
	lolCppIO \
	lolCppData \
	lolCppPlot \
	lolBasics-2.1.1 \
	lolFunctions-2.1.1 \
	lolBlockMesh-2.1.1 \
	lolMeshTools-2.1.1 \
	lolGeometry-2.1.1 \
	terrainBlockMesher-2.1.1 

OF2.3.0: \
	lolCppIO \
	lolCppData \
	lolCppPlot \
	lolBasics-2.3.0 \
	lolFunctions-2.3.0 \
	lolBlockMesh-2.3.0 \
	lolMeshTools-2.3.0 \
	lolGeometry-2.3.0 \
	terrainBlockMesher-2.3.0 

OF2.3.1: \
	lolCppIO \
	lolCppData \
	lolCppPlot \
	lolBasics-2.3.1 \
	lolFunctions-2.3.1 \
	lolBlockMesh-2.3.1 \
	lolMeshTools-2.3.1 \
	lolGeometry-2.3.1 \
	terrainBlockMesher-2.3.1 

#############################################
#
# doxygen documentation:
#
#############################################

doxygen:
	@cd doc/doxygen; ./Allwmake; cd ..; ln -sf doxygen/html/index.html iwesol.html

cleanDoxygen: 
	@rm -rf doc/doxygen/html doc/doxygen/DTAGS doc/iwesol.html


#############################################
#
# nonOF/c++ libraries:
#
#############################################

# make target:
lolCppIO:
	@cd nonOF/c++/src/io; make -j ; cd $(OLDEV)

# clean target:
cleanLolCppIO:
	@cd nonOF/c++/src/io; make clean; cd $(OLDEV)

# make target:
lolCppData: lolCppIO
	@cd nonOF/c++/src/data; make -j ; cd $(OLDEV)

# clean target:
cleanLolCppData:
	@cd nonOF/c++/src/data; make clean; cd $(OLDEV)

# make target:
lolCppPlot: lolCppData 
	@cd nonOF/c++/src/plot; make -j ; cd $(OLDEV)

# clean target:
cleanLolCppPlot: 
	@cd nonOF/c++/src/plot; make clean; cd $(OLDEV)


#############################################
#
# OF/OF-2.1.1 libraries:
#
#############################################

# make target:
lolBasics-2.1.1: lolCppData
	@wmake libso OF/OF-2.1.1/src/basics

# clean target:
cleanLolBasics-2.1.1:
	@wclean OF/OF-2.1.1/src/basics

# make target:
lolFunctions-2.1.1: lolCppPlot lolBasics-2.1.1
	@wmake libso OF/OF-2.1.1/src/functions

# clean target:
cleanLolFunctions-2.1.1:
	@wclean OF/OF-2.1.1/src/functions

# make target:
lolBlockMesh-2.1.1: lolFunctions-2.1.1 
	@wmake libso OF/OF-2.1.1/src/mesh/blockMesh

# clean target:
cleanLolBlockMesh-2.1.1:
	@wclean OF/OF-2.1.1/src/mesh/blockMesh

# make target:
lolGeometry-2.1.1: lolFunctions-2.1.1 
	@wmake libso OF/OF-2.1.1/src/geometry

# clean target:
cleanLolGeometry-2.1.1:
	@wclean OF/OF-2.1.1/src/geometry
	
# make target:
lolMeshTools-2.1.1: lolGeometry-2.1.1
	@wmake libso OF/OF-2.1.1/src/mesh/meshTools

# clean target:
cleanLolMeshTools-2.1.1:
	@wclean OF/OF-2.1.1/src/mesh/meshTools
	


#############################################
#
# OF/OF-2.1.1 applications:
#
#############################################

# make target:
terrainBlockMesher-2.1.1: lolBlockMesh-2.1.1 lolMeshTools-2.1.1 
	@wmake OF/OF-2.1.1/applications/utilities/mesh/terrainBlockMesher

# clean target:
cleanTerrainBlockMesher-2.1.1:
	@wclean OF/OF-2.1.1/applications/utilities/mesh/terrainBlockMesher


#############################################
#
# OF/OF-2.3.0 libraries:
#
#############################################

# make target:
lolBasics-2.3.0: lolCppData
	@wmake libso OF/OF-2.3.0/src/basics

# clean target:
cleanLolBasics-2.3.0:
	@wclean OF/OF-2.3.0/src/basics

# make target:
lolFunctions-2.3.0: lolCppPlot lolBasics-2.3.0
	@wmake libso OF/OF-2.3.0/src/functions

# clean target:
cleanLolFunctions-2.3.0:
	@wclean OF/OF-2.3.0/src/functions

# make target:
lolBlockMesh-2.3.0: lolFunctions-2.3.0 
	@wmake libso OF/OF-2.3.0/src/mesh/blockMesh

# clean target:
cleanLolBlockMesh-2.3.0:
	@wclean OF/OF-2.3.0/src/mesh/blockMesh

# make target:
lolGeometry-2.3.0: lolFunctions-2.3.0 
	@wmake libso OF/OF-2.3.0/src/geometry

# clean target:
cleanLolGeometry-2.3.0:
	@wclean OF/OF-2.3.0/src/geometry
	
# make target:
lolMeshTools-2.3.0: lolGeometry-2.3.0
	@wmake libso OF/OF-2.3.0/src/mesh/meshTools

# clean target:
cleanLolMeshTools-2.3.0:
	@wclean OF/OF-2.3.0/src/mesh/meshTools
	


#############################################
#
# OF/OF-2.3.0 applications:
#
#############################################

# make target:
terrainBlockMesher-2.3.0: lolBlockMesh-2.3.0 lolMeshTools-2.3.0 
	@wmake OF/OF-2.3.0/applications/utilities/mesh/terrainBlockMesher

# clean target:
cleanTerrainBlockMesher-2.3.0:
	@wclean OF/OF-2.3.0/applications/utilities/mesh/terrainBlockMesher


#############################################
#
# OF/OF-2.3.1 libraries:
#
#############################################

# make target:
lolBasics-2.3.1: lolCppData
	@wmake libso OF/OF-2.3.1/src/basics

# clean target:
cleanLolBasics-2.3.1:
	@wclean OF/OF-2.3.1/src/basics

# make target:
lolFunctions-2.3.1: lolCppPlot lolBasics-2.3.1
	@wmake libso OF/OF-2.3.1/src/functions

# clean target:
cleanLolFunctions-2.3.1:
	@wclean OF/OF-2.3.1/src/functions

# make target:
lolBlockMesh-2.3.1: lolFunctions-2.3.1 
	@wmake libso OF/OF-2.3.1/src/mesh/blockMesh

# clean target:
cleanLolBlockMesh-2.3.1:
	@wclean OF/OF-2.3.1/src/mesh/blockMesh

# make target:
lolGeometry-2.3.1: lolFunctions-2.3.1 
	@wmake libso OF/OF-2.3.1/src/geometry

# clean target:
cleanLolGeometry-2.3.1:
	@wclean OF/OF-2.3.1/src/geometry
	
# make target:
lolMeshTools-2.3.1: lolGeometry-2.3.1
	@wmake libso OF/OF-2.3.1/src/mesh/meshTools

# clean target:
cleanLolMeshTools-2.3.1:
	@wclean OF/OF-2.3.1/src/mesh/meshTools
	


#############################################
#
# OF/OF-2.3.1 applications:
#
#############################################

# make target:
terrainBlockMesher-2.3.1: lolBlockMesh-2.3.1 lolMeshTools-2.3.1 
	@wmake OF/OF-2.3.1/applications/utilities/mesh/terrainBlockMesher

# clean target:
cleanTerrainBlockMesher-2.3.1:
	@wclean OF/OF-2.3.1/applications/utilities/mesh/terrainBlockMesher


#############################################
#
# clean all:
#
#############################################

# Remove everything we created (up to doxygen):
clean: \
	cleanLolCppIO \
	cleanLolCppData \
	cleanLolCppPlot \
	cleanLolBasics-2.1.1 \
	cleanLolFunctions-2.1.1 \
	cleanLolBlockMesh-2.1.1 \
	cleanLolMeshTools-2.1.1 \
	cleanLolGeometry-2.1.1 \
	cleanTerrainBlockMesher-2.1.1  \
	cleanLolBasics-2.3.0 \
	cleanLolFunctions-2.3.0 \
	cleanLolBlockMesh-2.3.0 \
	cleanLolMeshTools-2.3.0 \
	cleanLolGeometry-2.3.0 \
	cleanTerrainBlockMesher-2.3.0  \
	cleanLolBasics-2.3.1 \
	cleanLolFunctions-2.3.1 \
	cleanLolBlockMesh-2.3.1 \
	cleanLolMeshTools-2.3.1 \
	cleanLolGeometry-2.3.1 \
	cleanTerrainBlockMesher-2.3.1 
