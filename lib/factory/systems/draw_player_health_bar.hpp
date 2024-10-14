/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** draw_player_health_bar
*/

#pragma once

#include <SFML/Graphics.hpp>
#include "Registry.hpp"

namespace ecs::systems {

void drawPlayerHealthBar(Registry &reg, const sf::Vector2u &windowSize);

} // namespace ecs::systems
