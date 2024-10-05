/*
** EPITECH PROJECT, 2024
** R-type
** File description:
** parallax
*/

#pragma once

#include <cstddef>
#include <limits>

namespace ecs::component {

/**
 * @struct parallax
 * @brief Component that makes an entity print on a specific layer.
 *
 * The `parallax` component marks an entity as parallax.
 * It can be used to print the entity on a specific layer.
 */
struct Parallax {
    size_t layer = 0;
    int repeatX = std::numeric_limits<int>::min();
    int repeatY = std::numeric_limits<int>::min();
    int respawnX = 0;
    int respawnY = 0;
};

} // namespace ecs::component
