/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ai_actor
*/

#pragma once

#include <functional>
#include "core/entity.hpp"
#include "core/registry.hpp"

namespace ecs::component {

/**
 * @struct ai_actor
 * @brief Component that define ai behaviour.
 *
 * The `act` function take the @param registry and the @param entity
 * to modify the current ai.
 */
struct ai_actor {
    bool val = false; // ! For testing
    std::function<void (registry &registry, entity_t entity)> act;
};

}
