#ifndef _EPIDEMIE_AGENT_PATHOGENE_HPP_
#define _EPIDEMIE_AGENT_PATHOGENE_HPP_
#include <random>
#include <utility>

namespace epidemie 
{
class AgentPathogene 
{
public:
    AgentPathogene( unsigned int graine_aleatoire );


    int nombreJoursAsymptomatique()
    {
        return int(m_generateur_symptomatique(m_moteur_stochastique));
    }

    int nombreJoursSymptomatique() 
    {
        return int(m_generateur_symptomatique(m_moteur_stochastique));
    }

    /* Parametres de l'agent pathogene avec valeurs par defaut */
    double taux_infection;
    double taux_cas_rapporte = 1.;
    std::pair<int,int> periode_contagieux = {0, 2}; // en jours : 1er chiffre apr_es contagion, second chiffre : apres symptomatique
    int    nombre_jours_immunite = 0; // Si different de zero, individus developpent une immunite pendant un certain nombre de jours

    double theta_BA          = 1.; // Parametre d'interaction avec la grippe :    
private:
    std::default_random_engine      m_moteur_stochastique      ;
    std::gamma_distribution<double> m_generateur_asymptomatique;
    std::gamma_distribution<double> m_generateur_symptomatique ;
};
}
#endif