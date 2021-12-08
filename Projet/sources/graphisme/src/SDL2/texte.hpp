#ifndef _SDL2_TEXTE_HPP_
#define _SDL2_TEXTE_HPP_
#if defined(_WIN32)
#  include <SDL2/SDL_ttf.h>
#else
#  include <SDL_ttf.h>
#endif
#include "image.hpp"
#include "font.hpp"

namespace sdl2
{
    class texte : public image
    {
    public:
        static constexpr const std::int32_t left_justify = 0;
        static constexpr const std::int32_t right_justify = 1;
        static constexpr const std::int32_t middle_justify = 2;
        texte(const std::string& phrase, const font& fnt, window& win, const color& encre );
        texte(const std::string& phrase, const font& fnt, window& win, int maxwidth, int maxheight,
              const color& encre, const color& papier, std::int32_t justification = left_justify,
              int marge_width = 5, int marge_height = 5);
        texte(const std::string& phrase, const font& fnt, window& win, int maxwidth,
              const color& encre, const color& papier, std::int32_t justification = left_justify,
              int marge_width = 5, int marge_height = 0);
        ~texte();
    };
}

#endif