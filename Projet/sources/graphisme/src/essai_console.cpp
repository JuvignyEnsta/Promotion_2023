#include <iomanip>
#include <chrono>
#include <array>
#include <random>
#include <algorithm>
#include <cassert>
#include "ansi/console.hpp"

int main(int nargs, char* argv[])
{
    console::init();
    console::couleur couleur_fond(console::couleur::bleu);
    console::cls(couleur_fond);
    std::cout << console::couleur(console::couleur::jaune)
              << console::curseur({1,17}) //ligne, colonne 
              << console::style(console::style::double_taille, "ANSI ++")
              << console::curseur({3,12})
              << console::style(console::style::double_largeur, "Bibliothèque C++")
              << std::endl;

    console::restaure();
/*    std::cout << console::cls() << console::couleur(console::blanc,console::rouge) << "Hello world !";
    std::cout << console::couleur(console::azure,console::defaut);
    for ( int i = 1; i < 8; ++i )
        std::cout << console::curseur(i+1,i+1) << "Hello world !";

    for ( int i = 0; i < 256; ++i )
        std::cout << console::couleur(i,-1) << console::curseur(i/16 + 11, (5*i)%80) << std::setw(4) << i;

    con.restaure();

    std::cout << console::curseur(30, 1) << console::style(console::gras) << "Texte en gras "
              << console::style(console::normal) << console::style(console::souligné)
              << "Texte souligne " << console::style(console::normal) 
              << console::style(console::inverse) << "Texte inverse"
              << console::style(console::normal) << std::endl;

    // Fonction lambda permettant d'attendre un certain temps (passé en argument)
    auto sleep = [] (double dt) 
    {         
        auto deb = std::chrono::high_resolution_clock::now();
        auto fin = deb;
        std::chrono::duration<double> diff = fin-deb;
        while (diff.count() < dt)// Attend dt secondes
        {
            fin  = std::chrono::high_resolution_clock::now();
            diff = fin-deb;
        }
    };

    std::cout << console::curseur(31,1) << "Loading...     ";
    for ( int i = 0; i <= 100; ++i )
    {
        std::cout << console::curseur(console::gauche,4) << std::setw(3) << i << "%" << std::flush;
        sleep(0.1);
    }
    std::cout << console::cls();
    // Affiche maintenant quatre barres de progression. A chaque itération, une des quatre barres progresse
    // On arrete lorsque les quatre barres sont pleines :
    std::array barres{0.,0.,0.,0.};// Les quatres barres : valeurs allant de 0. à 1.
    std::random_device rd;
    std::uniform_int_distribution<int> distribution(0,3);// Tire au hasard un nombre entre 0 et 3 (compris, distribution uniforme)
    auto somme = std::accumulate(barres.begin(), barres.end(), 0.);
    std::cout << console::curseur(console::caché) << console::style(console::invisible) <<  "Multi loading\n\n\n\n\n";
    while (somme < 4.)
    {        
        std::cout << console::curseur(console::haut,4) << console::curseur(console::gauche,1000);
        // Affichage des quatre barres :
        // =========================================================================================
        for ( int i = 0; i < 4; ++i )
        {
            int ibarres = int(100*barres[i]);
            int rbarres = int(ibarres%4);
            std::cout << "[";
            for ( int j = 0; j < ibarres/4; ++j )
                std::cout << console::couleur(console::defaut, 255) << " ";
            if (rbarres > 0)
            {
                std::cout << console::couleur(console::defaut, 240 + 5*rbarres) << " ";
                std::cout << console::couleur(console::defaut);
                for ( int j = ibarres/4+1; j < 25; ++j) std::cout << " ";
            }
            else
            {
                std::cout << console::couleur(console::defaut);
                for ( int j = ibarres/4; j < 25; ++j) std::cout << " ";
            }
            std::cout << "]" << std::endl;
        }
        std::cout << std::flush;
        // Incrémentation au hasard d'une des quatre barres de 0.01 (1%) :
        int ind = distribution(rd);
        assert(ind>=0); assert(ind<barres.size());
        if (barres[ind] < 1.)
            barres[ind] += 0.01;
        barres[ind] = std::min(1.,barres[ind]);
        somme = std::accumulate(barres.begin(), barres.end(), 0.);
        sleep(0.01);
    }
    std::cout << console::couleur(console::defaut,console::noir);
    std::cout << console::cls() << console::curseur(2,20) << console::couleur(console::azure_brillant, console::bleu);
    std::cout << console::style(console::double_taille, "Hello world !") << std::endl;
    std::cout << console::curseur(5,20) << console::couleur(console::jaune_brillant);
    std::cout << console::style(console::double_largeur, "Salut le monde !") << std::endl;
    std::cout << console::couleur(console::jaune) << "Bin quoi, c'est pas mieux en francais ?" << std::endl;
    std::cout << console::couleur(console::jaune) << console::style(console::gras) << "Bin quoi, c'est pas mieux en francais ?" << std::endl;
    std::cout << console::couleur(console::jaune) << console::style(console::maigre) << "Bin quoi, c'est pas mieux en francais ?" << std::endl;
    std::cout << console::couleur(console::jaune) << console::style(console::fonte1) << "Bin quoi, c'est pas mieux en francais ?" << std::endl;
    std::cout << console::style(console::fonte2) << "Bin quoi, c'est pas mieux en francais ?" << std::endl;
    std::cout << console::style(console::fonte3) << "Bin quoi, c'est pas mieux en francais ?" << std::endl;
    std::cout << console::style(console::fonte4) << "Bin quoi, c'est pas mieux en francais ?" << std::endl;
    std::cout << console::style(console::fonte5) << "Bin quoi, c'est pas mieux en francais ?" << std::endl;
    std::cout << console::style(console::fonte6) << "Bin quoi, c'est pas mieux en francais ?" << std::endl;
    std::cout << console::style(console::fonte7) << "Bin quoi, c'est pas mieux en francais ?" << std::endl;
    std::cout << console::style(console::fonte8) << "Bin quoi, c'est pas mieux en francais ?" << std::endl;
    std::cout << console::style(console::fonte9) << "Bin quoi, c'est pas mieux en francais ?" << std::endl;
    std::cout << console::style(console::fonte_par_defaut) << "Bin quoi, c'est pas mieux en francais ?" << std::endl;*/
    return EXIT_SUCCESS;
}
