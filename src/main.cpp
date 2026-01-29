#include <iostream>
#include <ctime>
#include "BlackScholesModel.h"
#include "Option.h"
#include "MonteCarlo.h"
#include "HedgingSimulator.h"

using namespace std;

// Fonction utilitaire pour saisir des nombres : Elle permet de contrôler la saisie
double getIn(string txt) {
    double v;
    cout << txt;
    while(!(cin >> v)) {
        cout << "Erreur de saisie. ";
        cin.clear();
        cin.ignore(1000, '\n');
        cout << txt;
    }
    return v;
}

// Fonction d'affichage et de comparaison
void afficherDetailsOption(Option* opt, BlackScholesModel& model, int N) { // Cette fonction prend un pointeur générique Option* (Polymorphisme)
    cout << "\n--- Analyse : " << opt->getName() << " ---" << endl;

    double priceMC = MonteCarlo::price(*opt, model, N);
    double deltaMC = MonteCarlo::delta(*opt, model, N);
    double gammaMC = MonteCarlo::gamma(*opt, model, N);
    double vegaMC = MonteCarlo::vega(*opt, model, N);

    cout << "Prix (Monte Carlo) : " << priceMC << endl;
    cout << "Delta (Monte Carlo): " << deltaMC << endl;
    cout << "Gamma (Monte Carlo): " << gammaMC << endl;
    cout << "Vega (Monte Carlo) : " << vegaMC << endl;

    // Comparaison BS si c'est Européen
    if (dynamic_cast<CallEuropeen*>(opt) || dynamic_cast<PutEuropeen*>(opt)) // Si (C'est un CallEuropeen) OU (C'est un PutEuropeen)
    {
        bool isCall = (dynamic_cast<CallEuropeen*>(opt) != nullptr);
        double K = opt->getStrike();
        double T = opt->getMaturity();

        cout << "\n[Verifications Formules Black-Scholes]" << endl;
        cout << "Prix BS  : " << model.bsPrice(K, T, isCall) << endl;
        cout << "Delta BS : " << model.bsDelta(K, T, isCall) << endl;
        cout << "Gamma BS : " << model.bsGamma(K, T) << endl;
        cout << "Vega BS  : " << model.bsVega(K, T) << endl;
    }
    delete opt; // Nettoyage Mémoire
}

int main() {

    // Date et heure courantes
    std::time_t now = std::time(nullptr);
    char buffer[100];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    cout << "\n==========================================" << endl;
    cout << "     Date & heure : " << buffer ;
    cout << "\n==========================================" << endl;

    // -------------Initialisation du Marché -------------
    double S0 = getIn("Spot (S0): ");
    double r = getIn("Taux (r): ");
    double sigma = getIn("Volatilite (sigma): ");

    BlackScholesModel model(S0, r, sigma);
    int N = 50000; //Nombre de simulation que la méthode MonteCarlo va utiliser

    //----------------------------------------------MENU-------------------------------------------------------------
    // Le programme tourne tant que l'utilisateur ne choisit pas 0.
    int choix = -1;
    while(choix != 0) {
        cout << "\n================ MENU AVANCE ================" << endl;
        cout << "1. Europeennes (Call/Put) + Grecs complets" << endl;
        cout << "2. Asiatiques (Moyenne arithmetique)" << endl;
        cout << "3. Digitales (Binaires)" << endl;
        cout << "4. Lookback (Fixed Strike)" << endl;
        cout << "5. Simulation de Replication (Hedging)" << endl;
        cout << "6. Changer parametres de marche / Nb Simu" << endl;
        cout << "0. Quitter" << endl;
        cout << "Choix : ";
        cin >> choix;

        if (choix >= 1 && choix <= 4) {
            double K = getIn("Strike (K): ");
            double T = getIn("Maturite (T): ");
            int type;
            cout << "1 pour CALL, 2 pour PUT : "; cin >> type;

            Option* opt = nullptr; // Pointeur polymorphe

            if (choix == 1) {
                if(type == 1) opt = new CallEuropeen(T, K);
                else opt = new PutEuropeen(T, K);
            }
            else if (choix == 2) {
                if(type == 1) opt = new CallAsiatique(T, K);
                else opt = new PutAsiatique(T, K);
            }
            else if (choix == 3) {
                if(type == 1) opt = new CallDigital(T, K);
                else opt = new PutDigital(T, K);
            }
            else if (choix == 4) {
                if(type == 1) opt = new CallLookback(T, K);
                else opt = new PutLookback(T, K);
            }

            if(opt) afficherDetailsOption(opt, model, N); // Si l'objet a bien été créé, on lance l'analyse
        }
        else if (choix == 5) {
            double K = getIn("Strike (K): ");
            double T = getIn("Maturite (T): ");
            int type; cout << "1 pour CALL, 2 pour PUT : "; cin >> type;
            // Appel de la méthode statique (pas besoin d'objet Option ici)
            HedgingSimulator::run(model, K, T, (type == 1));
        }
        else if (choix == 6) {
            S0 = getIn("Nouveau Spot: ");
            r = getIn("Nouveau Taux: ");
            sigma = getIn("Nouvelle Volatilite: ");
            N = (int)getIn("Nouveau nb simulations (ex: 50000): ");
            // On écrase l'ancien modèle avec un nouveau
            model = BlackScholesModel(S0, r, sigma);
        }
    }
    return 0;
}
