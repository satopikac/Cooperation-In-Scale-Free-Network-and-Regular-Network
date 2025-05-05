// network.cpp
#include "network.h"
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <chrono>
#include <numeric>
#include <omp.h> // OpenMP 
#include "random_utils.h"
void Network::buildRegular() {
    for (int i = 0; i < N; ++i) {
        for (int j = 1; j <= z/2; ++j) {
            int neighbor = (i + j) % N;
            adj[i].push_back(neighbor);
            adj[neighbor].push_back(i);
        }
    }
}

void Network::buildScaleFree(int m0) {
    for (int i = 0; i < m0; ++i) {
        for (int j = i+1; j < m0; ++j) {
            adj[i].push_back(j);
            adj[j].push_back(i);
        }
    }
    for (int i = m0; i < N; ++i) {
        std::vector<double> probs;
        int total_degree = 0;
        for (int j = 0; j < i; ++j) {
            total_degree += adj[j].size();
        }
        for (int j = 0; j < i; ++j) {
            probs.push_back(adj[j].size() / (double)total_degree);
        }
        std::discrete_distribution<int> dist(probs.begin(), probs.end());
        for (int j = 0; j < z/2; ++j) {
            int target = dist(thread_rng);
            if (std::find(adj[i].begin(), adj[i].end(), target) == adj[i].end()) {
                adj[i].push_back(target);
                adj[target].push_back(i);
            }
        }
    }
}

Network::Network(int n, int k) : N(n), z(k), adj(n) {}

// 返回节点度数
int Network::degree(int node) {
    return adj[node].size();
}