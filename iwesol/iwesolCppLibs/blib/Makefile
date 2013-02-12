# the compiler:
CXX = g++

# define paths:
LIBOBJPATH = $(IWESOL_BLIB)/obj
LIBSRCPATH = $(IWESOL_BLIB)/src
LIBDEPPATH = $(IWESOL_BLIB)/dep

# the includes:
INCS = -I$(LIBSRCPATH)

# the linker flags:
LDFLAGS =

# the compiler options:
CXXOPTS = -ansi -Wall -Wextra -Werror -fPIC 

# the sources:
SOURCES = 	BLIB.cpp \
			IO.cpp \
			Outputable.cpp \
			String.cpp \
			OManager.cpp \
			FileLinked.cpp \
			Point2D.cpp \
			Point3D.cpp \
			Metric.cpp \
			EuclideanMetric.cpp \
			MultiIndexed.cpp \
			Grid.cpp \
			UnstructuredGrid1D.cpp \
			Ordered.cpp

# the library name:
LIBRARY = $(IWESOL_CPP_LIB)/libblib.so

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
