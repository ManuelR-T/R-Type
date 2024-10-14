/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ServerTickRate
*/

#pragma once

#include <unordered_map>

namespace rts {

enum class TickRate : std::size_t {
    ENTITY_MOVEMENT,
    AI_ACTING,
    SEND_PACKETS
};

const std::unordered_map<rts::TickRate, float> SERVER_TICKRATE = {
    {rts::TickRate::ENTITY_MOVEMENT, 60.0},
    {rts::TickRate::AI_ACTING, 60.0},
    {rts::TickRate::SEND_PACKETS, 60.0}, // Warning: Other TickRate are capped by this one
};

} // namespace rts
