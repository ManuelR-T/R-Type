/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** register_ecs
*/

#include "components/position.hpp"
#include "components/velocity.hpp"
#include "rtype_server.hpp"

void rts::registerUdpResponse(ecs::Registry &reg, ecs::ResponseHandler<rt::UdpCommand, rt::UdpPacket> &responseHandler)
{
    responseHandler.registerHandler(rt::UdpCommand::NEW_PLAYER, [&reg](const rt::UdpPacket &msg) {
        rts::createPlayer(reg, msg.shared_entity_id);
    });

    responseHandler.registerHandler(rt::UdpCommand::MOD_ENTITY, [&reg](const rt::UdpPacket &msg) {
        reg.getComponent<ecs::component::Position>(reg.getLocalEntity().at(msg.shared_entity_id)).value() =
            msg.body.share_movement.pos;
        reg.getComponent<ecs::component::Velocity>(reg.getLocalEntity().at(msg.shared_entity_id)).value() =
            msg.body.share_movement.vel;
    });
    responseHandler.registerHandler(rt::UdpCommand::NEW_ENTITY, [&reg](const rt::UdpPacket &msg) {
        rts::createMissile(reg, msg);
    });
}
