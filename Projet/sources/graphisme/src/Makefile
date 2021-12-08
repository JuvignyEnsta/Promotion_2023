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
ALL= sdl2_documentation.exe test_sdl.exe

default:	help

all: $(ALL)

clean:
	@rm -fr SDL2/*.o *.exe SDL2/*~ 

OBJS = SDL2/sdl2.o SDL2/geometry.o SDL2/window.o SDL2/font.o SDL2/event.o SDL2/texte.o SDL2/image.o SDL2/formated_text.o

SDL2/sdl2.o:	SDL2/sdl2.hpp SDL2/sdl2.cpp
	$(CXX) $(CXXFLAGS) -c SDL2/sdl2.cpp -o SDL2/sdl2.o

SDL2/geometry.o:	SDL2/geometry.hpp SDL2/window.hpp SDL2/geometry.cpp
	$(CXX) $(CXXFLAGS) -c SDL2/geometry.cpp -o SDL2/geometry.o

SDL2/window.o:	SDL2/window.hpp SDL2/window.cpp
	$(CXX) $(CXXFLAGS) -c SDL2/window.cpp -o SDL2/window.o

SDL2/font.o:	SDL2/window.hpp SDL2/font.hpp SDL2/font.cpp
	$(CXX) $(CXXFLAGS) -c SDL2/font.cpp -o SDL2/font.o

SDL2/event.o : SDL2/window.hpp SDL2/event.hpp SDL2/event.cpp
	$(CXX) $(CXXFLAGS) -c SDL2/event.cpp -o SDL2/event.o

SDL2/texte.o : SDL2/window.hpp SDL2/font.hpp SDL2/texte.hpp SDL2/texte.cpp
	$(CXX) $(CXXFLAGS) -c SDL2/texte.cpp -o SDL2/texte.o

SDL2/formated_texte.o : SDL2/window.hpp SDL2/font.hpp SDL2/formated_text.hpp SDL2/formated_text.cpp
	$(CXX) $(CXXFLAGS) -c SDL2/formated_text.cpp -o SDL2/formated_text.o


SDL2/image.o : SDL2/window.hpp SDL2/image.hpp SDL2/image.cpp
	$(CXX) $(CXXFLAGS) -c SDL2/image.cpp -o SDL2/image.o

sdl2_documentation.exe : $(OBJS) sdl2_documentation.cpp
	$(CXX) $(CXXFLAGS) $(OBJS) sdl2_documentation.cpp -o sdl2_documentation.exe $(LIBS)

test_sdl.exe : $(OBJS) test_sdl.cpp
	$(CXX) $(CXXFLAGS) $(OBJS) test_sdl.cpp -o test_sdl.exe $(LIBS)


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
