#ifndef _SDL2_EVENT_HPP_
#define _SDL2_EVENT_HPP_
#if defined(_WIN32)
#  include <SDL2/SDL.h>
#else
#  include <SDL.h>
#endif
#include <cstdint>
#include <vector>
#include <memory>
#include <algorithm>
#include <array>

namespace sdl2
{
    class event
    {
    public:
        static constexpr const std::int32_t none              = -1;

        static constexpr const std::int32_t quit              = SDL_QUIT;

        static constexpr const std::int32_t mouse_motion      = SDL_MOUSEMOTION;

        static constexpr const std::int32_t mouse_button_down = SDL_MOUSEBUTTONDOWN;
        static constexpr const std::int32_t mouse_button_up   = SDL_MOUSEBUTTONUP;
        static constexpr const std::int32_t mouse_button      = mouse_button_down;

        static constexpr const std::int32_t mouse_wheel       = SDL_MOUSEWHEEL;

        static constexpr const std::int32_t key_down          = SDL_KEYDOWN;
        static constexpr const std::int32_t key_up            = SDL_KEYUP;
        static constexpr const std::int32_t key               = key_down;

        static constexpr const std::int32_t finger_motion     = SDL_FINGERMOTION;
        static constexpr const std::int32_t finger_down       = SDL_FINGERDOWN;
        static constexpr const std::int32_t finger_up         = SDL_FINGERUP;
        static constexpr const std::int32_t touch_finger      = finger_motion;

        event() = default;
        virtual ~event() = default;

        virtual std::int32_t kind_of_event() const
        {
            return none;
        }
    };
    // =============================================================================================
    class event_quit : public event
    {
    public:
        event_quit() = default;
        ~event_quit() = default;

        virtual std::int32_t kind_of_event() const override
        {
            return event::quit;
        }

    };
    // =============================================================================================
    class event_mouse_motion : public event
    {
    public:
        event_mouse_motion(SDL_MouseMotionEvent* event)
            : position_x(event->x), position_y(event->y),
              relative_x(event->xrel), relative_y(event->yrel)
        {}

        virtual std::int32_t kind_of_event() const override
        {
            return event::mouse_motion;
        }

        std::array<std::int32_t,2> get_relative_movement() const
        { return {this->relative_x, this->relative_y}; }

        std::array<std::int32_t,2> position() const
        { return {this->position_x, this->position_y}; }
    private:
        std::int32_t position_x, position_y;
        std::int32_t relative_x, relative_y;
    };
    // =============================================================================================
    class event_mouse_button : public event
    {
    public:
        static constexpr const std::int32_t left    = SDL_BUTTON_LEFT;
        static constexpr const std::int32_t middle  = SDL_BUTTON_MIDDLE;
        static constexpr const std::int32_t right   = SDL_BUTTON_RIGHT;
        static constexpr const std::int32_t x1      = SDL_BUTTON_X1;
        static constexpr const std::int32_t x2      = SDL_BUTTON_X2;
        static constexpr const std::uint8_t pressed = SDL_PRESSED;
        static constexpr const std::uint8_t released= SDL_RELEASED;

        event_mouse_button(SDL_MouseButtonEvent* event) :
            _id_button(event->button),
            _state_of_button(event->state),
            _number_of_clicks(event->clicks),
            _position_x(event->x), _position_y(event->y)
        {}

        virtual std::int32_t kind_of_event() const override
        {
            return event::mouse_button;
        }

        std::int32_t type_of_event() const
        {
            if (this->_state_of_button == pressed)
                return event::mouse_button_down;
            else
                return event::mouse_button_up;
        }

        std::int32_t button() const
        { return this->_id_button; }

        std::int8_t state() const
        { return this->_state_of_button; }

        std::int8_t number_of_clicks() const
        { return this->_number_of_clicks; }

