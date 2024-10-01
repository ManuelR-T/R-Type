/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ai_act
*/

#pragma once

#include "core/Registry.hpp"

namespace ecs::systems {

/**
 * @brief Make ai act with respect with their `ai_actor` component and his callback `act`.
 *
 * @param reg Reference to the registry managing entities and their components.
 */
void aiAct(Registry &reg);

} // namespace ecs::systems
