#ifndef _NCURSES_STYLE_HPP_
#define _NCURSES_STYLE_HPP_
#include <iostream>
#include <cassert>
#if defined(_WIN32)
#  include <ncurses/ncurses.h>
#else
#  include <ncurses.h>
#endif
#include "window.hpp"
#include "palette.hpp"

namespace ncurses
{
    class style
    {
    public:
        static constexpr const int normal        = A_NORMAL;
        static constexpr const int hightlighting = A_STANDOUT;
        static constexpr const int underline     = A_UNDERLINE;
        static constexpr const int reverse       = A_REVERSE;
        static constexpr const int blink         = A_BLINK;
        static constexpr const int half_bright   = A_DIM;
        static constexpr const int bold          = A_BOLD;
        static constexpr const int protect       = A_PROTECT;
        static constexpr const int blank         = A_INVIS;
        static constexpr const int alternative_character_set = A_ALTCHARSET;
        static constexpr const int bitmask       = A_CHARTEXT;

        style(int mode)
            : value_mode(mode)
        {}
    
        style( const std::initializer_list<int>& modes)
            : value_mode(0)
        {
            for (const auto& mode : modes)
                value_mode |= mode;
        }

        ~style()
        {
            attroff(this->value_mode);
        }

        style& add_mode(int mode)
        {
            this->value_mode |= mode;
            return *this;
        }

        static int number_of_color_attribut()
        {
            return COLOR_PAIRS;
        }

        static void define_color_attribut(int num, int encre, int fond)
        {
            assert(num>=0); assert(num < COLOR_PAIRS);
            assert(encre>=0); assert(encre < COLORS);
            assert(fond >=0); assert(fond  < COLORS);
            init_pair(num, encre, fond);
        }

        style& add_color_attribut(int num)
        {
            this->value_mode |= COLOR_PAIR(num);
            return *this;
        }
        // Renvoit les composants rgb de l'encre et du fond
        std::pair<palette::rgb,palette::rgb> get_color_attribut(int num) const
        {
            assert(num >= 0); assert(num < COLOR_PAIRS);
            short e, f;
            pair_content(num, &e, &f);
            palette::rgb encre, fond;
            short r,g,b;
            color_content(e, &r, &g, &b);
            encre = {r,g,b};
            color_content(f, &r, &g, &b);
            fond  = {r,g,b};
            return {encre, fond};
        }

        void remove_mode(int mode)
        {
            this->value_mode &= ~mode;
        }

        void apply_mode(window& w) const
        {
            wattron(w.get_native_window(), this->value_mode);
        }

        void unapply_style() const
        {
            attroff(this->value_mode);
        }

    private:
        int value_mode;

    };
}

inline ncurses::window& operator << ( ncurses::window& out, const ncurses::style& st )
{
    st.apply_mode(out);
    return out;
}

#endif