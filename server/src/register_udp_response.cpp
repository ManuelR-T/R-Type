/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** register_ecs
*/

#include <utility>
#include "RTypeServer.hpp"
#include "RTypeUDPProtol.hpp"
#include "components/position.hpp"
#include "components/velocity.hpp"

void rts::registerUdpResponse(
    ecs::Registry &reg,
    ntw::ResponseHandler<rt::UDPCommand, rt::UDPClientPacket> &responseHandler,
    std::list<rt::UDPServerPacket> &datasToSend
)
{
    responseHandler.registerHandler(rt::UDPCommand::NEW_PLAYER, [&reg](const rt::UDPClientPacket &msg) {
        rts::createPlayer(reg, msg.body.sharedEntityId);
    });

    responseHandler.registerHandler(rt::UDPCommand::MOD_ENTITY, [&reg](const rt::UDPClientPacket &msg) {
        try {
            reg.getComponent<ecs::component::Position>(reg.getLocalEntity().at(msg.body.sharedEntityId)).value() =
                msg.body.b.shareMovement.pos;
            reg.getComponent<ecs::component::Velocity>(reg.getLocalEntity().at(msg.body.sharedEntityId)).value() =
                msg.body.b.shareMovement.vel;
        } catch (...) {
        }
    });
    responseHandler.registerHandler(rt::UDPCommand::NEW_ENTITY, [&reg, &datasToSend](const rt::UDPClientPacket &msg) {
        datasToSend.push_back(rt::UDPServerPacket(
            {.header = {.magic = 0x43434343, .cmd = rt::UDPCommand::NEW_ENTITY}, .body = std::move(msg.body)}
        ));
        rts::createMissile(reg, msg);
    });
}
