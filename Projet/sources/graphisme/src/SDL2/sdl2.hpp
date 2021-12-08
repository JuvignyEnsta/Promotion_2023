#ifndef _SDL2_HPP_
#define _SDL2_HPP_
#include "window.hpp"
#include "font.hpp"
#include "geometry.hpp"
#include "image.hpp"
#include "texte.hpp"
#include "event.hpp"

namespace sdl2
{
    void init(int argc, char* argv[]);
    void finalize();
}

#endif