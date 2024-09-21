/*
** EPITECH PROJECT, 2024
** R-type
** File description:
** position
*/

#include "../components/position.hpp"
#include "../components/velocity.hpp"
#include "../components/share_position.hpp"
#include "../core/registry.hpp"
#include "../core/zipper.hpp"
#include "../systems/share_position.hpp"
#include "GameProtocol.hpp"

namespace ecs::systems {

void share_position(registry &reg, client::UDPClient &udpClient)
{
    auto &shared_position = reg.get_components<ecs::component::share_position>();
    auto &positions = reg.get_components<ecs::component::position>();

    zipper<ecs::component::share_position, ecs::component::position> zip(shared_position, positions);

    for (auto [_, pos] : zip) {
        pos.x;
        pos.y;

        ecs::protocol msg = {
            .action = ecs::ntw_action::MOD_ENTITY,
            .entity = 1000, // !!! to repla with future uuid
            .data = ecs::component::position{pos.x, pos.y}
        };
        udpClient.send(reinterpret_cast<const char *>(&msg), sizeof(msg));
    }
}

} // namespace ecs::systems
