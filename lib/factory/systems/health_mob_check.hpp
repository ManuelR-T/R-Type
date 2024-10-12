/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** health_mob_check
*/

#pragma once

#include "Registry.hpp"
#include "WaveManager.hpp"

/**
 * @brief Checks the health of mobs and updates the wave manager accordingly.
 *
 * This function iterates through the entities in the registry, checks their health status,
 * and performs necessary actions such as removing dead mobs and updating the wave manager.
 *
 * @param reg Reference to the registry containing the entities.
 * @param waveManager Reference to the wave manager responsible for handling waves of mobs.
 */
namespace ecs::systems {

void healthMobCheck(Registry &reg, WaveManager &waveManager);

} // namespace ecs::systems
