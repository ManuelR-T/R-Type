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
    AI_ACTING,
    SEND_PACKETS
};

const std::unordered_map<rts::TickRate, float> SERVER_TICKRATE = {
    {rts::TickRate::SEND_PACKETS, 60.0},
    {rts::TickRate::AI_ACTING, 60.0}
};

} // namespace rts