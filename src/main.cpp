// main.cpp
#include <iostream>
#include <fstream>
#include <vector>
#include "game_simulation.h"

// 模拟参数
const int allgenerations = 4000;
const int thetransient = 3000;
const int N = 400;
const int repeats = 50;

const double pd_b_start = 1.0;
const double pd_b_end = 2.0;
const int pd_steps = 21;

const double sg_b_start = 0.0;
const double sg_b_end = 1.0;
const int sg_steps = 11;

int main() {
    std::vector<int> Z{4, 8, 12, 16, 32};
    for (auto z : Z) {
        std::cout << "Now begin z=" << z << " simulation" << std::endl;
        auto pd_regular = parameterSweep("regular", "PD", pd_b_start, pd_b_end, pd_steps, N, z, repeats);
        auto pd_sf = parameterSweep("scale-free", "PD", pd_b_start, pd_b_end, pd_steps, N, z, repeats);

        auto sg_regular = parameterSweep("regular", "SG", sg_b_start, sg_b_end, sg_steps, N, z, repeats);
        auto sg_sf = parameterSweep("scale-free", "SG", sg_b_start, sg_b_end, sg_steps, N, z, repeats);

        std::cout << "Begin writing to files..." << std::endl;

        std::string pd_filename = "pd_results_z=" + std::to_string(z) + ".csv";
        std::ofstream out_pd(pd_filename);
        out_pd << "b,regular,scale-free\n";
        for (int i = 0; i < pd_steps; ++i) {
            out_pd << pd_regular[i][0] << "," << pd_regular[i][1] << "," << pd_sf[i][1] << "\n";
        }
        out_pd.close();

        std::string sg_filename = "sg_results_z=" + std::to_string(z) + ".csv";
        std::ofstream out_sg(sg_filename);
        out_sg << "b,regular,scale-free\n";
        for (int i = 0; i < sg_steps; ++i) {
            if (i == 0) continue;
            out_sg << sg_regular[i][0] << "," << sg_regular[i][1] << "," << sg_sf[i][1] << "\n";
        }
        out_sg.close();

        std::cout << "z=" << z << " Simulations completed.\n";
    }

    return 0;
}