#ifndef _NCURSES_NCURSES_HPP_
#define _NCURSES_NCURSES_HPP_
#include "window.hpp"
#include "style.hpp"
#include "keyboard.hpp"
#include "palette.hpp"

namespace ncurses
{
    void init();
    void finalize();
}

#endif
