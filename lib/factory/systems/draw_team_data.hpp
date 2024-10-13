/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** draw_player_beam_bar
*/

#pragma once

#include <SFML/Graphics.hpp>
#include "Registry.hpp"
#include "imgui.h"

namespace ecs::systems {

void drawTeamData(Registry &reg, const sf::Vector2u &windowSize);

} // namespace ecs::systems
