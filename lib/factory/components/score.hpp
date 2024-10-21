/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** score
*/

#pragma once

#include <memory>
#include "imgui.h"
#include <imgui-SFML.h>

namespace ecs::component {

/**
 * @brief Represents the score of a player.
 *
 * The score is an integer that increase when player kill enemies
 */

struct Score {
    std::shared_ptr<ImFont> font;
    int value = 0;
};

} // namespace ecs::component
