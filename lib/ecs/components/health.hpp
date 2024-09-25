/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** health
*/

#pragma once

namespace ecs::component {

/**
 * @brief Represents the health of an entity.
 *
 * The health is an integer that decrease when the entity enter in collision
 * with an enemy
 */
struct health {
    int max_hp;
    int curr_hp;
};

} // namespace ecs::component
