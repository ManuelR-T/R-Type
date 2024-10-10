/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** music_component
*/

#pragma once

#include <SFML/Audio.hpp>
#include <string>

namespace ecs::component {

/**
 * @brief Component representing background music.
 */
struct MusicComponent {
    std::string musicFilePath;
    float volume;
    bool loop;
    bool isPlaying;
};

} // namespace ecs::component
