/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** register_ecs
*/

#include <iostream>
#include <utility>
#include "RTypeServer.hpp"
#include "RTypeUDPProtol.hpp"
#include "components/position.hpp"
#include "components/velocity.hpp"
#include "ServerEntityFactory.hpp"

void rts::registerUdpResponse(
    ecs::Registry &reg,
    ntw::ResponseHandler<rt::UDPCommand, rt::UDPClientPacket> &responseHandler,
    std::list<rt::UDPServerPacket> &datasToSend,
    std::list<std::function<void()>> &networkCallbacks,
    ecs::ServerEntityFactory &entityFactory
)
{
    responseHandler.registerHandler(
        rt::UDPCommand::NEW_PLAYER,
        [&reg, &networkCallbacks, &entityFactory](const rt::UDPClientPacket &msg) {
            networkCallbacks.push_back([&reg, &entityFactory, sharedEntityId = msg.body.sharedEntityId]() {
                std::cout << "Start callback" << std::endl;
                std::cout << sharedEntityId << std::endl;
                rts::createPlayer(reg, sharedEntityId);
                //entityFactory.createEntityFromJSON("assets/player.json", INT32_MAX, INT32_MAX, sharedEntityId);
                std::cout << "End Callback" << std::endl;
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
