#include <fstream>
#include <cstring>
#include <algorithm>
#include <string_view>
#include <cctype>
#include <iterator>
#include <sstream>
#if defined(DEBUG)
# include <iostream>
#endif
#include "formated_text.hpp"

auto
sdl2::formated_text::font_manager_t::load_and_add(const std::string &key, 
                                                  const std::string &fonte, std::uint8_t size) -> iterator
{
    auto [iter, ok] = dico_font.insert_or_assign(key,std::make_shared<font>(fonte, size));
    if (!ok)
    {
        std::string errmesg = "Failed to add " + key + " element (with loaded font)";
        throw std::runtime_error(errmesg);
    }
    return iter;
}
// =================================================================================================
sdl2::formated_text::formated_text( const window& win, const std::string& path_for_font)
    : formated_text( {0,0, win.dimensions()[0], win.dimensions()[1]}, path_for_font )
{}
// =================================================================================================
sdl2::formated_text::formated_text( const std::array<std::int32_t, 4>& z, const std::string& path_for_font)
    : image(),
      font_manager(),
      splitted_string(), begin_line(0),
      zone(z), formats(), ordered_list_formats(), unordered_list_formats(),
      qformat()
{
#   if defined(DEBUG)
    std::cerr << __PRETTY_FUNCTION__ << std::endl << std::flush;
#   endif
    font_manager.load_and_add("normal", path_for_font + "/Lato-Medium.ttf", 16);
    font_manager.load_and_add("italic", path_for_font + "/Lato-Italic.ttf", 16);
    font_manager.load_and_add("bold", path_for_font + "/Lato-Bold.ttf", 17);
    font_manager.load_and_add("bolditalic", path_for_font + "/Lato-BoldItalic.ttf", 17);
    font_manager.load_and_add("heading1", path_for_font + "/Lato-Heavy.ttf", 40);
    font_manager.load_and_add("heading2", path_for_font + "/Lato-Heavy.ttf", 36);
    font_manager.load_and_add("heading3", path_for_font + "/Lato-HeavyItalic.ttf", 32);
    font_manager.load_and_add("heading4", path_for_font + "/Lato-Medium.ttf", 28);
    font_manager.load_and_add("heading5", path_for_font + "/Lato-Medium.ttf", 24);
    font_manager.load_and_add("heading6", path_for_font + "/Lato-Light.ttf",  20);
    font_manager.load_and_add("code", path_for_font + "/Lato-Regular.ttf", 16);
    font_manager.load_and_add("link", path_for_font + "/Lato-HairlineItalic.ttf", 16);

    formats[style::texte] = { this->font_manager["normal"], {0xA0,0xA0,0xA0,0xFF}, 
                              {0x00, 0x00, 0x00, 0xFF}, 8 }; 
    formats[style::italic] = { this->font_manager["italic"], {0xB0,0xB0,0xB0,0xFF},
                               {0x00, 0x00, 0x00, 0xFF}, 8 };
    formats[style::bold] = { this->font_manager["bold"], {0xFF,0xFF,0xFF,0xFF},
                               {0x00, 0x00, 0x00, 0xFF}, 8 };
    formats[style::italic_bold] = { this->font_manager["bolditalic"], {0xE0,0xFF,0xFF,0xFF},
                                    {0x00, 0x00, 0x00, 0xFF}, 8 };
    formats[style::heading_1] = { this->font_manager["heading1"], {0xFF,0xFF,0x00,0xFF},
                                  {0x00, 0x00, 0xF0, 0xFF}, 0 };
    formats[style::heading_2] = { this->font_manager["heading2"], {0x00,0xFF,0xFF,0xFF},
                                  {0x00, 0x00, 0x00, 0xFF}, 2 };
    formats[style::heading_3] = { this->font_manager["heading3"], {0xFF,0xFF,0xFF,0xFF},
                                  {0x00, 0x00, 0x00, 0xFF}, 4 };
    formats[style::heading_4] = { this->font_manager["heading4"], {0xFF,0xFF,0xFF,0xFF},
                                  {0x00, 0x00, 0x00, 0xFF}, 6 };
    formats[style::heading_5] = { this->font_manager["heading5"], {0xFF,0xA0,0xA0,0xFF},
                                  {0x00, 0x00, 0x00, 0xFF},  8};
    formats[style::heading_6] = { this->font_manager["heading6"], {0xFF,0x00,0x00,0xFF},
                                  {0x00, 0x00, 0x00, 0xFF}, 10 };
    formats[style::code] = { this->font_manager["code"], {0xFF,0xFF,0xA0,0xFF},
                             {0x00, 0x00, 0xA0, 0xFF}, 0 };
    formats[style::link] = { this->font_manager["link"], {0xFF,0xA0,0xFF,0xFF},
                             {0x00, 0x00, 0x00, 0xFF}, 0 };
    this->qformat = { {0xFF,0xFF,0xFF,0xFF}, {0x40, 0x40, 0x40, 0xFF}, 32 };
    for ( int i = 0; i < max_list_level; ++i )
    {
        this->ordered_list_formats[i] = { {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}, 32 + 16*i,
                                          numbering::latin};
        this->unordered_list_formats[i] = { {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}, 32 + 16*i,
                                             L'•'};
    }
}
// =================================================================================================
bool 
sdl2::formated_text::load_simplified_markdown( const std::string& path )
{
#   if defined(DEBUG)
    std::cerr << __PRETTY_FUNCTION__ << std::endl << std::flush;
#   endif
    char buffer[16384];
    std::ifstream fich(path.c_str());
    std::vector<std::string> lines;
#   if defined(DEBUG)
    std::cerr << "Fichier chargé : " << std::endl << std::flush;
#   endif
    while (fich)
    {
        fich.getline(buffer, 16384);
        if (fich)
        {
            lines.emplace_back(buffer);
#           if defined(DEBUG)
            std::cerr << lines.back() << std::endl << std::flush;
#           endif
        }
    }
    fich.close();
#   if defined(DEBUG)
    std::cerr << "Fin chargement ligne à ligne" << std::endl << std::flush;
#   endif

    // On va adapter maintenant le texte à la zone dans laquelle on veut afficher et préparer les
    // formats spéciaux (italic, bolc, etc.) dans splitted_string
    splitted_string.reserve(lines.size());
    style current_style = style::texte;
    int start_texte = 0;
    for ( auto line : lines)
    {
#   if defined(DEBUG)
        std::cerr << "Traitement ligne " << line << std::endl << std::flush;
#   endif
        if (line.size() == 0)
        {
            this->splitted_string.emplace_back();
            this->splitted_string.back().push_back("\n");
            continue;
        }
        int width, height = 0;
        std::shared_ptr<font> current_font;
        // Traitement des headings
        if (std::string_view(line.data(), 7) == "###### ")
        {
            current_style = style::heading_6;
            start_texte = 7;
        }
        else if (std::string_view(line.data(), 6) == "##### ")
        {
            current_style = style::heading_5;
            start_texte = 6;
        }
        else if (std::string_view(line.data(), 5) == "#### ")
        {
            current_style = style::heading_4;
            start_texte = 5;
        }
        else if (std::string_view(line.data(), 4) == "### ")
        {
            current_style = style::heading_3;
            start_texte = 4;
        }
        else if (std::string_view(line.data(), 3) == "## ")
        {
            current_style = style::heading_2;
            start_texte = 3;
        }
        else if (std::string_view(line.data(), 2) == "# ")
        {
            current_style = style::heading_1;
            start_texte = 2;
        }
        else if (std::string_view(line.data(), 2) == "> ")
        {
#           if defined(DEBUG)
            std::cerr << "Ligne de code : " << std::endl << std::flush;
#           endif
            current_style = style::code;
            start_texte = 2;
        }
        else
            current_style = style::texte;
        if (current_style != style::texte)
        {
            current_font  = this->formats[current_style].pt_fonte;
            TTF_SizeText(current_font->m_pt_ttf, line.data() + start_texte, &width, &height);
            if (width > this->zone[2])
                throw std::runtime_error("A heading message must be only on one row actually.");
            this->splitted_string.emplace_back();
            this->splitted_string.back().emplace_back(line);
            current_style = style::texte;
#           if defined(DEBUG)
            std::cerr << "Fin d'une commande uni ligne" << std::endl << std::flush;
#           endif            
            continue;
        }
        else
        {
#   if defined(DEBUG)
            std::cerr << "C'est du texte..." << std::endl << std::flush;
#   endif
            // On doit décomposer en plusieur bouts selon le formatage voulu ou si c'est une liste :
            // est-ce une liste ?
            if (std::isdigit(line[0]) && (line[1] == '.'))
                width = 32;// Alors on doit décaler de trente-deux pixels
            else
                width = 0 ;
            current_style = style::texte;
            std::istringstream iline(line);
            // Si on est en mode texte, on décompose mot par mot le paragraphe (ou la liste) :
            std::vector<std::string> words{std::istream_iterator<std::string>(iline),
                                           std::istream_iterator<std::string>()};
            std::vector<std::int32_t> position; position.reserve(words.size());
            position.push_back(0);
            for ( const auto& word : words)
            {
                int beg_word = 0;
                if (std::string_view(word.data(), 3) == "***")
                {
                    current_style = style::italic_bold;
                    beg_word = 3;
                }
                else if (std::string_view(word.data(), 2) == "**")
                {
                    if (current_style == style::italic)
                    {
                        current_style = style::italic_bold;
                    }
                    else
                        current_style = style::bold;
                    beg_word = 2;
                }
                else if (word[0] == '*')
                {
                    if (current_style == style::bold)
                        current_style = style::italic_bold;
                    else
                        current_style = style::italic;
                    beg_word = 1;
                }
                std::size_t end_word = word.find("*", beg_word);
                if (end_word == std::string::npos)
                    end_word = word.size();
                std::string true_word(word.data()+beg_word, word.data()+end_word);
                true_word += " ";
                current_font = this->formats[current_style].pt_fonte;
                int w,h;
                TTF_SizeText(current_font->m_pt_ttf, true_word.c_str(), &w, &h);
                width += w;
                position.emplace_back(width);
                if (end_word != word.size())
                {
                    std::string end_style(word.data()+end_word, word.data()+word.size());
                    if (end_style == "***")
                        current_style = style::texte;
                    else if (end_style == "**")
                    {
                        if (current_style == style::italic_bold)
                            current_style = style::italic;
                        else
                            current_style = style::texte;
                    }
                    else if (end_style == "*")
                    {
                        if (current_style == style::italic_bold)
                            current_style = style::bold;
                        else
                            current_style = style::texte;
                    }
                }
            }// for (const auto& word : words )
            assert(words.size()+1 == position.size());
            this->splitted_string.emplace_back();
            std::size_t index = 0;
            std::int32_t pos_rel = 0;
            while (index < words.size())
            {
                std::vector<std::string>& last = this->splitted_string.back();
                do
                {
                    //pos_rel += position[index];
                    last.emplace_back(words[index]);
                    index++;
                } while ( (index < words.size()) && (pos_rel + position[index+1] < this->zone[2]) );
                pos_rel = -position[index];
                this->splitted_string.emplace_back();
            }
        }// If current_style == style::texte
    }// for (auto line : lines)
    return true;
}
// =================================================================================================
void 
sdl2::formated_text::begin_at_line(window& w, int iline)
{
#   if defined(DEBUG)
    std::cerr << __PRETTY_FUNCTION__ << std::endl << std::flush;
#   endif
    int maxwidth  = this->zone[2];
    int maxheight = this->zone[3];
    // Première étape, compter le nombre de lignes imposées (par des \n) et leur emplacement
    SDL_Surface* panneau = SDL_CreateRGBSurface(SDL_SWSURFACE, maxwidth, maxheight, 32, 0, 0, 0, 0);
    SDL_FillRect(panneau, nullptr, SDL_MapRGB(panneau->format, 
                 this->formats[style::texte].papier.red, this->formats[style::texte].papier.green, 
                 this->formats[style::texte].papier.blue));
    style current_style = style::texte;
    SDL_Rect pos; pos.y = 0;
    for ( std::size_t iwords = iline; 
          (iwords < this->splitted_string.size()) && (pos.y < maxheight); ++iwords )
    {            
#   if defined(DEBUG)
                std::cerr << "Analyse ligne n°" << iwords << std::endl << std::flush;
                if (this->splitted_string[iwords].size() > 0)
                    std::cerr << "Premier mot : " << this->splitted_string[iwords].front()
                              << ", dernier mot : " << this->splitted_string[iwords].back()
                              << std::endl << std::flush;
#   endif
        if (this->splitted_string[iwords].size() == 1)
        {
            auto word = this->splitted_string[iwords][0];            
            if (word == "\n")
            {
                current_style = style::texte;
                int wt, ht;
                TTF_SizeText(this->formats[current_style].pt_fonte->m_pt_ttf, " ", &wt, &ht);
                pos.y += ht;
                continue;
            }
            // Il faut d'abord gérer si ce n'est pas par hasard des barres horizontales à afficher !
            if (word == "****")
            {
#   if defined(DEBUG)
                std::cerr << "Affichage ligne type 1 en " << pos.y << std::endl << std::flush;
#   endif
                // Cas d'une ligne horizontale de type 1
                SDL_Rect line = {0, pos.y, maxwidth, 8};
                SDL_FillRect(panneau, &line, SDL_MapRGB(panneau->format, 0xFF, 0x00, 0x00));
                pos.y += 10;
                continue;
            }
            if (word == "====")
            {
                // Cas d'une ligne horizontale de type 2
                SDL_Rect line = {0, pos.y, maxwidth, 4};
                SDL_FillRect(panneau, &line, SDL_MapRGB(panneau->format, 0xFF, 0xFF, 0x00));
                pos.y += 6;
                continue;
            }
            if (word == "----")
            {
                // Cas d'une ligne horizontale de type 3
                SDL_Rect line = {0, pos.y, maxwidth, 2};
                SDL_FillRect(panneau, &line, SDL_MapRGB(panneau->format, 0x00, 0xFF, 0xFF));
                pos.y += 4;

                continue;
            }
            if (word == "____")
            {
                // Cas d'une ligne horizontale de type 4
                SDL_Rect line = {0, pos.y, maxwidth, 1};
                SDL_FillRect(panneau, &line, SDL_MapRGB(panneau->format, 0xFF, 0xFF, 0xFF));
                pos.y += 3;

                continue;
            }
            if (word == "....")
            {
                // Cas d'une ligne horizontale de type 4
                SDL_Rect line = {0, pos.y, maxwidth, 1};
                SDL_FillRect(panneau, &line, SDL_MapRGB(panneau->format, 0xA0, 0xA0, 0xA0));
                pos.y += 3;

                continue;
            }
        }
#   if defined(DEBUG)
        std::cerr << "Pas une ligne. On analyse" << std::endl << std::flush;
#   endif

        const std::vector<std::string>& words = this->splitted_string[iwords];
        pos.x = 0;
        int hmax = 0;
        for ( std::size_t iword = 0; iword < words.size(); ++iword)
        {
            bool add_space = true;
            const std::string& word = words[iword];
            int beg_word = 0;
            std::size_t end_word = 0;
            if ( (word.size() > 7) && (std::string_view(word.data(), 7) == "###### ") )
            {
                current_style = style::heading_6;
                beg_word = 7;
                add_space = false;
            }
            else if ( (word.size() > 6) && (std::string_view(word.data(), 6) == "##### ") )
            {
                current_style = style::heading_5;
                beg_word = 6;
                end_word = word.size();
                add_space = false;
            }
            else if ( (word.size() > 5) && (std::string_view(word.data(), 5) == "#### ") )
            {
                current_style = style::heading_4;
                beg_word = 5;
                end_word = word.size();
                add_space = false;
            }
            else if ( (word.size() > 4) && (std::string_view(word.data(), 4) == "### ") )
            {
                current_style = style::heading_3;
                beg_word = 4;
                end_word = word.size();
                add_space = false;
            }
            else if ( (word.size() > 3) && (std::string_view(word.data(), 3) == "## ") )
            {
                current_style = style::heading_2;
                beg_word = 3;
                end_word = word.size();
                add_space = false;
            }
            else if ( (word.size() > 2) && (std::string_view(word.data(), 2) == "# ") )
            {
#               if defined(DEBUG)
                std::cerr << "Préparation d'un titre" << std::endl << std::flush;
#               endif
                current_style = style::heading_1;
                beg_word = 2;
                end_word = word.size();
                add_space = false;
            }
            else if ( (word.size() > 2) && (std::string_view(word.data(), 2) == "> ") )
            {
                current_style = style::code;
                beg_word = 2;
                end_word = word.size();
                add_space = false;
            }
            else if ( (word.size() > 3) && (std::string_view(word.data(), 3) == "***"))
            {
                current_style = style::italic_bold;
                beg_word = 3;
            }
            else if ( (word.size() > 2) && (std::string_view(word.data(), 2) == "**" ))
            {
                current_style = style::bold;
                beg_word = 2;
            }
            else if ( (word.size() > 1) && (std::string_view(word.data(), 1) == "*"  ))
            {
                current_style = style::italic;
                beg_word = 1;
            }
            else if ( (current_style != style::italic) && (current_style != style::bold) &&
                      (current_style != style::italic_bold) )
                current_style = style::texte;
            if (end_word < word.size())
            {
                end_word = word.find("*", beg_word);
                if (end_word == std::string::npos)
                    end_word = word.size();
            }
            std::string true_word(word.data()+beg_word, word.data()+end_word);
            if ( (add_space) and (iword != words.size()-1) ) true_word += " ";
#           if defined(DEBUG)
            std::cerr << "On rajoute le mot : " << true_word << std::endl << std::flush;
#           endif            
            int wt, ht;
            TTF_SizeText(this->formats[current_style].pt_fonte->m_pt_ttf, true_word.c_str(), &wt, &ht);
#           if defined(DEBUG)
            std::cerr << "Taille du mot en pixel : " << wt << " x " << ht << std::endl << std::flush;
#           endif            
            if (current_style == style::heading_1)
            {
                pos.x = (maxwidth - wt)/2;
            }
#           if defined(DEBUG)
            std::cerr << "Affichage en : " << pos.x << ", " << pos.y << std::endl << std::flush;
            std::cerr << "Couleur du papier : " << int(this->formats[current_style].papier.red) << ", " 
                      << int(this->formats[current_style].papier.green) << ", "
                      << int(this->formats[current_style].papier.blue) << std::endl;
            std::cerr << "Couleur du papier : " << int(this->formats[current_style].encre.red) << ", " 
                      << int(this->formats[current_style].encre.green) << ", "
                      << int(this->formats[current_style].encre.blue) << std::endl << std::flush;
#           endif
            SDL_Rect papier_fill = {pos.x, pos.y, wt, ht};
            if ( (current_style == style::code) || (current_style == style::heading_1) )
            {
                papier_fill.w = maxwidth;
                papier_fill.x = 0;
            }
            SDL_FillRect(panneau, &papier_fill, SDL_MapRGB(panneau->format, 
                         this->formats[current_style].papier.red, this->formats[current_style].papier.green, 
                         this->formats[current_style].papier.blue));

            SDL_Color col;
            col.r = this->formats[current_style].encre.red;
            col.g = this->formats[current_style].encre.green;
            col.b = this->formats[current_style].encre.blue;
            SDL_Surface* textSurface = 
                               TTF_RenderUTF8_Solid(this->formats[current_style].pt_fonte->m_pt_ttf, 
                                                    true_word.c_str(), col);
            hmax = std::max(hmax, textSurface->h);
            SDL_BlitSurface(textSurface, NULL, panneau, &pos);
            pos.x += textSurface->w;
            SDL_FreeSurface(textSurface);
            // Si on a des * en fin de chaine, il faut rétablir les styles :
            if (end_word < word.size())
            {
                if (end_word == word.size()-3)
                    current_style = style::texte;
                else if (end_word == word.size()-2)
                {
                    if (current_style == style::italic_bold)
                        current_style = style::italic;
                    else
                        current_style = style::texte;
                }
                else if (end_word == word.size()-1)
                {
                    if (current_style == style::italic_bold)
                        current_style = style::bold;
                    else
                        current_style = style::texte;
                }
            }
        }
        pos.y += hmax;// + 8;
    }// for (const auto& words : this->splitted_string)
    //Create texture from surface pixels
    this->pt_texture = SDL_CreateTextureFromSurface( w.get_native_renderer(), panneau );
    if (this->pt_texture == nullptr)
    {
        std::string errMsg = "Incapable de créer une texture du texte rendu : " +
                             std::string(SDL_GetError());
        throw std::runtime_error(errMsg);
    }
    this->position_and_stretching.x = 0; this->position_and_stretching.y = 0;
    this->position_and_stretching.w = panneau->w;
    this->position_and_stretching.h = panneau->h;
    //Get rid of old surface
    SDL_FreeSurface( panneau );
}
