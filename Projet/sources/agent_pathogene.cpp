#include "agent_pathogene.hpp"

epidemie::AgentPathogene::AgentPathogene( unsigned int graine_aleatoire )
    :  taux_infection(0.10),
        m_moteur_stochastique(graine_aleatoire),
        m_generateur_asymptomatique(21.*21./25., 25./21.),
        m_generateur_symptomatique (144./16., 16./12.)
{
}
