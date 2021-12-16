#ifndef _WINDOW_HPP_
#define _WINDOW_HPP_
#include <array>
#include "ioconsole.hpp"

/** Cree une fenetre sur une console. La fenetre sera geree comme un flot de donnees */
class window
{
public:
    window( const std::array<int,2>& pos, const std::array<int,2>& dim, const char* couleur_arriere_plan,
            const char* couleur_encre, const char* couleur_bordure );
private:
    int posx, posy;
    int width, height;
    int background_color, border_color, foreground_color;
};

#endif
