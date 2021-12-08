#ifndef _SDL2_GEOMETRY_HPP_
#define _SDL2_GEOMETRY_HPP_
#include <cstdint>
#include <array>
#include <vector>
#include "color.hpp"
#include "window.hpp"

namespace sdl2
{
    struct rectangle
    {
        rectangle( const std::array<std::uint16_t,2>& position, 
                   const std::array<std::uint16_t,2>& dimension, const color& col,
                   bool fill = true);
        void render(window& win) const;

        std::uint16_t x, y, width, height;
        bool is_filled;
        color ink;
    };

    struct line
    {
        line( const std::array<std::uint16_t,2>& beg_pos,
              const std::array<std::uint16_t,2>& end_pos, const color& col );
        void render(window& win) const;

        std::uint16_t beg_x,beg_y, end_x, end_y;
        color ink;
    };

    struct polygon
    {
        polygon( const std::initializer_list<std::array<int,2>>& coords, const color& col);
        polygon( const std::vector<std::array<int,2>>& coords, const color& col);
        void render(window& win) const;

        std::vector<int> vertices_coordinates;
        color ink;
    };
}

#endif