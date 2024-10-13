/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** game_runner
*/

#include "GameRunner.hpp"
#include <cstddef>
#include <string>
#include "Logger.hpp"
#include "RTypeServer.hpp"
#include "Registry.hpp"

rts::GameRunner::GameRunner(int port, std::size_t stage) // ! Use the stage argument
    : _udpServer(port),
      _window(sf::VideoMode(720, 480), "R-Type") // ! for debug
{
    eng::logWarning("Selected stage: " + std::to_string(stage) + ".");

    rts::registerUdpResponse(_responseHandler, _datasToSend, _networkCallbacks);
    _udpServer.registerCommand([this](char *data, std::size_t size) {
        this->_responseHandler.handleResponse(data, size);
    });
    _udpServer.run();

    _window.setFramerateLimit(60); // ! for debug
    rts::registerComponents(_reg);
    rts::registerSystems(
        _reg, _window, _dt, _tickRateManager, _udpServer, _datasToSend, _networkCallbacks, _waveManager
    );
    rts::init_waves(_waveManager, _datasToSend);
}

void rts::GameRunner::runGame(bool &stopGame)
{
    sf::Clock clock;

    while (_window.isOpen() && !stopGame) {
        _dt = clock.restart().asSeconds();

        // ! for debug
        sf::Event event;
        while (_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                _window.close();
            }
        }
        // ! for debug
        _reg.runSystems();
    }
    if (_window.isOpen()) {
        _window.close();
    }
}
