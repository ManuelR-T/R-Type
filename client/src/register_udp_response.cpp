/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** register_udp_response
*/

#include <cstddef>
#include <exception>
#include <string>
#include "ClientEntityFactory.hpp"
#include "GameManager.hpp"
#include "RTypeUDPProtol.hpp"
#include "Registry.hpp"
#include "SpriteManager.hpp"
#include "components/controllable.hpp"
#include "components/player.hpp"
#include "imgui.h"
#include "udp/UDPClient.hpp"
#include "components/share_movement.hpp"
#include <imgui-SFML.h>

static void handleStaticCreation(
    ecs::SpriteManager &spriteManager,
    ntw::UDPClient &udpClient,
    std::list<std::function<void(ecs::Registry &)>> &_networkCallbacks,
    const rt::UDPPacket<rt::UDPBody::NEW_ENTITY_STATIC> &packet
)
{
    auto &[pos, _] = packet.body.moveData;

    _networkCallbacks.push_back([pos, &spriteManager, &udpClient](ecs::Registry &reg) {
        ecs::ClientEntityFactory::createClientEntityFromJSON(
            reg, spriteManager, udpClient, "assets/static.json", pos.x, pos.y
        );
    });
}

static void handlePlayerCreation(
    std::size_t userId,
    ecs::SpriteManager &spriteManager,
    ntw::UDPClient &udpClient,
    std::list<std::function<void(ecs::Registry &)>> &_networkCallbacks,
    const rt::UDPPacket<rt::UDPBody::NEW_ENTITY_PLAYER> &packet,
    std::shared_ptr<ImFont> font
)
{
    _networkCallbacks.push_back([packet, &spriteManager, &udpClient, userId, font](ecs::Registry &reg) {
        auto entity = ecs::ClientEntityFactory::createClientEntityFromJSON(
            reg,
            spriteManager,
            udpClient,
            "assets/player" + std::to_string(packet.body.playerIndex) + ".json",
            packet.body.moveData.pos.x,
            packet.body.moveData.pos.y,
            packet.sharedEntityId,
            0.0f,
            0.0f,
            font
        );
        reg.getComponent<ecs::component::Player>(entity) =
            ecs::component::Player{.name = packet.body.playerName, .id = userId};
        if (packet.body.playerId != userId) {
            reg.removeComponent<ecs::component::Controllable>(reg.getLocalEntity().at(packet.sharedEntityId));
            reg.removeComponent<ecs::component::ShareMovement>(reg.getLocalEntity().at(packet.sharedEntityId));
        }
    });
}

template <typename T>
static void handleSharedCreation(
    const std::string &jsonFilePath,
    ecs::SpriteManager &spriteManager,
    ntw::UDPClient &udpClient,
    std::list<std::function<void(ecs::Registry &)>> &_networkCallbacks,
    const rt::UDPPacket<T> &packet
)
{
    auto &[pos, _] = packet.body.moveData;
    auto sharedEntityId = packet.sharedEntityId;

    _networkCallbacks.push_back([sharedEntityId, pos, &spriteManager, &udpClient, jsonFilePath](ecs::Registry &reg) {
        ecs::ClientEntityFactory::createClientEntityFromJSON(
            reg, spriteManager, udpClient, jsonFilePath, pos.x, pos.y, sharedEntityId
        );
    });
}

