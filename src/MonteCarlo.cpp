#include "MonteCarlo.h"
#include <cmath>
#include <random>

using namespace std;

double MonteCarlo::price(const Option& option, const BlackScholesModel& model, int nbSimulations) {
    mt19937 gen(random_device{}());
    vector<double> path; // Ce vecteur recevra les prix simulés
    double payoffSum = 0.0;
    // Calcul du Pas de Temps (Discrétisation) : une année contient 252 jours de trading
    int steps = 252 * option.getMaturity();
    if(steps < 1) steps = 1; //Ceci permet d'éviter la dévision par 0

    for (int i = 0; i < nbSimulations; ++i) {
        model.generatePath(option.getMaturity(), steps, path, gen); // Générer une trajectoire de prix
        payoffSum += option.payoff(path); // Grâce au polymorphisme, "option.payoff" appelle la bonne formule de l'option
    }
    // Moyenne = Somme / N
    // Actualisation = Moyenne * exp(-r * T)
    return exp(-model.getRate() * option.getMaturity()) * payoffSum / nbSimulations;
}

// -------------------------------------------LES GRECS---------------------------------------------------

// On ne dérive pas les formules. On choque les paramètres

double MonteCarlo::delta(const Option& option, const BlackScholesModel& model, int nbSimulations, double epsilon) {
    // Formule : (Prix(S+e) - Prix(S-e)) / 2e
    BlackScholesModel up(model.getSpot() + epsilon, model.getRate(), model.getVolatility());
    BlackScholesModel down(model.getSpot() - epsilon, model.getRate(), model.getVolatility());

    double pUp = price(option, up, nbSimulations);
    double pDown = price(option, down, nbSimulations);

    return (pUp - pDown) / (2.0 * epsilon);
}

double MonteCarlo::gamma(const Option& option, const BlackScholesModel& model, int nbSimulations, double epsilon) {
    // Formule : (Prix(S+e) - 2*Prix(S) + Prix(S-e)) / e^2
    BlackScholesModel up(model.getSpot() + epsilon, model.getRate(), model.getVolatility());
    BlackScholesModel down(model.getSpot() - epsilon, model.getRate(), model.getVolatility());

    double pUp = price(option, up, nbSimulations);
    double pBase = price(option, model, nbSimulations);
    double pDown = price(option, down, nbSimulations);

    return (pUp - 2.0 * pBase + pDown) / (epsilon * epsilon);
}

double MonteCarlo::vega(const Option& option, const BlackScholesModel& model, int nbSimulations, double epsilon) {
    // Formule : (Prix(Vol+e) - Prix(Vol)) / e
    BlackScholesModel volUp(model.getSpot(), model.getRate(), model.getVolatility() + epsilon);
    BlackScholesModel volBase(model.getSpot(), model.getRate(), model.getVolatility());

    double pUp = price(option, volUp, nbSimulations);
    double pBase = price(option, volBase, nbSimulations);

    return (pUp - pBase) / epsilon;
}
