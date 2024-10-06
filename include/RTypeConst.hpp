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

constexpr unsigned int SCREEN_WIDTH = 720;
constexpr unsigned int SCREEN_HEIGHT = 480;

constexpr unsigned int MIN_SCREEN_WIDTH = 1920 * 0.2;
constexpr unsigned int MIN_SCREEN_HEIGHT = 1080 * 0.2;

constexpr std::size_t TCP_MAGIC = 0x5443503e; // '<RT-TCP>'

constexpr std::size_t MAX_ROOM_NAME_SIZE = 32;
constexpr std::size_t MAX_USER_NAME_SIZE = 32;

constexpr std::size_t NB_STAGE = 10;
} // namespace rt
