#include "window.hpp"
#include "style.hpp"

ncurses::window::window()
    : pt_ncurses_window(nullptr),
      pt_border_window(nullptr)
{
    this->pt_ncurses_window = stdscr;
}


ncurses::window::window(const std::array<int, 2> &dimension, const std::array<int, 2> &start, 
                        bool has_border, bool enable_scrolling)
    : pt_ncurses_window(nullptr),
      pt_border_window(nullptr)
{
    this->pt_ncurses_window = newwin(dimension[0], dimension[1], start[0], start[1]);
    if(has_border)
    {
        this->pt_border_window = newwin(dimension[0]+2, dimension[1]+2, start[0]-1, start[1]-1);
        box(this->pt_border_window, 0, 0);
        wrefresh(this->pt_border_window);
    }
    scrollok(this->pt_ncurses_window, enable_scrolling);
    wrefresh(this->pt_ncurses_window);
}

std::array<int,2> ncurses::window::dimensions() const
{
    int row, col;
    getmaxyx(this->pt_ncurses_window, row, col);
    return {row, col};
}

void ncurses::window::flush()
{
    if (this->pt_border_window != nullptr)
        wrefresh(this->pt_border_window);
    wrefresh(this->pt_ncurses_window);
}

void ncurses::window::cls( int num_attribut_cols )
{
    auto [h,w] = this->dimensions();
    style st(style::bold);
    this->at(0,0) << st.add_color_attribut(num_attribut_cols);
    for ( int i = 0; i <= w*h; ++i ) 
    *this << st.add_color_attribut(num_attribut_cols) << " ";
    this->flush();
    this->at(0,0);
}

ncurses::window& 
ncurses::window::at(int row, int col)
{
    wmove(this->pt_ncurses_window, row, col);
    return *this;
}

void ncurses::window::print(const std::string& phrase)
{
    wprintw(this->pt_ncurses_window, "%s", phrase.c_str());
}

void ncurses::window::print(const ncurses::symbol& symb )
{
    waddch(this->pt_ncurses_window,symb.val);
}

std::array<int,2> 
ncurses::window::get_cursor_position() const
{
    int y,x;
    getyx(this->pt_ncurses_window, y, x);
    return {y,x};
}

ncurses::window& operator<< (ncurses::window& win, const ncurses::flush_t& )
{
    win.flush();
    return win;
}

ncurses::window& operator<< (ncurses::window& win, const ncurses::endl_t& )
{
    win.print("\n");
    win.flush();
    return win;
}

ncurses::window& operator<< (ncurses::window& win, const std::string& phrase)
{
    win.print(phrase);
    return win;
}

ncurses::window& operator<< (ncurses::window& win, const ncurses::symbol& symb)
{
    win.print(symb);
    return win;
}

ncurses::window& operator<< (ncurses::window& win, const char* phrase)
{
    win.print(std::string(phrase));
    return win;
}
