/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ai_actor
*/

#pragma once

#include <functional>
#include "Registry.hpp"
#include "entity.hpp"

namespace ecs::component {

/**
 * @struct ai_actor
 * @brief Component that define ai behaviour.
 *
 * The `act` function take the @param registry and the @param entity
 * to modify the current ai.
 */
struct AiActor {
    std::function<void(Registry &registry, entity_t entity)> act;
};

} // namespace ecs::component
