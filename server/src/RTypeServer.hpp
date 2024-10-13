/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** RTypeServer
*/

#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <list>
#include <vector>
#include "Registry.hpp"
#include "RoomManager.hpp"
#include "ServerTickRate.hpp"
#include "TCPResponseHandler.hpp"
#include "TickRateManager.hpp"
#include "UDPResponseHandler.hpp"
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
    std::list<std::vector<char>> &datasToSend,
    std::list<std::function<void(ecs::Registry &reg)>> &networkCallbacks,
    ecs::WaveManager &waveManager
);
void registerUdpResponse(
    rt::UDPResponseHandler &responseHandler,
    std::list<std::vector<char>> &datasToSend,
    std::list<std::function<void(ecs::Registry &reg)>> &networkCallbacks
);
void registerTcpResponse(
    rts::RoomManager &roomManager,
    ntw::TCPServer &tcpServer,
    rt::TCPResponseHandler &responseHandler
);

void init_waves(ecs::WaveManager &waveManager, std::list<std::vector<char>> &_datasToSend);

} // namespace rts
