/*
** EPITECH PROJECT, 2024
** R-type
** File description:
** colision
*/

#pragma once

#include "../core/registry.hpp"

namespace systems {

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
void collision(registry &reg);

} // namespace systems
