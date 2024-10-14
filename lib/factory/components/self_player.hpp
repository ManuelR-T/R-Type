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
 * @struct SelfPlayer
 * @brief Component that mark etity as SelfPlayer.
 *
 * The `SelfPlayer` component marks an entity as SelfPlayer.
 */
struct SelfPlayer {
    std::string name;
    std::size_t id = 0;
};

} // namespace ecs::component
