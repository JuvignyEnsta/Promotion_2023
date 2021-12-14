CXX = g++
LIBS = -lm -lpthread
CXXFLAGS = -std=c++11 -fPIC  -fopenmp
ifdef DEBUG
CXXFLAGS += -g -O0 -Wall -fbounds-check -pedantic -Weffc++
else
CXXFLAGS += -O3 -march=native
endif

ALL=TestProduct.exe TestProduct_opt.exe TestProduct_blockomp.exe dotproduct.exe dotproduct_thread.exe dotproduct_openmp.exe bitonic.exe bitonic_thread.exe bhudda.exe bhudda_omp.exe

default: help

all: $(ALL)

clean:
	@rm -fr *.o *.exe *~

%.exe: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

TestProduct.exe: Matrix.cpp ProdMatMat.cpp
TestProduct_opt.exe: Matrix.cpp ProdMatMat_opt.cpp
TestProduct_blockomp.exe: Matrix.cpp ProdMatMat_blockomp.cpp
bitonic.exe: Vecteur.cpp
bitonic_thread.exe: Vecteur.cpp


help:
	@echo "Available targets : "
	@echo "    all             : compile all executables"
	@echo "    dotproduct.exe  : Compile dot product executable"
	@echo "    TestProduct.exe : Compile matrix-matrix product executable"
	@echo "    bitonic.exe     : Compile bitonic sort example executable"
	@echo "    bhudda.exe      : Compile bhuddabrot set executable"
	@echo "Add DEBUG=yes to compile in debug"
	@echo "Configuration :"
	@echo "    CXX      :    $(CXX)"
	@echo "    CXXFLAGS :    $(CXXFLAGS)"


# Rapport
%.html: %.md
	pandoc -s --toc $< --css=./github-pandoc.css -o $@
