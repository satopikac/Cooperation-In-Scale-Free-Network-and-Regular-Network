// game_simulation.h
#ifndef GAME_SIMULATION_H
#define GAME_SIMULATION_H

#include "network.h"
#include <string>
#include <vector>

class GameSimulation {
public:
    Network& network;
    std::string gameType;
    double b;
    double r;
    std::vector<int> strategies;
    std::vector<double> payoffs;

    GameSimulation(Network& net, const std::string& type, double param_b, double param_r = 0);

    void initializeStrategies();
    void playRound();
    void updateStrategies();
    double cooperatorFraction();
    double runSimulation(int generations = 4000, int transient = 3000);
};

std::vector<std::vector<double>> parameterSweep(const std::string& networkType, const std::string& gameType,
                                               double param_start, double param_end, int steps,
                                               int N = 400, int z = 4, int repeats = 50);

#endif // GAME_SIMULATION_H