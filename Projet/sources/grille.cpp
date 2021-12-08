#include "grille.hpp"

épidémie::Grille::Grille( std::size_t population )
{
    int taille = int(sqrt(population));
    m_dim_x = (population+taille-1)/taille;
    m_dim_y = taille;
    std::vector<StatistiqueParCase>(m_dim_x*m_dim_y).swap(m_statistiques);
}
//
std::size_t 
épidémie::Grille::nombreTotalContaminésGrippe() const
{
    std::size_t total = 0;
    for (auto statistique : m_statistiques)
    {
        total += statistique.nombre_contaminant_grippé_et_contaminé_par_agent;
        total += statistique.nombre_contaminant_seulement_grippé;
    }
    return total;
}
//
std::size_t 
épidémie::Grille::nombreTotalContaminésAgentPathogène() const
{
    std::size_t total = 0;
    for (auto statistique : m_statistiques )
    {
        total += statistique.nombre_contaminant_seulement_contaminé_par_agent;
        total += statistique.nombre_contaminant_grippé_et_contaminé_par_agent;
    }
    return total;
}
//
