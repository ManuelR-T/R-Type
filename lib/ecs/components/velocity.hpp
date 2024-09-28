/*
** EPITECH PROJECT, 2024
** R-type
** File description:
** velocity
*/

#pragma once

namespace ecs::component {

/**
 * @brief Represents a velocity component for an entity.
 *
 * The `velocity` component stores the velocity of an entity along the x and y axes.
 * It is used to update the entity's position over time.
 */
struct Velocity {
    float vx = 0.0f; /**< The velocity along the x-axis */
    float vy = 0.0f; /**< The velocity along the y-axis */
};

} // namespace ecs::component
