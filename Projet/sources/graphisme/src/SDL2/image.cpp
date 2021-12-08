#if defined(_WIN32)
#  include <SDL2/SDL_image.h>
#else
#  include <SDL_image.h>
#endif
#include <stdexcept>
#include <string>
#include "image.hpp"

sdl2::image::image(const std::string &file, window& win, const color& transparent)
    : pt_texture(nullptr),
      position_and_stretching(),
      center_rotation_x(-1), center_rotation_y(-1),
      angle(0.),
      filter_color{0xFF,0xFF,0xFF,0x00},
      m_direction(SDL_FLIP_NONE)
{
    // On charge l'image d'abord dans une surface temporaire
    SDL_Surface* loadedSurface = IMG_Load( file.c_str() );
    if( loadedSurface == nullptr )
    {
        std::string errmsg = "Incapable de charger le fichier " + file + " : " +
                             std::string(IMG_GetError());
        throw std::runtime_error(errmsg);
    }
    // On applique la colorkey si besoin :
    if (transparent.alpha != 0x00)
    {
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, transparent.red,
                        transparent.green, transparent.blue));
    }
    //Create texture from surface pixels
    this->pt_texture = SDL_CreateTextureFromSurface( win.get_native_renderer(), loadedSurface );
    if( this->pt_texture == nullptr )
    {
        std::string errmsg = "Incapable de créer une texture de " + file + " : "
                           + std::string(SDL_GetError());
        throw std::runtime_error(errmsg);
    }
    // Initialisation du rectangle d'affichage :
    this->position_and_stretching.x = 0; this->position_and_stretching.y = 0;
    this->position_and_stretching.w = loadedSurface->w;
    this->position_and_stretching.h = loadedSurface->h;
    // Destrution de la surface temporaire
    SDL_FreeSurface( loadedSurface );
}
// =================================================================================================
sdl2::image::~image()
{
    SDL_DestroyTexture(this->pt_texture);
}
// =================================================================================================
auto
sdl2::image::at(int x, int y) -> image&
{
    this->position_and_stretching.x = x;
    this->position_and_stretching.y = y;
    return *this;
}
// =================================================================================================
auto
sdl2::image::stretch(const std::array<int,2>& dimension) -> image&
{
    this->position_and_stretching.w = dimension[0];
    this->position_and_stretching.h = dimension[1];
    return *this;
}
// =================================================================================================
auto 
sdl2::image::flip( sdl2::image::sens dir ) -> image&
{
    switch(dir)
    {
    case none:
        this->m_direction = SDL_FLIP_NONE;
        break;
    case horizontal:
        this->m_direction = SDL_FLIP_HORIZONTAL;
        break;
    case vertical:
        this->m_direction = SDL_FLIP_VERTICAL;
        break;
    }
    return *this;
}
// =================================================================================================
auto
sdl2::image::rotate( const std::array<int,2>& center, double angle) -> image&
{
    this->center_rotation_x = center[0];
    this->center_rotation_y = center[1];
    this->angle = angle;
    return *this;
}
// =================================================================================================
auto
sdl2::image::color_filter(const color& col) -> image&
{
    this->filter_color = col;
    SDL_SetTextureColorMod(this->pt_texture, col.red, col.green, col.blue);
    return *this;
}
// =================================================================================================
void 
sdl2::image::render(window& win) const
{
    //Render to screen
    SDL_Point center;
    center.x = this->center_rotation_x;
    center.y = this->center_rotation_y;
    SDL_RenderCopyEx( win.get_native_renderer(), this->pt_texture, 
                      (const SDL_Rect*)nullptr, &this->position_and_stretching, this->angle, 
                      &center, this->m_direction );
 }
