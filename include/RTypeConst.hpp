/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** config
*/

#pragma once

#include <cstddef>

namespace rt {
constexpr unsigned int FPS_LIMIT = 60;

constexpr float MOVEMENT_TICK_RATE = 30.0;
constexpr float AI_ACTING_TICK_RATE = 10.0;
constexpr float SEND_PACKETS_TICK_RATE = 15.0;
constexpr float CALL_NETWORK_CALLBACKS_TICK_RATE = 25.0;

constexpr unsigned int SCREEN_WIDTH = 1920 / 2.4;
constexpr unsigned int SCREEN_HEIGHT = 1080 / 2.4;

constexpr std::size_t TCP_MAGIC = 0x5443503e; // '<RT-TCP>'

constexpr std::size_t MAX_ROOM_NAME_SIZE = 32;
constexpr std::size_t MAX_USER_NAME_SIZE = 32;
} // namespace rt
