/*
** EPITECH PROJECT, 2024
** Rtype
** File description:
** position
*/

#pragma once

#include <SFML/Graphics.hpp>
#include "../core/registry.hpp"

namespace ecs::systems {

/**
 * @brief Renders all drawable entities to the provided window.
 *
 * The `draw` function iterates through all entities with a `drawable` component
 * and renders their associated shapes to the given SFML render window.
 *
 * @param reg Reference to the registry managing entities and their components.
 * @param window Reference to the SFML render window where entities are drawn.
 */
void draw(registry &reg, sf::RenderWindow &window);

} // namespace ecs::systems
