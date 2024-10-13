/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** score
*/

#pragma once

#include "imgui.h"
#include <imgui-SFML.h>

namespace ecs::component {

/**
 * @brief Represents the score of a player.
 *
 * The score is an integer that increase when player kill enemies
 */

struct Score {
    ImFont *font;
    std::string text;
    // sf::Font font;
    // sf::Text text;
    int value;
};

} // namespace ecs::component
