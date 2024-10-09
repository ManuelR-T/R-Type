/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** sound_emitter_system
*/

#pragma once

#include "Registry.hpp"
#include "../SoundManager.hpp"

namespace ecs::systems {

/**
 * @brief System to handle sound effects emitted by entities.
 *
 * Plays sounds based on the SoundEmitter component.
 *
 * @param reg The ECS registry containing entities and components.
 * @param soundManager Reference to the SoundManager.
 * @param dt Delta time since the last update.
 */
void SoundEmitterSystem(ecs::Registry &reg, ecs::SoundManager &soundManager);

} // namespace ecs::systems
