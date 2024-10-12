/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** register_udp_response
*/

#include <cstddef>
#include <string>
#include "ClientEntityFactory.hpp"
#include "GameManager.hpp"
#include "RTypeUDPProtol.hpp"
#include "Registry.hpp"
#include "SpriteManager.hpp"
#include "TrackedException.hpp"
#include "components/controllable.hpp"
#include "udp/UDPClient.hpp"
#include "components/share_movement.hpp"

static void handleStaticCreation(
    ecs::SpriteManager &spriteManager,
    ntw::UDPClient &udpClient,
    std::list<std::function<void(ecs::Registry &)>> &_networkCallbacks,
    const rt::UDPServerPacket &packet
)
{
    auto &[pos, _] = packet.body.b.newEntityData.moveData;

    _networkCallbacks.push_back([pos, &spriteManager, &udpClient](ecs::Registry &reg) {
        ecs::ClientEntityFactory::createClientEntityFromJSON(
            reg, spriteManager, udpClient, "assets/static.json", pos.x, pos.y
        );
    });
}

static void handleSharedCreation(
    const std::string &jsonFilePath,
    ecs::SpriteManager &spriteManager,
    ntw::UDPClient &udpClient,
    std::list<std::function<void(ecs::Registry &)>> &_networkCallbacks,
    const rt::UDPServerPacket &packet
)
{
    auto &[pos, _] = packet.body.b.newEntityData.moveData;
    auto sharedEntityId = packet.body.sharedEntityId;

    _networkCallbacks.push_back([sharedEntityId, pos, &spriteManager, &udpClient, jsonFilePath](ecs::Registry &reg) {
        ecs::ClientEntityFactory::createClientEntityFromJSON(
            reg, spriteManager, udpClient, jsonFilePath, pos.x, pos.y, sharedEntityId
        );
    });
}

static void handlePlayerCreation(
    std::size_t userId,
    ecs::SpriteManager &spriteManager,
    ntw::UDPClient &udpClient,
    std::list<std::function<void(ecs::Registry &)>> &_networkCallbacks,
    const rt::UDPServerPacket &packet
)
{
    auto &[pos, _] = packet.body.b.newEntityData.moveData;
    auto sharedEntityId = packet.body.sharedEntityId;

    _networkCallbacks.push_back([pos,
                                 sharedEntityId,
                                 playerIndex = packet.body.b.newEntityData.playerIndex,
                                 playerId = packet.body.b.newEntityData.playerId,
                                 &spriteManager,
                                 &udpClient,
                                 userId](ecs::Registry &reg) {
        ecs::ClientEntityFactory::createClientEntityFromJSON(
            reg,
            spriteManager,
            udpClient,
            "assets/player" + std::to_string(playerIndex) + ".json",
            pos.x,
            pos.y,
            sharedEntityId
        );
        if (playerId != userId) {
            reg.removeComponent<ecs::component::Controllable>(reg.getLocalEntity().at(sharedEntityId));
            reg.removeComponent<ecs::component::ShareMovement>(reg.getLocalEntity().at(sharedEntityId));
        }
    });
}

void rtc::GameManager::_registerUdpResponse(
    ecs::Registry &reg,
    ecs::SpriteManager &spriteManager,
    ntw::UDPClient &udpClient
)
{
    _udpResponseHandler.registerHandler(
        rt::UDPCommand::NEW_ENTITY,
        [this, &spriteManager, &udpClient](const rt::UDPServerPacket &packet) {
            switch (packet.body.b.newEntityData.type) {
                case rt::EntityType::PLAYER:
                    handlePlayerCreation(_userId, spriteManager, udpClient, this->_networkCallbacks, packet);
                    break;
                case rt::EntityType::MISSILE:
                    handleSharedCreation(
                        "assets/missile.json", spriteManager, udpClient, this->_networkCallbacks, packet
                    );
                    break;
                case rt::EntityType::MISSILE_BALL:
                    handleSharedCreation(
                        "assets/missileBall.json", spriteManager, udpClient, this->_networkCallbacks, packet
                    );
                    break;
                case rt::EntityType::STATIC:
                    handleStaticCreation(spriteManager, udpClient, this->_networkCallbacks, packet);
                    break;
                case rt::EntityType::BYDOS_WAVE:
                    handleSharedCreation(
                        "assets/bydosWave.json", spriteManager, udpClient, this->_networkCallbacks, packet
                    );
                    break;
                case rt::EntityType::ROBOT_GROUND:
                    handleSharedCreation(
                        "assets/robotGround.json", spriteManager, udpClient, this->_networkCallbacks, packet
                    );
                    break;
                default:
                    throw eng::TrackedException(
                        "Entity type(" + std::to_string(int(packet.body.b.newEntityData.type)) + ") not handled."
                    );
                    break;
            }
        }
    );
    _udpResponseHandler.registerHandler(rt::UDPCommand::MOVE_ENTITY, [&reg](const rt::UDPServerPacket &packet) {
        auto &sharedEntityId = packet.body.sharedEntityId;

        try {
            reg.getComponent<ecs::component::Position>(reg.getLocalEntity().at(sharedEntityId)).value() =
                packet.body.b.shareMovement.pos;
            reg.getComponent<ecs::component::Velocity>(reg.getLocalEntity().at(sharedEntityId)).value() =
                packet.body.b.shareMovement.vel;
        } catch (...) {
            // If entity does not exist, maybe server is late or ahead.
        }
    });
}
