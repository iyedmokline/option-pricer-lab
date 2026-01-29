# 💱 Option Pricer & Hedging Simulator

C++ project to **price European options** under the **Black–Scholes model** and to **simulate delta-hedging** (self-financing replication) over discrete rebalancing dates.  
Includes **closed-form pricing**, **Monte Carlo pricing**, and a **hedging simulator** to study replication error.

---

## ✨ Features

- **Black–Scholes model** (risk-neutral dynamics)
- **European Call/Put pricing**
  - Closed-form Black–Scholes formula
  - Monte Carlo estimator
- **Greeks (Delta)** used for hedging
- **Delta-hedging simulator**
  - Discrete rebalancing (e.g., weekly steps)
  - Tracks portfolio value vs. option payoff
  - Reports replication / hedging error
- Clean OOP structure (separation of model / option / MC / hedging)

---

## 📁 Project Structure

Typical files:
- `main.cpp` — entry point, runs pricing + simulation demos
- `BlackScholesModel.h/.cpp` — model parameters + BS pricing / delta
- `Option.h/.cpp` — option definition (K, T, Call/Put)
- `MonteCarlo.h/.cpp` — Monte Carlo pricer
- `HedgingSimulator.h/.cpp` — delta-hedging replication simulator

---

## ✅ Requirements

- A C++ compiler supporting C++11+ (g++, clang++)
- (Windows) PowerShell / (Linux/macOS) terminal

---

## ⚙️ Build & Run

### Linux / macOS
```bash
g++ -std=c++11 main.cpp BlackScholesModel.cpp Option.cpp MonteCarlo.cpp HedgingSimulator.cpp -o pricer
./pricer

