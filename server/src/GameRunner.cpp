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
#include "RTypeUDPProtol.hpp"
#include "Registry.hpp"

rts::GameRunner::GameRunner(int port, std::size_t stage) // ! Use the stage argument
    : _port(port), _udpServer(port),
      _responseHandler([](const rt::UDPClientPacket &packet) { return packet.header.cmd; }),
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

    // Faire un init des waves dans un autre fichier + le monstre volant.
    rts::init_waves(_waveManager, _datasToSend);
}

void rts::GameRunner::runGame()
{
    sf::Clock clock;

    while (_window.isOpen()) {
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
}
