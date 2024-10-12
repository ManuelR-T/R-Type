/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** register_ecs
*/

#include <cstddef>
#include <limits>
#include <string>
#include <utility>
#include "RTypeServer.hpp"
#include "RTypeUDPProtol.hpp"
#include "Registry.hpp"
#include "ServerEntityFactory.hpp"
#include "components/position.hpp"
#include "components/velocity.hpp"

static void handlePlayerCreation(
    std::list<rt::UDPServerPacket> &datasToSend,
    std::list<std::function<void(ecs::Registry &reg)>> &networkCallbacks,
    const rt::UDPClientPacket &msg
)
{
    shared_entity_t sharedEntityId = msg.body.sharedEntityId;
    std::size_t playerIndex = msg.body.b.newEntityData.playerIndex;
    const auto &pos = msg.body.b.newEntityData.moveData.pos;

    networkCallbacks.push_back([playerIndex, sharedEntityId, pos](ecs::Registry &reg) {
        ecs::ServerEntityFactory::createServerEntityFromJSON(
            reg,
            "assets/player" + std::to_string(playerIndex) + ".json",
            pos.x,
            pos.y,
            sharedEntityId
        );
    });
    datasToSend.push_back({.header = {.cmd = rt::UDPCommand::NEW_ENTITY}, .body = std::move(msg.body)});
}

static void handleMissileCreation(
    std::list<rt::UDPServerPacket> &datasToSend,
    std::list<std::function<void(ecs::Registry &reg)>> &networkCallbacks,
    const rt::UDPClientPacket &msg
)
{
    const auto &pos = msg.body.b.newEntityData.moveData.pos;
    const auto &vel = msg.body.b.newEntityData.moveData.vel;

    networkCallbacks.push_back([pos, vel](ecs::Registry &reg) {
        ecs::ServerEntityFactory::createServerEntityFromJSON(
            reg, "assets/missile.json", pos.x, pos.y, std::numeric_limits<size_t>::max(), vel.vx, vel.vy
        );
    });
    datasToSend.push_back({.header = {.cmd = rt::UDPCommand::NEW_ENTITY}, .body = std::move(msg.body)});
}

void rts::registerUdpResponse(
    ntw::ResponseHandler<rt::UDPCommand, rt::UDPClientPacket> &responseHandler,
    std::list<rt::UDPServerPacket> &datasToSend,
    std::list<std::function<void(ecs::Registry &reg)>> &networkCallbacks
)
{
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
            switch (msg.body.b.newEntityData.type) {
                case rt::EntityType::PLAYER:
                    handlePlayerCreation(datasToSend, networkCallbacks, msg);
                    break;
                case rt::EntityType::MISSILE:
                    handleMissileCreation(datasToSend, networkCallbacks, msg);
                    break;
                default:
                    throw eng::TrackedException(
                        "Entity type(" + std::to_string(int(msg.body.b.newEntityData.type)) + ") not handled."
                    );
                    break;
            }
        }
    );
}
