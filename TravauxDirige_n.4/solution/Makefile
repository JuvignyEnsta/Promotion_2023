include Make_msys2.inc
#include Make_linux.inc
#include Make_osx.inc

CXXFLAGS = -std=c++11 -fPIC
ifdef DEBUG
CXXFLAGS += -g -O0 -Wall -fbounds-check -pedantic -D_GLIBCXX_DEBUG -fopenmp
else
CXXFLAGS += -O3 -march=native -Wall 
endif

default:	help

all:	Mandelbrot_omp_master_slave.exe raytracer_tbb.exe

Mandelbrot_omp_master_slave.exe:	Mandelbrot_omp_master_slave.cpp
	$(CXX) -fopenmp $(CXXFLAGS) -o Mandelbrot_omp_master_slave.exe Mandelbrot_omp_master_slave.cpp $(LIB)

raytracer_tbb.exe:	raytracer_tbb.cpp
	$(CXX) $(CXXFLAGS) -o raytracer_tbb.exe raytracer_tbb.cpp $(LIB) -ltbb


help: 
	@echo "Available targets : "
	@echo "    all                      : compile all executables"
	@echo "    Mandelbrot_omp_master_slave.exe           : compile Mandelbrot omp mpi"
	@echo "    raytracer_tbb.exe    : compile Raytracer with tbb parallelism"
	@echo "Add DEBUG=yes to compile in debug"
	@echo "Configuration :"
	@echo "    CXX      :    $(CXX)"
	@echo "    CXXFLAGS :    $(CXXFLAGS)"

clean:
	@rm -f *.exe *~ 

