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
#include "../components/shared_entity.hpp"
#include "../systems/share_movement.hpp"

namespace ecs::systems {

void share_movement(registry &reg, client::UDPClient &udpClient)
{
    auto &shared_mov = reg.get_components<ecs::component::share_movement>();
    auto &positions = reg.get_components<ecs::component::position>();
    auto &velocitys = reg.get_components<ecs::component::velocity>();
    auto &shared_entity = reg.get_components<ecs::component::shared_entity>();

    zipper<
        ecs::component::share_movement,
        ecs::component::position,
        ecs::component::velocity,
        ecs::component::shared_entity>
        zip(shared_mov, positions, velocitys, shared_entity);

    for (auto [_, pos, vel, shared_entity] : zip) {
        ecs::protocol msg = {
            .action = ecs::ntw_action::MOD_ENTITY,
            .shared_entity_id = shared_entity.shared_entity_id,
            .data = ecs::ntw::movement{.pos = {pos.x, pos.y}, .vel = {.vx = vel.vx, .vy = vel.vy}}
        };
        udpClient.send(reinterpret_cast<const char *>(&msg), sizeof(msg));
    }
}

} // namespace ecs::systems
