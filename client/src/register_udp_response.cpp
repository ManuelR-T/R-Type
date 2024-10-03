/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** register_udp_response
*/

#include "GameManager.hpp"
#include "RTypeClient.hpp"
#include "core/Registry.hpp"

void rtc::GameManager::_registerUdpResponse(ecs::Registry &reg, ecs::SpriteManager &spriteManager)
{
    _udpResponseHandler.registerHandler(
        rt::UDPCommand::NEW_ENTITY,
        [&spriteManager, &reg, this](const rt::UDPServerPacket &packet) {
            if (packet.body.b.newEntityData.type == 1) {
                auto &[pos, _] = packet.body.b.newEntityData.moveData;
                auto sharedEntityId = packet.body.sharedEntityId;

                _servModifierQueue.push_back([sharedEntityId, pos, &reg, &spriteManager]() {
                    rtc::createMissile(reg, spriteManager, sharedEntityId, pos.x, pos.y);
                });
            }
        }
    );
}
