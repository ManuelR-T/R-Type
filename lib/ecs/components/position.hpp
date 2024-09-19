/*
** EPITECH PROJECT, 2024
** R-type
** File description:
** position
*/

#pragma once

namespace ecs::component {

/**
 * @brief Represents a position component for an entity.
 *
 * The `position` component stores the x and y coordinates of an entity in the game world.
 * It is used to track and update the entity's location.
 */
struct position {
    float x = 0.0f; /**< The x-coordinate of the entity's position */
    float y = 0.0f; /**< The y-coordinate of the entity's position */
};

} // namespace ecs::component
