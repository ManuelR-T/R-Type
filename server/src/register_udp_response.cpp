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
    std::list<rt::UDPServerPacket> &datasToSend,
    std::list<std::function<void()>> &networkCallbacks
)
{
    responseHandler.registerHandler(
        rt::UDPCommand::NEW_PLAYER,
        [&reg, &networkCallbacks](const rt::UDPClientPacket &msg) {
            networkCallbacks.push_back([&reg, sharedEntityId = msg.body.sharedEntityId]() {
                rts::createPlayer(reg, sharedEntityId);
            });
        }
    );
    responseHandler.registerHandler(
        rt::UDPCommand::MOVE_ENTITY,
        [&reg, &networkCallbacks](const rt::UDPClientPacket &msg) {
            networkCallbacks.push_back([&reg, msg]() {
                try {
                    reg.getComponent<ecs::component::Position>(reg.getLocalEntity().at(msg.body.sharedEntityId))
                        .value() = msg.body.b.shareMovement.pos;
                    reg.getComponent<ecs::component::Velocity>(reg.getLocalEntity().at(msg.body.sharedEntityId))
                        .value() = msg.body.b.shareMovement.vel;
                } catch (...) {
                }
            });
        }
    );
    responseHandler.registerHandler(
        rt::UDPCommand::NEW_ENTITY,
        [&reg, &datasToSend, &networkCallbacks](const rt::UDPClientPacket &msg) {
            datasToSend.push_back(
                rt::UDPServerPacket({.header = {.cmd = rt::UDPCommand::NEW_ENTITY}, .body = std::move(msg.body)})
            );
            networkCallbacks.push_back([&reg, msg]() { rts::createMissile(reg, msg); });
        }
    );
}
