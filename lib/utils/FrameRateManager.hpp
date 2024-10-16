/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** FrameRateManager
*/

#pragma once

#include <chrono>

namespace eng {

class FramerateManager {
    public:
    FramerateManager(double targetFPS);

    void setFrameRate(double targetFPS);
    void start();
    void end();

    private:
    std::chrono::duration<double> targetFrameTime;
    std::chrono::steady_clock::time_point startTime;
};

} // namespace eng
