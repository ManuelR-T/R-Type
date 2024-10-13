/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** game_runner
*/

#include "GameRunner.hpp"
#include <cstddef>
#include <string>
#include "IndexedZipper.hpp"
#include "Logger.hpp"
#include "RTypeServer.hpp"
#include "RTypeUDPProtol.hpp"
#include "Registry.hpp"
#include "components/player.hpp"
#include "components/shared_entity.hpp"

rts::GameRunner::GameRunner(int port, std::size_t stage) // ! Use the stage argument
    : _udpServer(port)
{
    eng::logWarning("Selected stage: " + std::to_string(stage) + ".");

    rts::registerUdpResponse(_responseHandler, _datasToSend, _networkCallbacks);
    _udpServer.registerCommand([this](char *data, std::size_t size) {
        this->_responseHandler.handleResponse(data, size);
    });
    _udpServer.run();

    rts::registerComponents(_reg);
    rts::registerSystems(
        _reg, _window, _dt, _tickRateManager, _udpServer, _datasToSend, _networkCallbacks, _waveManager
    );
    rts::init_waves(_waveManager, _datasToSend);
}

void rts::GameRunner::killPlayer(size_t playerId)
{
    _networkCallbacks.push_back([playerId, this](ecs::Registry &reg) {
        ecs::IndexedZipper<ecs::component::Player, ecs::component::SharedEntity> zip(
            reg.getComponents<ecs::component::Player>(), reg.getComponents<ecs::component::SharedEntity>()
        );

        for (auto [e, player, shared] : zip) {
            if (player.id == playerId) {
                _datasToSend.push_back(rt::UDPPacket<rt::UDPBody::DEL_ENTITY>({.cmd = rt::UDPCommand::DEL_ENTITY,
                                                                               .sharedEntityId = shared.sharedEntityId})
                                           .serialize());
                reg.killEntity(e);
                return;
            }
        }
    });
}

void rts::GameRunner::addWindow(sf::VideoMode &&videomode, const std::string &title)
{
    _window.create(videomode, title);
    _window.setFramerateLimit(60); // ! for debug
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
