/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** control_move
*/

#pragma once

#include <SFML/Window/Event.hpp>
#include "InputManager.hpp"
#include "Registry.hpp"

namespace ecs::systems {

/**
 * @brief Handles entity control movement.
 *
 * The `control_move` function processes player input with the object @param input
 * and retrieve its keyboard states.
 *
 * @param reg Reference to the registry managing entities and their components.
 * @param input Input manager object that contains states of keyboard values.
 */
void controlMove(Registry &reg, ecs::InputManager &input);

} // namespace ecs::systems
