/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** RTypeUDPProtol
*/

#pragma once

#include "components/position.hpp"
#include "components/velocity.hpp"
#include "core/shared_entity.hpp"

namespace rt {
enum class UDPCommand : std::size_t {
    NONE,
    NEW_PLAYER,
    NEW_ENTITY,
    MOD_ENTITY,
    DEL_ENTITY
};

struct UDPPacket {
    UDPCommand cmd;

    shared_entity_t sharedEntityId;

    union {
        struct ShareMovement {
            ecs::component::Position pos;
            ecs::component::Velocity vel;
        } shareMovement;
    } body = {};
};
} // namespace rt
