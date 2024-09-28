/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** tick_rate_manager
*/

#pragma once

#include <SFML/Window/Event.hpp>
#include <unordered_map>

namespace ecs {
class TickRateManager {
    public:
    void addTickRate(float tickRate)
    {
        _tickRate[tickRate] = {1 / tickRate, 0};
    }

    void removeTickRate(float tickRate)
    {
        _tickRate.erase(tickRate);
    }

    bool needUpdate(float tickRate, float dt)
    {
        auto &[time_between_tick, elapsed_time] = _tickRate[tickRate];
        elapsed_time += dt;
        if (elapsed_time >= time_between_tick) {
            elapsed_time -= time_between_tick;
            return true;
        }
        return false;
    }

    private:
    struct TickData {
        float _time_between_tick = 0;
        float _elapsed_time = 0;
    };

    std::unordered_map<float, TickData> _tickRate;
};
} // namespace ecs