void rtc::GameManager::_registerUdpResponse(
    ecs::Registry &reg,
    ecs::SpriteManager &spriteManager,
    ntw::UDPClient &udpClient
)
{
    _udpResponseHandler.registerHandler<rt::UDPBody::NEW_ENTITY_STATIC>(
        rt::UDPCommand::NEW_ENTITY_STATIC,
        [this, &spriteManager, &udpClient](const rt::UDPPacket<rt::UDPBody::NEW_ENTITY_STATIC> &packet) {
            handleStaticCreation(spriteManager, udpClient, this->_networkCallbacks, packet);
        }
    );
    _udpResponseHandler.registerHandler<rt::UDPBody::NEW_ENTITY_PLAYER>(
        rt::UDPCommand::NEW_ENTITY_PLAYER,
        [this, &spriteManager, &udpClient](const rt::UDPPacket<rt::UDPBody::NEW_ENTITY_PLAYER> &packet) {
            handlePlayerCreation(_userId, spriteManager, udpClient, this->_networkCallbacks, packet, _font);
        }
    );
    _udpResponseHandler.registerHandler<rt::UDPBody::NEW_ENTITY_MISSILE>(
        rt::UDPCommand::NEW_ENTITY_MISSILE,
        [this, &spriteManager, &udpClient](const rt::UDPPacket<rt::UDPBody::NEW_ENTITY_MISSILE> &packet) {
            handleSharedCreation<rt::UDPBody::NEW_ENTITY_MISSILE>(
                "assets/missile.json", spriteManager, udpClient, this->_networkCallbacks, packet
            );
        }
    );
    _udpResponseHandler.registerHandler<rt::UDPBody::NEW_ENTITY_MISSILE_BALL>(
        rt::UDPCommand::NEW_ENTITY_MISSILE_BALL,
        [this, &spriteManager, &udpClient](const rt::UDPPacket<rt::UDPBody::NEW_ENTITY_MISSILE_BALL> &packet) {
            handleSharedCreation<rt::UDPBody::NEW_ENTITY_MISSILE_BALL>(
                "assets/missileBall.json", spriteManager, udpClient, this->_networkCallbacks, packet
            );
        }
    );
    _udpResponseHandler.registerHandler<rt::UDPBody::NEW_ENTITY_BYDOS_WAVE>(
        rt::UDPCommand::NEW_ENTITY_BYDOS_WAVE,
        [this, &spriteManager, &udpClient](const rt::UDPPacket<rt::UDPBody::NEW_ENTITY_BYDOS_WAVE> &packet) {
            handleSharedCreation<rt::UDPBody::NEW_ENTITY_BYDOS_WAVE>(
                "assets/bydosWave.json", spriteManager, udpClient, this->_networkCallbacks, packet
            );
        }
    );
    _udpResponseHandler.registerHandler<rt::UDPBody::NEW_ENTITY_ROBOT_GROUND>(
        rt::UDPCommand::NEW_ENTITY_ROBOT_GROUND,
        [this, &spriteManager, &udpClient](const rt::UDPPacket<rt::UDPBody::NEW_ENTITY_ROBOT_GROUND> &packet) {
            handleSharedCreation<rt::UDPBody::NEW_ENTITY_ROBOT_GROUND>(
                "assets/robotGround.json", spriteManager, udpClient, this->_networkCallbacks, packet
            );
        }
    );
    _udpResponseHandler.registerHandler<rt::UDPBody::MOVE_ENTITY>(
        rt::UDPCommand::MOVE_ENTITY,
        [&reg](const rt::UDPPacket<rt::UDPBody::MOVE_ENTITY> &packet) {
            try {
                reg.getComponent<ecs::component::Position>(reg.getLocalEntity().at(packet.sharedEntityId)).value() =
                    packet.body.pos;
                reg.getComponent<ecs::component::Velocity>(reg.getLocalEntity().at(packet.sharedEntityId)).value() =
                    packet.body.vel;
            } catch (const std::exception &e) {
                // If entity does not exist, maybe server is late or ahead.
                // auto currentTime = std::chrono::system_clock::now().time_since_epoch();
                // auto currentTimeMs = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime).count();
                // eng::logWarning(std::to_string(currentTimeMs) + ": " + e.what());
            }
        }
    );
    _udpResponseHandler.registerHandler<rt::UDPBody::DEL_ENTITY>(
        rt::UDPCommand::DEL_ENTITY,
        [this](const rt::UDPPacket<rt::UDPBody::DEL_ENTITY> &packet) {
            try {
                _networkCallbacks.push_back([sharedEntityId = packet.sharedEntityId](ecs::Registry &reg) {
                    reg.killEntity(reg.getLocalEntity().at(sharedEntityId));
                });
            } catch (...) {
            }
        }
    );
}
