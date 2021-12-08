#ifndef _ANSI_STYLE_HPP_
#define _ANSI_STYLE_HPP_
#include <string>
#include <iostream>
#include <initializer_list>
#include <cassert>

namespace console
{
    class style
    {
    public:
        static constexpr const char normal[]   = "\u001b[0m";
        static constexpr const char gras[]     = "\u001b[1m";
        static constexpr const char maigre[]   = "\u001b[2m";
        static constexpr const char italique[] = "\u001b[3m";
        static constexpr const char souligné[] = "\u001b[4m";
        static constexpr const char clignotement_lent[]   = "\u001b[5m";
        static constexpr const char clignotement_rapide[] = "\u001b[6m";
        static constexpr const char inverse[]  = "\u001b[7m";
        static constexpr const char invisible[]= "\u001b[8m";
        static constexpr const char fonte_par_defaut[] = "\u001b[10m";
        static constexpr const char fonte1[]   = "\u001b[11m";
        static constexpr const char fonte2[]   = "\u001b[12m";
        static constexpr const char fonte3[]   = "\u001b[13m";
        static constexpr const char fonte4[]   = "\u001b[14m";
        static constexpr const char fonte5[]   = "\u001b[15m";
        static constexpr const char fonte6[]   = "\u001b[16m";
        static constexpr const char fonte7[]   = "\u001b[17m";
        static constexpr const char fonte8[]   = "\u001b[18m";
        static constexpr const char fonte9[]   = "\u001b[19m";

        static constexpr const int taille_defaut = 0;
        static constexpr const int double_taille = 1;
        static constexpr const int double_largeur= 2;

        style(const char* commande)
            : ansi_escape(commande)
        {}

        style(const std::initializer_list<const char*>& commandes)
            : ansi_escape()
        {
            for ( auto commande : commandes )
                this->ansi_escape += commande;
        }
        style(int commande, const std::string& phrase)
            : ansi_escape()
        {
            if (commande == 1)
            {
                this->ansi_escape += "\u001b[s\u001b#3" + phrase
                                  +  "\u001b[u\u001b[B\u001b#4" + phrase;
            }
            else if (commande==2)
            {
                this->ansi_escape += "\u001b#6" + phrase;
            }
            else
            {
                assert(commande==0);
                this->ansi_escape += "\u001b#5" + phrase;
            }

        }

        const std::string& ansi_code() const
        {
            return this->ansi_escape;
        }

    private:
        std::string ansi_escape;
    };
}

inline std::ostream& 
operator << ( std::ostream& out, const console::style& s)
{
    out << s.ansi_code();
    return out;
}

#endif
