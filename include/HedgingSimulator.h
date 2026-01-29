#pragma once

// Le simulateur a besoin du modèle de Black-Scholes pour deux raisons principales :
// 1) Connaître les paramètres de marché (taux r, volatilité sigma).
// 2) Utiliser les formules mathématiques (bsDelta, bsPrice) pour calculer la couverture.
#include "BlackScholesModel.h"

class HedgingSimulator {
public:
    static void run(const BlackScholesModel& initModel, double K, double T, bool isCall);
};

