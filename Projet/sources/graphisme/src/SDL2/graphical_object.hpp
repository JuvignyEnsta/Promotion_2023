#ifndef _SDL2PP_GRAPHICAL_OBJECT_HPP_
#define _SDL2PP_GRAPHICAL_OBJECT_HPP_
#include "window.hpp"

namespace sdl2
{
namespace graphic
{
    class object
    {
    public:
        virtual void render(window& win) const = 0;
        virtual void smooth_render(window& win) const = 0;
        virtual void grender(window& win) const = 0;
    };
}
}

#endif