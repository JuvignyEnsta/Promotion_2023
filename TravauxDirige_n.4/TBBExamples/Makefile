#include Make_mac.inc
include Make_other.inc
LIBS = -lm -ltbb
CXXFLAGS = -std=c++17 -fPIC
ifdef DEBUG
CXXFLAGS += -g -O0 -Wall -fbounds-check -pedantic -Weffc++ -D_GLIBCXX_DEBUG
CXXFLAGS2 = CXXFLAGS
else
CXXFLAGS += -O3 -march=native
CXXFLAGS2 = ${CXXFLAGS} -O2 -march=native -Wall
endif

ALL = hello_world.exe parallel_for_example1.exe compute_pi.exe Mandelbrot_tbb.exe Mandelbrot_seq.exe

default: help

all: $(ALL)

clean:
	@rm -fr *.o *.exe *~

%.exe: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

# dependances particulieres :
hello_world.exe: hello_world.cpp
	$(CXX) $(CXXFLAGS2) $^ -o $@ $(LIBS)	

parallel_for_example1.exe: parallel_for_example1.cpp
	$(CXX) $(CXXFLAGS2) $^ -o $@ $(LIBS)	

compute_pi.exe: compute_pi.cpp
	$(CXX) $(CXXFLAGS2) $^ -o $@ $(LIBS)	

Mandelbrot_tbb.exe: Mandelbrot_tbb.cpp 
	$(CXX) $(CXXFLAGS2) $^ -o $@ $(LIBS)

Mandelbrot_seq.exe: Mandelbrot_seq.cpp 
	$(CXX) $(CXXFLAGS2) $^ -o $@ $(LIBS)

help:
	@echo "Available targets:"
	@echo "    all: compile all executables"
	@echo "       $(ALL)"
	@echo "Configuration:"
	@echo "    CXX     : $(CXX)"
	@echo "    CXXFLAGS: $(CXXFLAGS)"
	@echo "Add DEBUG=yes to compile in debug"


# Rapport
%.html: %.md
	pandoc -s --toc $< --css=./github-pandoc.css -o $@
