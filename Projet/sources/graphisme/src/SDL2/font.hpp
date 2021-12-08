#ifndef _SDL2_FONT_HPP_
#define _SDL2_FONT_HPP_
#if defined(_WIN32)
#  include <SDL2/SDL_ttf.h>
#else
#  include <SDL_ttf.h>
#endif
#include <string>
#include <memory>

namespace sdl2
{
    class font
    {
    public:
        font(const std::string& file, std::uint8_t size);
        font(const font& ) = delete;
        font(font&&) = default;
        ~font();

        font& operator = ( const font& ) = delete;
        font& operator = ( font&& ) = default;
    //private:
        TTF_Font *m_pt_ttf;
    };
}

#endif