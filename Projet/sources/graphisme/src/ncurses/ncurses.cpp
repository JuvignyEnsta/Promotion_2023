#include "ncurses.hpp"
#include "symbols.hpp"
void ncurses::init( )
{
    initscr( );
    raw( );
    keypad(stdscr, true);
    nodelay(stdscr, true);
    noecho( );
    start_color( );
    symbol::block              = symbol(ACS_BLOCK);
    symbol::board              = symbol(ACS_BOARD);
    symbol::bottom_tee         = symbol(ACS_BTEE);
    symbol::bullet             = symbol(ACS_BULLET);
    symbol::checker_board      = symbol(ACS_CKBOARD);
    symbol::down_arrow         = symbol(ACS_DARROW);
    symbol::degree             = symbol(ACS_DEGREE);
    symbol::diamond            = symbol(ACS_DIAMOND);
    symbol::greater_or_equal   = symbol(ACS_GEQUAL);
    symbol::horizontal_line    = symbol(ACS_HLINE);
    symbol::lantern            = symbol(ACS_LANTERN);
    symbol::left_arrow         = symbol(ACS_LARROW);
    symbol::lesser_or_equal    = symbol(ACS_LEQUAL);
    symbol::lower_left_corner  = symbol(ACS_LLCORNER);
    symbol::lower_right_corner = symbol(ACS_LRCORNER);
    symbol::left_tee           = symbol(ACS_LTEE);
    symbol::not_equal          = symbol(ACS_NEQUAL);
    symbol::pi                 = symbol(ACS_PI);
    symbol::plus_minus         = symbol(ACS_PLMINUS);
    symbol::plus               = symbol(ACS_PLUS);
    symbol::right_arrow        = symbol(ACS_RARROW);
    symbol::right_tee          = symbol(ACS_RTEE);
    symbol::scanline1          = symbol(ACS_S1);
    symbol::scanline3          = symbol(ACS_S3);
    symbol::scanline7          = symbol(ACS_S7);
    symbol::scanline9          = symbol(ACS_S9);
    symbol::sterling           = symbol(ACS_STERLING);
    symbol::top_tee            = symbol(ACS_TTEE);
    symbol::up_arrow           = symbol(ACS_UARROW);
    symbol::upper_left_corner  = symbol(ACS_ULCORNER);
    symbol::upper_right_corner = symbol(ACS_URCORNER);
    symbol::vertical_line      = symbol(ACS_VLINE);
}

void ncurses::finalize( ) 
{ 
    echo();
    nodelay(stdscr, false);
    keypad(stdscr, false);
    endwin( ); 
}
