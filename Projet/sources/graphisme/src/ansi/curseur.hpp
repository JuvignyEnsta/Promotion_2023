#ifndef _ANSI_CURSEUR_HPP_
#define _ANSI_CURSEUR_HPP_
#include <iostream>
#include <array>
#include <string>

namespace console
{
    using position=std::array<int,2>;

    class curseur
    {
    public:
        static constexpr const char haut[] = "A";
        static constexpr const char bas[] = "B";
        static constexpr const char droit[] = "C";
        static constexpr const char gauche[]  = "D";
        curseur() = default;

        curseur(const position& pos)
            : ansi_escape("\u001b[")
        {
            this->ansi_escape += std::to_string(pos[0]) + ";" + std::to_string(pos[1]) + "H";
        }

        curseur(const char* direction, int nombre_pas)
            : ansi_escape("\u001b[")
        {
            ansi_escape += std::to_string(nombre_pas) + direction;
        }

        static constexpr const char sauve_état[] = "\u001b[s";
        static constexpr const char restaure_état[] = "\u001b[u";

        curseur(const char* commande)
            : ansi_escape(commande)
        {}

        const std::string& code_ansi() const
        {
            return this->ansi_escape;
        }

    private:
        std::string ansi_escape;
    };
}

inline std::ostream& 
operator << ( std::ostream& out, const console::curseur& cur)
{
    out << cur.code_ansi();
    return out;
}

#endif
