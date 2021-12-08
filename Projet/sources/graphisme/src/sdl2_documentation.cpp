#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <algorithm>
#include "SDL2/sdl2.hpp"
#include "SDL2/formated_text.hpp"


void lecture_document(sdl2::window& fenêtre, const std::string& fich_doc)
{
#   if defined(DEBUG)
    std::cout << __PRETTY_FUNCTION__ << std::endl << std::flush;
#   endif
    std::size_t num_ligne = 0;
    sdl2::formated_text fmt_text(fenêtre, "./data");

    fmt_text.load_simplified_markdown(fich_doc);
    fmt_text.begin_at_line(fenêtre, num_ligne);

    bool quitting = false;
    sdl2::event_queue queue;
    bool mouse_button_pressed = false;
    bool is_left_button = false;
    bool key_pressed = false;
    std::int32_t current_code_of_key = 0;
    while (not quitting)
    {
        auto events = queue.pull_events();
        for ( const auto& e : events)
        {
            if (e->kind_of_event() == sdl2::event::quit)
                quitting = true;
            if (e->kind_of_event() == sdl2::event::key)
            {
                auto& key_ev = dynamic_cast<sdl2::event_keyboard&>(*e);
                if (key_ev.type_of_event() == sdl2::event::key_up)
                    key_pressed = false;
                if (key_ev.type_of_event() == sdl2::event::key_down)
                {
                    current_code_of_key = 0;
                    key_pressed = false;
                    switch(key_ev.code())
                    {
                    case sdl2::event_keyboard::down:
                    case sdl2::event_keyboard::up:
                    case sdl2::event_keyboard::pageup:
                    case sdl2::event_keyboard::pagedown:
                        key_pressed = true;
                        current_code_of_key = key_ev.code();
                        break;
                    case sdl2::event_keyboard::home:
                        num_ligne = 0;
                        fenêtre.cls({0,0,0,0xFF});
                        fmt_text.begin_at_line(fenêtre, num_ligne);                    
                        break;
                    case sdl2::event_keyboard::end:
                        num_ligne = std::max(fmt_text.nb_lines()-25,std::size_t(0));
                        fenêtre.cls({0,0,0,0xFF});
                        fmt_text.begin_at_line(fenêtre, num_ligne);                    
                        break;
                    default:
                        if (key_ev.ascci_code() == 'q')
                        {
                            quitting = true;
                        }
                    }
                }
            }
            if (key_pressed)
            {
                switch(current_code_of_key)
                {
                case sdl2::event_keyboard::down:
                    num_ligne = std::min(num_ligne+1, fmt_text.nb_lines()-1);
                    fenêtre.cls({0,0,0,0xFF});
                    fmt_text.begin_at_line(fenêtre, num_ligne);
                    break;
                case sdl2::event_keyboard::up:
                    num_ligne = std::max(num_ligne, std::size_t(1))-1;
                    fenêtre.cls({0,0,0,0xFF});
                    fmt_text.begin_at_line(fenêtre, num_ligne);
                    break;
                case sdl2::event_keyboard::pageup:
                    num_ligne = std::max(num_ligne, std::size_t(25))-25;
                    fenêtre.cls({0,0,0,0xFF});
                    fmt_text.begin_at_line(fenêtre, num_ligne);
                    break;
                case sdl2::event_keyboard::pagedown:
                    num_ligne = std::min(num_ligne+25, fmt_text.nb_lines()-1);
                    fenêtre.cls({0,0,0,0xFF});
                    fmt_text.begin_at_line(fenêtre, num_ligne);
                    break;
                }
            }
            if (e->kind_of_event() == sdl2::event::mouse_button)
            {
                auto& mouse_ev = dynamic_cast<sdl2::event_mouse_button&>(*e);
                if (mouse_ev.state() == sdl2::event_mouse_button::pressed)
                {
                    mouse_button_pressed = true;             
                    if (mouse_ev.button() == sdl2::event_mouse_button::left)
                        is_left_button = true;
                    else if (mouse_ev.button() == sdl2::event_mouse_button::right)
                        is_left_button = false;
                    else
                        mouse_button_pressed = false;
                }
                else if (mouse_ev.state() == sdl2::event_mouse_button::released)
                    mouse_button_pressed = false;
            }
            // Gestion de l'écran tactil :
            if (e->kind_of_event() == sdl2::event::touch_finger)
            {
                auto& touch_ev = dynamic_cast<sdl2::event_touch&>(*e);
                if (touch_ev.pression() > 0.f)
                {
                    auto [dx,dy] = touch_ev.displacement();
                    if (dy > 0)
                    {
                        std::size_t delta_ligne = std::size_t(50*dy);
                        num_ligne = std::max(num_ligne, delta_ligne)-delta_ligne;
                        fenêtre.cls({0,0,0,0xFF});
                        fmt_text.begin_at_line(fenêtre, num_ligne);
                    }
                    if (dy < 0)
                    {
                        std::size_t delta_ligne = std::size_t(-50*dy);
                        num_ligne = std::min(num_ligne + delta_ligne, fmt_text.nb_lines()-1);
                        fenêtre.cls({0,0,0,0xFF});
                        fmt_text.begin_at_line(fenêtre, num_ligne);                        
                    }
                }
            }
            if (mouse_button_pressed)
            {
                mouse_button_pressed = false;
                if (is_left_button)
                {
                    num_ligne = std::min(num_ligne+5, fmt_text.nb_lines()-1);
                    fenêtre.cls({0,0,0,0xFF});
                    fmt_text.begin_at_line(fenêtre, num_ligne);
                }
                else
                {
                    num_ligne = std::max(num_ligne, std::size_t(5))-5;
                    fenêtre.cls({0,0,0,0xFF});
                    fmt_text.begin_at_line(fenêtre, num_ligne);
                }    
            }
        }
        fenêtre << fmt_text << sdl2::flush;
    }
}

int main(int argc, char* args[])
{
#   if defined(DEBUG)
    std::cerr << __PRETTY_FUNCTION__ << std::endl << std::flush;
#   endif
    sdl2::init(argc, args);
    {
        sdl2::window écran("Documentation de SDL2 ++", {1280,1024});

        lecture_document(écran, "SDL2_documentation.md");
    }
    sdl2::finalize();
    return EXIT_SUCCESS;
}