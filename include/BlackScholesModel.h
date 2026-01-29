#pragma once

#include <vector> // Elle permet de stocker l'historique des prix (le "path")
#include <random>


class BlackScholesModel {
private:
    double spot; // S0 : Prix actuel de l'actif sous-jacent
    double rate; // r  : Taux d'int�r�t sans risque
    double volatility; // sigma : Volatilit� annuelle

    double normalCDF(double x) const; // Fonction de R�partition
    double normalPDF(double x) const; // Densit� de probabilit�

public:
    BlackScholesModel(double S0, double r, double sigma); // Constructeur

    // Getters :lecture seule ("const")
    double getSpot() const;
    double getRate() const;
    double getVolatility() const;

    // Formules ferm�es (Black-Scholes) : // Elles donnent la "Vraie Valeur" th�orique instantan�ment.
    // bool isCall: Si true, la fonction utilise la formule du Call; snn la formule du Put
    double bsPrice(double K, double T, bool isCall) const; // Calcule le prix exact (Call ou Put)

    // Les Grecs (Sensibilit�s)
    double bsDelta(double K, double T, bool isCall) const; // C'est la d�riv�e 1�re par rapport au Spot
    double bsGamma(double K, double T) const; // C'est la d�riv�e seconde par rapport au Spot (Convexit�)
    double bsVega(double K, double T) const; // C'est la d�riv�e par rapport � la Volatilit�

    // G�n�ration de trajectoire du Mouvement Brownien G�om�trique pour Monte Carlo
    void generatePath(double T, int steps, std::vector<double>& path, std::mt19937& gen) const; // Ex�cute une simulation pas-�-pas de couverture dynamique (Delta Hedging) pour mesurer l erreur de r�plication (P&L) finale.
};

