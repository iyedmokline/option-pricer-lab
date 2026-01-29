#pragma once

// La méthode Monte Carlo a besoin de des 2 classes pour fonctionner :
// 1. Une option (Option.h)
// 2. Un Générateur de scénarios de marché (BlackScholesModel.h)
#include "Option.h"
#include "BlackScholesModel.h"

class MonteCarlo {
public:
    static double price(const Option& option, const BlackScholesModel& model, int nbSimulations); //Calcule le juste prix de l'option aujourd'hui en faisant la moyenne actualisée des gains
    // On choque le prix du spot de epsilon
    static double delta(const Option& option, const BlackScholesModel& model, int nbSimulations, double epsilon = 0.01); //Calcule la sensibilité au prix du Spot
    static double gamma(const Option& option, const BlackScholesModel& model, int nbSimulations, double epsilon = 1.0); //Calcule la sensibilité de la courbure
    static double vega(const Option& option, const BlackScholesModel& model, int nbSimulations, double epsilon = 0.001); //Calcule la sensibilité à la volatilité.
};

