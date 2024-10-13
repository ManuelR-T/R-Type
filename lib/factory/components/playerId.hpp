/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** playerId
*/

#pragma once

#include <cstddef>

namespace ecs::component {

/**
 * @struct Player
 * @brief Component that mark the entity as a player with a specific id.
 *
 * The id is usually given by the client by network communication and is used
 * for manipulating a specific player entity based on his id.
 */
struct Player {
    size_t playerId;
};
} // namespace ecs::component
