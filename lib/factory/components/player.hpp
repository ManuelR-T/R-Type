/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** player_name
*/

#pragma once

#include <string>

namespace ecs::component {

/**
 * @struct player
 * @brief Component that store information on players.
 *
 * The `player` component marks an entity as player.
 * It contain.
 */
struct Player {
    std::string name;
    std::size_t id;
};

} // namespace ecs::component
