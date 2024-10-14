/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ClientTickRate
*/

#pragma once

#include <unordered_map>

namespace rtc {

enum class TickRate : std::size_t {
    ENTITY_MOVEMENT,
    CALL_NETWORK_CALLBACKS
};

const std::unordered_map<rtc::TickRate, float> CLIENT_TICKRATE = {
    {rtc::TickRate::ENTITY_MOVEMENT, 60.0},
    {rtc::TickRate::CALL_NETWORK_CALLBACKS, 60.0}
};

} // namespace rtc
