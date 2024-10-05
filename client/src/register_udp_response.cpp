/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** register_udp_response
*/

#include "EntityFactory.hpp"
#include "GameManager.hpp"
#include "Registry.hpp"

void rtc::GameManager::_registerUdpResponse(ecs::Registry &reg, ecs::EntityFactory &entityFactory)
{
    _udpResponseHandler.registerHandler(
        rt::UDPCommand::NEW_ENTITY,
        [&entityFactory, this](const rt::UDPServerPacket &packet) {
            if (packet.body.b.newEntityData.type == 1) {
                auto &[pos, _] = packet.body.b.newEntityData.moveData;
                auto sharedEntityId = packet.body.sharedEntityId;

                _networkCallbacks.push_back([sharedEntityId, pos, &entityFactory]() {
                    entityFactory.createEntityFromJSON("assets/missile.json", pos.x, pos.y, sharedEntityId);
                });
            }
            if (packet.body.b.newEntityData.type == 0) {
                auto &[pos, _] = packet.body.b.newEntityData.moveData;

                _networkCallbacks.push_back([pos, &entityFactory]() {
                    entityFactory.createEntityFromJSON("assets/static.json", pos.x, pos.y);
                });
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
