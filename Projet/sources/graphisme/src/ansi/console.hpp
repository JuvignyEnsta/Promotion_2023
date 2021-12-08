#ifndef _IOCONSOLE_HPP_
#define _IOCONSOLE_HPP_
#include "couleur.hpp"
#include "style.hpp"
#include "curseur.hpp"

namespace console
{

    void init();

    /** Efface tout l'écran */
    void cls(couleur col);

    void restaure();
}

#endif
