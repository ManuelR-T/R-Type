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
struct Health {
    int maxHp;
    int currHp;
};

} // namespace ecs::component
