/*
** EPITECH PROJECT, 2024
** Rtype
** File description:
** position
*/

#pragma once

#include "../core/registry.hpp"

namespace ecs::systems {

/**
 * @brief Updates the position of all entities based on their velocity.
 *
 * The `position` function iterates through all entities with both `position` and
 * `velocity` components, updating their positions based on their velocities and
 * the elapsed time `dt`.
 *
 * @param reg Reference to the registry managing entities and their components.
 * @param dt The delta time since the last update, used to scale velocity.
 */
void position(registry &reg, float dt);

} // namespace ecs::systems
