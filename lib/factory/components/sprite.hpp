/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** sprite
*/

#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <string>

namespace ecs::component {

/**
 * @brief Component that represents a renderable sprite for an entity.
 *
 * The `sprite` component contains the necessary data to render an entity using a sprite.
 * It holds the texture identifier and the SFML `Sprite` object, which can be used for drawing.
 */
struct Sprite {
    std::string textureId; /**< Identifier for the texture used by the sprite. */
    sf::Sprite spriteObj;  /**< The SFML sprite object used for rendering. */
};

} // namespace ecs::component
