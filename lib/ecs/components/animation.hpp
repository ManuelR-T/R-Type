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
#include "core/Registry.hpp"
#include <unordered_map>

namespace ecs::component {

/**
 * @brief Component that represents animation data for a sprite.
 *
 * The `animation` component holds information about the frames of an animation,
 * timing between frames, and manages the current state of the animation.
 */
struct Animation {
    std::unordered_map<std::string, std::vector<sf::IntRect>>
        frames;                    /**< Rectangles defining each frame of the animation. */
    float frameTime = 0.2f;        /**< Duration of each frame in seconds. */
    float elapsedTime = 0.0f;      /**< Time elapsed since the last frame change. */
    size_t currentFrame = 0;       /**< Index of the current frame in the animation. */
    std::string state = "idle"; /**< State of the animation. */
    std::function<void(Registry &, entity_t, ecs::component::Animation &)> updateState =
        [](Registry &, entity_t, ecs::component::Animation &) {}; /**< Update the state of the animation. */
};

} // namespace ecs::component
