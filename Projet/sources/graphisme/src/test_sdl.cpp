#include "SDL2/sdl2.hpp"
#include <iostream>

void creer_fenetre()
{
    sdl2::font fonte_titre("./data/Lato-Bold.ttf", 28);
    // Pour choisir une fonte courante pour afficher du texte (pas encore mis en oeuvre pour l'affichage du texte)
    sdl2::font fonte_texte("./data/Lato-Thin.ttf", 18);

    // Créer une fenêtre avec un titre et la résolution de la fenêtre (ici 800 x 600 pixels )
    sdl2::window fenêtre("Nuclear Alert : Console principale", {800, 600});

    // Charge une image jpg (on peut aussi charger une image au format png)
    sdl2::image homer("data/homer.jpg", fenêtre);
    homer.at(310,110);
    homer.stretch({128,128});
    homer.flip(sdl2::image::horizontal);
    homer.rotate({64,64}, 45);
    homer.color_filter({0xFF,0xF0,0xA0,0xFF});

    sdl2::texte titre ("Console d'application élaborée", fonte_titre, fenêtre, {0x00,0xFF,0xFF,0xFF});
    titre.at(10,310);
    auto [wph, hph] = titre.dimension();
    titre.rotate({wph/2,hph/2}, 30);

    sdl2::texte phrase("Taux de bore détecté : 30%", fonte_texte, fenêtre, {0xFF,0xFF,0xFF,0xFF});
    phrase.at(10,360);

    sdl2::texte doc("Ceci est de la documentation très poussée. On peut faire des sauts de ligne, etc.\n"
                    "Comme on peut le voir ici !", fonte_texte, fenêtre, 320, 200, {0xFF,0xFF,0x00,0xFF},
                    {0x00,0x00,0xFF,0xFF});
    doc.at(400, 360);
    doc.rotate({160,100}, 5);
    bool quitting = false;
    sdl2::event_queue queue;
    int xinput = 100, yinput = 400;
    char buffername[255];
    buffername[0] = '\0';
    int posbuffer = 0;
    bool iskey_down = false;
    while (not quitting)
    {
        // On utilise des flux pour afficher le graphisme
        // Affichage d'un rectangle plein avec coin
        // 0 - 9 A = 10, B = 11, C= 12, D = 13, E = 14, F = 15 en base 16
        // 0x  => Ecriture hexadécimale (en base 16) 0x3E = 3*16 + 14 = 62 en décimal
        // 0b  => Ecriture binaire 0b100010 => écriture binaire = 2^{1} + 2^{5} = 33 en décimal
        // 
        // Donc 0xAB = 10*16 + 11 = 171 en décimal
        //      0xFF = 15*16 + 15 = 255 en décimal
        fenêtre << sdl2::rectangle({100,100}, {100,50}, {0,0xFF,0xFF,0xFF}, true);
        // Affichage d'un segment de droite
        fenêtre << sdl2::line({10,10}, {50,30}, {0xFF,0x00,0x00,0xFF});
        // Affichage d'un polygone.
        fenêtre << sdl2::polygon({ std::array{200,200}, {250,250}, {200,300}, {150,250} }, {0xFF,0xFF,0x00,0xFF})
                << homer << titre << phrase << doc;
        if (posbuffer > 0)
        {
            sdl2::texte name(buffername, fonte_texte, fenêtre, {0xFF,0xFF,0xFF,0xFF});
            name.at(xinput,yinput);
            fenêtre << name;
        }
        fenêtre << sdl2::flush;
        auto events = queue.pull_events();
        for ( const auto& e : events)
        {
            if (e->kind_of_event() == sdl2::event::quit)
                quitting = true;
            if ( (e->kind_of_event() == sdl2::event::key_down) ||
                 (e->kind_of_event() == sdl2::event::key_up) )
            {
                auto& key_ev = dynamic_cast<sdl2::event_keyboard&>(*e);

                if ( (key_ev.type_of_event() == sdl2::event::key_down) &&  (iskey_down == false) )
                {
                    char keychar = key_ev.ascci_code();
                    if ( (keychar >= 'a') && (keychar <= 'z') )
                    {
                        if (posbuffer < 254)
                        {
                            buffername[posbuffer  ] = keychar;
                            buffername[posbuffer+1] = '\0';
                            posbuffer ++;
                        }
                    }
                    iskey_down = true;
                }
                if (key_ev.type_of_event() == sdl2::event::key_up)
                {
                    iskey_down = false;
                }
            }
        }
    }
}

int main(int argc, char* args[])
{
    // Initialisation de la librairie SDL 2 (System Direct Layer 2)
    sdl2::init(argc, args);

    creer_fenetre();

    sdl2::finalize();

    return EXIT_SUCCESS;
}