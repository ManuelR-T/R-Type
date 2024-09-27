/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** animation
*/

#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <functional>
#include <string>
#include <vector>
#include "core/registry.hpp"
#include <unordered_map>

namespace ecs::component {

/**
 * @brief Component that represents animation data for a sprite.
 *
 * The `animation` component holds information about the frames of an animation,
 * timing between frames, and manages the current state of the animation.
 */
struct animation {
    std::unordered_map<std::string, std::vector<sf::IntRect>>
        frames;                    /**< Rectangles defining each frame of the animation. */
    float frame_time = 0.2f;       /**< Duration of each frame in seconds. */
    float elapsed_time = 0.0f;     /**< Time elapsed since the last frame change. */
    size_t current_frame = 0;      /**< Index of the current frame in the animation. */
    std::string state = "neutral"; /**< State of the animation. */
    std::function<void(registry &, entity_t, ecs::component::animation &)> updateState =
        [](registry &, entity_t, ecs::component::animation &) {}; /**< Update the state of the animation. */
};

} // namespace ecs::component
