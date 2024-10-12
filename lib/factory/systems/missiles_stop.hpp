/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** missiles_stop
*/

#pragma once

#include "Registry.hpp"

/**
 * @brief Stops the missiles in the ECS registry.
 *
 * This function is responsible for stopping the missiles that are currently
 * active in the ECS registry. It processes the entities and components
 * related to missiles and kill the missiles entities when needed.
 *
 * @param reg Reference to the ECS registry containing the entities and components.
 */
namespace ecs::systems {

void missilesStop(ecs::Registry &reg);

} // namespace ecs::systems
