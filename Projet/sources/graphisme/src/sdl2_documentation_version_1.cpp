#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <algorithm>
#include "SDL2/sdl2.hpp"

std::vector<std::string> load_md_document(const std::string& path)
{
    char buffer[4096];
    std::ifstream fich(path.c_str());
    std::vector<std::string> output;
    while (fich)
    {
        fich.getline(buffer, 4096);
        output.emplace_back(buffer);
    }
    fich.close();
    return output;
}

void afficher_titre(sdl2::window& fenêtre, const std::string_view& titre)
{
    auto [w,h] = fenêtre.dimensions();
    std::uint16_t maxx = std::uint16_t(w)-1;
    sdl2::font fonte_titre("./data/Lato-Heavy.ttf", 38);
    sdl2::texte bannière (titre.data(), fonte_titre, fenêtre, 
                          w, 64, {0xFF,0xFF,0x00,0xFF}, {0x00,0x00,0xFF,0xFF},
                          sdl2::texte::middle_justify, 0, 11);
    fenêtre << bannière.at(0,0) << sdl2::line({0,64}, {maxx,64}, {0x00,0x00,0xA0,0xFF})
            << sdl2::line({maxx,64}, {maxx,0}, {0,0,0,255})
            << sdl2::line({0,0}, {maxx,0}, {0,0xA0,0xFF,0xFF})
            << sdl2::line({0,0}, {0,64}, {0,0xA0,0xFF,0xFF});
}

void affiche_document(sdl2::window& fenêtre, 
                      const std::vector<std::string>& doc, int première_ligne = 0)
{
    sdl2::font fonte_sec1("./data/Lato-Bold.ttf", 24);
    sdl2::font fonte_sec2("./data/Lato-Bold.ttf", 20);
    sdl2::font fonte_sec3("./data/Lato-Bold.ttf", 18);
    sdl2::font fonte_texte("./data/Lato-Regular.ttf", 16);
    int posy = 70;
    std::size_t index = première_ligne;
    auto [w,h] = fenêtre.dimensions();
    while ( (posy < h) && (index < doc.size()) )
    {
        sdl2::font* pt_fonte = &fonte_texte;
        std::string_view sub_string(doc[index]);
        int marge = 20;
        sdl2::color encre{0xFF,0xFF,0xFF,0xFF};
        if (std::string_view(doc[index].begin(), doc[index].begin()+3) == "## ")
        {
            pt_fonte = &fonte_sec1;
            sub_string = std::string_view(doc[index].data()+3);
            marge = 4;
            encre = {0xFF,0xFF,0xA0,0xFF};
        }
        if (std::string_view(doc[index].begin(), doc[index].begin()+4) == "### ")
        {
            pt_fonte = &fonte_sec2;
            sub_string = std::string_view(doc[index].data()+4);
            marge = 9;
            encre = {0xA0,0xFF,0xFF,0xFF};
        }
        if (std::string_view(doc[index].begin(), doc[index].begin()+5) == "#### ")
        {
            pt_fonte = &fonte_sec3;
            sub_string = std::string_view(doc[index].data()+5);
            marge = 14;
            encre = {0xFF,0x00,0x00,0xFF};
        }
        sdl2::texte texte(sub_string.data(), *pt_fonte, fenêtre, w, encre,
                          {0x00,0x00,0x00,0x00}, sdl2::texte::left_justify, 0);
        fenêtre << texte.at(marge, posy);
        posy += texte.dimension()[1] + 5;
        index += 1;
    }
}

void lecture_document(sdl2::window& fenêtre, const std::vector<std::string>& doc)
{
    std::size_t num_ligne = 1;
    std::string_view titre_view(doc[0].data()+2);
    afficher_titre(fenêtre, titre_view);

    affiche_document(fenêtre, doc, num_ligne);
    bool quitting = false;
    sdl2::event_queue queue;
    while (not quitting)
    {
        auto events = queue.pull_events();
        for ( const auto& e : events)
        {
            if (e->kind_of_event() == sdl2::event::quit)
                quitting = true;
            if (e->kind_of_event() == sdl2::event::key_down)
            {
                auto& key_ev = dynamic_cast<sdl2::event_keyboard&>(*e);
                if (key_ev.code() == sdl2::event_keyboard::down)
                {
                    num_ligne = std::min(num_ligne+1, doc.size()-1);
                    fenêtre.cls({0,0,0,0xFF});
                    afficher_titre(fenêtre, titre_view);
                    affiche_document(fenêtre, doc, num_ligne);
                }
                if (key_ev.code() == sdl2::event_keyboard::up)
                {
                    num_ligne = std::max(num_ligne-1, 1ULL);
                    fenêtre.cls({0,0,0,0xFF});
                    afficher_titre(fenêtre, titre_view);
                    affiche_document(fenêtre, doc, num_ligne);
                }
                if (key_ev.ascci_code() == 'q')
                {
                    quitting = true;
                }
            }
        }
        fenêtre << sdl2::flush;
    }
}

int main(int argc, char* args[])
{
    sdl2::init(argc, args);
    {
        auto doc = load_md_document("SDL2_documentation.md");
        sdl2::window écran("Documentation de SDL2 ++", {1280,1024});
        lecture_document(écran, doc);
    }
    sdl2::finalize();
    return EXIT_SUCCESS;
}