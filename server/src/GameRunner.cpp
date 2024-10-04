/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** game_runner
*/

#include "GameRunner.hpp"
#include "RTypeServer.hpp"
#include "RTypeUDPProtol.hpp"

rts::GameRunner::GameRunner(int port)
    : _port(port), _udpServer(port),
      _responseHandler([](const rt::UDPClientPacket &packet) { return packet.header.cmd; }),
      _window(sf::VideoMode(1000, 700), "R-Type") // ! for debug
{
    rts::registerUdpResponse(_reg, _responseHandler, _datasToSend, _networkCallbacks);
    _udpServer.registerCommand([this](char *data, std::size_t size) {
        this->_responseHandler.handleResponse(data, size);
    });
    _udpServer.run();

    _window.setFramerateLimit(60); // ! for debug
    rts::registerComponents(_reg);
    rts::registerSystems(_reg, _window, _dt, _tickRateManager, _udpServer, _datasToSend, _networkCallbacks);

    // * create static
    for (int i = 0; i < 10; ++i) {
        _datasToSend.push_back(rt::UDPServerPacket(
            {.header = {.magic = 0x43434343, .cmd = rt::UDPCommand::NEW_ENTITY},
             .body = {.sharedEntityId = 0, .b = {.newEntityData = {0, {{100.f * i, 100.f * i}, {0}}}}}}
        ));
        rts::createStatic(_reg, 100.f * i, 100.f * i);
    }
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
