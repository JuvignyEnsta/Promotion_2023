#include <cassert>
#include <tuple>
#include <iostream>
#include "individu.hpp"

epidemie::Individu::Individu( unsigned int graine_aleatoire, int esperance_de_vie, bool estNe, int deplacement_max )
    :   m_age{}, 
        m_esperance_de_vie{esperance_de_vie},
        m_position{},
        m_moteur_stochastique{graine_aleatoire},
        m_generateur_quantite_deplacement(1,deplacement_max),
        m_generateur_deplacement(1,4),
        m_generateur_maladie(0.,1.),
        m_generateur_age(1,esperance_de_vie)
{
    if (estNe)
    {
        m_age = 1;
    }
    else m_age = m_generateur_age(m_moteur_stochastique);

    m_grippe.sensibilite = Sensibilite::Sensible;
    m_agent_pathogene.sensibilite = Sensibilite::Sensible;
}
// --------------------------------------------------------------------------------------------------------------------
void 
epidemie::Individu::veillirDUnJour()
{
    // Gestion de la grippe :
    if (m_grippe.temps_incubation > 0)
    {
        m_grippe.sensibilite = Sensibilite::Contagieux;
        m_grippe.temps_incubation -= 1;
    }
    else if (m_grippe.temps_symptomatique > 0)
    {
        m_grippe.temps_symptomatique -= 1;
        m_grippe.temps_contagieux = m_grippe.temps_symptomatique+2;
    }
    else if (m_grippe.temps_contagieux > 0)
    {
        m_grippe.temps_contagieux -= 1;
    }
    else if (m_grippe.sensibilite != Sensibilite::Sensible )
    {
        m_grippe.sensibilite = Sensibilite::Immunise;
    }

    // Gestion de l'agent pathogene :
    if (m_agent_pathogene.temps_asymptomatique > 0)
    {
        // Normalement, dans ce cas, l'individu est deja contagieux
        assert(m_agent_pathogene.sensibilite == Sensibilite::Contagieux);
        m_agent_pathogene.temps_asymptomatique -= 1;
        m_agent_pathogene.temps_contagieux     -= 1;
        ;
    }
    else if (m_agent_pathogene.temps_symptomatique > 0)
    {
        m_agent_pathogene.sensibilite = Sensibilite::Infecte;
        m_agent_pathogene.temps_symptomatique -= 1;
        m_agent_pathogene.temps_contagieux    -= 1;
    }
    else if (m_agent_pathogene.temps_contagieux > 0)
    {
        m_agent_pathogene.sensibilite = Sensibilite::Contagieux;
        m_agent_pathogene.temps_contagieux -= 1;
    }
    else if (m_agent_pathogene.temps_passe_immunise > 0)
    {
        m_agent_pathogene.sensibilite = Sensibilite::Immunise;
        m_agent_pathogene.temps_passe_immunise -= 1;
    }
    else 
    {
        m_agent_pathogene.sensibilite = Sensibilite::Sensible;
    }
    m_age += 1;
}
// --------------------------------------------------------------------------------------------------------------------
void epidemie::Individu::seDeplace( Grille& grille )
{
    enum Direction {
        Ouest = 1, Est = 2, Nord = 3, Sud = 4
    };
    int nbre_deplacement = m_generateur_quantite_deplacement(m_moteur_stochastique);
    for ( int i = 0; i < nbre_deplacement; ++i )
    {
        int direction = m_generateur_deplacement(m_moteur_stochastique);
        switch(direction)
        {
            case Ouest:
                m_position.x -= 1;
                break;
            case Est:
                m_position.x += 1;
                break;
            case Nord:
                m_position.y += 1;
                break;
            case Sud:
                m_position.y -= 1;
                break;
        };
        auto [largeur, hauteur] = grille.dimension();
        if (m_position.x < 0) m_position.x = largeur-1;
        if (m_position.x >= largeur) m_position.x = 0;
        if (m_position.y < 0) m_position.y = hauteur-1;
        if (m_position.y >= hauteur) m_position.y = 0;
    }
}
// --------------------------------------------------------------------------------------------------------------------
void 
epidemie::Individu::estContamine( Grippe& grippe )
{
    if (m_grippe.sensibilite == Sensibilite::Sensible)
    {
        m_grippe.temps_incubation = grippe.nombreJoursIncubation();
        m_grippe.temps_symptomatique = grippe.nombreJoursSymptomatique();
    }
}
// --------------------------------------------------------------------------------------------------------------------
void 
epidemie::Individu::estContamine( AgentPathogene& agent )
{
    if (m_agent_pathogene.sensibilite == Sensibilite::Sensible )
    {
        m_agent_pathogene.temps_asymptomatique = agent.nombreJoursAsymptomatique();
        m_agent_pathogene.temps_symptomatique  = agent.nombreJoursSymptomatique();
        m_agent_pathogene.temps_passe_immunise = agent.nombre_jours_immunite;
        m_agent_pathogene.temps_contagieux     = m_agent_pathogene.temps_asymptomatique+m_agent_pathogene.temps_symptomatique +
                                                 agent.periode_contagieux.second;
        m_agent_pathogene.sensibilite          = Sensibilite::Contagieux;
    }
}
// --------------------------------------------------------------------------------------------------------------------
bool 
epidemie::Individu::testContaminationGrippe( Grille const& grille, Interactions const& t_interactions, 
                                             Grippe& grippe, AgentPathogene& agent)
{
    auto [largeur, hauteur] = grille.dimension();
    Grille::StatistiqueParCase const& statistiques = grille.getStatistiques()[m_position.x + m_position.y*largeur];


    // Test infection par la grippe avec interaction avec l'agent pathogene 
    // Autant de chance de l'attraper que d'individu ayant le virus de la grippe :
    // On traite d'abord les contaminants ayant le virus mais pas l'agent pathogene :
    double probabilite_attraper_grippe = t_interactions.tauxTransmissionGrippe(grippe.tauxTransmission(), false,
                                                                               this->m_agent_pathogene.sensibilite == Sensibilite::Infecte,
                                                                               this->m_agent_pathogene.sensibilite == Sensibilite::Immunise);
    for ( int i = 0; i < statistiques.nombre_contaminant_seulement_grippe; ++i )
    {
        if (m_generateur_maladie(m_moteur_stochastique) < probabilite_attraper_grippe)
        {
            return true;
        }
    }
    probabilite_attraper_grippe = t_interactions.tauxTransmissionGrippe(grippe.tauxTransmission(), true,
                                                                        this->m_agent_pathogene.sensibilite == Sensibilite::Infecte,
                                                                        this->m_agent_pathogene.sensibilite == Sensibilite::Immunise);
    for ( int i = 0; i < statistiques.nombre_contaminant_grippe_et_contamine_par_agent; ++i )
    {
        if (m_generateur_maladie(m_moteur_stochastique) < probabilite_attraper_grippe)
        {
            return true;
        }
    }
    return false;
}
//
bool 
epidemie::Individu::testContaminationAgent( Grille const& grille, AgentPathogene& agent)
{
    if (this->m_agent_pathogene.temps_contagieux > 0) return false;
    auto [largeur, hauteur] = grille.dimension();
    Grille::StatistiqueParCase const& statistiques = grille.getStatistiques()[m_position.x + m_position.y*largeur];

    for ( int i = 0; i < statistiques.nombre_contaminant_seulement_contamine_par_agent; ++i )
    {
        double jet = m_generateur_maladie(m_moteur_stochastique);
        if (jet < agent.taux_infection)
        {
            return true;
        }
    }
    for ( int i = 0; i < statistiques.nombre_contaminant_grippe_et_contamine_par_agent; ++i )
    {
        double jet = m_generateur_maladie(m_moteur_stochastique);
        if (jet < agent.taux_infection)
        {
            return true;
        }
    }
    return false;
}
//
void
epidemie::Individu::setPosition(int largeur, int hauteur)
{
    m_position.x = int(m_generateur_maladie(m_moteur_stochastique)*largeur);
    m_position.y = int(m_generateur_maladie(m_moteur_stochastique)*hauteur);
}
