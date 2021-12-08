#ifndef _EPIDEMIE_CONTEXT_HPP_
#define _EPIDEMIE_CONTEXT_HPP_
#include <cstdint>
#include <random>
#include "interactions.hpp"

namespace épidémie 
{
struct ContexteGlobal 
{
    std::size_t taux_population     = 100'000;// Nombre de personnes sur terre
    std::size_t espérance_de_vie    = 29'200 ;// Nombre de jours que vit un individu (80 ans = 365 * 80 jours )
    int         déplacement_maximal = 10     ;
    Interactions interactions;
};
}
#endif