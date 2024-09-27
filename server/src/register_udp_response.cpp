/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** register_ecs
*/

#include "rtype_server.hpp"

void rts::register_udp_response(
    ecs::registry &reg,
    ecs::response_handler<rt::udp_command, rt::udp_packet> &response_handler
)
{
    response_handler.register_handler(rt::udp_command::NEW_PLAYER, [&reg](const rt::udp_packet &msg) {
        rts::create_player(reg, msg.shared_entity_id);
    });

    response_handler.register_handler(rt::udp_command::MOD_ENTITY, [&reg](const rt::udp_packet &msg) {
        reg.get_component<ecs::component::position>(reg.get_local_entity().at(msg.shared_entity_id)).value() =
            msg.body.share_movement.pos;
        reg.get_component<ecs::component::velocity>(reg.get_local_entity().at(msg.shared_entity_id)).value() =
            msg.body.share_movement.vel;
    });
    response_handler.register_handler(rt::udp_command::NEW_ENTITY, [&reg](const rt::udp_packet &msg) {
        rts::create_missile(reg, msg);
    });
}
