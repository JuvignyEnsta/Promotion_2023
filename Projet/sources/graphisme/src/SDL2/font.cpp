#include <string>
#include <stdexcept>
#include "font.hpp"
#include <iostream>
#include <cassert>

using namespace std::string_literals;

sdl2::font::font(const std::string& file, std::uint8_t size)
    : m_pt_ttf(nullptr)
{
    this->m_pt_ttf = TTF_OpenFont( file.c_str(), size );
    if (this->m_pt_ttf == nullptr)
    {
        std::string errmsg = "Erreur lors du chargement d'une fonte : "s + std::string(TTF_GetError());
        throw std::runtime_error(errmsg);
    }
    assert(this->m_pt_ttf != nullptr);
}
// ======================================================================================
sdl2::font::~font()
{
    TTF_CloseFont(this->m_pt_ttf);
}
