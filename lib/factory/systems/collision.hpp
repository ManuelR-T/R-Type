/*
** EPITECH PROJECT, 2024
** R-type
** File description:
** collision
*/

#pragma once

#include "Registry.hpp"

namespace ecs::systems {

/**
 * @brief Handles collision detection and response for entities.
 *
 * The `collision` function iterates through entities managed by the provided
 * `registry`, checks for collisions between entities with `hitbox` and `position` components,
 * and updates their states accordingly. This function is part of the systems that
 * process game logic in the ECS (Entity-Component-System) architecture.
 *
 * @param reg Reference to the registry managing entities and their components.
 */
void collision(Registry &reg);

} // namespace ecs::systems
