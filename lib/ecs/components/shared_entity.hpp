/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** shared_entity
*/

#pragma once

#include "../shared_entity.hpp"

namespace ecs::component {

/**
 * @struct shared_entity
 * @brief Component that store shared_entity ID.
 *
 * The `shared_entity` component marks an entity as shared.
 */
struct SharedEntity {
    shared_entity_t sharedEntityId;
};

} // namespace ecs::component
