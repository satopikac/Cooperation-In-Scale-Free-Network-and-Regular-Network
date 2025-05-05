// network.h
#ifndef NETWORK_H
#define NETWORK_H

#include <vector>

class Network {
public:
    int N; // Number of nodes节点数
    int z; // Average degree平均度
    std::vector<std::vector<int>> adj; // Adjacency list邻接表
    Network(int n, int k);

    void buildRegular();
    void buildScaleFree(int m0 = 5);
    int degree(int node);
};

#endif // NETWORK_H