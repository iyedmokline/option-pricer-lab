#include "Option.h"
#include <iostream>
using namespace std;

// ==========================================
// 1. CLASSE MÈRE (OPTION)
// ==========================================

Option::Option(double T, double K, string n): maturity(T), strike(K), name(n) {} //Constructeur

Option::~Option() {} // Destructeur

double Option::getMaturity() const {
    return maturity;
}

double Option::getStrike() const {
    return strike;
}

string Option::getName() const {
    return name;
}

// ==========================================
// 2. OPTIONS EUROPÉENNES
// ==========================================

// --- Call Europeen ---
CallEuropeen::CallEuropeen(double T, double K): Option(T, K, "Call Europeen") {}

double CallEuropeen::payoff(const vector<double>& path) const {
    // Payoff = Max(S_T - K, 0)
    // On prend le dernier élément du vecteur (prix à maturité)
    return max(path.back() - strike, 0.0);
}

// --- Put Europeen ---
PutEuropeen::PutEuropeen(double T, double K): Option(T, K, "Put Europeen") {}

double PutEuropeen::payoff(const vector<double>& path) const {
    // Payoff = Max(K - S_T, 0)
    return max(strike - path.back(), 0.0);
}

// ==========================================
// 3. OPTIONS ASIATIQUES
// ==========================================

// --- Call Asiatique ---
CallAsiatique::CallAsiatique(double T, double K): Option(T, K, "Call Asiatique") {}

double CallAsiatique::payoff(const vector<double>& path) const {
    // Calcul de la moyenne arithmétique des prix du chemin
    double sum = 0.0;
    for (size_t i = 0; i < path.size(); ++i) {
        sum = sum + path[i];
    }
    double average = sum / path.size();

    // Payoff = Max(Moyenne - K, 0)
    return max(average - strike, 0.0);
}

// --- Put Asiatique ---
PutAsiatique::PutAsiatique(double T, double K): Option(T, K, "Put Asiatique") {}

double PutAsiatique::payoff(const vector<double>& path) const {
    // Payoff = Max(K - Moyenne, 0)
    double sum = 0.0;
    for (size_t i = 0; i < path.size(); ++i) {
        sum = sum + path[i];
    }
    double average = sum / path.size();
    return max(strike - average, 0.0);
}

// ==========================================
// 4. OPTIONS DIGITALES
// ==========================================

// --- Call Digital ---
CallDigital::CallDigital(double T, double K): Option(T, K, "Call Digital") {}

double CallDigital::payoff(const vector<double>& path) const {
    // Si S_T > K, on gagne 1, sinon 0
    double S_T = path.back(); // Le prix à maturité (dernier élément du vecteur)
    if (S_T > strike) {
        return 1.0; // Gagne
    } else {
        return 0.0; // Perdu.
    }
}
// --- Put Digital ---
PutDigital::PutDigital(double T, double K): Option(T, K, "Put Digital") {}

double PutDigital::payoff(const vector<double>& path) const {
    // Si S_T < K, on gagne 1, sinon 0
    double S_T = path.back(); // Le prix à maturité (dernier élément du vecteur)
    if (S_T < strike) {
        return 1.0; // Gagne
    } else {
        return 0.0; // Perdu.
    }
}

// ==========================================
// 5. OPTIONS LOOKBACK
// ==========================================

// --- Call Lookback  ---
CallLookback::CallLookback(double T, double K): Option(T, K, "Call Lookback") {}

double CallLookback::payoff(const vector<double>& path) const {
    // On cherche le prix MAXIMUM atteint sur toute la trajectoire
    double maxSpot = *max_element(path.begin(), path.end());

    // Payoff = Max(MaxSpot - K, 0)
    return max(maxSpot - strike, 0.0);
}

// --- Put Lookback  ---
PutLookback::PutLookback(double T, double K): Option(T, K, "Put Lookback") {}

double PutLookback::payoff(const vector<double>& path) const {
    // On cherche le prix MINIMUM atteint sur toute la trajectoire
    double minSpot = *min_element(path.begin(), path.end());

    // Payoff = Max(K - MinSpot, 0)
    return max(strike - minSpot, 0.0);
}

