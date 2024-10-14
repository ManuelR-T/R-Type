/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** health
*/

#pragma once

namespace ecs::component {

/**
 * @brief Represents the beam of an entity.
 *
 * The beam is an integer that increase when you press space bar
 */

struct Beam {
    float power;
};

} // namespace ecs::component
