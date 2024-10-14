/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** draw_player_beam_bar
*/

#pragma once

#include <SFML/Graphics.hpp>
#include "Registry.hpp"

namespace ecs::systems {

void drawScore(Registry &reg, sf::RenderWindow &window, const sf::Vector2u &windowSize);

} // namespace ecs::systems
