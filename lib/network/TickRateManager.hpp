/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** tick_rate_manager
*/

#pragma once

#include <SFML/Window/Event.hpp>
#include <unordered_map>

namespace ntw {

template <typename T>
class TickRateManager {
    public:
    void addTickRate(const T &id, float tickRate)
    {
        _tickRate[id] = {._time_between_tick = (1 / tickRate), ._elapsed_time = 0};
    }

    void removeTickRate(const T &id)
    {
        _tickRate.erase(id);
    }

    bool needUpdate(const T &id, float dt)
    {
        auto &[time_between_tick, elapsed_time] = _tickRate.at(id);
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

    std::unordered_map<T, TickData> _tickRate;
};

} // namespace ntw
