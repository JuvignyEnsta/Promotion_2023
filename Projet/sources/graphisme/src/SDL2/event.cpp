#include <iostream>
#include "event.hpp"

auto sdl2::event_queue::pull_events() const -> std::vector<std::unique_ptr<event>>
{
    std::vector<std::unique_ptr<event>> events;
    SDL_Event event;
    SDL_PumpEvents();
    while (SDL_PollEvent(&event) != 0)
    {
        switch(event.type)
        {
        case SDL_QUIT:
            events.emplace_back(std::make_unique<event_quit>());
            break;
        case SDL_MOUSEMOTION:
            events.emplace_back(std::make_unique<event_mouse_motion>((SDL_MouseMotionEvent *)&event));
            break;
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP  :
            events.emplace_back(std::make_unique<event_mouse_button>((SDL_MouseButtonEvent *)&event));
            break;
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            events.emplace_back(std::make_unique<event_keyboard>((SDL_KeyboardEvent *)&event));
            break;
        case SDL_FINGERMOTION:
        case SDL_FINGERDOWN:
        case SDL_FINGERUP:
            events.emplace_back(std::make_unique<event_touch>((SDL_TouchFingerEvent*)&event));
            break;
        }
    }
    return events;
}
