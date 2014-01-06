# the compiler:
CXX = g++

# define paths:
LIBOBJPATH = $(OLDEV_CPP_PLOT)/obj
LIBSRCPATH = $(OLDEV_CPP_PLOT)/src
LIBDEPPATH = $(OLDEV_CPP_PLOT)/dep

# the includes:
INCS = -I$(LIBSRCPATH) -I$(OLDEV_CPP_IO)/src -I$(OLDEV_CPP_DATA)/src

# the linker flags:
LIBS = -L$(OLDEV_CPP_LIB) -lolCppIO -lolCppData

# the compiler options:
CXXOPTS = -ansi -Wall -Wextra -Werror -Wno-unused-parameter -fPIC 

# the sources:
SOURCES = 		\
		GnuplotFitInfo.cpp \
		Plot.cpp \
		PlotableObject.cpp \
		PlotWriter.cpp \
		LogPlotWriter.cpp \
		PlotTableWriter.cpp \
		PolarPlotWriter.cpp \
		PolarPlotPythonWriter.cpp\
		PlotHistogramWriter.cpp

# the library name:
LIBRARY = $(OLDEV_CPP_LIB)/libolCppPlot.so

# Gather the object files:
OBJECTS = $(addprefix $(LIBOBJPATH)/,$(SOURCES:.cpp=.o))

# Gather dependency files:
DEPS = $(addprefix $(LIBDEPPATH)/,$(SOURCES:.cpp=.dep))
	
# Collect options and includes: 	
CXXFLAGS = $(CXXOPTS) $(INCS)
			
# Default target
all: $(OBJECTS) $(LIBRARY)

# Compile each source file with automatic dependency update:
$(LIBOBJPATH)/%.o : $(LIBSRCPATH)/%.cpp
	@echo "\nCompiling $*.cpp:"
	$(CXX) -MMD $(CXXFLAGS) -c $< -o $@ 
	@cp $(LIBOBJPATH)/$*.d $(LIBDEPPATH)/$*.dep; \
		sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < $(LIBOBJPATH)/$*.d >> $(LIBDEPPATH)/$*.dep; \
	rm -f $(LIBOBJPATH)/$*.d
	
-include $(DEPS)

# Link all object files to the dynamic library:
$(LIBRARY): $(OBJECTS)
	@echo "\nLinking library $@:"
	$(CXX) -shared -o $@ $(LIBS) $(OBJECTS) 

# Remove everything we created:
clean:
	@rm -f $(DEPS) $(OBJECTS) $(LIBRARY)
