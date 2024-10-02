/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** register_udp_response
*/

#include <iostream>
#include "GameManager.hpp"
#include "RTypeClient.hpp"
#include "core/Registry.hpp"

void rtc::GameManager::_registerUdpResponse(ecs::Registry &reg, ecs::SpriteManager &spriteManager)
{
    _udpResponseHandler.registerHandler(rt::UDPCommand::NEW_ENTITY, [&spriteManager, &reg](const rt::UDPServerPacket &packet) {
        if (packet.body.b.newEntityData.type == 1) {
            auto &[pos, vel] = packet.body.b.newEntityData.moveData;
            rtc::createMissile(reg, spriteManager, packet.body.sharedEntityId, pos.x, pos.y);
        }
    });
}
