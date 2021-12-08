#ifndef _SDL2_IMAGE_HPP_
#define _SDL2_IMAGE_HPP_
#if defined(_WIN32)
#  include <SDL2/SDL.h>
#else
#  include <SDL.h>
#endif
#include <array>
#include <string>
#include "window.hpp"
#include "color.hpp"

namespace sdl2
{
    class image
    {
    public:
        enum sens
        {
            none = SDL_FLIP_NONE, horizontal = SDL_FLIP_HORIZONTAL, vertical = SDL_FLIP_VERTICAL
        };
        image()
            : pt_texture(nullptr),
              position_and_stretching(),
              center_rotation_x(-1), center_rotation_y(-1),
              angle(0.),
              filter_color(0x00,0x00,0x00,0x00),
              m_direction(SDL_FLIP_NONE)
        {}

        image(const std::string& file, window& win, 
              const color& transparent = {0x00,0x00,0x00,0x00});
        virtual ~image();

        image& at( int x, int y );
        image& stretch(const std::array<int,2>& dimension);
        image& rotate( const std::array<int,2>& center, double angle);
        image& flip( sens dir );
        image& color_filter(const color& col);

        std::array<int,2> dimension() const
        { return {this->position_and_stretching.w, this->position_and_stretching.h}; }

        virtual void render(window& win) const;
    protected:
        SDL_Texture *pt_texture;
        SDL_Rect    position_and_stretching;
        int   center_rotation_x, center_rotation_y;
        double angle;
        color filter_color;
        SDL_RendererFlip m_direction;
    };
}

#endif