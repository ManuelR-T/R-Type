/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** register_ecs
*/

#include <utility>
#include "RTypeServer.hpp"
#include "RTypeUDPProtol.hpp"
#include "ServerEntityFactory.hpp"
#include "components/position.hpp"
#include "components/velocity.hpp"

void rts::registerUdpResponse(
    ntw::ResponseHandler<rt::UDPCommand, rt::UDPClientPacket> &responseHandler,
    std::list<rt::UDPServerPacket> &datasToSend,
    std::list<std::function<void(ecs::Registry &reg)>> &networkCallbacks
)
{
    responseHandler.registerHandler(
        rt::UDPCommand::NEW_PLAYER,
        [&datasToSend, &networkCallbacks](const rt::UDPClientPacket &msg) {
            networkCallbacks.push_back([sharedEntityId = msg.body.sharedEntityId](ecs::Registry &reg) {
                ecs::ServerEntityFactory::createServerEntityFromJSON(
                    reg, "assets/player.json", INT32_MAX, INT32_MAX, sharedEntityId
                );
            });
            datasToSend.push_back(
                rt::UDPServerPacket({.header = {.cmd = rt::UDPCommand::NEW_PLAYER}, .body = msg.body})
            );
        }
    );
    responseHandler.registerHandler(rt::UDPCommand::MOVE_ENTITY, [&networkCallbacks](const rt::UDPClientPacket &msg) {
        networkCallbacks.push_back([msg](ecs::Registry &reg) {
            try {
                reg.getComponent<ecs::component::Position>(reg.getLocalEntity().at(msg.body.sharedEntityId)).value() =
                    msg.body.b.shareMovement.pos;
                reg.getComponent<ecs::component::Velocity>(reg.getLocalEntity().at(msg.body.sharedEntityId)).value() =
                    msg.body.b.shareMovement.vel;
            } catch (...) {
            }
        });
    });
    responseHandler.registerHandler(
        rt::UDPCommand::NEW_ENTITY,
        [&datasToSend, &networkCallbacks](const rt::UDPClientPacket &msg) {
            datasToSend.push_back(
                rt::UDPServerPacket({.header = {.cmd = rt::UDPCommand::NEW_ENTITY}, .body = std::move(msg.body)})
            );
            networkCallbacks.push_back([msg](ecs::Registry &reg) { rts::createMissile(reg, msg); });
        }
    );
}
