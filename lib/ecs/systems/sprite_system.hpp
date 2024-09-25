/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** sprite_system
*/

#pragma once

#include "../core/registry.hpp"
#include "../core/SpriteManager.hpp"

namespace ecs::systems {

/**
 * @brief Updates sprite animations and synchronizes sprite positions.
 *
 * The `sprite_system` function processes entities with `sprite` components,
 * updates their animations if they have an `animation` component, and synchronizes
 * the sprite's position with the entity's `position` component.
 *
 * @param reg Reference to the registry managing entities and their components.
 * @param dt The delta time since the last update, used to update animations.
 * @param sprite_manager Reference to the `SpriteManager` for texture access.
 */
void sprite_system(registry &reg, float dt, SpriteManager &sprite_manager);

} // namespace ecs::systems
