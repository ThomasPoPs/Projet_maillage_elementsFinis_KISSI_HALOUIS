EXEC = main
TEST = test
CXX = g++
CWARNINGS = -Wunreachable-code -pedantic
CFLAGS = -O3 -s -I$(IDIR)
IDIR = eigen-3.4.0/Eigen/

all: $(EXEC)

$(EXEC): main.cpp Mesh.hpp PbEF2d.hpp Point.hpp Triangle.hpp
	$(CXX) $(CFLAGS) $(CWARNINGS) -o $@ $<

$(TEST): test.cpp Mesh.hpp PbEF2d.hpp Point.hpp Triangle.hpp
	$(CXX) $(CFLAGS) $(CWARNINGS) -o $@ $<
	
clean: 
	rm $(EXEC) $(TEST)