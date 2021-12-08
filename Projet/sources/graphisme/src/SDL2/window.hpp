#ifndef _SDL2_WINDOW_HPP_
#define _SDL2_WINDOW_HPP_
#if defined(_WIN32)
#  include <SDL2/SDL.h>
#else
#  include <SDL.h>
#endif
#include <array>
#include <string>
#include "color.hpp"

namespace sdl2
{
    class window
    {
    public:
        window();
        window(const std::string& titre, const std::array<int,2>& dimensions = {-1, -1}, 
               const std::array<int,2>& position = {SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED},
               bool affiche = true);

        ~window();

        void cls( const color& c = {0xFF, 0xFF, 0xFF});

        SDL_Renderer* get_native_renderer() { return m_pt_renderer; }

        std::array<std::int32_t,2> dimensions() const
        {
            int w, h;
            SDL_GetWindowSize(this->m_window, &w, &h);
            return {w,h};
        }

    private:
        SDL_Window* m_window;
        SDL_Renderer* m_pt_renderer;
    };
    struct flush_t
    {
        constexpr flush_t() {}
    };
    static constexpr flush_t flush;
}

sdl2::window& operator << ( sdl2::window& window, sdl2::flush_t );

template<typename obj>
sdl2::window& operator << ( sdl2::window& window, const obj& o)
{
    o.render(window);
    return window;
}

#endif