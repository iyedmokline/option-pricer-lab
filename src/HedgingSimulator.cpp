#include "HedgingSimulator.h"
#include <iostream>
#include <iomanip> // Configure l'affichage pour un format monétaire lisible
#include <cmath>
#include <random>

using namespace std;

void HedgingSimulator::run(const BlackScholesModel& initModel, double K, double T, bool isCall) {
    string type = isCall ? "CALL" : "PUT";
    cout << "\n--- Simulation Hedging " << type << " (Delta Neutre) ---\n";

    int steps = 50; // On simule une couverture hebdomadaire (50 semaines/an); On ne peut pas hedger en continu.
    double dt = T / steps; // Durée d'un pas de temps
    // Générateur aléatoire pour simuler l'évolution du marché
    mt19937 gen(random_device{}());
    normal_distribution<> norm(0.0, 1.0);

    double currentSpot = initModel.getSpot();

    // Calculs initiaux: Initialisation du portefeuille
    double initPrice = initModel.bsPrice(K, T, isCall);
    double delta = initModel.bsDelta(K, T, isCall);
    double bankAccount = initPrice - (delta * currentSpot); // Si on vend un Call, delta > 0, on achète des actions (on emprunte du cash).
                                                            // Si on vend un Put, delta < 0, on vend des actions (on place du cash).

    cout << fixed << setprecision(2); // Configure l'affichage pour un format monétaire lisible: 2 chiffres après la virgule
    cout << "INIT | S=" << currentSpot << " | OptPrice=" << initPrice << " | Delta=" << delta << " | Bank=" << bankAccount << endl;

    for(int i = 1; i <= steps; ++i) {
        // Temps restant avant maturité
        double timeLeft = T - (i * dt);
        if(timeLeft < 0) timeLeft = 0;
        // Simulation de S
        double Z = norm(gen);
        currentSpot *= exp((initModel.getRate() - 0.5 * pow(initModel.getVolatility(), 2)) * dt + initModel.getVolatility() * sqrt(dt) * Z);// Formule du Mouvement Brownien Géométrique

        bankAccount *= exp(initModel.getRate() * dt); //Actualisation du montant dans le compte bancaire

        //Réajustement du Delta au fur et à mesure
        if (i < steps) {
            // On recalcule le nouveau Delta avec le nouveau Spot et le nouveau temps
            BlackScholesModel temp(currentSpot, initModel.getRate(), initModel.getVolatility()); // On crée une instance temporaire du modèle avec le prix actuel (St) pour calculer le Delta à cet instant précis.
            double newDelta = temp.bsDelta(K, timeLeft, isCall);
            double deltaChange = newDelta - delta; //Le nombre d'actions nécessaires en plus ou en moins

            // Impact en compte bancaire :
            // Si achat des actions (change > 0), donc valeur en compte diminue.
            // Si vente des actions (change < 0), donc valeur en compte augmente.
            bankAccount -= deltaChange * currentSpot;
            // Mise à jour de la position prise
            delta = newDelta;
        }
    }

    // ------------------------------------- Bilan Final à la maturité ------------------------------------
    double payoff = isCall ? max(currentSpot - K, 0.0) : max(K - currentSpot, 0.0); //Payoff réel de l'option
    double portfolio = delta * currentSpot + bankAccount; // Ce qu'on possède (La stratégie de réplication): Valeur des actions + argent placé en banque
    double error = portfolio - payoff; // Le verdict (P&L)

    cout << "FIN  | S_Final=" << currentSpot << endl;
    cout << "Portefeuille : " << portfolio << endl;
    cout << "A payer      : " << payoff << endl;
    cout << "P&L Hedging  : " << error << " (Idealement 0)" << endl;
}
