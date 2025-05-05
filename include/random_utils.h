// random_utils.h
#ifndef RANDOM_UTILS_H
#define RANDOM_UTILS_H

#include <random>
#include <chrono>
#include <omp.h>

inline thread_local std::mt19937 thread_rng(
    std::chrono::steady_clock::now().time_since_epoch().count() ^ omp_get_thread_num()
);

#endif // RANDOM_UTILS_H