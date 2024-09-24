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
class tick_rate_manager {
    public:
    void add_tick_rate(float tick_rate)
    {
        _tick_rate[tick_rate] = {1 / tick_rate, 0};
    }

    void remove_tick_rate(float tick_rate)
    {
        _tick_rate.erase(tick_rate);
    }

    bool need_update(float tick_rate, float dt)
    {
        auto &[time_between_tick, elapsed_time] = _tick_rate[tick_rate];
        elapsed_time += dt;
        if (elapsed_time >= time_between_tick) {
            elapsed_time -= time_between_tick;
            return true;
        }
        return false;
    }

    private:
    struct tick_data_t {
        float _time_between_tick = 0;
        float _elapsed_time = 0;
    };

    std::unordered_map<float, tick_data_t> _tick_rate;
};
} // namespace ecs
