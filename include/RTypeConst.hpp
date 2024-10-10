/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** config
*/

#pragma once

#include <cstddef>

namespace rt {

/**
 * @brief The maximum frames per second limit for the game.
 */
constexpr unsigned int FPS_LIMIT = 60;

/**
 * @brief The default screen width for the game window.
 */
constexpr unsigned int SCREEN_WIDTH = 720;

/**
 * @brief The default screen height for the game window.
 */
constexpr unsigned int SCREEN_HEIGHT = 480;

/**
 * @brief The minimum screen width for the game window.
 */
constexpr unsigned int MIN_SCREEN_WIDTH = 1920 * 0.2;

/**
 * @brief The minimum screen height for the game window.
 */
constexpr unsigned int MIN_SCREEN_HEIGHT = 1080 * 0.2;

/**
 * @brief A magic number used for TCP communication.
 */
constexpr std::size_t TCP_MAGIC = 0x5443503e; // '<RT-TCP>'

/**
 * @brief The maximum size for a room name.
 */
constexpr std::size_t MAX_ROOM_NAME_SIZE = 32;

/**
 * @brief The maximum size for a user name.
 */
constexpr std::size_t MAX_USER_NAME_SIZE = 32;

/**
 * @brief The number of stages in the game.
 */
constexpr std::size_t NB_STAGE = 10;

} // namespace rt
