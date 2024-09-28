/*
** EPITECH PROJECT, 2024
** R-type
** File description:
** position
*/

#include "../components/share_movement.hpp"
#include "../components/position.hpp"
#include "../components/velocity.hpp"
#include "../core/registry.hpp"
#include "../core/zipper.hpp"
#include "GameProtocol.hpp"
#include "UDPClient.hpp"
#include "../components/shared_entity.hpp"

namespace ecs::systems {

void shareMovement(Registry &reg, client::UDPClient &udpClient)
{
    auto &sharedMov = reg.getComponents<ecs::component::ShareMovement>();
    auto &positions = reg.getComponents<ecs::component::Position>();
    auto &velocitys = reg.getComponents<ecs::component::Velocity>();
    auto &sharedEntity = reg.getComponents<ecs::component::SharedEntity>();

    Zipper<
        ecs::component::ShareMovement,
        ecs::component::Position,
        ecs::component::Velocity,
        ecs::component::SharedEntity>
        zip(sharedMov, positions, velocitys, sharedEntity);

    for (auto [_, pos, vel, shared_entity] : zip) {
        rt::UdpPacket msg = {.cmd = rt::UdpCommand::MOD_ENTITY, .shared_entity_id = shared_entity.shared_entity_id};
        msg.body.share_movement = {.pos = {pos.x, pos.y}, .vel = {.vx = vel.vx, .vy = vel.vy}};
        udpClient.send(reinterpret_cast<const char *>(&msg), sizeof(msg));
    }
}

} // namespace ecs::systems
