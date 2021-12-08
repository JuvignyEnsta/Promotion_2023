#ifndef _EPIDEMIE_INTERACTION_HPP_
#define _EPIDEMIE_INTERACTION_HPP_

namespace épidémie 
{
struct Interactions 
{
    double β = 20. ;// Modificateur du paramètre de transmission de la grippe
    double α = 1.5 ;// Paramètre de variabilité d'acquisition de la grippe par l'agent pathogène
    double θ = 1. ;// Paramètre modifiant transmission de la grippe par interaction avec l'agent pathogène
    double μ = 1. ;// Paramètre d'interaction sur l'immunité croisée.

    double tauxTransmissionGrippe( double taux_base, 
                                   bool contaminant_a_agent,
                                   bool contaminé_infecté_agent, bool contaminé_immunisé_agent ) const
    {
        double taux = taux_base * β;
        if (contaminant_a_agent)
            taux *= θ;
        if (contaminé_infecté_agent)
            taux *= α;
        if (contaminé_immunisé_agent)
            taux *= μ;
        return taux;
    }
};


}

#endif