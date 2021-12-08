#ifndef _ANSI_COULEUR_HPP_
#define _ANSI_COULEUR_HPP_
#include <iostream>
#include <cstdint>
#include <string>

namespace console
{
    class couleur
    {
    public:
        static constexpr const char* defaut           =  nullptr;
        static constexpr const char noir[]            = "\u001b[30m";
        static constexpr const char rouge[]           = "\u001b[31m";
        static constexpr const char vert[]            = "\u001b[32m";
        static constexpr const char jaune[]           = "\u001b[33m";
        static constexpr const char bleu[]            = "\u001b[34m";
        static constexpr const char magenta[]         = "\u001b[35m";
        static constexpr const char cyan[]            = "\u001b[36m";
        static constexpr const char blanc[]           = "\u001b[37m";
        static constexpr const char noir_brillant[]   = "\u001b[30;1m";
        static constexpr const char rouge_brillant[]  = "\u001b[31;1m";
        static constexpr const char vert_brillant[]   = "\u001b[32;1m";
        static constexpr const char jaune_brillant[]  = "\u001b[33;1m";
        static constexpr const char bleu_brillant[]   = "\u001b[34;1m";
        static constexpr const char magenta_brillant[]= "\u001b[35;1m";
        static constexpr const char cyan_brillant[]   = "\u001b[36;1m";
        static constexpr const char blanc_brillant[]  = "\u001b[37;1m";
        static constexpr const bool encre             = true;
        static constexpr const bool papier            = false;
        /** Créer une couleur. Par défaut pour l'encre, mais si encre est faux,
            ce sera pour la couleur du papier.
        */
        couleur(const char* code_couleur, bool pour_encre = true)
            : couleur_ansi(code_couleur)
        {
            if (not pour_encre) this->couleur_ansi[2] = '4';
        }

        couleur(int index_palette256, bool pour_encre = true)
            : couleur_ansi("\u001b[38;5;")
        {
            this->couleur_ansi += std::to_string(index_palette256) + "m";
            if (not pour_encre) this->couleur_ansi[2] = '4';
        }

        couleur(std::uint8_t rouge, std::uint8_t vert, std::uint8_t bleu, 
                bool pour_encre = true)
            : couleur_ansi("\u001b[38;2;")
        {
            couleur_ansi += std::to_string(rouge) + ";" + std::to_string(vert) + ";"
                          + std::to_string(bleu) + "m";
            if (not pour_encre) this->couleur_ansi[2] = '4';
        }

        void convertir_pour_encre()
        {
            this->couleur_ansi[2] = '3';
        }

        void convertir_pour_papier()
        {
            this->couleur_ansi[2] = '4';
        }

        const std::string& code_ansi() const
        {
            return this->couleur_ansi;
        }
    private:
        std::string couleur_ansi;
    };
}

inline std::ostream& 
operator << (std::ostream& out, const console::couleur& coul )
{
    out << coul.code_ansi();
    return out;
}

#endif
