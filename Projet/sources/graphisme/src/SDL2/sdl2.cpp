#include <stdexcept>
#include <string>
#if defined(_WIN32)
#  include <SDL2/SDL.h>
#  include <SDL2/SDL_ttf.h>
#  include <SDL2/SDL_image.h>
#else
#  include <SDL.h>
#  include <SDL_ttf.h>
#  include <SDL_image.h>
#endif
#include "sdl2.hpp"

using namespace std::string_literals;

void sdl2::init(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_EVENTS) < 0)
    {
        std::string error_mesg = "Impossible d'initialiser SDL : "s + std::string(SDL_GetError());
        throw std::runtime_error(error_mesg);
    }
    if (TTF_Init() == -1)
    {
        std::string error_mesg = "Impossible d'initialiser le manager de fontes : "s + std::string(SDL_GetError());
        throw std::runtime_error(error_mesg);
    }
    if (IMG_Init(IMG_INIT_JPG|IMG_INIT_PNG) == -1)
    {
        std::string error_mesg = "Impossible d'initialiser le module image : "s + std::string(SDL_GetError());
        throw std::runtime_error(error_mesg);
    }
}

void sdl2::finalize()
{
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}