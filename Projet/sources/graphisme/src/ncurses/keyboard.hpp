#ifndef _NCURSES_KEYBOARD_HPP_
#define _NCURSES_KEYBOARD_HPP_
#if defined(_WIN32)
#  include <ncurses/ncurses.h>
#else
#  include <ncurses.h>
#endif


namespace ncurses
{
    namespace keyboard
    {
        constexpr const int down_arrow  = KEY_DOWN;
        constexpr const int up_arrow    = KEY_UP;
        constexpr const int right_arrow = KEY_RIGHT;
        constexpr const int left_arrow  = KEY_LEFT;
        constexpr const int home        = KEY_HOME;
        constexpr const int enter       = KEY_ENTER;
        constexpr const int none        = ERR;

        inline int key_pressed()
        {
            return getch();
        }
    }
}

#endif