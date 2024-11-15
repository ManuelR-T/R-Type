/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** fireRandomMissileAi
*/

#pragma once

#include <array>
#include <functional>
#include <list>
#include <vector>
#include "Registry.hpp"
#include "TimeoutHandler.hpp"

namespace rts::ais {
entity_t fireRandomMissileAi(
    ecs::Registry &reg,
    entity_t e,
    std::list<std::vector<char>> &datasToSend,
    ntw::TimeoutHandler &timeoutHandler,
    ntw::UDPServer &udpServer,
    int missileSpawnRate,
    const std::function<bool()> &cond = nullptr,
    std::array<float, 2> randXRange = {-1, 1}, // ! Arrays used as trigonometrical limits
    std::array<float, 2> randYRange = {-1, 1}
);
} // namespace rts::ais
