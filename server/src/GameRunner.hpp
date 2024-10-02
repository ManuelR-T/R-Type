/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** game_runner
*/

#pragma once

#include <SFML/Graphics.hpp>
#include <list>
#include <thread>
#include "RTypeUDPProtol.hpp"
#include "ResponseHandler.hpp"
#include "TickRateManager.hpp"
#include "core/Registry.hpp"
#include "udp/UDPServer.hpp"

namespace rts {

using data_send = std::pair<char const *, size_t>;

class GameRunner {
    private:
    int _port = 0;
    ntw::UDPServer _udpServer;
    std::thread _receiveThread;
    ecs::Registry _reg;
    ntw::ResponseHandler<rt::UDPCommand, rt::UDPClientPacket> _responseHandler;
    float _dt = 0;
    ntw::TickRateManager _tickRateManager;
    std::list<rt::UDPServerPacket> _datasToSend;
    sf::RenderWindow _window;

    public:
    GameRunner(int port);

    void runGame();
};
} // namespace rts
