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
#include <vector>
#include "Registry.hpp"
#include "ServerTickRate.hpp"
#include "TickRateManager.hpp"
#include "UDPResponseHandler.hpp"
#include "WaveManager.hpp"
#include "udp/UDPServer.hpp"

namespace rts {

class GameRunner {
    private:
    ntw::UDPServer _udpServer;
    rt::UDPResponseHandler _responseHandler;

    ecs::Registry _reg;
    ecs::WaveManager _waveManager;

    float _dt = 0;
    ntw::TickRateManager<rts::TickRate> _tickRateManager;

    std::list<std::vector<char>> _datasToSend;
    std::list<std::function<void(ecs::Registry &reg)>> _networkCallbacks;

    sf::RenderWindow _window;

    public:
    GameRunner(int port, std::size_t stage);

    void runGame(bool &stopGame);
};
} // namespace rts
