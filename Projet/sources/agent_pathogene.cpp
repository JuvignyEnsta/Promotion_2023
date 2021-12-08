#include "agent_pathogene.hpp"

épidémie::AgentPathogène::AgentPathogène( unsigned int graine_aléatoire )
    :  taux_infection(0.10),
        m_moteur_stochastique(graine_aléatoire),
        m_générateur_asymptomatique(21.*21./25., 25./21.),
        m_générateur_symptomatique (144./16., 16./12.)
{
}
