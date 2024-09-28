/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** rtype_server
*/

#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

#include "GameProtocol.hpp"
#include "core/registry.hpp"
#include "core/response_handler.hpp"
#include "room_manager.hpp"

namespace rts {

void registerComponents(ecs::Registry &reg);
void registerSystems(ecs::Registry &reg, sf::RenderWindow &window, float &dt);
void registerUdpResponse(
    ecs::Registry &reg,
    ecs::ResponseHandler<rt::UdpCommand, rt::UdpPacket> &responseHandler
);
void registerTcpResponse(
    rts::RoomManager &roomManager,
    server::TCPServer &tcpServer,
    ecs::ResponseHandler<rt::TcpCommand, rt::TcpPacket> &responseHandler
);

// ! will be replace by factory
void createPlayer(ecs::Registry &reg, shared_entity_t sharedEntityId);
void createStatic(ecs::Registry &reg, float x, float y);
void createMissile(ecs::Registry &reg, const rt::UdpPacket &msg);
// !
} // namespace rts
