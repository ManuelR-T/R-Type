/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** game_runner
*/

#pragma once

#include <SFML/Graphics.hpp>
#include <thread>
#include "RTypeUDPProtol.hpp"
#include "ResponseHandler.hpp"
#include "UDPServer.hpp"
#include "core/Registry.hpp"

namespace rts {

class GameRunner {
    private:
    int _port = 0;
    ntw::UDPServer _udpServer;
    std::thread _receiveThread;
    ecs::Registry _reg;
    ntw::ResponseHandler<rt::UDPCommand, rt::UDPPacket> _responseHandler;
    float _dt = 0;
    sf::RenderWindow _window;

    public:
    GameRunner(int port);

    void runGame();
};
} // namespace rts