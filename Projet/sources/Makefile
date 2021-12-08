#include Make_linux.inc
include Make_msys2.inc
#include Make_osx.inc

CXXFLAGS = -std=c++17 `sdl2-config --cflags`
ifdef DEBUG
CXXFLAGS += -g -O0 -Wall -fbounds-check -pedantic -D_GLIBCXX_DEBUG -DDEBUG
else
CXXFLAGS += -O3 -march=native -Wall
endif

LIBS = $(LIB) `sdl2-config --cflags` -lSDL2_ttf -lSDL2_image  `sdl2-config --libs` 
ALL= simulation.exe

default:	help

all: $(ALL)

clean:
	@rm -fr graphisme/src/SDL2/*.o *.exe *~ *.o

OBJS = graphisme/src/SDL2/sdl2.o graphisme/src/SDL2/geometry.o graphisme/src/SDL2/window.o graphisme/src/SDL2/font.o graphisme/src/SDL2/event.o \
	   graphisme/src/SDL2/texte.o graphisme/src/SDL2/image.o graphisme/src/SDL2/formated_text.o \
	   agent_pathogene.o grille.o individu.o

graphisme/src/SDL2/sdl2.o:	graphisme/src/SDL2/sdl2.hpp graphisme/src/SDL2/sdl2.cpp
	$(CXX) $(CXXFLAGS) -c graphisme/src/SDL2/sdl2.cpp -o graphisme/src/SDL2/sdl2.o

graphisme/src/SDL2/geometry.o:	graphisme/src/SDL2/geometry.hpp graphisme/src/SDL2/window.hpp graphisme/src/SDL2/geometry.cpp
	$(CXX) $(CXXFLAGS) -c graphisme/src/SDL2/geometry.cpp -o graphisme/src/SDL2/geometry.o

graphisme/src/SDL2/window.o:	graphisme/src/SDL2/window.hpp graphisme/src/SDL2/window.cpp
	$(CXX) $(CXXFLAGS) -c graphisme/src/SDL2/window.cpp -o graphisme/src/SDL2/window.o

graphisme/src/SDL2/font.o:	graphisme/src/SDL2/window.hpp graphisme/src/SDL2/font.hpp graphisme/src/SDL2/font.cpp
	$(CXX) $(CXXFLAGS) -c graphisme/src/SDL2/font.cpp -o graphisme/src/SDL2/font.o

graphisme/src/SDL2/event.o : graphisme/src/SDL2/window.hpp graphisme/src/SDL2/event.hpp graphisme/src/SDL2/event.cpp
	$(CXX) $(CXXFLAGS) -c graphisme/src/SDL2/event.cpp -o graphisme/src/SDL2/event.o

graphisme/src/SDL2/texte.o : graphisme/src/SDL2/window.hpp graphisme/src/SDL2/font.hpp graphisme/src/SDL2/texte.hpp graphisme/src/SDL2/texte.cpp
	$(CXX) $(CXXFLAGS) -c graphisme/src/SDL2/texte.cpp -o graphisme/src/SDL2/texte.o

graphisme/src/SDL2/formated_texte.o : graphisme/src/SDL2/window.hpp graphisme/src/SDL2/font.hpp graphisme/src/SDL2/formated_text.hpp graphisme/src/SDL2/formated_text.cpp
	$(CXX) $(CXXFLAGS) -c graphisme/src/SDL2/formated_text.cpp -o graphisme/src/SDL2/formated_text.o


graphisme/src/SDL2/image.o : graphisme/src/SDL2/window.hpp graphisme/src/SDL2/image.hpp graphisme/src/SDL2/image.cpp
	$(CXX) $(CXXFLAGS) -c graphisme/src/SDL2/image.cpp -o graphisme/src/SDL2/image.o

simulation.exe:	$(OBJS) simulation.cpp
	$(CXX) $(CXXFLAGS) $(OBJS) simulation.cpp -o simulation.exe $(LIBS)

help:
	@echo "Available targets : "
	@echo "    all            : compile the executable"
	@echo "    sdl2_documentation.exe     : compile the executable"
	@echo "Add DEBUG=yes to compile in debug"
	@echo "Configuration :"
	@echo "    CXX      :    $(CXX)"
	@echo "    CXXFLAGS :    $(CXXFLAGS)"

show:
	@echo "LIB   : $(LIB)"
	@echo "LIBS  : $(LIBS)"

%.html: %.md
	pandoc -s --toc $< --css=./github-pandoc.css -o $@
