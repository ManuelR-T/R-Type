/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** sound_emitter
*/

#pragma once

#include <SFML/Audio.hpp>
#include <string>

namespace ecs::component {

/**
 * @brief Component representing an entity that can emit sound effects.
 */
struct SoundEmitter {
    std::string soundBufferId;
    float volume;
    bool loop;
};

} // namespace ecs::component
