/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** RTypeServer
*/

#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

#include "udp/UDPServer.hpp"
#include "RTypeTCPProtol.hpp"
#include "RTypeUDPProtol.hpp"
#include "ResponseHandler.hpp"
#include "RoomManager.hpp"
#include "TickRateManager.hpp"
#include "core/Registry.hpp"

namespace rts {

void registerComponents(ecs::Registry &reg);
void registerSystems(
    ecs::Registry &reg,
    sf::RenderWindow &window,
    float &dt,
    ntw::TickRateManager &tick_rate_manager,
    ntw::UDPServer &udpServer,
    std::list<rt::UDPServerPacket> &datasToSend
);
void registerUdpResponse(
    ecs::Registry &reg,
    ntw::ResponseHandler<rt::UDPCommand, rt::UDPClientPacket> &responseHandler,
    std::list<rt::UDPServerPacket> &datasToSend
);
void registerTcpResponse(
    rts::RoomManager &roomManager,
    ntw::TCPServer &tcpServer,
    ntw::ResponseHandler<rt::TCPCommand, rt::TCPPacket> &responseHandler
);

// ! will be replace by factory
void createPlayer(ecs::Registry &reg, shared_entity_t sharedEntityId);
void createStatic(ecs::Registry &reg, float x, float y);
void createMissile(ecs::Registry &reg, const rt::UDPClientPacket &msg);
// !
} // namespace rts
