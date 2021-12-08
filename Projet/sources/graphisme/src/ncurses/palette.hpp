#ifndef _NCURSES_PALETTE_HPP_
#define _NCURSES_PALETTE_HPP_
#if defined(_WIN32)
#  include <ncurses/ncurses.h>
#else
#  include <ncurses.h>
#endif
#include <array>
#include <vector>
#include <cassert>
#include <iostream>

namespace ncurses
{

    class palette
    {
    public:
        static constexpr const int black   = COLOR_BLACK;
        static constexpr const int red     = COLOR_RED;
        static constexpr const int green   = COLOR_GREEN;
        static constexpr const int yellow  = COLOR_YELLOW;
        static constexpr const int blue    = COLOR_BLUE;
        static constexpr const int magenta = COLOR_MAGENTA;
        static constexpr const int cyan    = COLOR_CYAN;
        static constexpr const int white   = COLOR_WHITE;
        class rgb 
        {
        public:
            rgb() 
                :   rgb_values{0,0,0}
            {}
            rgb (short r, short g, short b)
                : rgb_values{r,g,b}
            {
                assert(r>=0); assert(r<=1000);
                assert(g>=0); assert(g<=1000);
                assert(b>=0); assert(b<=1000);
            } 

            std::array<short,3> values() const
            {
                return this->rgb_values;
            }
        private:
            std::array<short, 3> rgb_values; 
        };

        palette()
            : local_palette(COLORS)
        {
            short r, g, b;
            // On prend par défaut la palette courante de ncurses
            for ( int i = 0; i < COLORS; ++i )
            {
                color_content(i, &r, &g, &b);
                local_palette[i] = rgb{r,g,b};
            }
        }
        ~palette() = default;
        int number_of_colors() const { return COLORS; }

        rgb& operator [] ( std::size_t i )
        {
            assert(i< local_palette.size());
            return local_palette[i];
        }

        const rgb& operator []( std::size_t i ) const
        {
            assert(i< local_palette.size());
            return local_palette[i];
        }

        void apply() const
        {
            for ( std::size_t i = 0; i < this->local_palette.size(); ++i )
            {
                auto [r,g,b] = this->local_palette[i].values();
                init_color(i, r, g, b);
            }
        }

    private:
        std::vector<rgb> local_palette;
    };
}

#endif