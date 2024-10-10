/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** RTypeServer
*/

#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <list>
#include "RTypeUDPProtol.hpp"
#include "Registry.hpp"
#include "ResponseHandler.hpp"
#include "RoomManager.hpp"
#include "ServerTickRate.hpp"
#include "TCPResponseHandler.hpp"
#include "TickRateManager.hpp"
#include "WaveManager.hpp"
#include "udp/UDPServer.hpp"

namespace rts {
void registerComponents(ecs::Registry &reg);
void registerSystems(
    ecs::Registry &reg,
    sf::RenderWindow &window,
    float &dt,
    ntw::TickRateManager<rts::TickRate> &tick_rate_manager,
    ntw::UDPServer &udpServer,
    std::list<rt::UDPServerPacket> &datasToSend,
    std::list<std::function<void(ecs::Registry &reg)>> &networkCallbacks,
    ecs::WaveManager &waveManager
);
void registerUdpResponse(
    ntw::ResponseHandler<rt::UDPCommand, rt::UDPClientPacket> &responseHandler,
    std::list<rt::UDPServerPacket> &datasToSend,
    std::list<std::function<void(ecs::Registry &reg)>> &networkCallbacks
);
void registerTcpResponse(
    rts::RoomManager &roomManager,
    ntw::TCPServer &tcpServer,
    rt::TCPResponseHandler &responseHandler
);

void init_waves(ecs::WaveManager &waveManager, std::list<rt::UDPServerPacket> &_datasToSend);

// ! will be replace by factory
void createPlayer(ecs::Registry &reg, shared_entity_t sharedEntityId);
void createStatic(ecs::Registry &reg, float x, float y);
void createMissile(ecs::Registry &reg, const rt::UDPClientPacket &msg);
// !
} // namespace rts
