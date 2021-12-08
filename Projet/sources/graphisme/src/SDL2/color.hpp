#ifndef _SDL2_COLOR_HPP_
#define _SDL2_COLOR_HPP_
#include <cstdint>
#include <array>

namespace sdl2
{
    struct color
    {
        color()
            : red(0x00), green(0x00), blue(0x00), alpha(0xFF)
        {}
        color(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a = 0xFF)
            : red(r), green(g), blue(b), alpha(a)
        {}

        std::uint8_t red, green, blue, alpha;
    };
}

#endif