        std::array<std::int32_t,2> position() const
        { return { this->_position_x, this->_position_y}; }
    private:
        std::uint8_t _id_button; // Quel est le bouton de la souris concerné ?
        std::uint8_t _state_of_button; // Etat du bouton (appuyé/relâché)
        std::uint8_t _number_of_clicks;// Nombre de cliques (double clique, triple clique, etc.)
        std::int32_t _position_x, _position_y;// Position de la souris au moment de l'événement
    };
    // =============================================================================================
    class event_keyboard : public event
    {
    public:
        static constexpr const std::int32_t unknown = SDLK_UNKNOWN;
        static constexpr const std::int32_t F1  = SDLK_F1;
        static constexpr const std::int32_t F2  = SDLK_F2;
        static constexpr const std::int32_t F3  = SDLK_F3;
        static constexpr const std::int32_t F4  = SDLK_F4;
        static constexpr const std::int32_t F5  = SDLK_F5;
        static constexpr const std::int32_t F6  = SDLK_F6;
        static constexpr const std::int32_t F7  = SDLK_F7;
        static constexpr const std::int32_t F8  = SDLK_F8;
        static constexpr const std::int32_t F9  = SDLK_F9;
        static constexpr const std::int32_t F10 = SDLK_F10;
        static constexpr const std::int32_t F11 = SDLK_F11;
        static constexpr const std::int32_t F12 = SDLK_F12;
        static constexpr const std::int32_t printscreen = SDLK_PRINTSCREEN;
        static constexpr const std::int32_t scrolllock  = SDLK_SCROLLLOCK;
        static constexpr const std::int32_t insert      = SDLK_INSERT;
        static constexpr const std::int32_t home        = SDLK_HOME;
        static constexpr const std::int32_t pageup      = SDLK_PAGEUP;
        static constexpr const std::int32_t pagedown    = SDLK_PAGEDOWN;
        static constexpr const std::int32_t end         = SDLK_END;
        static constexpr const std::int32_t right       = SDLK_RIGHT;
        static constexpr const std::int32_t left        = SDLK_LEFT;
        static constexpr const std::int32_t down        = SDLK_DOWN;
        static constexpr const std::int32_t up          = SDLK_UP;

        static constexpr const std::uint8_t pressed = SDL_PRESSED;
        static constexpr const std::uint8_t released= SDL_RELEASED;

        event_keyboard( SDL_KeyboardEvent* event )
            : _state(event->state),
              _code(event->keysym.sym),
              _repeat(event->repeat)
        {}

        virtual std::int32_t kind_of_event() const override
        {
            return event::key;
        }

        std::int32_t type_of_event() const
        {
            if (this->_state == pressed)
                return event::key_down;
            else
                return event::key_up;
        }

        char ascci_code() const
        {
            if (this->_code > 255)
                return char(unknown);
            return char(this->_code);
        }

        std::int32_t code() const
        {
            return this->_code;
        }

        std::uint8_t state() const
        { return this->_state; }

        std::uint8_t repeat() const
        { return this->_repeat; }

    private:
        std::uint8_t _state;
        std::int32_t _code;
        std::uint8_t _repeat; // Nombre de fois que la touche a été enfoncé rapidement
    };
    // ---------------------------------------------------------------------------------------------
    class event_touch : public event
    {
    public:
        event_touch( SDL_TouchFingerEvent* evt)
            :   x(evt->x),
                y(evt->y),
                dx(evt->dx),
                dy(evt->dy),
                pressure(evt->pressure),
                type(evt->type)
        {}

        virtual std::int32_t kind_of_event() const override
        {
            return event::touch_finger;
        }

        std::int32_t type_of_event() const
        {
            return this->type;
        }

        std::array<float,2> where() const
        {
            return {this->x, this->y};
        }

        std::array<float,2> displacement() const
        {
            return {this->dx, this->dy};
        }

        float pression() const { return this->pressure; }

    private:
        float x,y; // Où se trouve le doigt (normalisé entre 0 et 1)
        float dx, dy; // Mouvement relatif (normalisé entre -1 et 1)
        float pressure; // La quantité de pression exercée par le doigt (entre 0 et 1)
        std::int32_t type;
    };
    // =============================================================================================
    class event_queue
    {
    public:

        event_queue() = default;

        std::vector<std::unique_ptr<event>> pull_events() const;
    };
}

#endif
