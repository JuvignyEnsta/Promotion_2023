#include <cstdlib>
#include <string>
#include "ncurses/ncurses.hpp"

using namespace std::string_literals;

int main(int nargs, char* argv[])
{
    ncurses::init();

    ncurses::window console; // Toute la console d'origine
    auto [h,w] = console.dimensions();

    console << "La console fait " << h << " lignes et " << w << " colonnes de caracteres" << ncurses::endl;
    ncurses::palette pal;
    ncurses::style::define_color_attribut(2, ncurses::palette::yellow, ncurses::palette::blue);

    ncurses::window subwin({10,40}, {5,20}, true, false);
    auto [wh, ww] = subwin.dimensions();
    subwin.cls(2);
    subwin << "La fenetre fait " << wh << " lignes et " << ww << " colonnes de caracteres" << ncurses::flush;
    console.at(3, w/2-10) << "Je peux afficher ou je veux !";
    auto pos = console.get_cursor_position();
    console.at(h/2+7,1) << "Position du curseur : " << pos[0] << ", " << pos[1] << ncurses::endl;

    console.at(h/2+8,2) << "Nombre de couleurs dans une palette : " << pal.number_of_colors() << ncurses::endl;
    console.at(h/2+9,3) << ncurses::style(ncurses::style::bold).add_color_attribut(2)
                        << "Nombre de paires de couleurs possibles : " 
                        << ncurses::style::number_of_color_attribut() << ncurses::endl;
    console.at(h-2,0) << ncurses::style({ncurses::style::hightlighting, ncurses::style::bold,
                                         ncurses::style::underline}) 
                      << "Veuillez bien regarder le message ci-dessous" << ncurses::endl;

    std::array all_names   = { "block"s, "board"s, "bottom_tee"s, "bullet"s, "checker_board"s, 
                               "down_arrow"s, "degree"s, "diamond"s, "greater_or_equal"s, 
                               "horizontal_line"s, "lantern"s, "left_arrow"s, "lesser_or_equal"s, 
                               "lower_left_corner"s, "lower_right_corner"s, "left_tee"s, "not_equal"s,
                               "pi"s, "plus_minus"s, "plus"s, "right_arrow"s, "right_tee"s, "scanline1"s, 
                               "scanline3"s, "scanline7"s, "scanline9"s, "sterling"s, "top_tee"s, "up_arrow"s, 
                               "upper_left_corner"s, "upper_right_corner"s, "vertical_line"s};
    std::array all_symbols = { ncurses::symbol::block, ncurses::symbol::board, ncurses::symbol::bottom_tee, ncurses::symbol::bullet,
                               ncurses::symbol::checker_board, ncurses::symbol::down_arrow, ncurses::symbol::degree, ncurses::symbol::diamond,
                               ncurses::symbol::greater_or_equal, ncurses::symbol::horizontal_line, ncurses::symbol::lantern,
                               ncurses::symbol::left_arrow, ncurses::symbol::lesser_or_equal, ncurses::symbol::lower_left_corner,
                               ncurses::symbol::lower_right_corner, ncurses::symbol::left_tee, ncurses::symbol::not_equal,
                               ncurses::symbol::pi, ncurses::symbol::plus_minus, ncurses::symbol::plus, ncurses::symbol::right_arrow,
                               ncurses::symbol::right_tee, ncurses::symbol::scanline1, ncurses::symbol::scanline3,
                               ncurses::symbol::scanline7, ncurses::symbol::scanline9, ncurses::symbol::sterling,
                               ncurses::symbol::top_tee, ncurses::symbol::up_arrow, ncurses::symbol::upper_left_corner,
                               ncurses::symbol::upper_right_corner, ncurses::symbol::vertical_line};

    for ( std::size_t i = 0; i < 4; ++i )
      for (std::size_t j = 0; j < 8; ++j )
      {
        console.at(18+ 2*i, 20*j) << all_names[8*i+j];
        console.at(19+ 2*i, 20*j+all_names[8*i+j].size()/2) << all_symbols[8*i+j]; 
    }

    console.at(h-1,0) << "Appuyez sur une touche pour terminer" << ncurses::flush;
    while (ncurses::keyboard::key_pressed() == ncurses::keyboard::none);

    ncurses::finalize();
    return EXIT_SUCCESS;
}
