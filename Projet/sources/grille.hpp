#ifndef _EPIDEMIE_GRILLE_HPP_
#define _EPIDEMIE_GRILLE_HPP_
#include <cstdint>
#include <array>
#include "contexte.hpp"

namespace epidemie 
{
class Grille 
{
public:
    struct StatistiqueParCase
    {
        int nombre_contaminant_seulement_grippe;
        int nombre_contaminant_seulement_contamine_par_agent;
        int nombre_contaminant_grippe_et_contamine_par_agent;
    };
    // Taille de la grille depend de la taille de la population
    Grille( std::size_t population );

    std::vector<StatistiqueParCase>& getStatistiques() { return m_statistiques; }

    std::vector<StatistiqueParCase> const& getStatistiques() const { return m_statistiques; }

    std::size_t nombreTotalContaminesGrippe() const;

    std::size_t nombreTotalContaminesAgentPathogene() const;

    std::array<int,2> dimension() const 
    {
        return {m_dim_x, m_dim_y};
    }



private:
    int m_dim_x, m_dim_y;
    std::vector<StatistiqueParCase> m_statistiques;
};
}

#endif