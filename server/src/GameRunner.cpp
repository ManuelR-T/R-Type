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
#include "ServerEntityFactory.hpp"
#include "SpriteManager.hpp"

rts::GameRunner::GameRunner(int port, std::size_t stage) // ! Use the stage argument
    : _port(port), _udpServer(port),
      _responseHandler([](const rt::UDPClientPacket &packet) { return packet.header.cmd; }),
      _window(sf::VideoMode(720, 480), "R-Type") // ! for debug
{
    eng::logWarning("Selected stage: " + std::to_string(stage) + ".");
    ecs::SpriteManager spriteManager;
    rts::registerUdpResponse(_responseHandler, _datasToSend, _networkCallbacks);
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
            {.header = {.cmd = rt::UDPCommand::NEW_ENTITY},
             .body = {.sharedEntityId = 0, .b = {.newEntityData = {0, {{100.f * i, 100.f * i}, {0}}}}}}
        ));
        ecs::ServerEntityFactory::createServerEntityFromJSON(_reg, "assets/static.json", 100.f * i, 100.f * i);
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
