#include <stdexcept>
#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include <cassert>
#include "texte.hpp"

sdl2::texte::texte(const std::string& phrase, const font& fnt, window& win, const color& encre )
    :   image()
{
    SDL_Color col;
    col.r = encre.red; col.b = encre.blue; col.g = encre.green; col.a = encre.alpha;
    //Render text surface
    SDL_Surface* textSurface = TTF_RenderUTF8_Solid(fnt.m_pt_ttf, phrase.c_str(), col);
    if( textSurface == nullptr )
    {
        std::string errMsg = "Incapable de rendre un texte sur une surface : " + std::string(TTF_GetError());
        throw std::runtime_error(errMsg);
    }
    //Create texture from surface pixels
    this->pt_texture = SDL_CreateTextureFromSurface( win.get_native_renderer(), textSurface );
    if (this->pt_texture == nullptr)
    {
        std::string errMsg = "Incapable de créer une texture du texte rendu : " +
                             std::string(SDL_GetError());
        throw std::runtime_error(errMsg);
    }
    this->position_and_stretching.x = 0; this->position_and_stretching.y = 0;
    this->position_and_stretching.w = textSurface->w;
    this->position_and_stretching.h = textSurface->h;

    //Get rid of old surface
    SDL_FreeSurface( textSurface );
}
// =================================================================================================
namespace {
std::vector<std::string> split(const std::string& s, char delimiter)
{
   std::vector<std::string> tokens;
   std::string token;
   std::istringstream tokenStream(s);
   while (std::getline(tokenStream, token, delimiter))
   {
      tokens.push_back(token);
   }
   return tokens;
}
}
// =================================================================================================
sdl2::texte::texte(const std::string& phrase, const font& fnt, window& win, 
                   int maxwidth, int maxheight, const color& encre, const color& papier,
                   std::int32_t justification, int marge_width, int marge_height)
    : image()
{
    // Première étape, compter le nombre de lignes imposées (par des \n) et leur emplacement
    std::vector<int> positions_retour_chariot; positions_retour_chariot.reserve(1024);
#if __cplusplus <= 201703L
    {
    int pos = 0;
    for (const auto& car : phrase)
#else
    for (int pos = 0; const auto& car : phrase )
#endif
    {
        if (car == '\n') 
            positions_retour_chariot.push_back(pos);
        ++pos;
    }
#if __cplusplus <= 201703L
    }
#endif
    std::vector<std::string> lignes;
    std::size_t début = 0, fin = (positions_retour_chariot.size()>0 ? positions_retour_chariot[0] : phrase.size());
    std::size_t index = 0;
    std::vector<int> pos_vert;
    pos_vert.push_back(0);
    while ( (début < phrase.size()) && (pos_vert.back() < maxheight-marge_height) )
    {
        std::string paragraph(phrase.begin()+début, phrase.begin()+fin);

        int wt, ht;
        TTF_SizeText(fnt.m_pt_ttf, paragraph.c_str(), &wt, &ht);
        if (wt < maxwidth - 2*marge_width)
        {
            // Cas où on peut afficher le paragraph sur une seule ligne :
            lignes.emplace_back(std::move(paragraph));
        }
        else
        {
            auto mots = split(paragraph, ' ');
            std::string ligne = mots[0];
            std::size_t index = 0;
            TTF_SizeText(fnt.m_pt_ttf, ligne.c_str(), &wt, &ht);
            while (index+1 < mots.size())
            {
                index += 1;
                std::string tmp_ligne = ligne + " " + mots[index];
                int wt2, ht2;
                TTF_SizeText(fnt.m_pt_ttf, tmp_ligne.c_str(), &wt2, &ht2);
                if (wt2 < maxwidth - 2*marge_width)
                {
                    ligne.swap(tmp_ligne);
                }
                else
                {
                    lignes.emplace_back(ligne);
                    ligne = mots[index];
                    pos_vert.push_back(pos_vert.back()+ht2+2);
                }
            }
            lignes.emplace_back(ligne);
            pos_vert.push_back(pos_vert.back() + ht+3);
        }
        début = fin+1;
        index += 1;
        fin = (positions_retour_chariot.size()>index ? positions_retour_chariot[index] : phrase.size());
    }
    SDL_Surface* panneau = SDL_CreateRGBSurface(SDL_SWSURFACE, maxwidth, maxheight, 32, 0, 0, 0, 0);
    SDL_FillRect(panneau, nullptr, SDL_MapRGB(panneau->format, papier.red, papier.green, papier.blue)); 
    SDL_Color col;
    col.r = encre.red; col.b = encre.blue; col.g = encre.green; col.a = encre.alpha;
    SDL_Rect pos; pos.x = 0;
    for ( std::size_t i = 0; i < lignes.size(); ++i )
    {
        pos.y = pos_vert[i] + marge_height;
        if (pos.y < maxheight-marge_height)
        {
            if (justification != left_justify)
            {
                int wt, ht;
                TTF_SizeText(fnt.m_pt_ttf, lignes[i].c_str(), &wt, &ht);
                if (justification == right_justify)
                {
                    pos.x = maxwidth - wt - marge_width;
                }
                else
                {
                    assert(justification == middle_justify);
                    pos.x = (maxwidth -marge_width - wt)/2;
                }
            }
        }
        //Render text surface
        SDL_Surface* textSurface = TTF_RenderUTF8_Solid(fnt.m_pt_ttf, lignes[i].c_str(), col);
        SDL_BlitSurface(textSurface, NULL, panneau, &pos);
        SDL_FreeSurface(textSurface);
    }
    //Create texture from surface pixels
    this->pt_texture = SDL_CreateTextureFromSurface( win.get_native_renderer(), panneau );
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
// =================================================================================================
sdl2::texte::texte(const std::string& phrase, const font& fnt, window& win, int maxwidth,
                   const color& encre, const color& papier, std::int32_t justification,
                   int marge_width, int marge_height)
    :   image()
{
    //std::cout << __PRETTY_FUNCTION__ << " : " << phrase << std::endl;
    int maxheight = win.dimensions()[1] - marge_height;
    // Première étape, compter le nombre de lignes imposées (par des \n) et leur emplacement
    std::vector<int> positions_retour_chariot; positions_retour_chariot.reserve(1024);
#if __cplusplus <= 201703L
    {
    int pos = 0;
    for (const auto& car : phrase )    
#else    
    for (int pos = 0; const auto& car : phrase )
#endif
    {
        if (car == '\n') 
            positions_retour_chariot.push_back(pos);
        ++pos;
    }
#if __cplusplus <= 201703L
    }
#endif
    std::vector<std::string> lignes;
    std::size_t début = 0, fin = (positions_retour_chariot.size()>0 ? positions_retour_chariot[0] : 
                                                                      phrase.size());
    std::size_t index = 0;
    std::vector<int> pos_vert;
    pos_vert.push_back(0);
    while ( (début < phrase.size()) && (pos_vert.back() < maxheight) )
    {
        std::string paragraph(phrase.begin()+début, phrase.begin()+fin);

        int wt, ht;
        TTF_SizeText(fnt.m_pt_ttf, paragraph.c_str(), &wt, &ht);
        if (wt < maxwidth - 2*marge_width)
        {
            // Cas où on peut afficher le paragraphe sur une seule ligne :
            lignes.emplace_back(std::move(paragraph));
            pos_vert.push_back(pos_vert.back()+ht+2);
        }
        else
        {
            auto mots = split(paragraph, ' ');
            std::string ligne = mots[0];
            std::size_t index = 0;
            TTF_SizeText(fnt.m_pt_ttf, ligne.c_str(), &wt, &ht);
            while (index+1 < mots.size())
            {
                index += 1;
                std::string tmp_ligne = ligne + " " + mots[index];
                int wt2, ht2;
                TTF_SizeText(fnt.m_pt_ttf, tmp_ligne.c_str(), &wt2, &ht2);
                if (wt2 < maxwidth - 2*marge_width)
                {
                    ligne.swap(tmp_ligne);
                }
                else
                {
                    lignes.emplace_back(ligne);
                    ligne = mots[index];
                    pos_vert.push_back(pos_vert.back()+ht2+2);
                }
            }
            lignes.emplace_back(ligne);
            pos_vert.push_back(pos_vert.back() + ht+3);
        }
        début = fin+1;
        index += 1;
        fin = (positions_retour_chariot.size()>index ? positions_retour_chariot[index] : phrase.size());
    }
    SDL_Surface* panneau;
    if (pos_vert.size() == 1)
    {
        int wt, ht;
        TTF_SizeText(fnt.m_pt_ttf, " ", &wt, &ht);
        pos_vert.push_back(ht);
    }
    if (pos_vert.back() < maxheight)
    {
        panneau = SDL_CreateRGBSurface(SDL_SWSURFACE, maxwidth, pos_vert.back(), 
                                       32, 0, 0, 0, 0);
    }
    else
    {
        panneau = SDL_CreateRGBSurface(SDL_SWSURFACE, maxwidth, maxheight, 32, 0, 0, 0, 0);        
    }
    SDL_FillRect(panneau, nullptr, SDL_MapRGB(panneau->format, papier.red, papier.green, papier.blue)); 
    SDL_Color col;
    col.r = encre.red; col.b = encre.blue; col.g = encre.green; col.a = encre.alpha;
    SDL_Rect pos; pos.x = 0;
    for ( std::size_t i = 0; i < lignes.size(); ++i )
    {
        pos.y = pos_vert[i];
        if (pos.y < maxheight)
        {
            if (justification != left_justify)
            {
                int wt, ht;
                TTF_SizeText(fnt.m_pt_ttf, lignes[i].c_str(), &wt, &ht);
                if (pos.y + ht > maxheight) break;
                if (justification == right_justify)
                {
                    pos.x = maxwidth - wt - marge_width;
                }
                else
                {
                    assert(justification == middle_justify);
                    pos.x = (maxwidth -marge_width - wt)/2;
                }
            }
        }
        //Render text surface
        SDL_Surface* textSurface = TTF_RenderUTF8_Solid(fnt.m_pt_ttf, lignes[i].c_str(), col);
        SDL_BlitSurface(textSurface, NULL, panneau, &pos);
        SDL_FreeSurface(textSurface);
    }
    //Create texture from surface pixels
    this->pt_texture = SDL_CreateTextureFromSurface( win.get_native_renderer(), panneau );
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
// =================================================================================================
sdl2::texte::~texte()
{}
