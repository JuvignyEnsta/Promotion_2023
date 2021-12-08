#ifndef _SDL2_FORMATED_TEXT_HPP_
#define _SDL2_FORMATED_TEXT_HPP_
#include <array>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include <cassert>
#include <unordered_map>
#include "window.hpp"
#include "font.hpp"
#include "color.hpp"
#include "image.hpp"

namespace sdl2
{
    class formated_text : public image
    {
    public:
        static constexpr const int max_list_level = 5;

        enum style
        {
            texte = 0, italic, bold, italic_bold, heading_1, heading_2,
            heading_3, heading_4, heading_5, heading_6, code, link, end_style
        };

        enum numbering
        {
            latin = 0, roman, alpha
        };

        struct format
        {
            std::shared_ptr<font> pt_fonte;
            color encre, papier;
            int marge;
        };

        struct quote_format
        {
            color encre, papier;
            int marge;
        };

        struct ordered_list_format
        {
            color encre, papier;
            int marge;
            numbering kind;
        };

        struct unordered_list_format
        {
            color encre, papier;
            int marge;
            wchar_t symbol;
        };

        class font_manager_t
        {
        public:
            using dictionary = std::unordered_map<std::string, std::shared_ptr<font>>;
            using iterator = dictionary::iterator;
            using const_iterator = dictionary::const_iterator;

            font_manager_t() = default;
            font_manager_t(const font_manager_t&) = default;
            font_manager_t( font_manager_t&& ) = default;
            ~font_manager_t() = default;

            font_manager_t& operator = ( const font_manager_t& ) = default;
            font_manager_t& operator = ( font_manager_t&& )      = default;

            iterator load_and_add( const std::string& key, const std::string& fonte, std::uint8_t size);

            std::shared_ptr<font> operator [] ( const std::string& key )
            {
                return dico_font[key];
            }

            iterator begin() { return dico_font.begin(); }
            iterator end()   { return dico_font.end();   }
            const_iterator cbegin(){ return dico_font.cbegin();}
            const_iterator cend()  { return dico_font.cend  ();}

        private:
            dictionary dico_font;
        };

        // Affiche en prenant la totalité de la fenêtre
        formated_text(const window& fenêtre, const std::string& path_for_font);
        // Affiche en prenant une sous-zone de la fenêtre
        formated_text(const std::array<std::int32_t,4>& zone, const std::string& path_for_font);

        format& get_format(style kind) { return this->formats[kind]; }
        quote_format& get_quote_format() { return this->qformat; }
        unordered_list_format& get_unordered_list_format(int lvl)
        {
            assert(lvl >= 0);
            assert(lvl < int(unordered_list_formats.size()));
            return unordered_list_formats[lvl];
        }

        ordered_list_format& get_ordered_list_format(int lvl)
        {
            assert(lvl >= 0);
            assert(lvl < int(ordered_list_formats.size()));
            return ordered_list_formats[lvl];
        }

        bool load_simplified_markdown( const std::string& path );
        void begin_at_line ( window& w, int line );
        std::size_t nb_lines() const { return this->splitted_string.size(); }

        font_manager_t font_manager;

    private:
        // Le texte découpé par paragraphe
        std::vector<std::vector<std::string>> splitted_string;
        // La première ligne prise en compte lors de la l'affichage
        std::int32_t begin_line;
        // La zone où doit être affiché le texte (coord + longueur/hauteur)
        std::array<std::int32_t, 4> zone;
        std::array<format, end_style> formats;
        std::array<ordered_list_format, max_list_level> ordered_list_formats;
        std::array<unordered_list_format, max_list_level> unordered_list_formats;
        quote_format qformat;
    };
}

#endif