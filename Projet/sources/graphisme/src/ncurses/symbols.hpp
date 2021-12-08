#ifndef _NCURSES_SYMBOL_HPP_
#define  _NCURSES_SYMBOL_HPP_
#include <iostream>
#if defined(_WIN32)
#include <ncurses/ncurses.h>
#else
#include <ncurses.h>
#endif

namespace ncurses
{
    class symbol
    {
    public:
        symbol() : val(0) {}
        symbol(int code) : val(code) {}

        int val;

        static symbol block;
        static symbol board;
        static symbol  bottom_tee;
        static symbol  bullet;
        static symbol  checker_board;
        static symbol  down_arrow;
        static symbol  degree;
        static symbol  diamond;
        static symbol  greater_or_equal;
        static symbol  horizontal_line;
        static symbol  lantern;
        static symbol  left_arrow;
        static symbol  lesser_or_equal;
        static symbol  lower_left_corner;
        static symbol  lower_right_corner;
        static symbol  left_tee;
        static symbol  not_equal;
        static symbol  pi;
        static symbol  plus_minus;
        static symbol  plus;
        static symbol  right_arrow;
        static symbol  right_tee;
        static symbol  scanline1;
        static symbol  scanline3;
        static symbol  scanline7;
        static symbol  scanline9;
        static symbol  sterling;
        static symbol  top_tee;
        static symbol  up_arrow;
        static symbol  upper_left_corner;
        static symbol  upper_right_corner;
        static symbol  vertical_line;
    };
/*
static symbol  bottom_tee ACS_BTEE)
static symbol  bullet ACS_BULLET)
static symbol  checker_board ACS_CKBOARD)
static symbol  down_arrow ACS_DARROW)
static symbol  degree ACS_DEGREE)
static symbol  diamond ACS_DIAMOND)
static symbol  greater_or_equal ACS_GEQUAL)
static symbol  horizontal_line ACS_HLINE)
static symbol  lantern ACS_LANTERN)
static symbol  left_arrow ACS_LARROW)
static symbol  lesser_or_equal ACS_LEQUAL)
static symbol  lower_left_corner ACS_LLCORNER)
static symbol  lower_right_corner ACS_LRCORNER)
static symbol  left_tee ACS_LTEE)
static symbol  not_equal ACS_NEQUAL)
static symbol  pi ACS_PI)
static symbol  plus_minus ACS_PLMINUS)
static symbol  plus ACS_PLUS)
static symbol  right_arrow ACS_RARROW)
static symbol  right_tee ACS_RTEE)
static symbol  scanline1 ACS_S1)
static symbol  scanline3 ACS_S3)
static symbol  scanline7 ACS_S7)
static symbol  scanline9 ACS_S9)
static symbol  sterling ACS_STERLING)
static symbol  top_tee ACS_TTEE)
static symbol  up_arrow ACS_UARROW)
static symbol  upper_left_corner ACS_ULCORNER)
static symbol  upper_right_corner ACS_URCORNER)
static symbol  vertical_line ACS_VLINE)*/
} // namespace ncurses

#endif