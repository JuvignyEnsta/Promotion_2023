#ifndef _EPIDEMIE_INDIVIDU_HPP_
#define _EPIDEMIE_INDIVIDU_HPP_
#include <random>
#include "grille.hpp"
#include "grippe.hpp"
#include "agent_pathogene.hpp"
#include "position.hpp"

namespace épidémie 
{
class Individu 
{
public:
    enum class Sensibilité
    {
        Sensible, Contagieux, Infecté, Immunisé
    };

    Individu( unsigned int graine_aléatoire, int espérance_de_vie, bool estNé = false, int déplacement_max = 10 );

    Sensibilité getEtatGrippal() const 
    {
        return m_grippe.sensibilité;
    }

    Sensibilité getEtatAgentPathogène() const 
    {
        return m_agent_pathogène.sensibilité;
    }

    bool aGrippeContagieuse() const 
    {
        return m_grippe.temps_contagieux > 0;
    }

    bool aAgentPathogèneContagieux() const
    {
        return m_agent_pathogène.temps_contagieux > 0;
    }

    void veillirDUnJour();

    bool doitMourir() const 
    {
        return m_âge >= m_espérance_de_vie;
    }

    void seDéplace( Grille& grille );
    void setPosition(int largeur, int hauteur); // Positonne un individu au hasard...
    Position const& position() const { return this->m_position; }

    void devientImmuniséGrippe()
    {
        m_grippe.sensibilité = Sensibilité::Immunisé;
    }

    void redevientSensibleGrippe()
    {
        m_grippe.sensibilité = Sensibilité::Sensible;
    }

    bool testContaminationGrippe( Grille const& grille, Interactions const& t_interactions, Grippe& grippe, AgentPathogène& agent);
    bool testContaminationAgent( Grille const& grille, AgentPathogène& agent);

    void estContaminé( Grippe& grippe );
    void estContaminé( AgentPathogène& agent );



private:
    struct {
        Sensibilité sensibilité = Sensibilité::Sensible;
        int temps_incubation = 0;
        int temps_symptomatique = 0;
        int temps_contagieux = 0;
    } m_grippe;
    struct {
        Sensibilité sensibilité = Sensibilité::Sensible;
        int temps_asymptomatique = 0, temps_symptomatique = 0;
        int temps_passé_immunisé = 0, temps_contagieux    = 0;
    } m_agent_pathogène;
    int m_âge, m_espérance_de_vie;
    Position m_position;
    std::default_random_engine         m_moteur_stochastique;
    std::uniform_int_distribution<int> m_générateur_quantité_déplacement;
    std::uniform_int_distribution<int> m_générateur_déplacement;
    std::uniform_real_distribution<double> m_générateur_maladie;
    std::uniform_int_distribution<int> m_générateur_âge;
};
}

#endif