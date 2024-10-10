/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** game_runner
*/

#pragma once

#include <SFML/Graphics.hpp>
#include <cstddef>
#include <functional>
#include <list>
#include <thread>
#include "RTypeUDPProtol.hpp"
#include "Registry.hpp"
#include "ResponseHandler.hpp"
#include "ServerTickRate.hpp"
#include "TickRateManager.hpp"
#include "WaveManager.hpp"
#include "udp/UDPServer.hpp"

namespace rts {

class GameRunner {
    private:
    int _port = 0;
    ntw::UDPServer _udpServer;
    std::thread _receiveThread;
    ntw::ResponseHandler<rt::UDPCommand, rt::UDPClientPacket> _responseHandler;

    ecs::Registry _reg;
    ecs::WaveManager _waveManager;

    float _dt = 0;
    ntw::TickRateManager<rts::TickRate> _tickRateManager;

    std::list<rt::UDPServerPacket> _datasToSend;
    std::list<std::function<void(ecs::Registry &reg)>> _networkCallbacks;

    sf::RenderWindow _window;

    public:
    GameRunner(int port, std::size_t stage);

    void runGame();
};
} // namespace rts
