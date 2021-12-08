#include "geometry.hpp"

#if defined(_WIN32)
#  include <SDL2/SDL.h>
#else
#  include <SDL.h>
#endif

sdl2::rectangle::rectangle( const std::array<std::uint16_t,2>& position,
                            const std::array<std::uint16_t,2>& dimension, const sdl2::color& col,
                            bool fill)
    :   x(position[0]), y(position[1]), 
        width(dimension[0]), height(dimension[1]),
        is_filled(fill), ink(col)
{}
// =================================================================================================
void
sdl2::rectangle::render(window &win) const
{
    SDL_Rect rect = { this->x, this->y, this->width, this->height };
    SDL_SetRenderDrawColor( win.get_native_renderer(), 
                            this->ink.red, this->ink.green, this->ink.blue, this->ink.alpha );
    if (this->is_filled)
        SDL_RenderFillRect( win.get_native_renderer(), &rect );
    else
        SDL_RenderDrawRect( win.get_native_renderer(), &rect );
}
// =================================================================================================
sdl2::line::line( const std::array<std::uint16_t,2>& beg_pos,
                  const std::array<std::uint16_t,2>& end_pos, const color& col )
    : beg_x(beg_pos[0]), beg_y(beg_pos[1]),
      end_x(end_pos[0]), end_y(end_pos[1]),
      ink(col)
{}
// =================================================================================================
void 
sdl2::line::render(window& win) const
{
    SDL_SetRenderDrawColor(win.get_native_renderer(), ink.red, ink.green, ink.blue, ink.alpha);
    SDL_RenderDrawLine(win.get_native_renderer(), beg_x, beg_y, end_x, end_y);
}
// =================================================================================================
sdl2::polygon::polygon(const std::initializer_list<std::array<int,2>>& coords, const color& col)
    : vertices_coordinates(),
      ink(col)
{
    this->vertices_coordinates.reserve(2*coords.size()+2);
    for ( auto points : coords )
    {
        this->vertices_coordinates.emplace_back(points[0]);
        this->vertices_coordinates.emplace_back(points[1]);
    }
    this->vertices_coordinates.emplace_back(this->vertices_coordinates[0]);
    this->vertices_coordinates.emplace_back(this->vertices_coordinates[1]);
}
// =================================================================================================
sdl2::polygon::polygon( const std::vector<std::array<int,2>>& coords, const color& col)
    : vertices_coordinates(),
      ink(col)
{    
    this->vertices_coordinates.reserve(2*coords.size()+2);
    for ( auto points : coords )
    {
        this->vertices_coordinates.emplace_back(points[0]);
        this->vertices_coordinates.emplace_back(points[1]);
    }
    this->vertices_coordinates.emplace_back(this->vertices_coordinates[0]);
    this->vertices_coordinates.emplace_back(this->vertices_coordinates[1]);
}
// =================================================================================================
void 
sdl2::polygon::render(window& win) const
{
    SDL_SetRenderDrawColor(win.get_native_renderer(), ink.red, ink.green, ink.blue, ink.alpha);
    SDL_RenderDrawLines(win.get_native_renderer(), (SDL_Point*)this->vertices_coordinates.data(), 
                        this->vertices_coordinates.size()/2);
}
