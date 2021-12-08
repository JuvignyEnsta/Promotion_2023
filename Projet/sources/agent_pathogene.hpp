#ifndef _EPIDEMIE_AGENT_PATHOGENE_HPP_
#define _EPIDEMIE_AGENT_PATHOGENE_HPP_
#include <random>
#include <utility>

namespace épidémie 
{
class AgentPathogène 
{
public:
    AgentPathogène( unsigned int graine_aléatoire );


    int nombreJoursAsymptomatique()
    {
        return int(m_générateur_symptomatique(m_moteur_stochastique));
    }

    int nombreJoursSymptomatique() 
    {
        return int(m_générateur_symptomatique(m_moteur_stochastique));
    }

    /* Paramètres de l'agent pathogène avec valeurs par défaut */
    double taux_infection;
    double taux_cas_rapporté = 1.;
    std::pair<int,int> période_contagieux = {0, 2}; // en jours : 1er chiffre apr_ès contagion, second chiffre : après symptomatique
    int    nombre_jours_immunité = 0; // Si différent de zéro, individus développent une immunité pendant un certain nombre de jours

    double theta_BA          = 1.; // Paramètre d'interaction avec la grippe :    
private:
    std::default_random_engine      m_moteur_stochastique      ;
    std::gamma_distribution<double> m_générateur_asymptomatique;
    std::gamma_distribution<double> m_générateur_symptomatique ;
};
}
#endif