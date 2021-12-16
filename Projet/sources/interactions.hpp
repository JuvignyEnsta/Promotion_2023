#ifndef _EPIDEMIE_INTERACTION_HPP_
#define _EPIDEMIE_INTERACTION_HPP_

namespace epidemie 
{
struct Interactions 
{
    double beta = 20. ;// Modificateur du parametre de transmission de la grippe
    double alpha = 1.5 ;// Parametre de variabilite d'acquisition de la grippe par l'agent pathogene
    double theta = 1. ;// Parametre modifiant transmission de la grippe par interaction avec l'agent pathogene
    double mu = 1. ;// Parametre d'interaction sur l'immunite croisee.

    double tauxTransmissionGrippe( double taux_base, 
                                   bool contaminant_a_agent,
                                   bool contamine_infecte_agent, bool contamine_immunise_agent ) const
    {
        double taux = taux_base * beta;
        if (contaminant_a_agent)
            taux *= theta;
        if (contamine_infecte_agent)
            taux *= alpha;
        if (contamine_immunise_agent)
            taux *= mu;
        return taux;
    }
};


}

#endif