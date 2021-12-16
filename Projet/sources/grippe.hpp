#ifndef _EPIDEMIE_GRIPPE_HPP_
#define _EPIDEMIE_GRIPPE_HPP_
#include <random>

namespace epidemie 
{
class Grippe 
{
public:
    Grippe(int graine_aleatoire)
        :   m_moteur_stochastique(graine_aleatoire),
            m_generateur_transmission(0.033, 0.033/30),
            m_generateur_date_importation(71.*71./28., 28./71.),
            m_generateur_incubation(40, 0.05),
            m_generateur_symptomatique(16.,0.25)
    {
    }

    void calculNouveauTauxTransmission()
    {
        m_taux_transmission = m_generateur_transmission(m_moteur_stochastique);
    }

    double tauxTransmission() const { return m_taux_transmission; }

    // Renvoit un nombre de jour apres le premier Octobre
    int  dateCalculImportationGrippe  ()
    {
        return int(m_generateur_date_importation(m_moteur_stochastique));
    }

    int nombreJoursIncubation()
    {
        return int(m_generateur_incubation(m_moteur_stochastique));
    }

    int nombreJoursSymptomatique() 
    {
        return int(m_generateur_symptomatique(m_moteur_stochastique));
    }

private:
    double m_taux_transmission = 0.033;
    int    m_date_debut_grippe = 1;

    std::default_random_engine       m_moteur_stochastique;
    std::normal_distribution<double> m_generateur_transmission;
    std::gamma_distribution<double>  m_generateur_date_importation;
    std::gamma_distribution<double>  m_generateur_incubation;
    std::gamma_distribution<double>  m_generateur_symptomatique;
};
}

#endif