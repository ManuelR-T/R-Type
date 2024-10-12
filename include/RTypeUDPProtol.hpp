/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** RTypeUDPProtol
*/

#pragma once

#include <cstddef>
#include <cstdint>
#include "components/position.hpp"
#include "components/velocity.hpp"
#include "shared_entity.hpp"

namespace rt {
enum class UDPCommand : std::uint8_t {
    NONE,
    NEW_ENTITY,
    MOVE_ENTITY,

    MOD_ENTITY,
    MOD_ENTITES,
    DEL_ENTITY
};

enum class EntityType : std::uint8_t {
    NONE,
    STATIC,
    PLAYER,
    MISSILE,
    MISSILE_BALL,
    BYDOS_WAVE,
    ROBOT_GROUND,
};

/**
 * ! Later on we could have a timestamp member variable
 *   or an id to check late packets
 */
struct UDPHeader {
    std::size_t magic = 0x42424242;
    UDPCommand cmd;
};

struct UDPBody {
    struct ShareMovement {
        ecs::component::Position pos = {};
        ecs::component::Velocity vel = {};
    };

    struct NewEntityData {
        std::size_t playerId = 0;
        std::size_t playerIndex = 1;
        EntityType type; // ! One byte for the type of entity, depend on factory too
        ShareMovement moveData = {};
    };

    shared_entity_t sharedEntityId;

    union {
        ShareMovement shareMovement;
        NewEntityData newEntityData;
    } b = {};
};

struct UDPServerPacket {
    UDPHeader header;

    /*
     * Actually the same as UDPClientPacket because later on we will have pointer
     * and size to send multiple datas at the same time
     */
    UDPBody body;
};

struct UDPClientPacket {
    UDPHeader header;

    UDPBody body;
};

} // namespace rt
