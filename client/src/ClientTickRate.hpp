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
    MOVEMENT,
    AI_ACTING,
    CALL_NETWORK_CALLBACKS
};

const std::unordered_map<rtc::TickRate, float> CLIENT_TICKRATE =
    {{rtc::TickRate::MOVEMENT, 30.0}, {rtc::TickRate::AI_ACTING, 10.0}, {rtc::TickRate::CALL_NETWORK_CALLBACKS, 25.0}};

} // namespace rtc
