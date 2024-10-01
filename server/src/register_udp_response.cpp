/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** register_ecs
*/

#include "RTypeServer.hpp"
#include "components/position.hpp"
#include "components/velocity.hpp"

void rts::registerUdpResponse(ecs::Registry &reg, ntw::ResponseHandler<rt::UDPCommand, rt::UDPPacket> &responseHandler)
{
    responseHandler.registerHandler(rt::UDPCommand::NEW_PLAYER, [&reg](const rt::UDPPacket &msg) {
        rts::createPlayer(reg, msg.sharedEntityId);
    });

    responseHandler.registerHandler(rt::UDPCommand::MOD_ENTITY, [&reg](const rt::UDPPacket &msg) {
        try {
            reg.getComponent<ecs::component::Position>(reg.getLocalEntity().at(msg.sharedEntityId)).value() =
                msg.body.shareMovement.pos;
            reg.getComponent<ecs::component::Velocity>(reg.getLocalEntity().at(msg.sharedEntityId)).value() =
                msg.body.shareMovement.vel;
        } catch (...) {
        }
    });
    responseHandler.registerHandler(rt::UDPCommand::NEW_ENTITY, [&reg](const rt::UDPPacket &msg) {
        rts::createMissile(reg, msg);
    });
}
