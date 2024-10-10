/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** drawable
*/

#pragma once

#include <SFML/Graphics/RectangleShape.hpp>

namespace ecs::component {

/**
 * @brief Represents a drawable component for an entity.
 *
 * The `drawable` component contains a shape that can be rendered on the screen.
 * It utilizes SFML's `RectangleShape` to define the visual representation.
 */
struct Drawable {
    sf::RectangleShape shape; /**< The shape used for rendering the entity */
};

} // namespace ecs::component
