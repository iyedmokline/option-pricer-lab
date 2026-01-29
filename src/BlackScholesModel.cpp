#include "BlackScholesModel.h"
#include <cmath>
#include <algorithm>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace std;

BlackScholesModel::BlackScholesModel(double S0, double r, double sigma): spot(S0), rate(r), volatility(sigma) {}

double BlackScholesModel::getSpot() const { return spot; }
double BlackScholesModel::getRate() const { return rate; }
double BlackScholesModel::getVolatility() const { return volatility; }

double BlackScholesModel::normalCDF(double x) const {
    return 0.5 * erfc(-x / sqrt(2.0)); // Fonction de Répartition de la Loi Normale : N(x)
}

double BlackScholesModel::normalPDF(double x) const {  // Densité de Probabilité de la Loi Normale : N'(x)
    return (1.0 / sqrt(2.0 * M_PI)) * exp(-0.5 * x * x);
}

double BlackScholesModel::bsPrice(double K, double T, bool isCall) const {
    // Calcul de d1 et d2 (les probabilités ajustées)
    double d1 = (log(spot / K) + (rate + 0.5 * volatility * volatility) * T) / (volatility * sqrt(T));
    double d2 = d1 - volatility * sqrt(T);

    if (isCall)
        return spot * normalCDF(d1) - K * exp(-rate * T) * normalCDF(d2);
    else
        return K * exp(-rate * T) * normalCDF(-d2) - spot * normalCDF(-d1);
}

double BlackScholesModel::bsDelta(double K, double T, bool isCall) const {  // Calcul de Delta (Sensibilité au prix du sous-jacent)
    double d1 = (log(spot / K) + (rate + 0.5 * volatility * volatility) * T) / (volatility * sqrt(T));
    if (isCall) return normalCDF(d1);  // Delta Call doit être entre 0 et 1
    else
        return normalCDF(d1) - 1.0; // Delta Put doit être entre -1 et 0
}

double BlackScholesModel::bsGamma(double K, double T) const {  // Calcul du GAMMA (Convexité)
    double d1 = (log(spot / K) + (rate + 0.5 * volatility * volatility) * T) / (volatility * sqrt(T));
    return normalPDF(d1) / (spot * volatility * sqrt(T)); // Gamma = N'(d1) / (S * sigma * sqrt(T))
}

double BlackScholesModel::bsVega(double K, double T) const {  // Calcul du VEGA et c'est le même pour un Call et un Put
    double d1 = (log(spot / K) + (rate + 0.5 * volatility * volatility) * T) / (volatility * sqrt(T));
    return spot * sqrt(T) * normalPDF(d1); // Vega = S * sqrt(T) * N'(d1)
}

void BlackScholesModel::generatePath(double T, int steps, vector<double>& path, mt19937& gen) const {
    normal_distribution<> normal(0.0, 1.0); // Distribution Normale Standard N(0,1) pour générer le hasard
    double dt = T / steps;  // Pas de temps
    double currentSpot = spot;

    path.clear();
    path.push_back(currentSpot); // Le path commence  à S0

    for(int i=0; i<steps; ++i) {
        double Z = normal(gen); // Tirage aléatoire (Loi Normale)
        currentSpot *= exp((rate - 0.5 * volatility * volatility) * dt + volatility * sqrt(dt) * Z); // S(t+dt)=S(t)*exp( (r - 0.5*sigma^2)*dt + sigma*sqrt(dt)*Z )
        path.push_back(currentSpot);
    }
}
