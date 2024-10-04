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

constexpr unsigned int SCREEN_WIDTH = 720;
constexpr unsigned int SCREEN_HEIGHT = 480;

constexpr std::size_t TCP_MAGIC = 0x5443503e; // '<RT-TCP>'
} // namespace rt
