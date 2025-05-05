// game_simulation.cpp
#include "game_simulation.h"
#include <iostream>
#include <random>
#include <numeric>
#include <omp.h>
#include <algorithm>
#include <fstream>
#include <chrono>

#include "random_utils.h"
GameSimulation::GameSimulation(Network& net, const std::string& type, double param_b, double param_r)
    : network(net), gameType(type), b(param_b), r(param_r), strategies(net.N), payoffs(net.N) {}

void GameSimulation::initializeStrategies() {
    std::vector<int> indices(network.N);
    for (int i = 0; i < network.N; ++i) indices[i] = i;
    std::shuffle(indices.begin(), indices.end(), thread_rng);
    int half_N = network.N / 2;
    for (int i = 0; i < half_N; ++i) strategies[indices[i]] = 0; // Defector
    for (int i = half_N; i < network.N; ++i) strategies[indices[i]] = 1; // Cooperator
}

void GameSimulation::playRound() {
    fill(payoffs.begin(), payoffs.end(), 0.0);
    for (int i = 0; i < network.N; ++i) {
        for (int j : network.adj[i]) {
            if (j > i) continue;
            double payoff_i, payoff_j;
            if (gameType == "PD") {
                if (strategies[i] == 1 && strategies[j] == 1) { payoff_i = payoff_j = 1; }
                else if (strategies[i] == 1 && strategies[j] == 0) { payoff_i = 0; payoff_j = b; }
                else if (strategies[i] == 0 && strategies[j] == 1) { payoff_i = b; payoff_j = 0; }
                else { payoff_i = payoff_j = 0; }
            } else { // SG
                double beta = (1/b + 1)*0.5;
                if (strategies[i] == 1 && strategies[j] == 1) { payoff_i = payoff_j = beta - 0.5; }
                else if (strategies[i] == 1 && strategies[j] == 0) { payoff_i = beta - 1; payoff_j = beta; }
                else if (strategies[i] == 0 && strategies[j] == 1) { payoff_i = beta; payoff_j = beta - 1; }
                else { payoff_i = payoff_j = 0; }
            }
            payoffs[i] += payoff_i;
            payoffs[j] += payoff_j;
        }
    }
}

void GameSimulation::updateStrategies() {
    std::uniform_real_distribution<double> prob(0.0, 1.0);
    for (int i = 0; i < network.N; ++i) {
        if (network.adj[i].empty()) continue;
        std::uniform_int_distribution<int> neighbor_dist(0, network.adj[i].size()-1);
        int j = network.adj[i][neighbor_dist(thread_rng)];
        if (payoffs[j] > payoffs[i]) {
            double D = b;
            double k = std::max(network.degree(i), network.degree(j));
            double W = (payoffs[j] - payoffs[i]) / (D * k);
            if (prob(thread_rng) < W) {
                strategies[i] = strategies[j];
            }
        }
    }
}

double GameSimulation::cooperatorFraction() {
    int count = std::accumulate(strategies.begin(), strategies.end(), 0);
    return count / (double)network.N;
}

double GameSimulation::runSimulation(int generations, int transient) {
    initializeStrategies();
    std::vector<double> coop_fractions;
    for (int g = 1; g <= generations; ++g) {
        playRound();
        updateStrategies();
        if (g > transient) {
            coop_fractions.push_back(cooperatorFraction());
        }
    }
    return std::accumulate(coop_fractions.begin(), coop_fractions.end(), 0.0) / coop_fractions.size();
}

std::vector<std::vector<double>> parameterSweep(const std::string& networkType, const std::string& gameType,
                                               double param_start, double param_end, int steps,
                                               int N, int z, int repeats) {
    std::vector<std::vector<double>> results(steps, std::vector<double>(2));
    double step_size = (param_end - param_start) / (steps - 1);

    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < steps; ++i) {
        double param = param_start + i * step_size;
        double coop_sum = 0.0;

        #pragma omp parallel for reduction(+:coop_sum)
        for (int r = 0; r < repeats; ++r) {
            Network net(N, z);
            if (networkType == "regular") {
                net.buildRegular();
            } else {
                net.buildScaleFree();
            }
            GameSimulation sim(net, gameType, param, param);
            coop_sum += sim.runSimulation();
        }

        results[i][0] = param;
        results[i][1] = coop_sum / repeats;
    }
    return results;
}