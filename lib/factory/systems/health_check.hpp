/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** health_check
*/

#pragma once

#include "Registry.hpp"

/**
 * @brief Perform health checks on entities within the registry.
 *
 * This function iterates through the entities in the provided registry and performs necessary health checks.
 *
 * @param reg Reference to the registry containing the entities to be checked.
 */
namespace ecs::systems {

void healthCheck(Registry &reg);

} // namespace ecs::systems
