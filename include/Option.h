#pragma once
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>
#include <cmath>


// Classe Abstraite
class Option {
protected:
    double maturity;
    double strike;
    std::string name; // Nom pour l'affichage (ex: "Put Européen")

public:
    Option(double T, double K, std::string n); // Constructeur
    virtual ~Option(); // Destructeur

    // Ajout de const car ces méthodes ne modifient pas l'objet et elles permettent de lire ces variables
    double getMaturity() const;
    double getStrike() const;
    std::string getName() const;

    virtual double payoff(const std::vector<double>& path) const = 0;// Méthode virtuelle (=0) : Force chaque type d'option à définir sa propre formule de payoff.
                                                               // Utilise "const vector&" pour un accès sans copie à l'historique des prix.
};

// --- Options Européennes ---

// Héritage public : Ce sont des Options

class CallEuropeen : public Option {
public:
    CallEuropeen(double T, double K);
    double payoff(const std::vector<double>& path) const override; // On utilise override pour que le compilateur vérifie qu'on remplace bien la fonction virtuelle de la classe mère.
};

class PutEuropeen : public Option {
public:
    PutEuropeen(double T, double K);
    double payoff(const std::vector<double>& path) const override;
};

// --- Options Asiatiques ---
class CallAsiatique : public Option {
public:
    CallAsiatique(double T, double K);
    double payoff(const std::vector<double>& path) const override;
};

class PutAsiatique : public Option {
public:
    PutAsiatique(double T, double K);
    double payoff(const std::vector<double>& path) const override;
};

// --- Options Digitales ---
class CallDigital : public Option {
public:
    CallDigital(double T, double K);
    double payoff(const std::vector<double>& path) const override;
};
class PutDigital : public Option {
public:
    PutDigital(double T, double K);
    double payoff(const std::vector<double>& path) const override;
};

// --- Options Lookback ---
class CallLookback : public Option {
public:
    CallLookback(double T, double K);
    double payoff(const std::vector<double>& path) const override;
};

class PutLookback : public Option {
public:
    PutLookback(double T, double K);
    double payoff(const std::vector<double>& path) const override;
};

