# the compiler:
CXX = g++

# define paths:
LIBOBJPATH = $(OLDEV_CPP_DATA)/obj
LIBSRCPATH = $(OLDEV_CPP_DATA)/src
LIBDEPPATH = $(OLDEV_CPP_DATA)/dep

# the includes:
INCS = -I$(LIBSRCPATH) -I$(OLDEV_CPP_IO)/src 

# the linker flags:
LIBS = -L$(OLDEV_CPP_LIB) -lolCppIO 

# the compiler options:
CXXOPTS = -ansi -Wall -Wextra -Werror -Wno-unused-parameter -fPIC 

# the sources:
SOURCES = 		\
		BLIB.cpp \
		Point2D.cpp \
		Coordinate2D.cpp \
		CoordinateSystem2D.cpp \
		Geometrical2D.cpp \
		ScalarLike2D.cpp \
		VectorLike2D.cpp \
		Distance.cpp \
		LinePath2D.cpp \
		Line2D.cpp \
		RRFunction.cpp

# the library name:
LIBRARY = $(OLDEV_CPP_LIB)/libolCppData.so

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
