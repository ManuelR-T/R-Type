/*
** EPITECH PROJECT, 2024
** R-type
** File description:
** control
*/

#pragma once

#include "../core/registry.hpp"

namespace systems {

/**
 * @brief Handles entity control and input processing.
 *
 * The `control` function processes player input and updates the relevant
 * components of entities accordingly. This may involve modifying
 * position, velocity, or other components based on input actions.
 *
 * @param reg Reference to the registry managing entities and their components.
 */
void control(registry &reg);

} // namespace systems
