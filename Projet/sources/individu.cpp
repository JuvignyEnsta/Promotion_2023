#include <cassert>
#include <tuple>
#include <iostream>
#include "individu.hpp"

épidémie::Individu::Individu( unsigned int graine_aléatoire, int espérance_de_vie, bool estNé, int déplacement_max )
    :   m_âge{}, 
        m_espérance_de_vie{espérance_de_vie},
        m_position{},
        m_moteur_stochastique{graine_aléatoire},
        m_générateur_quantité_déplacement(1,déplacement_max),
        m_générateur_déplacement(1,4),
        m_générateur_maladie(0.,1.),
        m_générateur_âge(1,espérance_de_vie)
{
    if (estNé)
    {
        m_âge = 1;
    }
    else m_âge = m_générateur_âge(m_moteur_stochastique);

    m_grippe.sensibilité = Sensibilité::Sensible;
    m_agent_pathogène.sensibilité = Sensibilité::Sensible;
}
// --------------------------------------------------------------------------------------------------------------------
void 
épidémie::Individu::veillirDUnJour()
{
    // Gestion de la grippe :
    if (m_grippe.temps_incubation > 0)
    {
        m_grippe.sensibilité = Sensibilité::Contagieux;
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
    else if (m_grippe.sensibilité != Sensibilité::Sensible )
    {
        m_grippe.sensibilité = Sensibilité::Immunisé;
    }

    // Gestion de l'agent pathogène :
    if (m_agent_pathogène.temps_asymptomatique > 0)
    {
        // Normalement, dans ce cas, l'individu est déjà contagieux
        assert(m_agent_pathogène.sensibilité == Sensibilité::Contagieux);
        m_agent_pathogène.temps_asymptomatique -= 1;
        m_agent_pathogène.temps_contagieux     -= 1;
        ;
    }
    else if (m_agent_pathogène.temps_symptomatique > 0)
    {
        m_agent_pathogène.sensibilité = Sensibilité::Infecté;
        m_agent_pathogène.temps_symptomatique -= 1;
        m_agent_pathogène.temps_contagieux    -= 1;
    }
    else if (m_agent_pathogène.temps_contagieux > 0)
    {
        m_agent_pathogène.sensibilité = Sensibilité::Contagieux;
        m_agent_pathogène.temps_contagieux -= 1;
    }
    else if (m_agent_pathogène.temps_passé_immunisé > 0)
    {
        m_agent_pathogène.sensibilité = Sensibilité::Immunisé;
        m_agent_pathogène.temps_passé_immunisé -= 1;
    }
    else 
    {
        m_agent_pathogène.sensibilité = Sensibilité::Sensible;
    }
    m_âge += 1;
}
// --------------------------------------------------------------------------------------------------------------------
void épidémie::Individu::seDéplace( Grille& grille )
{
    enum Direction {
        Ouest = 1, Est = 2, Nord = 3, Sud = 4
    };
    int nbre_déplacement = m_générateur_quantité_déplacement(m_moteur_stochastique);
    for ( int i = 0; i < nbre_déplacement; ++i )
    {
        int direction = m_générateur_déplacement(m_moteur_stochastique);
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
épidémie::Individu::estContaminé( Grippe& grippe )
{
    if (m_grippe.sensibilité == Sensibilité::Sensible)
    {
        m_grippe.temps_incubation = grippe.nombreJoursIncubation();
        m_grippe.temps_symptomatique = grippe.nombreJoursSymptomatique();
    }
}
// --------------------------------------------------------------------------------------------------------------------
void 
épidémie::Individu::estContaminé( AgentPathogène& agent )
{
    if (m_agent_pathogène.sensibilité == Sensibilité::Sensible )
    {
        m_agent_pathogène.temps_asymptomatique = agent.nombreJoursAsymptomatique();
        m_agent_pathogène.temps_symptomatique  = agent.nombreJoursSymptomatique();
        m_agent_pathogène.temps_passé_immunisé = agent.nombre_jours_immunité;
        m_agent_pathogène.temps_contagieux     = m_agent_pathogène.temps_asymptomatique+m_agent_pathogène.temps_symptomatique +
                                                 agent.période_contagieux.second;
        m_agent_pathogène.sensibilité          = Sensibilité::Contagieux;
    }
}
// --------------------------------------------------------------------------------------------------------------------
bool 
épidémie::Individu::testContaminationGrippe( Grille const& grille, Interactions const& t_interactions, 
                                             Grippe& grippe, AgentPathogène& agent)
{
    auto [largeur, hauteur] = grille.dimension();
    Grille::StatistiqueParCase const& statistiques = grille.getStatistiques()[m_position.x + m_position.y*largeur];


    // Test infection par la grippe avec interaction avec l'agent pathogène 
    // Autant de chance de l'attraper que d'individu ayant le virus de la grippe :
    // On traite d'abord les contaminants ayant le virus mais pas l'agent pathogène :
    double probabilité_attraper_grippe = t_interactions.tauxTransmissionGrippe(grippe.tauxTransmission(), false,
                                                                               this->m_agent_pathogène.sensibilité == Sensibilité::Infecté,
                                                                               this->m_agent_pathogène.sensibilité == Sensibilité::Immunisé);
    for ( int i = 0; i < statistiques.nombre_contaminant_seulement_grippé; ++i )
    {
        if (m_générateur_maladie(m_moteur_stochastique) < probabilité_attraper_grippe)
        {
            return true;
        }
    }
    probabilité_attraper_grippe = t_interactions.tauxTransmissionGrippe(grippe.tauxTransmission(), true,
                                                                        this->m_agent_pathogène.sensibilité == Sensibilité::Infecté,
                                                                        this->m_agent_pathogène.sensibilité == Sensibilité::Immunisé);
    for ( int i = 0; i < statistiques.nombre_contaminant_grippé_et_contaminé_par_agent; ++i )
    {
        if (m_générateur_maladie(m_moteur_stochastique) < probabilité_attraper_grippe)
        {
            return true;
        }
    }
    return false;
}
//
bool 
épidémie::Individu::testContaminationAgent( Grille const& grille, AgentPathogène& agent)
{
    if (this->m_agent_pathogène.temps_contagieux > 0) return false;
    auto [largeur, hauteur] = grille.dimension();
    Grille::StatistiqueParCase const& statistiques = grille.getStatistiques()[m_position.x + m_position.y*largeur];

    for ( int i = 0; i < statistiques.nombre_contaminant_seulement_contaminé_par_agent; ++i )
    {
        double jet = m_générateur_maladie(m_moteur_stochastique);
        if (jet < agent.taux_infection)
        {
            return true;
        }
    }
    for ( int i = 0; i < statistiques.nombre_contaminant_grippé_et_contaminé_par_agent; ++i )
    {
        double jet = m_générateur_maladie(m_moteur_stochastique);
        if (jet < agent.taux_infection)
        {
            return true;
        }
    }
    return false;
}
//
void
épidémie::Individu::setPosition(int largeur, int hauteur)
{
    m_position.x = int(m_générateur_maladie(m_moteur_stochastique)*largeur);
    m_position.y = int(m_générateur_maladie(m_moteur_stochastique)*hauteur);
}
