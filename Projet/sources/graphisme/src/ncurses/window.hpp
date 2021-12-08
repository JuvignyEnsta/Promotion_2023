#ifndef _NCURSES_WINDOW_HPP_
#define _NCURSES_WINDOW_HPP_
#if defined(_WIN32)
#  include <ncurses/ncurses.h>
#else
#  include <ncurses.h>
#endif
#include "symbols.hpp"
#include <array>
#include <string>
#include "palette.hpp"

namespace ncurses
{
    class window
    {
    public:
        window();
        window(const std::array<int,2> &dimension, const std::array<int,2>& start, bool show_border = true,
               bool enable_scrolling = false );

        std::array<int,2> dimensions() const;

        void flush();

        void cls( int num_color_attribut = 0 );

        window& at(int row, int col);
        void print(const std::string& phrase);
        void print(const symbol& symb);

        std::array<int,2> get_cursor_position() const;

        WINDOW* get_native_window() { return this->pt_ncurses_window; }
    private:
        WINDOW* pt_ncurses_window;
        WINDOW* pt_border_window;
    };
    struct endl_t
    {
        constexpr endl_t() {}
    };
    static constexpr const endl_t endl;

    struct flush_t
    {
        constexpr flush_t() {}
    };
    static constexpr flush_t flush;

}

ncurses::window& operator<< (ncurses::window& win, const ncurses::flush_t&);
ncurses::window& operator<< (ncurses::window& win, const ncurses::endl_t&);

ncurses::window& operator<< (ncurses::window& win, const std::string& phrase);
ncurses::window& operator<< (ncurses::window& win, const ncurses::symbol& symb);
ncurses::window& operator<< (ncurses::window& win, const char* phrase);
template<typename K>
ncurses::window& operator<< (ncurses::window& win, const K& value )
{
    win.print(std::to_string(value));
    return win;
}

#endif
