/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** RTypeServer
*/

#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include "RTypeUDPProtol.hpp"
#include "ResponseHandler.hpp"
#include "RoomManager.hpp"
#include "TCPResponseHandler.hpp"
#include "core/Registry.hpp"

namespace rts {

void registerComponents(ecs::Registry &reg);
void registerSystems(ecs::Registry &reg, sf::RenderWindow &window, float &dt);
void registerUdpResponse(ecs::Registry &reg, ntw::ResponseHandler<rt::UDPCommand, rt::UDPPacket> &responseHandler);
void registerTcpResponse(
    rts::RoomManager &roomManager,
    ntw::TCPServer &tcpServer,
    rt::TCPResponseHandler &responseHandler
);

// ! will be replace by factory
void createPlayer(ecs::Registry &reg, shared_entity_t sharedEntityId);
void createStatic(ecs::Registry &reg, float x, float y);
void createMissile(ecs::Registry &reg, const rt::UDPPacket &msg);
// !
} // namespace rts
