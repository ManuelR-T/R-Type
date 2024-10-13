/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** register_ecs
*/

#include <cstddef>
#include <exception>
#include <limits>
#include <string>
#include <utility>
#include "RTypeServer.hpp"
#include "RTypeUDPProtol.hpp"
#include "Registry.hpp"
#include "ServerEntityFactory.hpp"
#include "components/player.hpp"
#include "components/position.hpp"
#include "components/velocity.hpp"

static void handlePlayerCreation(
    std::list<std::vector<char>> &datasToSend,
    std::list<std::function<void(ecs::Registry &reg)>> &networkCallbacks,
    const rt::UDPPacket<rt::UDPBody::NEW_ENTITY_PLAYER> &msg
)
{
    shared_entity_t sharedEntityId = msg.sharedEntityId;
    std::size_t playerIndex = msg.body.playerIndex;
    const auto &pos = msg.body.moveData.pos;

    networkCallbacks.push_back([playerIndex, sharedEntityId, pos, playerId = msg.body.playerId](ecs::Registry &reg) {
        auto entity = ecs::ServerEntityFactory::createServerEntityFromJSON(
            reg, "assets/player" + std::to_string(playerIndex) + ".json", pos.x, pos.y, sharedEntityId
        );
        reg.getComponent<ecs::component::Player>(entity)->id = playerId;
    });
    datasToSend.push_back(std::move(msg).serialize());
}

static void handleMissileCreation(
    std::list<std::vector<char>> &datasToSend,
    std::list<std::function<void(ecs::Registry &reg)>> &networkCallbacks,
    const rt::UDPPacket<rt::UDPBody::NEW_ENTITY_MISSILE> &msg
)
{
    const auto &pos = msg.body.moveData.pos;
    const auto &vel = msg.body.moveData.vel;

    networkCallbacks.push_back([pos, vel](ecs::Registry &reg) {
        ecs::ServerEntityFactory::createServerEntityFromJSON(
            reg, "assets/missile.json", pos.x, pos.y, std::numeric_limits<size_t>::max(), vel.vx, vel.vy
        );
    });
    datasToSend.push_back(std::move(msg).serialize());
}

void rts::registerUdpResponse(
    rt::UDPResponseHandler &responseHandler,
    std::list<std::vector<char>> &datasToSend,
    std::list<std::function<void(ecs::Registry &reg)>> &networkCallbacks
)
{
    responseHandler.registerHandler<rt::UDPBody::NEW_ENTITY_PLAYER>(
        rt::UDPCommand::NEW_ENTITY_PLAYER,
        [&datasToSend, &networkCallbacks](const rt::UDPPacket<rt::UDPBody::NEW_ENTITY_PLAYER> &msg) {
            datasToSend.push_back(std::move(msg).serialize());
            handlePlayerCreation(datasToSend, networkCallbacks, msg);
        }
    );
    responseHandler.registerHandler<rt::UDPBody::NEW_ENTITY_MISSILE>(
        rt::UDPCommand::NEW_ENTITY_MISSILE,
        [&datasToSend, &networkCallbacks](const rt::UDPPacket<rt::UDPBody::NEW_ENTITY_MISSILE> &msg) {
            datasToSend.push_back(std::move(msg).serialize());
            handleMissileCreation(datasToSend, networkCallbacks, msg);
        }
    );
    responseHandler.registerHandler<rt::UDPBody::MOVE_ENTITY>(
        rt::UDPCommand::MOVE_ENTITY,
        [&networkCallbacks](const rt::UDPPacket<rt::UDPBody::MOVE_ENTITY> &msg) {
            networkCallbacks.push_back([msg](ecs::Registry &reg) {
                try {
                    reg.getComponent<ecs::component::Position>(reg.getLocalEntity().at(msg.sharedEntityId)).value() =
                        msg.body.pos;
                    reg.getComponent<ecs::component::Velocity>(reg.getLocalEntity().at(msg.sharedEntityId)).value() =
                        msg.body.vel;
                } catch (const std::exception &e) {
                    // auto currentTime = std::chrono::system_clock::now().time_since_epoch();
                    // auto currentTimeMs = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime).count();
                    // eng::logWarning(std::to_string(currentTimeMs) + ": " + e.what());
                }
            });
        }
    );
}
