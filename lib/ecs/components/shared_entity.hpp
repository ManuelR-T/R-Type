/*
** EPITECH PROJECT, 2024
** R-type
** File description:
** shared_entity
*/

#pragma once

#include "../core/shared_entity.hpp"

namespace ecs::component {

/**
 * @struct shared_entity
 * @brief Component that store shared_entity ID.
 *
 * The `shared_entity` component marks an entity as shared.
 */
struct shared_entity {
    shared_entity_t shared_entity_id;
};

} // namespace ecs::component
