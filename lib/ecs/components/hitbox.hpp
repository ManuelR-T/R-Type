/*
** EPITECH PROJECT, 2024
** R-type
** File description:
** hitbox
*/

#pragma once

namespace ecs::component {

/**
 * @brief Represents a hitbox component for collision detection.
 *
 * The `hitbox` component defines the dimensions of an entity's collision area.
 * It is used to detect and handle collisions with other entities.
 */
struct hitbox {
    float width = 0.0f;  /**< The width of the hitbox */
    float height = 0.0f; /**< The height of the hitbox */
};

} // namespace ecs::component
