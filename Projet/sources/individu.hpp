#ifndef _EPIDEMIE_INDIVIDU_HPP_
#define _EPIDEMIE_INDIVIDU_HPP_
#include <random>
#include "grille.hpp"
#include "grippe.hpp"
#include "agent_pathogene.hpp"
#include "position.hpp"

namespace epidemie 
{
class Individu 
{
public:
    enum class Sensibilite
    {
        Sensible, Contagieux, Infecte, Immunise
    };

    Individu( unsigned int graine_aleatoire, int esperance_de_vie, bool estNe = false, int deplacement_max = 10 );

    Sensibilite getEtatGrippal() const 
    {
        return m_grippe.sensibilite;
    }

    Sensibilite getEtatAgentPathogene() const 
    {
        return m_agent_pathogene.sensibilite;
    }

    bool aGrippeContagieuse() const 
    {
        return m_grippe.temps_contagieux > 0;
    }

    bool aAgentPathogeneContagieux() const
    {
        return m_agent_pathogene.temps_contagieux > 0;
    }

    void veillirDUnJour();

    bool doitMourir() const 
    {
        return m_age >= m_esperance_de_vie;
    }

    void seDeplace( Grille& grille );
    void setPosition(int largeur, int hauteur); // Positonne un individu au hasard...
    Position const& position() const { return this->m_position; }

    void devientImmuniseGrippe()
    {
        m_grippe.sensibilite = Sensibilite::Immunise;
    }

    void redevientSensibleGrippe()
    {
        m_grippe.sensibilite = Sensibilite::Sensible;
    }

    bool testContaminationGrippe( Grille const& grille, Interactions const& t_interactions, Grippe& grippe, AgentPathogene& agent);
    bool testContaminationAgent( Grille const& grille, AgentPathogene& agent);

    void estContamine( Grippe& grippe );
    void estContamine( AgentPathogene& agent );



private:
    struct {
        Sensibilite sensibilite = Sensibilite::Sensible;
        int temps_incubation = 0;
        int temps_symptomatique = 0;
        int temps_contagieux = 0;
    } m_grippe;
    struct {
        Sensibilite sensibilite = Sensibilite::Sensible;
        int temps_asymptomatique = 0, temps_symptomatique = 0;
        int temps_passe_immunise = 0, temps_contagieux    = 0;
    } m_agent_pathogene;
    int m_age, m_esperance_de_vie;
    Position m_position;
    std::default_random_engine         m_moteur_stochastique;
    std::uniform_int_distribution<int> m_generateur_quantite_deplacement;
    std::uniform_int_distribution<int> m_generateur_deplacement;
    std::uniform_real_distribution<double> m_generateur_maladie;
    std::uniform_int_distribution<int> m_generateur_age;
};
}

#endif