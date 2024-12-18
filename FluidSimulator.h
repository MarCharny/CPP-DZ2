#ifndef FLUID_SIMULATOR_H
#define FLUID_SIMULATOR_H

#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <functional>
#include "ThreadPool.h"

template <typename T>
class FluidSimulator {
public:
    FluidSimulator(size_t width, size_t height, T gravity, size_t numThreads);
    void loadField(const std::string &filename);
    void simulate(size_t steps);

private:
    size_t width_;
    size_t height_;
    T gravity_;
    std::vector<std::vector<T>> pressure_;
    std::vector<std::vector<char>> field_;
    ThreadPool threadPool;

    void parallelForEachCell(const std::function<void(size_t, size_t)> &func);

    void applyExternalForces();
    void calculatePressures();
    void updateField();
    void printField(size_t step);
};

#endif // FLUID_SIMULATOR_H