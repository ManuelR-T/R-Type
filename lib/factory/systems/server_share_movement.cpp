/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** server_share_movement
*/

#include "components/server_share_movement.hpp"
#include <list>
#include "RTypeUDPProtol.hpp"
#include "Registry.hpp"
#include "Zipper.hpp"
#include "components/position.hpp"
#include "components/velocity.hpp"
#include "components/shared_entity.hpp"

namespace ecs::systems {

void serverShareMovement(Registry &reg, std::list<std::vector<char>> &datasToSend)
{
    auto &sharedMov = reg.getComponents<ecs::component::ServerShareMovement>();
    auto &positions = reg.getComponents<ecs::component::Position>();
    auto &velocities = reg.getComponents<ecs::component::Velocity>();
    auto &sharedEntity = reg.getComponents<ecs::component::SharedEntity>();

    ecs::Zipper<
        ecs::component::ServerShareMovement,
        ecs::component::Position,
        ecs::component::Velocity,
        ecs::component::SharedEntity>
        zip(sharedMov, positions, velocities, sharedEntity);

    for (auto [_, pos, vel, shared_entity] : zip) {
        datasToSend.push_back(rt::UDPPacket<rt::UDPBody::MOVE_ENTITY>({.cmd = rt::UDPCommand::MOVE_ENTITY,
                                                                       .sharedEntityId = shared_entity.sharedEntityId,
                                                                       .body = {.pos = pos, .vel = vel}}
        ).serialize());
    }
}

} // namespace ecs::systems
