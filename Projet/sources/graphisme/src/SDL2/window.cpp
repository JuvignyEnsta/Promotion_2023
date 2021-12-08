#include <stdexcept>
#include "window.hpp"

sdl2::window::window()
    :   window("SDL2", {-1,-1}, {SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED}, true)
{
}
// ======================================================================================
sdl2::window::window(const std::string& titre, const std::array<int,2>& dimensions, 
                     const std::array<int,2>& position, bool affiche)
    :   m_window(nullptr),
        m_pt_renderer(nullptr)
{
    SDL_DisplayMode dispMode;
    SDL_GetCurrentDisplayMode(0, &dispMode);
    // Tentative de création de la fenêtre :
    m_window = SDL_CreateWindow( titre.c_str(), position[0], position[1], 
                                 (dimensions[0] == -1 ? dispMode.w : dimensions[0]),
                                 (dimensions[1] == -1 ? dispMode.h : dimensions[1]), 
                                 affiche ? SDL_WINDOW_SHOWN : SDL_WINDOW_HIDDEN );

    if( m_window == nullptr )
    {
        std::string error_mesg = "Echec de création de fenêtre : " + std::string(SDL_GetError());
        throw std::runtime_error(error_mesg);
    }
    // Création du moteur de rendu pour la fenêtre :
    this->m_pt_renderer = SDL_CreateRenderer( this->m_window, -1, SDL_RENDERER_ACCELERATED );
    if( this->m_pt_renderer == nullptr )
    {
        std::string error_mesg = "Echec de création du moteur de rendu : " + std::string(SDL_GetError());
        throw std::runtime_error(error_mesg);
    }
    // Initialisation de la couleur par défaut :
    SDL_SetRenderDrawColor( this->m_pt_renderer, 0xFF, 0xFF, 0xFF, 0xFF );
}
// ======================================================================================
sdl2::window::~window()
{
    SDL_DestroyRenderer(this->m_pt_renderer);
    SDL_DestroyWindow  (this->m_window     );
}
// ======================================================================================
void sdl2::window::cls(const color &c)
{
    // Initialisation de la couleur pour remplir la fenêtre :
    SDL_SetRenderDrawColor( this->m_pt_renderer, c.red, c.green, c.blue, c.alpha );
    // On efface la fenêtre en la remplissant avec l'encre courante :
    SDL_RenderClear(this->m_pt_renderer);
}
// ======================================================================================
sdl2::window& operator << ( sdl2::window& window, sdl2::flush_t )
{
    SDL_RenderPresent( window.get_native_renderer() );
    return window;
}
